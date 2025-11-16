#include "datagram.h"

#define SERVER_HOST "127.0.0.1"
#define SERVER_PORT 2137

int main() {
    // only for windows
    #ifdef _WIN32
        WSADATA wsaData;
        int wsa_result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (wsa_result != 0) {
            fprintf(stderr, "WSAStartup did not succeed: %d\n", wsa_result);
            return 1;
        }
    #endif

    SOCKET sock = INVALID_SOCKET;
    SOCKADDR_IN server_addr;
    char buffer[MAX_BUFFER_SIZE];
    int recv_len;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) {
        fprintf(stderr, "Could not create socket: %d\n",
            #ifdef _WIN32
                WSAGetLastError()
            #else
                errno
            #endif
        );
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_HOST);

    struct Datagram messages[3];

    messages[0].pair_count = 2;
    strcpy_s(messages[0].names[0], FIELD_LENGTH + 1, "name");
    strcpy_s(messages[0].values[0], FIELD_LENGTH + 1, "Kacper");
    strcpy_s(messages[0].names[1], FIELD_LENGTH + 1, "task");
    strcpy_s(messages[0].values[1], FIELD_LENGTH + 1, "UDP");

    messages[1].pair_count = 2;
    strcpy_s(messages[1].names[0], FIELD_LENGTH + 1, "city");
    strcpy_s(messages[1].values[0], FIELD_LENGTH + 1, "Warsaw");
    strcpy_s(messages[1].names[1], FIELD_LENGTH + 1, "value");
    strcpy_s(messages[1].values[1], FIELD_LENGTH + 1, "2137");

    messages[2].pair_count = 1;
    strcpy_s(messages[2].names[0], FIELD_LENGTH + 1, "hello");
    strcpy_s(messages[2].values[0], FIELD_LENGTH + 1, "world");

    for (int i = 0; i < 3; i++) {
        printf("\n[CLIENT] Sending message %d...\n", i + 1);

        int encoded_len = encode_datagram(&messages[i], buffer, MAX_BUFFER_SIZE);
        if (encoded_len < 0) {
            fprintf(stderr, "[CLIENT] Could not encode message %d\n", i + 1);
            continue;
        }

        if (sendto(sock, buffer, encoded_len, 0, (SOCKADDR*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
            fprintf(stderr, "[CLIENT] sendto did not succeed: %d\n",
                #ifdef _WIN32
                    WSAGetLastError()
                #else
                    errno
                #endif
            );
            continue;
        }

        SOCKADDR_IN from_addr;
        int from_len = sizeof(from_addr);
        recv_len = recvfrom(sock, buffer, MAX_BUFFER_SIZE, 0, (SOCKADDR*)&from_addr, &from_len);

        if (recv_len == SOCKET_ERROR) {
            fprintf(stderr, "[CLIENT] recvfrom did not succeed: %d\n",
                #ifdef _WIN32
                    WSAGetLastError()
                #else
                    errno
                #endif
            );
        } else {
            struct Datagram response_dg;
            if (decode_datagram(buffer, recv_len, &response_dg) == 0) {
                printf("[CLIENT] Got response:\n");
                print_datagram(&response_dg);
            } else {
                fprintf(stderr, "[CLIENT] Could not get server response.\n");
            }
        }
    }

    printf("\n[CLIENT] Ended.\n");

    closesocket(sock);
    #ifdef _WIN32
        WSACleanup();
    #endif
    return 0;
}