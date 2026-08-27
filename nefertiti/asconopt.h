#ifndef ASCONOPT_H
#define ASCONOPT_H

#include <stddef.h>
#include <stdint.h>

int ascon_encrypt_wrapper(uint8_t *out, const uint8_t *in, size_t len);
int ascon_decrypt_wrapper(uint8_t *out, const uint8_t *in, size_t len);

#endif
