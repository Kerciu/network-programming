#include "datagram.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define SERVER_HOST "127.0.0.1"
#define SERVER_PORT 2138


int main() {

    int socketfd;
    struct sockaddr_in server_addr;
    char buffer[MAX_BUFFER_SIZE];
    int recv_len;

    if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        fprintf(stderr, "Failed to open socket\n");
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_HOST);

    struct Datagram messages[3];

    messages[0].pair_count = 2;
    snprintf(messages[0].names[0], FIELD_LENGTH + 1, "%s", "name");
    snprintf(messages[0].values[0], FIELD_LENGTH + 1, "%s", "Kacper");
    snprintf(messages[0].names[1], FIELD_LENGTH + 1, "%s", "task");
    snprintf(messages[0].values[1], FIELD_LENGTH + 1, "%s", "UDP");

    messages[1].pair_count = 2;
    snprintf(messages[1].names[0], FIELD_LENGTH + 1, "%s", "city");
    snprintf(messages[1].values[0], FIELD_LENGTH + 1, "%s", "Warsaw");
    snprintf(messages[1].names[1], FIELD_LENGTH + 1, "%s", "value");
    snprintf(messages[1].values[1], FIELD_LENGTH + 1, "%s", "2137");

    messages[2].pair_count = 1;
    snprintf(messages[2].names[0], FIELD_LENGTH + 1, "%s", "hello");
    snprintf(messages[2].values[0], FIELD_LENGTH + 1, "%s", "world");

    for (int i = 0; i < 3; i++) {
        printf("\n[CLIENT] Sending message %d...\n", i + 1);

        int encoded_len = encode_datagram(&messages[i], buffer, MAX_BUFFER_SIZE);
        if (encoded_len < 0) {
            fprintf(stderr, "[CLIENT] Could not encode message %d\n", i + 1);
            continue;
        }

        if (sendto(socketfd, buffer, encoded_len, 0, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            fprintf(stderr, "[CLIENT] sendto did not succeed\n");
            continue;
        }

        struct sockaddr_in from_addr;
        socklen_t from_addr_len = sizeof(from_addr);
        recv_len = recvfrom(socketfd, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr*)&from_addr, &from_addr_len);

        if (recv_len < 0) {
            fprintf(stderr, "[CLIENT] recvfrom did not succeed\n");
        } 
        else {
            struct Datagram response_dg;
            if (decode_datagram(buffer, recv_len, &response_dg) == 0) {
                printf("[CLIENT] Got response from %s:%d\n", inet_ntoa(from_addr.sin_addr), ntohs(from_addr.sin_port));
                print_datagram(&response_dg);
            } else {
                fprintf(stderr, "[CLIENT] Could not get server response.\n");
            }
        }
    }

    printf("\n[CLIENT] Ended.\n");

    close(socketfd);
    return 0;
}