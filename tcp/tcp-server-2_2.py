from params import ServerParams
from datagram import Datagram
from server import Server

import socket
import struct
import threading


class ThreadedTCPServer(Server):
    def __init__(self, params):
        super().__init__(params)
        self.threads = []

    def listen(self) -> None:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as self.socket:
            self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            self.socket.bind(self.params)
            self.socket.listen()

            print(f"[SERVER] Listening on {self.host}:{self.port}")

            try:
                while True:
                    conn, address = self.socket.accept()
                    print(f"[SERVER] Accepted {address}")

                    t = threading.Thread(target=self.handle_client, args=(conn, address))
                    t.start()
                    self.threads.append(t)

            except KeyboardInterrupt:
                print("\n[SERVER] Stopping...")
                for t in self.threads:
                    t.join()

    def handle_client(self, conn, address):
        with conn:
            try:
                count_bytes = self._recv_all(conn, 4)
                if not count_bytes: return

                count = struct.unpack("!I", count_bytes)[0]
                print(f"[SERVER] Expecting {count} datagrams")

                for i in range(count):
                    header = self._recv_all(conn, Datagram.HEADER_SIZE)
                    if not header: break

                    _, _, txt_len = struct.unpack(Datagram.NETWORK_BIG_ENDIAN_FORMAT, header)
                    body = self._recv_all(conn, txt_len)

                    decoded = Datagram.decode(header + body)
                    print(f"[SERVER] Decoded: {decoded.val_s}, {decoded.val_i}, '{decoded.text}'")

            except Exception as e:
                print(f"[SERVER] Error: {e}")

    def _recv_all(self, sock, n):
        data = b""
        while len(data) < n:
            packet = sock.recv(n - len(data))
            if not packet: return None
            data += packet
        return data


if __name__ == "__main__":
    SERVER_HOST = "z53_tcp_server"
    SERVER_PORT = 2137
    server = ThreadedTCPServer(ServerParams(host=SERVER_HOST, port=SERVER_PORT))
    server.listen()