from params import ServerParams
import socket


class UDPClient():

    def __init__(self, params: ServerParams):
        self.BUFFER_SIZE = 1024
        self.sock = socket.socket(
            family=socket.AF_INET, type=socket.SOCK_DGRAM
        )
        self.address = params.host
        self.port = params.port

    def send(self, message):
        self.sock.sendto(
            self._build_message(message),
            (self.address, self.port)
        )

        msg, _ = self.sock.recvfrom(self.BUFFER_SIZE)
        print(f"Message from server: {msg}")

    def _build_message(self, message) -> bytes:
        return str.encode(
            message
        )


if __name__ == '__main__':
    SERVER_HOST = "127.0.0.1"
    SERVER_PORT = 2137

    server = UDPClient(
        ServerParams(
            host=SERVER_HOST,
            port=SERVER_PORT
        )
    )

    server.send("Hello from UDP Client !!!")
