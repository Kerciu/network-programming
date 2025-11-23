FROM python:3.9

RUN apt-get update && apt-get install -y iproute2 && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

CMD ["python3", "-u", "udp_server_ipv6.py"]