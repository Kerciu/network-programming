FROM gcc:4.9

WORKDIR /app

COPY . .

RUN gcc -o server_c udp-server.c -std=c11
RUN gcc -o client_c udp-client.c -std=c11

CMD ["/bin/bash"]