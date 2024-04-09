#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tools/signal_handler.h"
#include "internal/parser.h"
#include "internal/history.h"

#define MAX_DIRECTORY_LENGTH 1024
#define MAX_COMMAND_LENGTH 1024

char currentDirectory[MAX_DIRECTORY_LENGTH] = "";

int main() {
    char buffer[1024] = "";
    FILE *fp = popen("pwd", "r");

    if (fgets(currentDirectory, sizeof(currentDirectory), fp) == NULL) {
        printf("\033[0;31m✘ nsh: Failed to read the current directory: \n\033[0m");
        pclose(fp);
        return EXIT_FAILURE;
    }

    currentDirectory[strcspn(currentDirectory, "\n")] = 0;
    pclose(fp);
    setupSignalHandler();
    init_history();

    while (!exitSignal) {
        printf("%s", "NebuShell-0.3$ ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            if (feof(stdin)) {
                printf("\nCtrl-D detected! Exiting...\n");
                break;
            }
            continue;
        }

        buffer[strcspn(buffer, "\n")] = 0;

        if (strcmp(buffer, "exit") == 0) {
            break;
        } else {
            command_tokenizer(buffer, currentDirectory);
            add_to_history(buffer);
        }
    }

    printf("Closing NebuShell.\n");
    return EXIT_SUCCESS;
}
