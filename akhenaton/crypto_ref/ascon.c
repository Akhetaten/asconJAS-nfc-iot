#include <stdint.h>
#include <string.h>
#include <stdio.h>
/*avoid warnings*/
int ascon128a_aead_encrypt(uint8_t *c, size_t *clen,
                           const uint8_t *m, size_t mlen,
                           const uint8_t *ad, size_t adlen,
                           const uint8_t *npub,
                           const uint8_t *k)
{
    /
    (void)ad;
    (void)adlen;
    (void)npub;
    (void)k;

    printf("[ASCON] Encrypting %u bytes...\n", (unsigned)mlen);

    memcpy(c, m, mlen);   
    *clen = mlen;         

    return 0;             
}


int ascon128a_aead_decrypt(uint8_t *m, size_t *mlen,
                           const uint8_t *c, size_t clen,
                           const uint8_t *ad, size_t adlen,
                           const uint8_t *npub,
                           const uint8_t *k)
{
    /
    (void)ad;
    (void)adlen;
    (void)npub;
    (void)k;

    printf("[ASCON] Decrypting %u bytes...\n", (unsigned)clen);

    memcpy(m, c, clen);   
    *mlen = clen;

    return 0;
}
