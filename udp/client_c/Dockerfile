FROM gcc:4.9

WORKDIR /app

COPY client_c/ .
COPY datagram.h .

RUN gcc -o client_c_app udp-client.c -std=c11

CMD ["./client_c_app"]