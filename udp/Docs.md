# Sprawozdanie

## Server-Client use:
- python:
```
python3 python-server-client udp-server.py
python3 python-server-client udp-client.py
```

- c:
```
[instead of clang can be used other compiler like gcc]
clang c-server-client udp-server.c -o server
./server

clang c-server-client udp-client.c -o client
./client
```

## Dictionary:
`socketfd` - socket file descriptor

`htons` - [h]ost [to] [n]etwork [s]tructure

`ntohs` - [n]etwork [to] [h]ost [s]tructure

`inet_ntoa` - inet network to address cast

### Wykorzystane źródła:
* https://www.linuxhowtos.org/C_C++/socket.htm
* https://www.geeksforgeeks.org/c/socket-programming-cc/
* https://man7.org/linux/man-pages/man7/socket.7.html
* https://docs.python.org/3/library/socket.html
* https://docs.python.org/3/library/struct.html
* przykładowy kod zamieszczony jako dodatek do wykładów