from dataclasses import dataclass


@dataclass
class ServerParams:
    host: str
    port: str