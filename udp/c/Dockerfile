FROM gcc:4.9

WORKDIR /app

COPY . .

RUN gcc -o server_c udp-server.c -std=c99
RUN gcc -o client_c udp-client.c -std=c99

CMD ["/bin/bash"]