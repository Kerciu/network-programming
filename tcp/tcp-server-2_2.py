from params import ServerParams
from datagram import Datagram
from server import Server

import threading
import socket
import time


class TCPServer(Server):
    def __init__(self, params: ServerParams):
        super().__init__(params)
        self.threads = []

    def handle_client(self, conn: socket.socket, addr: tuple) -> None:
        print(f"[SERVER] Connected with {addr}")
        print(f"[SERVER] 5 sec delay for testing...")
        time.sleep(5)
        with conn:
            try:
                data = conn.recv(self.buffer_size)
                if data:
                    decoded = Datagram.decode(data)
                    print(f"[SERVER] Received {len(decoded)} nodes from {addr}")
                    
                    for i, node in enumerate(decoded):
                        print(f"  - Node {i}: {node}")

            except Exception as e:
                print(f"[SERVER] Error: {e}")
        
        print(f"[SERVER] Connection with {addr} closed")

    def listen(self) -> None:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as self.socket:
            self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            self.socket.bind(self.params)
            self.socket.listen()

            print(f"[SERVER] Listening on {self.host}:{self.port}")

            try:
                while True:
                    conn, addr = self.socket.accept()
                    
                    t = threading.Thread(target=self.handle_client, args=(conn, addr))
                    t.start()
                    
                    self.threads.append(t)
                    self.threads = [t for t in self.threads if t.is_alive()]

            except KeyboardInterrupt:
                print("\n[SERVER] Shutting down...")
            
            finally:
                for t in self.threads:
                    t.join()


if __name__ == "__main__":
    SERVER_HOST = "127.0.0.1"
    SERVER_PORT = 12345

    server = TCPServer(ServerParams(host=SERVER_HOST, port=SERVER_PORT))
    server.listen()