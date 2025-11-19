#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define FIELD_LENGTH 20
#define MAX_PAIRS 50
#define MAX_BUFFER_SIZE 1024

#define error(msg) {perror(msg); exit(1);}


struct Datagram {
    unsigned short pair_count; // 2 bytes = short = uint16_t
    char names[MAX_PAIRS][FIELD_LENGTH + 1];    // one extra place for null terminator
    char values[MAX_PAIRS][FIELD_LENGTH + 1];
};

static int encode_datagram(const struct Datagram* dg, char* buffer, size_t buffer_len) {
    if (dg->pair_count > MAX_PAIRS) {
        // fprintf(stderr, "Error: Too much pairs to encode.\n");
        // return -1;
        error("Error: Too much pairs to encode.\n");
    }

    size_t required_len = 2 + (size_t)dg->pair_count * (2 * FIELD_LENGTH);
    if (buffer_len < required_len) {
        // fprintf(stderr, "Error: Buffer to small to encode.\n");
        // return -1;
        error("Error: Buffer to small to encode.\n")
    }

    unsigned short pair_count_net = htons(dg->pair_count);
    memcpy(buffer, &pair_count_net, 2);

    char* ptr = buffer + 2;

    for (int i = 0; i < dg->pair_count; i++) {
        memset(ptr, 0, FIELD_LENGTH);
        strncpy(ptr, dg->names[i], FIELD_LENGTH);
        ptr += FIELD_LENGTH;

        memset(ptr, 0, FIELD_LENGTH);
        strncpy(ptr, dg->values[i], FIELD_LENGTH);
        ptr += FIELD_LENGTH;
    }

    return (int)required_len;
}

static int decode_datagram(const char* buffer, size_t data_len, struct Datagram* dg) {
    if (data_len < 2) {
        // fprintf(stderr, "Errpr: Datagram too short (less than 2 bytes).\n");
        // return -1;
        error("Error: Datagram too short (less than 2 bytes).\n");
    }

    unsigned short pair_count_net;
    memcpy(&pair_count_net, buffer, 2);
    dg->pair_count = ntohs(pair_count_net);

    if (dg->pair_count > MAX_PAIRS) {
        fprintf(stderr, "Error: Datagram declares %d pairs (max %d).\n", dg->pair_count, MAX_PAIRS);
        return -1;
    }

    size_t expected_len = 2 + (size_t)dg->pair_count * (2 * FIELD_LENGTH);
    expected_len = expected_len <= MAX_BUFFER_SIZE ? expected_len : MAX_BUFFER_SIZE;
    if (data_len != expected_len) {
        fprintf(stderr, "Error: Invalid datagram len. Expected %zu, got %zu.\n", expected_len, data_len);
        return -1;
    }

    const char* ptr = buffer + 2;

    for (int i = 0; i < dg->pair_count; i++) {
        memcpy(dg->names[i], ptr, FIELD_LENGTH);
        dg->names[i][FIELD_LENGTH] = '\0';
        ptr += FIELD_LENGTH;

        memcpy(dg->values[i], ptr, FIELD_LENGTH);
        dg->values[i][FIELD_LENGTH] = '\0';
        ptr += FIELD_LENGTH;
    }

    return 0;
}

static void print_datagram(const struct Datagram* dg) {
    printf("Decoded datagram (%d pairs):\n", dg->pair_count);
    for (int i = 0; i < dg->pair_count; i++) {
        printf("  - '%s': '%s'\n", dg->names[i], dg->values[i]);
    }
}
