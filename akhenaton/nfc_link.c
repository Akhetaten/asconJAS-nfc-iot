#include <stdio.h>
#include "nfc_link.h"

void nfc_init(void)
{

}

int nfc_send(const uint8_t *data, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        putchar(data[i]);
    }
    fflush(stdout);
    return (int)len;
}

int nfc_recv(uint8_t *data, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        int c = getchar();
        if (c == EOF) {
            return -1;
        }
        data[i] = (uint8_t)c;
    }
    return (int)len;
}

