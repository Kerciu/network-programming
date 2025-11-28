#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAX_BUFFER_SIZE 65507
#define HEADER_SIZE 10

#define error(msg) {perror(msg); exit(1);}

struct Datagram {
    short val_s;
    int val_i;
    char* text;
    struct Datagram* next;
};

static int encode_datagram(const struct Datagram* dg, char* buffer, size_t buffer_len) {
    size_t text_len = strlen(dg->text);
    size_t required_len = HEADER_SIZE + text_len;

    if (buffer_len < required_len) {
        fprintf(stderr, "Error: Buffer too small\n");
        return -1;
    }

    char* ptr = buffer;

    short val_s_net = htons(dg->val_s);
    memcpy(ptr, &val_s_net, 2);
    ptr += 2;

    int val_i_net = htonl(dg->val_i);
    memcpy(ptr, &val_i_net, 4);
    ptr += 4;

    int text_len_net = htonl((int)text_len);
    memcpy(ptr, &text_len_net, 4);
    ptr += 4;

    memcpy(ptr, dg->text, text_len);

    return (int)required_len;
}
