# Sprawozdanie

[0–1]   uint16 - num of pairs (N)
for every pair:
[2 + i*(20+20)]      20 ASCII bytes – name
[2 + i*(20+20)+20]   20 ASCII bytes – value

## Server-Client use:
* To run and test all services just use prepared docker-compose file 
```sh
docker-compose up --build
```

## Dictionary
* names are completly made up by Kuba and probably wrong but made me understand and remember functions :P

`socketfd` - socket file descriptor

`htons` - [h]ost [to] [n]etwork [s]tructure

`ntohs` - [n]etwork [to] [h]ost [s]tructure

`inet_ntoa` - inet network to address cast

`hints` - host internet structure ?

`getaddrinfo()` used instead of `gethostbyname2()` showed on tutorials as `gethostbyname2()` is deprecated


### Wykorzystane źródła:
* https://www.linuxhowtos.org/C_C++/socket.htm
* https://www.geeksforgeeks.org/c/socket-programming-cc/
* https://man7.org/linux/man-pages/man7/socket.7.html
* https://docs.python.org/3/library/socket.html
* https://docs.python.org/3/library/struct.html
* przykładowy kod zamieszczony jako dodatek do wykładów