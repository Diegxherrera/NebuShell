#include <stdio.h>
#include <stdlib.h>
#include "bootstrap.h"
#include "../internal/history.h"
#include "signal_handler.h"

int bootstrap() {
    int history_result = init_history();
    int signal_handler_result = set_up_signal_handler();
    int builtins_help_pages_result;

    if (history_result != 0) {
        fprintf(stderr, "✘ nsh: Failed to initialize history. Error Code: 0001%d\n", history_result);
        fflush(stdout);
        return history_result;
    }

    if (signal_handler_result != 0) {
        fprintf(stderr, "✘ nsh: Failed to initialize signal handler. Error Code: 0002 %d\n", signal_handler_result);
        fflush(stdout);
        return signal_handler_result;
    }

    return EXIT_SUCCESS; // Success
}