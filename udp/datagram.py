from typing import Dict
import struct


class Datagram:

    NETWORK_BIG_ENDIAN_FORMAT = "!H"
    FIELDS_LENGTH = 20
    MAX_PAIRS = 99
    BUFFER_SIZE = 65507

    @staticmethod
    def encode(pairs: Dict[str, str]) -> bytes:
        result = struct.pack(
            Datagram.NETWORK_BIG_ENDIAN_FORMAT,
            len(pairs) if len(pairs) <= Datagram.MAX_PAIRS else Datagram.MAX_PAIRS,
        )

        for name, value in list(pairs.items())[: Datagram.MAX_PAIRS]:
            l_name = name.encode("ascii")[: Datagram.FIELDS_LENGTH].ljust(
                Datagram.FIELDS_LENGTH, b"\0"
            )

            l_value = value.encode("ascii")[: Datagram.FIELDS_LENGTH].ljust(
                Datagram.FIELDS_LENGTH, b"\0"
            )

            result += l_name + l_value

        return result

    @staticmethod
    def decode(data: bytes) -> Dict[str, str]:
        if len(data) < 2:
            raise Exception("Datagram too short - no data was stored")

        pairs_number = struct.unpack(Datagram.NETWORK_BIG_ENDIAN_FORMAT, data[:2])[0]

        if pairs_number > Datagram.MAX_PAIRS:
            raise Exception("Datagram too large - data stored not properly")

        expected_length = 2 + pairs_number * (2 * Datagram.FIELDS_LENGTH)

        if len(data) != expected_length:
            raise ValueError(
                f"Invalid datagram length {len(data)}, expected {expected_length}"
            )

        offset = 2
        pairs = {}

        for _ in range(pairs_number):
            name = (
                data[offset : offset + Datagram.FIELDS_LENGTH]
                .rstrip(b"\0")
                .decode("ascii")
            )
            offset += Datagram.FIELDS_LENGTH

            value = (
                data[offset : offset + Datagram.FIELDS_LENGTH]
                .rstrip(b"\0")
                .decode("ascii")
            )
            offset += Datagram.FIELDS_LENGTH

            pairs[name] = value

        return pairs
