from params import ServerParams
from datagram import Datagram
from server import Server

import socket
import sys


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
                    msg = "OK"
                    print(f"[SERVER] Decoded: {decoded}")

                except Exception as e:
                    msg = "ERROR"
                    print(f"[SERVER] Error: {e}")

                response = Datagram.encode({"status": msg, "dg_size": f"{len(data)}"})

                self.socket.sendto(response, address)

        # context manager is used so socket don't have to be closed manually


if __name__ == "__main__":
    SERVER_HOST = "z53_udp_server_py"
    SERVER_PORT = 2137

    server = UDPServer(ServerParams(host=SERVER_HOST, port=SERVER_PORT))

    server.listen()
