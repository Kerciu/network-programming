from params import ServerParams
from datagram import Datagram
from server import Server

import socket
import struct
from concurrent.futures import ThreadPoolExecutor


class ConcurrentTCPServer(Server):
    def __init__(self, params):
        super().__init__(params)
        self.executor = ThreadPoolExecutor(max_workers=5)

    def listen(self) -> None:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as self.socket:
            self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            self.socket.bind(self.params)
            self.socket.listen()

            print(f"[SERVER] Listening on {self.host}:{self.port}")

            try:
                while True:
                    conn, address = self.socket.accept()
                    print(f"[SERVER] Connected by {address}")
                    self.executor.submit(self.handle_client, conn, address)
            except KeyboardInterrupt:
                self.executor.shutdown(wait=True)

    def handle_client(self, conn, address):
        with conn:
            try:
                count_bytes = self._recv_all(conn, 4)
                if not count_bytes: return

                count = struct.unpack("!I", count_bytes)[0]
                print(f"[{address}] Expecting {count} datagrams")

                for i in range(count):
                    header = self._recv_all(conn, Datagram.HEADER_SIZE)
                    if not header: break

                    _, _, txt_len = struct.unpack(Datagram.NETWORK_BIG_ENDIAN_FORMAT, header)
                    body = self._recv_all(conn, txt_len)

                    decoded = Datagram.decode(header + body)
                    print(f"[{address}] Decoded {i+1}: {decoded.val_s}, {decoded.val_i}, '{decoded.text}'")

            except Exception as e:
                print(f"[{address}] Error: {e}")

    def _recv_all(self, sock, n):
        data = b""
        while len(data) < n:
            packet = sock.recv(n - len(data))
            if not packet: return None
            data += packet
        return data


if __name__ == "__main__":
    SERVER_HOST = "z53_udp_server_py"
    SERVER_PORT = 2137
    server = ConcurrentTCPServer(ServerParams(host=SERVER_HOST, port=SERVER_PORT))
    server.listen()