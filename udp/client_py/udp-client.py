from typing import List, Dict
from params import ServerParams
from datagram import Datagram

import socket

IP_MTU_DISCOVER = 10
IP_PMTUDISC_DO = 2  # forces no dg fragmentation


class UDPClient:

    def __init__(self, params: ServerParams) -> None:
        self.BUFFER_SIZE = Datagram.BUFFER_SIZE
        self.address = params.host
        self.port = params.port

    def send(
        self, messages: List[Dict[str, str]], check_overflow: bool = False
    ) -> None:
        with socket.socket(
            family=socket.AF_INET, type=socket.SOCK_DGRAM
        ) as self.socket:
            self.socket.setsockopt(socket.IPPROTO_IP, IP_MTU_DISCOVER, IP_PMTUDISC_DO)
            last_msg_len = 0
            for msg in messages:
                try:
                    encoded = Datagram.encode(msg)
                    msg_len = len(encoded)
                    self.socket.sendto(encoded, (self.address, self.port))

                    response, _ = self.socket.recvfrom(self.BUFFER_SIZE)
                    decoded = Datagram.decode(response)

                    print(f"[CLIENT] Response: {decoded}")

                except Exception as e:
                    print("========================================================")
                    print(f"Communication MTP_ERROR occourred\n")
                    print("========================================================")
                    if check_overflow:
                        self._find_max_capacity(last_msg_len, msg_len)
                    break

                last_msg_len = msg_len

        print("[CLIENT] Done.")

    def _find_max_capacity(self, last_ok_len, first_fail_len):

        print("\n[CLIENT] Starts finding bridge MTP limit\n")

        while first_fail_len - last_ok_len > 1:
            middle_len = (last_ok_len + first_fail_len) // 2
            bytes_to_encode = middle_len - 2

            if bytes_to_encode <= 0:
                first_fail_len = middle_len
                continue

            pairs = bytes_to_encode // (2 * Datagram.FIELDS_LENGTH)
            if bytes_to_encode % (2 * Datagram.FIELDS_LENGTH):
                pairs += 1

            dg = f"{pairs}" + "a" * bytes_to_encode
            dg = dg.encode("ascii")
            print("Datagram size: %s" % len(dg))

            try:
                self.socket.sendto(dg, (self.address, self.port))

                response, _ = self.socket.recvfrom(self.BUFFER_SIZE)
                decoded = Datagram.decode(response)
            except Exception as e:
                print(f"[CLIENT] Communication MTP_ERROR occourred - datagram too long")
                decoded["status"] = "MTP_ERROR"

            print(f"Communication status: {decoded["status"]}")

            if decoded.get("status") == "MTP_ERROR":
                first_fail_len = middle_len
            else:
                last_ok_len = middle_len

        print(f"Max server capacity is {last_ok_len} bytes")


if __name__ == "__main__":
    SERVER_HOST = "z53_udp_server_c"
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
    for counter in range(100):
        data = {}
        for i in range(counter):
            data[f"msg_{counter}_{i}"] = "a" * 20
        datagrams.append(data)
    server.send(messages=datagrams, check_overflow=True)
