#include "datagram.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>

#define SERVER_HOST "z53_udp_server_py"
#define SERVER_PORT "2137"


int main() {

    int socketfd;
    struct addrinfo hints, *res;
    char buffer[MAX_BUFFER_SIZE];
    int recv_len;

    if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        fprintf(stderr, "Failed to open socket\n");
        return 1;
    }

    int df = IP_PMTUDISC_DO;
    if (setsockopt(socketfd, IPPROTO_IP, IP_MTU_DISCOVER, &df, sizeof(df)) < 0) {
        fprintf(stderr, "Failed to disable datagram fragmentation\n");
        close(socketfd);
        return 1;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    int addr_info;

    if ((addr_info = getaddrinfo(SERVER_HOST, SERVER_PORT, &hints, &res)) ) {
        fprintf(stderr, "getaddrinfo(%s:%s): %s\n", SERVER_HOST, SERVER_PORT, gai_strerror(addr_info));
        close(socketfd);
        return 1;
    }
    printf("[CLIENT] Host address resolved...\n");

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

        if (sendto(socketfd, buffer, encoded_len, 0, res->ai_addr, res->ai_addrlen) < 0) {
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
    freeaddrinfo(res);
    return 0;
}