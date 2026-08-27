#ifndef NFC_LINK_H
#define NFC_LINK_H

#include <stdint.h>
#include <stddef.h>

void nfc_init(void);
int nfc_send(const uint8_t *data, size_t len);
int nfc_recv(uint8_t *data, size_t len);

#endif
