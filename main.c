#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tools/signal_handler.h"
#include "internal/parser.h"
#include "internal/history.h"
#include "tools/bootstrap.h"

#define MAX_DIRECTORY_LENGTH 1024
#define MAX_COMMAND_LENGTH 1024

char currentDirectory[MAX_DIRECTORY_LENGTH] = "";

int main() {
    char buffer[MAX_COMMAND_LENGTH] = "";
    FILE *fp = popen("pwd", "r");

    // Check if popen succeeded before using the returned file pointer
    if (fp == NULL) {
        fprintf(stderr, "\033[0;31m✘ nsh: Failed to open pipe for reading the current directory: \n\033[0m");
        return EXIT_FAILURE;
    }

    // Check fgets for errors reading the current directory from the pipe
    if (fgets(currentDirectory, sizeof(currentDirectory), fp) == NULL) {
        fprintf(stderr, "\033[0;31m✘ nsh: Failed to read the current directory: \n\033[0m");
        pclose(fp);
        return EXIT_FAILURE;
    }

    // Remove the trailing newline character
    currentDirectory[strcspn(currentDirectory, "\n")] = 0;
    pclose(fp); // Always close the pipe to avoid resource leaks

    // Initialize all the utils needed for the shell to work properly
    bootstrap();

    // Main command loop
    while (!exitSignal) {
        printf("%s", "NebuShell-0.5$ ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            if (feof(stdin)) {
                printf("\nCtrl-D detected! Exiting...\n");
                break;
            }
            continue; // Ignore other errors and retry reading the command
        }

        // Remove the trailing newline from the input buffer
        buffer[strcspn(buffer, "\n")] = 0;

        // Exit condition
        if (strcmp(buffer, "exit") == 0) {
            break;
        } else {
            // Execute the command using command_tokenizer
            command_tokenizer(buffer, currentDirectory);
        }
    }

    printf("Closing NebuShell.\n");
    return EXIT_SUCCESS;
}
