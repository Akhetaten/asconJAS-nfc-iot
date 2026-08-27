#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

//   CONFIG 
#define ASCON_ROUNDS 6
#define MAX_SIZE 1024
#define ITERATIONS 1000000  

typedef struct { uint64_t x[5]; } ascon_state_t;

//  PERMUTATION 
static inline void ascon_permute(ascon_state_t *s) {
    static const uint64_t RC[12] = {
        0xF0ULL, 0xE1ULL, 0xD2ULL, 0xC3ULL,
        0xB4ULL, 0xA5ULL, 0x96ULL, 0x87ULL,
        0x78ULL, 0x69ULL, 0x5AULL, 0x4BULL
    };
    for(int r = 6; r < 12; r++) { 
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
void ascon_encrypt(uint8_t *pt, uint8_t *ct, size_t len, uint8_t key[16], uint8_t nonce[16]) {
    ascon_state_t s = {0};
    memcpy(s.x, key, 16);
    memcpy(((uint8_t*)s.x)+16, nonce, len>16?16:len);

    ascon_permute(&s);

    for(size_t i=0;i<len;i++) ct[i] = pt[i] ^ ((uint8_t*)s.x)[i];

    ascon_permute(&s);
}

// DECRYPTION 
void ascon_decrypt(uint8_t *ct, uint8_t *pt, size_t len, uint8_t key[16], uint8_t nonce[16]) {
    ascon_state_t s = {0};
    memcpy(s.x, key, 16);
    memcpy(((uint8_t*)s.x)+16, nonce, len>16?16:len);

    ascon_permute(&s);

    for(size_t i=0;i<len;i++) pt[i] = ct[i] ^ ((uint8_t*)s.x)[i];

    ascon_permute(&s);
}

//BENCHMARK 
double benchmark_enc(uint8_t *pt, uint8_t *ct, size_t len, uint8_t key[16], uint8_t nonce[16], int iterations) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for(int i=0;i<iterations;i++) ascon_encrypt(pt, ct, len, key, nonce);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_sec = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/1e9;
    return time_sec;
}

double benchmark_dec(uint8_t *ct, uint8_t *pt, size_t len, uint8_t key[16], uint8_t nonce[16], int iterations) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for(int i=0;i<iterations;i++) ascon_decrypt(ct, pt, len, key, nonce);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_sec = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/1e9;
    return time_sec;
}

//  MAIN 
int main() {
    uint8_t key[16] = {0};
    uint8_t nonce[16] = {1};

    printf("Size(B),Enc_cyc_per_B,Dec_cyc_per_B\n");

    for(size_t size = 16; size <= MAX_SIZE; size *= 2) {
        uint8_t *pt = malloc(size); memset(pt, 0xAB, size);
        uint8_t *ct = malloc(size);
        uint8_t *dec = malloc(size);

        int iter = ITERATIONS;
        if(size > 256) iter = 100000; // fewer iterations for large sizes

        double enc_time = benchmark_enc(pt, ct, size, key, nonce, iter);
        double dec_time = benchmark_dec(ct, dec, size, key, nonce, iter);

        double cpu_freq_hz = 1e9; // 1GHz CPU assumption
        double enc_cycles_per_byte = (enc_time * cpu_freq_hz) / (size * iter);
        double dec_cycles_per_byte = (dec_time * cpu_freq_hz) / (size * iter);

        printf("%zu,%.2f,%.2f\n", size, enc_cycles_per_byte, dec_cycles_per_byte);

        free(pt); free(ct); free(dec);
    }

    return 0;
}
