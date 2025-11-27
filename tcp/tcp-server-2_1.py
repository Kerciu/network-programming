from params import ServerParams
from datagram import Datagram
from server import Server

import socket
import sys


class TCPServer(Server):
    def listen(self) -> None:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as self.socket:
            self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            self.socket.bind(self.params)
            self.socket.listen()

            print(f"[SERVER] Listening on {self.host}:{self.port}")

            while True:
                conn, addr = self.socket.accept()
                print(f"[SERVER] Connected with {addr}")

                with conn:
                    try:
                        data = conn.recv(self.buffer_size)
                        if not data:
                            continue

                        decoded = Datagram.decode(data)
                        print(f"[SERVER] Received {len(decoded)} nodes from {addr}")
                        
                        for i, node in enumerate(decoded):
                            print(f"  - Node {i}: {node}")

                    except Exception as e:
                        print(f"[SERVER] Error: {e}")
                
                print(f"[SERVER] Connection with {addr} closed")


if __name__ == "__main__":
    SERVER_HOST = "127.0.0.1"
    SERVER_PORT = 12345

    server = TCPServer(ServerParams(host=SERVER_HOST, port=SERVER_PORT))
    server.listen()