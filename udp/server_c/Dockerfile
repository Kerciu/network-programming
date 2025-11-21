FROM gcc:11.1

WORKDIR /app

COPY server_c/ .
COPY datagram.h .

RUN gcc -o server_c_app udp-server.c

CMD ["./server_c_app"]