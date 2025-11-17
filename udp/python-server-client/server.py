from params import ServerParams

from abc import ABC, abstractmethod
import dataclasses

BUFFER_SIZE = 1024


class Server(ABC):
    def __init__(self, params: ServerParams):
        self.params = dataclasses.astuple(params)
        self.host, self.port = self.params
        self.buffer_size = BUFFER_SIZE

    @abstractmethod
    def listen(self): ...
