#include "datagram.h"

#include <sys/socket.h>
#include <unistd.h>

#define SERVER_HOST "127.0.0.1"
#define SERVER_PORT 2138

int main() {
    
    int socketfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[MAX_BUFFER_SIZE];
    int recv_len;

    if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        fprintf(stderr, "Error opening socket\n");
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_HOST);   // can be used gethostbyname() logic as well

    if (bind(socketfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        fprintf(stderr, "Bind did not succeed\n");
        close(socketfd);
        return 1;
    }

    printf("[SERVER] Listening on - %s:%d \n", SERVER_HOST, SERVER_PORT);

    while (1) {
        recv_len = recvfrom(socketfd, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &client_addr_len);
        printf("Received buffer size: %zu", sizeof(recv_len));

        if (recv_len < 0) {
            fprintf(stderr, "Could not recive datagram\n");
            continue;
        }

        printf("[SERVER] Got datagram from %s:%d\n",
            inet_ntoa(client_addr.sin_addr),
            ntohs(client_addr.sin_port)
        );

        struct Datagram received_dg;
        char response_buffer[MAX_BUFFER_SIZE];
        struct Datagram response_dg;
        int response_len;

        char *msg;
        //hotfix for datagram decode needed as datagram above 1024 have OK status
        if (decode_datagram(buffer, recv_len, &received_dg) == 0) {
            printf("[SERVER] Decoded successfully:\n");
            print_datagram(&received_dg);
            msg = "OK";

        } else {
            fprintf(stderr, "[SERVER] Error while decoding datagram.\n");
            msg = "ERROR";
        }
        response_dg.pair_count = 2;
        snprintf(response_dg.names[0], FIELD_LENGTH + 1, "%s", "status");
        snprintf(response_dg.values[0], FIELD_LENGTH + 1, "%s", msg);
        snprintf(response_dg.names[1], FIELD_LENGTH + 1, "%s", "dg_size");
        snprintf(response_dg.values[1], FIELD_LENGTH + 1, "%d", recv_len);

        response_len = encode_datagram(&response_dg, response_buffer, MAX_BUFFER_SIZE);
        if (response_len > 0) {
            sendto(socketfd, response_buffer, response_len, 0, (struct sockaddr*)&client_addr, client_addr_len);
        }
    }

    close(socketfd);
    return 0;
}