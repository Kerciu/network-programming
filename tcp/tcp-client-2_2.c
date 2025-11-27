#include "datagram.h"

#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#define SERVER_HOST "127.0.0.1"
#define SERVER_PORT 12345


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
        fprintf(stderr, "Connection failed\n");
        close(socketfd);
        return 1;
    }

    printf("[CLIENT] Connected to server.\n");

    struct Datagram msg;
    msg.node_count = 1;
    
    msg.nodes[0].data1 = 123;
    msg.nodes[0].data2 = 2137;
    snprintf(msg.nodes[0].str_data, MAX_STR_LEN, "%s", "Hejj ;)");

    int encoded_len = encode_datagram(&msg, buffer, MAX_BUFFER_SIZE);
    
    if (encoded_len < 0) {
        fprintf(stderr, "[CLIENT] Encoding failed.\n");
        close(socketfd);
        return 1;
    }

    printf("[CLIENT] Sending %d bytes...\n", encoded_len);

    if (send(socketfd, buffer, encoded_len, 0) < 0) {
        fprintf(stderr, "[CLIENT] Send failed.\n");
    } else {
        printf("[CLIENT] Data sent successfully.\n");
    }

    close(socketfd);
    return 0;
}