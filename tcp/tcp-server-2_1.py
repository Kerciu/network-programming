from params import ServerParams
from datagram import Datagram
from server import Server

import socket
import struct


class TCPServer(Server):
    def listen(self) -> None:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as self.socket:
            self.socket.bind(self.params)
            self.socket.listen()

            print(f"[SERVER] Listening on {self.host}:{self.port}")

            while True:
                conn, address = self.socket.accept()
                print(f"[SERVER] Connected by {address}")
                self.handle_client(conn)

    def handle_client(self, conn):
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
                    print(f"[SERVER] Decoded {i+1}: {decoded.val_s}, {decoded.val_i}, '{decoded.text}'")

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
    server = TCPServer(ServerParams(host=SERVER_HOST, port=SERVER_PORT))
    server.listen()