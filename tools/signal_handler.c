#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "signal_handler.h"

volatile bool exitSignal = false;

static void signal_handler(int signal) {
    if (signal == SIGINT) {
        printf("\nCtrl-C detected! Exiting...\n");
        exitSignal = true;
    }
}

void setupSignalHandler() {
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        printf("Error: Unable to set signal handler.\n");
        exit(EXIT_FAILURE);
    }
}
