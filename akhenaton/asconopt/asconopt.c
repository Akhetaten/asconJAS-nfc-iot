#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// CONFIG 
#define ASCON_ROUNDS 6
#define MSG_SIZE 16

typedef struct { uint64_t x[5]; } ascon_state_t;

// PERMUTATION (optimized for 16B) 
static inline void ascon_permute(ascon_state_t *s) {
    static const uint64_t RC[12] = {
        0xF0ULL, 0xE1ULL, 0xD2ULL, 0xC3ULL,
        0xB4ULL, 0xA5ULL, 0x96ULL, 0x87ULL,
        0x78ULL, 0x69ULL, 0x5AULL, 0x4BULL
    };

    for(int r = 6; r < 12; r++) { // reduced rounds
        s->x[2] ^= RC[r];

        uint64_t t0 = s->x[0] ^ s->x[4];
        uint64_t t1 = s->x[1] ^ s->x[3];
        s->x[0] ^= t1; s->x[1] ^= t0;
        s->x[2] ^= t0; s->x[3] ^= t1;

        s->x[0] ^= (s->x[0] >> 19) ^ (s->x[0] << 45);
        s->x[1] ^= (s->x[1] >> 61) ^ (s->x[1] << 3);
        s->x[2] ^= (s->x[2] >> 1)  ^ (s->x[2] << 63);
        s->x[3] ^= (s->x[3] >> 10) ^ (s->x[3] << 54);
        s->x[4] ^= (s->x[4] >> 7)  ^ (s->x[4] << 57);
    }
}

// ENCRYPTION 
void ascon_encrypt_16B(uint8_t *pt, uint8_t *ct, uint8_t key[16], uint8_t nonce[16]) {
    ascon_state_t s = {0};
    memcpy(s.x, key, 16);
    memcpy(((uint8_t*)s.x)+16, nonce, MSG_SIZE);

    ascon_permute(&s);

    ct[0]  = pt[0]  ^ ((uint8_t*)s.x)[0];
    ct[1]  = pt[1]  ^ ((uint8_t*)s.x)[1];
    ct[2]  = pt[2]  ^ ((uint8_t*)s.x)[2];
    ct[3]  = pt[3]  ^ ((uint8_t*)s.x)[3];
    ct[4]  = pt[4]  ^ ((uint8_t*)s.x)[4];
    ct[5]  = pt[5]  ^ ((uint8_t*)s.x)[5];
    ct[6]  = pt[6]  ^ ((uint8_t*)s.x)[6];
    ct[7]  = pt[7]  ^ ((uint8_t*)s.x)[7];
    ct[8]  = pt[8]  ^ ((uint8_t*)s.x)[8];
    ct[9]  = pt[9]  ^ ((uint8_t*)s.x)[9];
    ct[10] = pt[10] ^ ((uint8_t*)s.x)[10];
    ct[11] = pt[11] ^ ((uint8_t*)s.x)[11];
    ct[12] = pt[12] ^ ((uint8_t*)s.x)[12];
    ct[13] = pt[13] ^ ((uint8_t*)s.x)[13];
    ct[14] = pt[14] ^ ((uint8_t*)s.x)[14];
    ct[15] = pt[15] ^ ((uint8_t*)s.x)[15];

    ascon_permute(&s);
}

//  DECRYPTION 
void ascon_decrypt_16B(uint8_t *ct, uint8_t *pt, uint8_t key[16], uint8_t nonce[16]) {
    ascon_state_t s = {0};
    memcpy(s.x, key, 16);
    memcpy(((uint8_t*)s.x)+16, nonce, MSG_SIZE);

    ascon_permute(&s);

    pt[0]  = ct[0]  ^ ((uint8_t*)s.x)[0];
    pt[1]  = ct[1]  ^ ((uint8_t*)s.x)[1];
    pt[2]  = ct[2]  ^ ((uint8_t*)s.x)[2];
    pt[3]  = ct[3]  ^ ((uint8_t*)s.x)[3];
    pt[4]  = ct[4]  ^ ((uint8_t*)s.x)[4];
    pt[5]  = ct[5]  ^ ((uint8_t*)s.x)[5];
    pt[6]  = ct[6]  ^ ((uint8_t*)s.x)[6];
    pt[7]  = ct[7]  ^ ((uint8_t*)s.x)[7];
    pt[8]  = ct[8]  ^ ((uint8_t*)s.x)[8];
    pt[9]  = ct[9]  ^ ((uint8_t*)s.x)[9];
    pt[10] = ct[10] ^ ((uint8_t*)s.x)[10];
    pt[11] = ct[11] ^ ((uint8_t*)s.x)[11];
    pt[12] = ct[12] ^ ((uint8_t*)s.x)[12];
    pt[13] = ct[13] ^ ((uint8_t*)s.x)[13];
    pt[14] = ct[14] ^ ((uint8_t*)s.x)[14];
    pt[15] = ct[15] ^ ((uint8_t*)s.x)[15];

    ascon_permute(&s);
}

// BENCHMARK 
int main() {
    uint8_t key[16] = {0};
    uint8_t nonce[16] = {1};
    uint8_t plaintext[MSG_SIZE] = "HELLO_NFC_TEST!";
    uint8_t ciphertext[MSG_SIZE];
    uint8_t decrypted[MSG_SIZE];

    clock_t start, end;
    double enc_cycles, dec_cycles;

    
    start = clock();
    for(int i=0;i<1000000;i++) ascon_encrypt_16B(plaintext, ciphertext, key, nonce);
    end = clock();
    enc_cycles = (double)(end - start) / CLOCKS_PER_SEC * 1e6; 
    
    printf("Encryption time (us for 1M runs): %.2f\n", enc_cycles);

   
    start = clock();
    for(int i=0;i<1000000;i++) ascon_decrypt_16B(ciphertext, decrypted, key, nonce);
    end = clock();
    dec_cycles = (double)(end - start) / CLOCKS_PER_SEC * 1e6;
    printf("Decryption time (us for 1M runs): %.2f\n", dec_cycles);

    printf("Plaintext : %s\n", plaintext);
    printf("Ciphertext: ");
    for(int i=0;i<MSG_SIZE;i++) printf("%02X ", ciphertext[i]);
    printf("\nDecrypted : %s\n", decrypted);

    return 0;
}
