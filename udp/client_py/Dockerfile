FROM python:3.9

WORKDIR /app

COPY client_py/ .
COPY datagram.py .
COPY params.py .

CMD ["python3", "udp-client.py"]