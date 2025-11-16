from params import ServerParams
from datagram import Datagram
import socket


class UDPServer:

    def __init__(self, params: ServerParams) -> None:
        self.BUFFER_SIZE = 1024
        self.sock = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)

        self.sock.bind((params.host, params.port))

        print(f"[SERVER] Listening on {params.host}:{params.port}")

    def listen(self) -> None:
        while True:
            data, address = self.sock.recvfrom(self.BUFFER_SIZE)

            print(f"[SERVER] Received datagram from {address}")

            try:
                decoded = Datagram.decode(data)
                print(f"[SERVER] Decoded: {decoded}")

                response = Datagram.encode({"status": "OK"})
            except Exception as e:
                print(f"[SERVER] Error: {e}")
                response = Datagram.encode({"status": "ERROR"})

            self.sock.sendto(response, address)


if __name__ == "__main__":
    SERVER_HOST = "127.0.0.1"
    SERVER_PORT = 2137

    server = UDPServer(ServerParams(host=SERVER_HOST, port=SERVER_PORT))

    server.listen()
