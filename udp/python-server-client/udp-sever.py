from params import ServerParams
from datagram import Datagram
from server import Server

import socket


TIMEOUT = 5


class UDPServer(Server):
    def listen(self) -> None:
        with socket.socket(
            family=socket.AF_INET, type=socket.SOCK_DGRAM
        ) as self.socket:

            self.socket.bind(self.params)

            print(f"[SERVER] Listening on {self.host}:{self.port}")

            while True:
                data, address = self.socket.recvfrom(self.buffer_size)

                print(f"[SERVER] Received datagram from {address}")

                try:
                    decoded = Datagram.decode(data)
                    print(f"[SERVER] Decoded: {decoded}")

                    response = Datagram.encode({"status": "OK"})

                except Exception as e:
                    print(f"[SERVER] Error: {e}")
                    response = Datagram.encode({"status": "ERROR"})

                self.socket.sendto(response, address)

        # context manager is used so socket don't have to be closed manually


if __name__ == "__main__":
    SERVER_HOST = "127.0.0.1"
    SERVER_PORT = 2137

    server = UDPServer(ServerParams(host=SERVER_HOST, port=SERVER_PORT))

    server.listen()
