FROM gcc:latest

RUN apt-get update && apt-get install -y iproute2 && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN gcc -o client client_ipv6.c -std=c11

CMD ["./client"]