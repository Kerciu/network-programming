FROM gcc:4.9

WORKDIR /app

COPY . .

RUN gcc -o server_c udp-server.c -std=c11

CMD ["./server_c"]