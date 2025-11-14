from params import ServerParams
import socket


class UDPServer:

    def __init__(self, params: ServerParams):
        self.BUFFER_SIZE = 1024
        self.sock = socket.socket(
            family=socket.AF_INET, type=socket.SOCK_DGRAM
        )

        self.sock.bind((params.host, params.port))

        print(f"Server up and listening on {params.host}:{params.port}")

    def listen(self):
        while True:
            message, address = self.sock.recvfrom(self.BUFFER_SIZE)

            print(f"Message from client: {message}")
            print(f"Client IP address: {address}")

            self.sock.sendto(self._respond(), address)

    def _respond(self) -> bytes:
        bytes_to_send = str.encode(
            "This is response from UDP server in python"
        )
        return bytes_to_send


if __name__ == '__main__':
    SERVER_HOST = "127.0.0.1"
    SERVER_PORT = 2137

    server = UDPServer(
        ServerParams(
            host=SERVER_HOST,
            port=SERVER_PORT
        )
    ).listen()
