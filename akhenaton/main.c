#include <stdio.h>
#include "trust.h"
#include "xtimer.h"

int main(void) {
    printf("[Node] Akhenaton booting...\n");

    while (1) {
        printf("[Node] Akhenaton: Initiator sending message...\n");
        trust_run_initiator();       

        xtimer_usleep(500000);       

        printf("[Node] Akhenaton: Responder receiving message...\n");
        trust_run_responder();      

        xtimer_usleep(500000);       
    }

    return 0;
}
