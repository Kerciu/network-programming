from params import ServerParams
from datagram import Datagram

from abc import ABC, abstractmethod
import dataclasses


class Server(ABC):
    def __init__(self, params: ServerParams):
        self.params = dataclasses.astuple(params)
        self.host, self.port = self.params
        self.buffer_size = Datagram.BUFFER_SIZE

    @abstractmethod
    def listen(self): ...
