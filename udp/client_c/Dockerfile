FROM gcc:11.1

WORKDIR /app

COPY client_c/ .
COPY datagram.h .

RUN gcc -o client_c_app udp-client.c

CMD ["./client_c_app"]