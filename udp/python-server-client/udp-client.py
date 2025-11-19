from typing import List, Dict
from params import ServerParams
from datagram import Datagram

import socket
import sys


class UDPClient:

    def __init__(self, params: ServerParams) -> None:
        self.BUFFER_SIZE = 1024
        self.address = params.host
        self.port = params.port

    def send(self, messages: List[Dict[str, str]]) -> None:
        with socket.socket(
            family=socket.AF_INET, type=socket.SOCK_DGRAM
        ) as self.socket:
            for msg in messages:
                encoded = Datagram.encode(msg)
                self.socket.sendto(encoded, (self.address, self.port))

                response, _ = self.socket.recvfrom(self.BUFFER_SIZE)
                decoded = Datagram.decode(response)

                print(f"[CLIENT] Response: {decoded}")

        print("[CLIENT] Done.")

    def test_max_capacity(self):
        with socket.socket(
            family=socket.AF_INET, type=socket.SOCK_DGRAM
        ) as self.socket:
            for counter in range(100):
                data = {}
                for i in range(counter):
                    data[str(i)] = "a" * counter

                try:
                    encoded_msg = Datagram.encode(data)
                    print(f"datagram size: {len(encoded_msg)}")
                    self.socket.sendto(encoded_msg, (self.address, self.port))

                    response, _ = self.socket.recvfrom(self.BUFFER_SIZE)
                    decoded_msg = Datagram.decode(response)
                    print(f"Decoded response: {decoded_msg}")

                    print("=================================")
                    if len(encoded_msg) > int(decoded_msg["dg_size"]):
                        print(f"Server max capacity is {decoded_msg['dg_size']}")
                        break

                except Exception as e:
                    print(f"ERROR: {e}")


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

    server.test_max_capacity()
