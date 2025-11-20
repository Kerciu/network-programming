FROM python:3.9-slim

RUN apt-get update && apt-get install -y iproute2 && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . /app

CMD ["python", "-u", "udp_server_ipv6.py"]