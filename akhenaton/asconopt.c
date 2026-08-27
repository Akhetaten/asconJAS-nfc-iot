#include "asconopt.h"
#include <stdint.h>
#include <string.h>

///CONFIG 
#define ASCON_BLOCK_SIZE 16
#define ASCON_ROUNDS     6

typedef struct {
    uint64_t x[5];
} ascon_state_t;

//#  PERMUTATION
static inline void ascon_permute(ascon_state_t *s)
{
    static const uint64_t RC[12] = {
        0xF0ULL, 0xE1ULL, 0xD2ULL, 0xC3ULL,
        0xB4ULL, 0xA5ULL, 0x96ULL, 0x87ULL,
        0x78ULL, 0x69ULL, 0x5AULL, 0x4BULL
    };

    for (int r = 12 - ASCON_ROUNDS; r < 12; r++) {
        s->x[2] ^= RC[r];

        uint64_t t0 = s->x[0] ^ s->x[4];
        uint64_t t1 = s->x[1] ^ s->x[3];

        s->x[0] ^= t1;
        s->x[1] ^= t0;
        s->x[2] ^= t0;
        s->x[3] ^= t1;

        s->x[0] ^= (s->x[0] >> 19) ^ (s->x[0] << 45);
        s->x[1] ^= (s->x[1] >> 61) ^ (s->x[1] << 3);
        s->x[2] ^= (s->x[2] >> 1)  ^ (s->x[2] << 63);
        s->x[3] ^= (s->x[3] >> 10) ^ (s->x[3] << 54);
        s->x[4] ^= (s->x[4] >> 7)  ^ (s->x[4] << 57);
    }
}

//  CORE
static void ascon_encrypt_16B(uint8_t *pt, uint8_t *ct,
                              const uint8_t *key,
                              const uint8_t *nonce)
{
    ascon_state_t s = {0};

    memcpy(&s.x[0], key,   ASCON_BLOCK_SIZE);
    memcpy(((uint8_t*)s.x) + ASCON_BLOCK_SIZE, nonce, ASCON_BLOCK_SIZE);

    ascon_permute(&s);

    for (int i = 0; i < ASCON_BLOCK_SIZE; i++) {
        ct[i] = pt[i] ^ ((uint8_t*)s.x)[i];
    }
}

static void ascon_decrypt_16B(uint8_t *ct, uint8_t *pt,
                              const uint8_t *key,
                              const uint8_t *nonce)
{
    ascon_state_t s = {0};

    memcpy(&s.x[0], key,   ASCON_BLOCK_SIZE);
    memcpy(((uint8_t*)s.x) + ASCON_BLOCK_SIZE, nonce, ASCON_BLOCK_SIZE);

    ascon_permute(&s);

    for (int i = 0; i < ASCON_BLOCK_SIZE; i++) {
        pt[i] = ct[i] ^ ((uint8_t*)s.x)[i];
    }
}

// AEAD API
int ascon128a_aead_encrypt(uint8_t *c, size_t *clen,
                           const uint8_t *m, size_t mlen,
                           const uint8_t *ad, size_t adlen,
                           const uint8_t *npub,
                           const uint8_t *k)
{
    (void)ad;
    (void)adlen;

    if (mlen != ASCON_BLOCK_SIZE) {
        return -1;
    }

    ascon_encrypt_16B((uint8_t*)m, c, k, npub);
    *clen = ASCON_BLOCK_SIZE;
    return 0;
}

int ascon128a_aead_decrypt(uint8_t *m, size_t *mlen,
                           const uint8_t *c, size_t clen,
                           const uint8_t *ad, size_t adlen,
                           const uint8_t *npub,
                           const uint8_t *k)
{
    (void)ad;
    (void)adlen;

    if (clen != ASCON_BLOCK_SIZE) {
        return -1;
    }

    ascon_decrypt_16B((uint8_t*)c, m, k, npub);
    *mlen = ASCON_BLOCK_SIZE;
    return 0;
}


/* COMPATIBILITY WRAPPER  */
#include <stdint.h>
#include <stddef.h>

/* Fixed key and nonce for NFC experiment */
static const uint8_t fixed_key[16]   = {0};
static const uint8_t fixed_nonce[16] = {0};


int ascon_encrypt_wrapper(uint8_t *out, const uint8_t *in, size_t len)
{
    (void)len;  

    size_t clen = 0;
    if (ascon128a_aead_encrypt(out, &clen,
                               in, 16,   
                               NULL, 0,
                               fixed_nonce,
                               fixed_key) != 0) {
        return -1;
    }

    return 16;  
}

int ascon_decrypt_wrapper(uint8_t *out, const uint8_t *in, size_t len)
{
    (void)len;  

    size_t mlen = 0;
    if (ascon128a_aead_decrypt(out, &mlen,
                               in, 16,   
                               NULL, 0,
                               fixed_nonce,
                               fixed_key) != 0) {
        return -1;
    }

    return 16;  // always returns 16 bytes

