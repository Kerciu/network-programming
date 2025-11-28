#include "datagram.h"

#include <sys/socket.h>
#include <unistd.h>

#define SERVER_HOST "127.0.0.1"
#define SERVER_PORT 2137


int main() {

    int socketfd;
    struct sockaddr_in server_addr;
    char buffer[MAX_BUFFER_SIZE];

    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "Failed to open socket\n");
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_HOST);

    if (connect(socketfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        fprintf(stderr, "Failed to connect\n");
        return 1;
    }

    struct Datagram d3 = { 30, 300000, "Node 3 (tail)", NULL };
    struct Datagram d2 = { 20, 200000, "Node 2", &d3 };
    struct Datagram d1 = { 10, 100000, "Node 1 (head)", &d2 };

    int count = 3;
    printf("[CLIENT] Sending list of %d elements...\n", count);

    int count_net = htonl(count);
    if (write(socketfd, &count_net, 4) < 0) {
        fprintf(stderr, "[CLIENT] Sending failed\n");
    }

    struct Datagram* curr = &d1;
    int i = 0;

    while (curr != NULL) {
        printf("\n[CLIENT] Sending message %d...\n", i + 1);

        int encoded_len = encode_datagram(curr, buffer, MAX_BUFFER_SIZE);
        if (encoded_len < 0) {
            fprintf(stderr, "[CLIENT] Could not encode message %d\n", i + 1);
            break;
        }

        if (write(socketfd, buffer, encoded_len) < 0) {
            fprintf(stderr, "[CLIENT] Write failed\n");
            break;
        }

        curr = curr->next;
        i++;
    }

    printf("\n[CLIENT] Ended.\n");

    close(socketfd);
    return 0;
}