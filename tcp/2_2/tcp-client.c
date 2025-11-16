#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    typedef int SOCKET;
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define closesocket close
#endif

const char* SERVER_HOST = "127.0.0.1";
const int SERVER_PORT = 12345;

int main() {
    #ifdef _WIN32
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            fprintf(stderr, "[CLIENT] WSAStartup fail.\n");
            return 1;
        }
    #endif
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        fprintf(stderr, "[CLIENT] Socket creation fail.\n");
        return 1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_HOST);

    if (server_addr.sin_addr.s_addr == INADDR_NONE) {
        fprintf(stderr, "[CLIENT] ERROR: invalid address.");
        closesocket(sock);
        return 1;
    }

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        fprintf(stderr, "[CLIENT] Connection failed.\n");
        closesocket(sock);
        return 1;
    }

    printf("[CLIENT] Connected to server: %s:%d\n", SERVER_HOST, SERVER_PORT);

    char data[] = {
        // Node count
        0x00, 0x00, 0x00, 0x01,

        // data1
        0x00, 0x7B,

        // data2
        0x00, 0x00, 0x08, 0x59,

        // str_len
        0x00, 0x7,

        // str_data
        'H', 'e', 'j', 'j', ' ', ';', ')'
    };
    size_t data_size = sizeof(data);

    printf("[CLIENT] Sending %zu bytes to server...\n", data_size);

    if(send(sock, data, data_size, 0) == SOCKET_ERROR) {
        fprintf(stderr, "[CLIENT] ERROR: send failed.\n");
    }
    else {
        printf("[CLIENT] Data successfully sent to server.\n");
    }

    closesocket(sock);

    #ifdef _WIN32
        WSACleanup();
    #endif

    return 0;
}