#include <stdio.h>
#include <stdlib.h>
#include "bootstrap.h"
#include "history.h"
#include "../tools/alias.h"
#include "../tools/signal_handler.h"

int bootstrap() {
    int history_result = init_history();
    int signal_handler_result = set_up_signal_handler();
    int aliases_file_result = set_up_aliases();

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

    if (aliases_file_result != 0) {
        fprintf(stderr, "✘ nsh: Failed to initialize aliases. Error Code: 0003 %d\n", signal_handler_result);
        fflush(stdout);
        return aliases_file_result;
    }

    return EXIT_SUCCESS; // Success
}