#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "signal_handler.h"

volatile bool exitSignal = false;

static void signal_handler(int signal) {
    if (signal == SIGINT) {
        printf("\n Ctrl-C detected! Exiting...\n");
        exitSignal = true;
    }
}

int set_up_signal_handler() {
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        printf("✘ nsh: Unable to set signal handler.\n");
        return EXIT_FAILURE;
    }
}
