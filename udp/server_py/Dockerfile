FROM python:3.9

WORKDIR /app

COPY server_py/ .
COPY datagram.py .
COPY params.py .

CMD ["python3", "udp-server.py"]