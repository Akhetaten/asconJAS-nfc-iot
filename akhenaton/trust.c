#include "trust.h"
#include "crypto.h"
#include <stdio.h>

bool trust_run_initiator(void)
{
    printf("[Trust] Initiator starting...\n");

    uint8_t pt[] = "HelloSecure";
    uint8_t ct[64];
    size_t ct_len = 0;

    crypto_encrypt(ct, &ct_len, pt, sizeof(pt));

    printf("[Trust] Initiator: message encrypted (len=%d)\n", (int)ct_len);

    return true;
}

bool trust_run_responder(void)
{
    printf("[Trust] Responder waiting...\n");
    return true;
}
