from typing import List, Dict, Any
import struct


class Datagram:

    COUNT_FORMAT = "!I"
    NODE_FIXED_FORMAT = "!hiH"
    NODE_FIXED_SIZE = struct.calcsize(NODE_FIXED_FORMAT)
    BUFFER_SIZE = 65535

    @staticmethod
    def encode(nodes: List[Dict[str, Any]]) -> bytes:
        result = struct.pack(Datagram.COUNT_FORMAT, len(nodes))

        for node in nodes:
            data1 = int(node["data1"])
            data2 = int(node["data2"])
            str_data = str(node["str_data"]).encode("utf-8")
            str_len = len(str_data)

            result += struct.pack(
                Datagram.NODE_FIXED_FORMAT, data1, data2, str_len
            )
            result += str_data

        return result

    @staticmethod
    def decode(data: bytes) -> List[Dict[str, Any]]:
        if len(data) < 4:
            raise Exception("Datagram too short")

        offset = 0
        count = struct.unpack(Datagram.COUNT_FORMAT, data[offset : offset + 4])[0]
        offset += 4

        nodes = []

        for _ in range(count):
            if offset + Datagram.NODE_FIXED_SIZE > len(data):
                raise Exception("Incomplete node header")

            data1, data2, str_len = struct.unpack(
                Datagram.NODE_FIXED_FORMAT,
                data[offset : offset + Datagram.NODE_FIXED_SIZE],
            )
            offset += Datagram.NODE_FIXED_SIZE

            if offset + str_len > len(data):
                raise Exception("Incomplete string data")

            str_data = data[offset : offset + str_len].decode("utf-8")
            offset += str_len

            nodes.append({"data1": data1, "data2": data2, "str_data": str_data})

        return nodes