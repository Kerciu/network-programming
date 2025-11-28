import dataclasses

@dataclasses.dataclass
class ServerParams:
    host: str
    port: int