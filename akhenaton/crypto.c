#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include "crypto.h"
#include "asconopt.h"  /

void crypto_encrypt(uint8_t *ct, size_t *ct_len,
                    const uint8_t *pt, size_t pt_len)
{
    printf("[Crypto] Encrypting message...\n");
    *ct_len = ascon_encrypt_wrapper(ct, pt, pt_len);
}

void crypto_decrypt(uint8_t *pt, size_t *pt_len,
                    const uint8_t *ct, size_t ct_len)
{
    printf("[Crypto] Decrypting message...\n");
    *pt_len = ascon_decrypt_wrapper(pt, ct, ct_len);
}

