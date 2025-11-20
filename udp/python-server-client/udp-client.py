from typing import List, Dict
from params import ServerParams
from datagram import Datagram

import socket


class UDPClient:

    def __init__(self, params: ServerParams) -> None:
        self.BUFFER_SIZE = 1024
        self.address = params.host
        self.port = params.port

    def send(
        self, messages: List[Dict[str, str]], check_overflow: bool = False
    ) -> None:
        with socket.socket(
            family=socket.AF_INET, type=socket.SOCK_DGRAM
        ) as self.socket:
            for msg in messages:
                try:
                    encoded = Datagram.encode(msg)
                    self.socket.sendto(encoded, (self.address, self.port))

                    response, _ = self.socket.recvfrom(self.BUFFER_SIZE)
                    decoded = Datagram.decode(response)

                    print(f"[CLIENT] Response: {decoded}")

                    if check_overflow and len(encoded) > int(decoded["dg_size"]):
                        print("=" * 50)
                        print(f"Server max capacity is {decoded['dg_size']}")
                        break

                except Exception as e:
                    print(f"ERROR: {e}")

        print("[CLIENT] Done.")


if __name__ == "__main__":
    SERVER_HOST = "127.0.0.1"
    SERVER_PORT = 2138

    server = UDPClient(ServerParams(host=SERVER_HOST, port=SERVER_PORT))

    print("Test ex 1.1 - sending datagrams over UDP\n")
    messages = [
        {"name": "Kacper", "task": "UDP"},
        {"city": "Warsaw", "value": "2137"},
        {"hello": "world"},
    ]

    server.send(messages)

    print("Test ex 1.2 - finding max server capacity\n")
    datagrams = []
    for counter in range(30):
        data = {}
        for i in range(counter):
            data[f"msg_{counter}_{i}"] = "a" * 20
        datagrams.append(data)
    server.send(messages=datagrams, check_overflow=True)
