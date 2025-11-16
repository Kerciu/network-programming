#include "datagram.h"

#define SERVER_HOST "127.0.0.1"
#define SERVER_PORT 2138

int main() {
    // only fur windows
    #ifdef _WIN32
        WSADATA wsaData;
        int wsa_result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (wsa_result != 0) {
            fprintf(stderr, "WSAStartup did not succeed: %d\n", wsa_result);
            return 1;
        }
    #endif

    SOCKET sock = INVALID_SOCKET;
    SOCKADDR_IN server_addr, client_addr;
    int client_addr_len = sizeof(client_addr);
    char buffer[MAX_BUFFER_SIZE];
    int recv_len;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) {
        fprintf(stderr, "Cannot create socket: %d\n",
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

    if (bind(sock, (SOCKADDR*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        fprintf(stderr, "Bind did not succeed: %d\n",
            #ifdef _WIN32
                WSAGetLastError()
            #else
                errno
            #endif
        );
        closesocket(sock);
        #ifdef _WIN32
            WSACleanup();
        #endif
        return 1;
    }

    printf("[SERVER] Listening on %s:%d\n", SERVER_HOST, SERVER_PORT);

    while (1) {
        recv_len = recvfrom(sock, buffer, MAX_BUFFER_SIZE, 0, (SOCKADDR*)&client_addr, &client_addr_len);

        if (recv_len == SOCKET_ERROR) {
            fprintf(stderr, "recvfrom could not succeed: %d\n",
                #ifdef _WIN32
                    WSAGetLastError()
                #else
                    errno
                #endif
            );
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

        if (decode_datagram(buffer, recv_len, &received_dg) == 0) {
            printf("[SERVER] Decoded successfully:\n");
            print_datagram(&received_dg);

            response_dg.pair_count = 1;
            strcpy_s(response_dg.names[0], FIELD_LENGTH + 1, "status");
            strcpy_s(response_dg.values[0], FIELD_LENGTH + 1, "OK");

        } else {
            fprintf(stderr, "[SERVER] Error while decoding datagram.\n");

            response_dg.pair_count = 1;
            strcpy_s(response_dg.names[0], FIELD_LENGTH + 1, "status");
            strcpy_s(response_dg.values[0], FIELD_LENGTH + 1, "ERROR");
        }

        response_len = encode_datagram(&response_dg, response_buffer, MAX_BUFFER_SIZE);
        if (response_len > 0) {
            sendto(sock, response_buffer, response_len, 0, (SOCKADDR*)&client_addr, client_addr_len);
        }
    }

    closesocket(sock);
    #ifdef _WIN32
        WSACleanup();
    #endif
    return 0;
}