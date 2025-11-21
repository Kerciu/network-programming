FROM gcc:4.9

WORKDIR /app

COPY . .

RUN gcc -o server_c_app udp-server.c -std=c11

CMD ["./server_c_app"]