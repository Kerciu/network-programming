from typing import List, Dict
from params import ServerParams
from datagram import Datagram
import socket


class UDPClient:

    def __init__(self, params: ServerParams) -> None:
        self.BUFFER_SIZE = 1024
        self.sock = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
        self.address = params.host
        self.port = params.port

    def send(self, messages: List[Dict[str, str]]) -> None:
        for msg in messages:
            encoded = Datagram.encode(msg)
            self.sock.sendto(encoded, (self.address, self.port))

            response, _ = self.sock.recvfrom(self.BUFFER_SIZE)
            decoded = Datagram.decode(response)

            print(f"[CLIENT] Response: {decoded}")

        print("[CLIENT] Done.")


if __name__ == "__main__":
    SERVER_HOST = "127.0.0.1"
    SERVER_PORT = 2138

    server = UDPClient(ServerParams(host=SERVER_HOST, port=SERVER_PORT))

    messages = [
        {"name": "Kacper", "task": "UDP"},
        {"city": "Warsaw", "value": "2137"},
        {"hello": "world"},
    ]

    server.send(messages)
