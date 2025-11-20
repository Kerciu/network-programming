#include "datagram.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>

#define SERVER_PORT 2137
#define TIMEOUT_SEC 2

int main(int argc, char *argv[]) {
    int socketfd;
    struct sockaddr_in6 server_addr;
    char buffer[MAX_BUFFER_SIZE];
    int recv_len;
    struct timeval tv;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <server_ipv6>\n", argv[0]);
        return 1;
    }

    printf("[CLIENT] Waiting 5 seconds...\n");
    sleep(5);
    printf("[CLIENT] Starting...\n");

    if ((socketfd = socket(AF_INET6, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        return 1;
    }

    const char* server_ip = argv[1];

    tv.tv_sec = TIMEOUT_SEC;
    tv.tv_usec = 0;
    if (setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("setsockopt");
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET6, server_ip, &server_addr.sin6_addr) <= 0) {
        perror("inet_pton");
        return 1;
    }

    struct Datagram messages[3];

    messages[0].pair_count = 2;
    snprintf(messages[0].names[0], FIELD_LENGTH + 1, "%s", "name");
    snprintf(messages[0].values[0], FIELD_LENGTH + 1, "%s", "Kacper");
    snprintf(messages[0].names[1], FIELD_LENGTH + 1, "%s", "task");
    snprintf(messages[0].values[1], FIELD_LENGTH + 1, "%s", "UDP-IPv6");

    messages[1].pair_count = 2;
    snprintf(messages[1].names[0], FIELD_LENGTH + 1, "%s", "city");
    snprintf(messages[1].values[0], FIELD_LENGTH + 1, "%s", "Warsaw");
    snprintf(messages[1].names[1], FIELD_LENGTH + 1, "%s", "value");
    snprintf(messages[1].values[1], FIELD_LENGTH + 1, "%s", "2137");

    messages[2].pair_count = 1;
    snprintf(messages[2].names[0], FIELD_LENGTH + 1, "%s", "hello");
    snprintf(messages[2].values[0], FIELD_LENGTH + 1, "%s", "world");

    int seq_bit = 0;

    for (int i = 0; i < 3; i++) {
        printf("\n[CLIENT] Processing message %d (Seq: %d)...\n", i + 1, seq_bit);

        while (1) {
            buffer[0] = (char)seq_bit;

            int encoded_len = encode_datagram(&messages[i], buffer + 1, MAX_BUFFER_SIZE - 1);
            if (encoded_len < 0) {
                fprintf(stderr, "[CLIENT] Encoding error\n");
                break;
            }

            if (sendto(socketfd, buffer, encoded_len + 1, 0, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
                perror("sendto");
            } else {
                printf("[CLIENT] Sent packet, waiting for ACK...\n");
            }

            struct sockaddr_in6 from_addr;
            socklen_t from_addr_len = sizeof(from_addr);
            recv_len = recvfrom(socketfd, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr*)&from_addr, &from_addr_len);

            if (recv_len < 0) {
                if (errno == EWOULDBLOCK || errno == EAGAIN) {
                    printf("[CLIENT] Timeout. Retransmitting...\n");
                    continue;
                } else {
                    perror("recvfrom");
                    break;
                }
            }

            if (recv_len > 0) {
                int recv_seq = (unsigned char)buffer[0];
                if (recv_seq == seq_bit) {
                    printf("[CLIENT] ACK received for Seq %d\n", recv_seq);

                    struct Datagram response_dg;
                    if (decode_datagram(buffer + 1, recv_len - 1, &response_dg) == 0) {
                        print_datagram(&response_dg);
                    }
                    break;
                } else {
                    printf("[CLIENT] Wrong Seq (got %d, expected %d). Ignoring.\n", recv_seq, seq_bit);
                }
            }
        }
        seq_bit = 1 - seq_bit;
    }

    printf("\n[CLIENT] Ended.\n");
    close(socketfd);
    return 0;
}