#ifndef CRYPTO_H
#define CRYPTO_H

#include <stdint.h>
#include <stddef.h>

void crypto_encrypt(uint8_t *ct, size_t *ct_len,
                    const uint8_t *pt, size_t pt_len);

void crypto_decrypt(uint8_t *pt, size_t *pt_len,
                    const uint8_t *ct, size_t ct_len);

#endif
