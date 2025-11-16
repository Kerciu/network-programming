import socket
import struct


class TCPServer:
    def __init__(self, host: str, port: int) -> None:
        self.host = host
        self.port = port
        self.sock = None
        print(f"[SERVER] Listening on {self.host}:{self.port}")

    def _read_bytes(self, conn: socket.socket, bytes_count: int) -> bytes:
        byte_blocks = []
        bytes_received = 0

        while bytes_received < bytes_count:
            bytes_left = bytes_count - bytes_received
            block = conn.recv(bytes_left)

            if not block:
                return b''

            byte_blocks.append(block)
            bytes_received += len(block)

        return b"".join(byte_blocks)

    def _handle_client(self, conn: socket.socket, addr: tuple[str, int]) -> None:
        print(f"[SERVER] Connected with {addr}.")

        try:
            nodes_bytes = self._read_bytes(conn, 4)

            if not nodes_bytes:
                print(f"[SERVER] Client {addr} disconnected.")
                return

            nodes_count = struct.unpack('!I', nodes_bytes)[0]
            print(f"[SERVER] Waiting for {nodes_count} nodes from {addr}...")

            for i in range(nodes_count):
                data1_bytes = self._read_bytes(conn, 2)
                if not data1_bytes:
                    print(f"[SERVER] Client {addr} disconnected during: (node {i}, data1).")
                    break
                data1 = struct.unpack('!h', data1_bytes)[0]

                data2_bytes = self._read_bytes(conn, 4)
                if not data2_bytes:
                    print(f"[SERVER] Client {addr} disconnected during: (node {i}, data2).")
                    break
                data2 = struct.unpack('!i', data2_bytes)[0]

                str_len_bytes = self._read_bytes(conn, 2)
                if not str_len_bytes:
                    print(f"[SERVER] Client {addr} disconnected during: (node {i}, str_len).")
                    break
                str_len = struct.unpack('!H', str_len_bytes)[0]

                str_data = ""
                if str_len > 0:
                    str_data_bytes = self._read_bytes(conn, str_len)
                    if not str_data_bytes:
                        print(f"[SERVER] Client {addr} disconnected early (node {i}, str_data).")
                        break
                    str_data = str_data_bytes.decode('utf-8')

                print(f'[SERVER] [Node: {i}]: data1={data1}, data2={data2}, str_data="{str_data}"')

            print(f"[SERVER] Processing {nodes_count} nodes complete.")

        except Exception as e:
            print(f"[SERVER] ERROR: {e}")

        finally:
            print(f"[SERVER] Closing connection with {addr}.")
            conn.close()

    def run(self) -> None:
        try:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            self.sock.bind((self.host, self.port))
            self.sock.listen()

            while True:
                try:
                    conn, addr = self.sock.accept()
                    self._handle_client(conn, addr)

                except Exception as e:
                    print(f"[SERVER] ERROR: {e}")

        except KeyboardInterrupt:
            print("[SERVER] Shutting down.")
        except Exception as e:
            print(f"[SERVER] ERROR: {e}")

        finally:
            if self.sock:
                self.sock.close()
            print("[SERVER] Server down.")


if __name__ == "__main__":
    HOST = "127.0.0.1"
    PORT = 12345

    server = TCPServer(HOST, PORT)
    server.run()