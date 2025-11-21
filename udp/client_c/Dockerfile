FROM gcc:4.9

WORKDIR /app

COPY . .

RUN gcc -o client_c udp-client.c -std=c11

CMD ["./client_c"]