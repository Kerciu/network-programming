FROM gcc:latest

RUN apt-get update && apt-get install -y iproute2 && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . /app

RUN gcc -o client client_ipv6.c

CMD ["./client"]