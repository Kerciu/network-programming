#ifndef DATAGRAM_H
#define DATAGRAM_H

#include <stdint.h>
#include <stdlib.h>

#define FIELD_LENGTH 20

typedef struct KeyValuePair {
    char name[FIELD_LENGTH + 1];
    char value[FIELD_LENGTH + 1];
} KeyValuePair;


int encode_datagram(KeyValuePair *pairs, uint16_t count, unsigned char *buffer, size_t size);

int decode_datagram(char *data, size_t len, uint16_t count);


#endif // DATAGRAM_H