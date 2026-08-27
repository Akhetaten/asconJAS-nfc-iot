#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "xtimer.h"
#include "trust.h"
#include "crypto.h"

#define LOOP_DELAY_US 500000  

int main(void) {
    printf("[Node] Nefertiti booting...\n");

    int msg_count = 0;

    while (true) {
        printf("[Node] Nefertiti: Initiator sending message #%d...\n", msg_count + 1);

        
        trust_run_initiator();

        printf("[Node] Nefertiti: Responder receiving message #%d...\n", msg_count + 1);
        trust_run_responder();

        msg_count++;
        xtimer_usleep(LOOP_DELAY_US); 
    }

    return 0;
}

