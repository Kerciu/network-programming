from typing import Dict
import struct


class Datagram:

    NETWORK_BIG_ENDIAN_FORMAT = "!hiI"
    HEADER_SIZE = 10
    BUFFER_SIZE = 65507

    def __init__(self, val_s, val_i, text):
        self.val_s = val_s
        self.val_i = val_i
        self.text = text

    @staticmethod
    def encode(dg: "Datagram") -> bytes:
        text_bytes = dg.text.encode("ascii")

        result = struct.pack(
            Datagram.NETWORK_BIG_ENDIAN_FORMAT, dg.val_s, dg.val_i, len(text_bytes)
        )

        result += text_bytes
        return result

    @staticmethod
    def decode(data: bytes) -> "Datagram":
        if len(data) < Datagram.HEADER_SIZE:
            raise Exception("Datagram too short")

        val_s, val_i, text_len = struct.unpack(
            Datagram.NETWORK_BIG_ENDIAN_FORMAT, data[: Datagram.HEADER_SIZE]
        )

        text = data[Datagram.HEADER_SIZE : Datagram.HEADER_SIZE + text_len].decode(
            "ascii"
        )

        return Datagram(val_s, val_i, text)
