#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAX_NODES 10
#define MAX_STR_LEN 256
#define MAX_BUFFER_SIZE 65535

struct Node {
    short data1;
    int data2;
    char str_data[MAX_STR_LEN];
};

struct Datagram {
    unsigned int node_count;
    struct Node nodes[MAX_NODES];
};

static int encode_datagram(const struct Datagram* dg, char* buffer, size_t buffer_len) {
    char* ptr = buffer;
    size_t current_len = 0;

    if (buffer_len < 4) return -1;

    unsigned int count_net = htonl(dg->node_count);
    memcpy(ptr, &count_net, 4);
    ptr += 4;
    current_len += 4;

    for (unsigned int i = 0; i < dg->node_count; i++) {
        unsigned short str_len = (unsigned short)strlen(dg->nodes[i].str_data);
        size_t node_size = 2 + 4 + 2 + str_len; 

        if (current_len + node_size > buffer_len) return -1;

        short d1_net = htons(dg->nodes[i].data1);
        memcpy(ptr, &d1_net, 2);
        ptr += 2;

        int d2_net = htonl(dg->nodes[i].data2);
        memcpy(ptr, &d2_net, 4);
        ptr += 4;

        unsigned short sl_net = htons(str_len);
        memcpy(ptr, &sl_net, 2);
        ptr += 2;

        memcpy(ptr, dg->nodes[i].str_data, str_len);
        ptr += str_len;

        current_len += node_size;
    }

    return (int)current_len;
}