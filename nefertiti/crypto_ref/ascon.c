#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "ascon.h"


void ascon128a_aead_encrypt(uint8_t *c, size_t *clen,
                            const uint8_t *m, size_t mlen,
                            const uint8_t *ad, size_t adlen,
                            const uint8_t *npub,
                            const uint8_t *k)
{

    (void)ad;
    (void)adlen;
    (void)npub;
    (void)k;


    memcpy(c, m, mlen);
    *clen = mlen;
}

int ascon128a_aead_decrypt(uint8_t *m, size_t *mlen,
                           const uint8_t *c, size_t clen,
                           const uint8_t *ad, size_t adlen,
                           const uint8_t *npub,
                           const uint8_t *k)
{
   
    (void)ad;
    (void)adlen;
    (void)npub;
    (void)k;

    
    memcpy(m, c, clen);
    *mlen = clen;

    return 0; // 0 = success
}
