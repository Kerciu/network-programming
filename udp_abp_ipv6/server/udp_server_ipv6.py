from params import ServerParams
from datagram import Datagram
from server import Server
import socket
import struct


class UDPServer(Server):
    def listen(self) -> None:
        with socket.socket(socket.AF_INET6, socket.SOCK_DGRAM) as self.socket:
            self.socket.bind(self.params)

            print(f"[SERVER] Listening on {self.host}:{self.port}")

            last_client_addr = None
            last_seq = -1
            last_response = None

            while True:
                data, address = self.socket.recvfrom(self.buffer_size + 1)

                print(f"[SERVER] Received datagram from {address}")

                if len(data) < 1:
                    continue

                seq = data[0]
                payload = data[1:]

                if address == last_client_addr and seq == last_seq:
                    print(f"[SERVER] seq duplicate: {seq}")
                    if last_response:
                        self.socket.sendto(last_response, address)
                    continue

                try:
                    decoded = Datagram.decode(payload)
                    msg = "OK"
                    print(f"[SERVER] Decoded: {decoded}")
                except Exception as e:
                    msg = "ERROR"
                    print(f"[SERVER] Error: {e}")

                response_payload = Datagram.encode({"status": msg, "dg_size": f"{len(data)}"})
                response = struct.pack("B", seq) + response_payload

                last_client_addr = address
                last_seq = seq
                last_response = response

                self.socket.sendto(response, address)



if __name__ == "__main__":
    SERVER_HOST = "::"
    SERVER_PORT = 2137

    server = UDPServer(ServerParams(host=SERVER_HOST, port=SERVER_PORT))

    server.listen()