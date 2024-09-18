#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tools/signal_handler.h"
#include "internal/parser.h"
#include "internal/history.h"
#include "tools/bootstrap.h"

#define MAX_DIRECTORY_LENGTH 1024
#define MIN_BUFFER_LENGTH 32

char current_directory[MAX_DIRECTORY_LENGTH] = "";

int main() {
    char *buffer = malloc(sizeof(char) * MIN_BUFFER_LENGTH);
    size_t buffer_size = MIN_BUFFER_LENGTH;
    FILE *fp = popen("pwd", "r");

    // Check if popen succeeded before using the returned file pointer
    if (fp == NULL) {
        fprintf(stderr, "\033[0;31m✘ nsh: Failed to open pipe for reading the current directory: \n\033[0m");
        return EXIT_FAILURE;
    }

    // Check fgets for errors reading the current directory from the pipe
    if (fgets(current_directory, sizeof(current_directory), fp) == NULL) {
        fprintf(stderr, "\033[0;31m✘ nsh: Failed to read the current directory: \n\033[0m");
        pclose(fp);
        return EXIT_FAILURE;
    }

    // Remove the trailing newline character
    current_directory[strcspn(current_directory, "\n")] = 0;
    pclose(fp); // Always close the pipe to avoid resource leaks

    // Initialize all the utils needed for the shell to work properly
    //bootstrap();

    // Main command loop
    while (!exitSignal) {
        printf("%s", "NebuShell-0.5$ ");
        ssize_t characters = getline(&buffer, &buffer_size, stdin);
        if (characters == -1) {
            // Handle error or EOF
            free(buffer);
            return EXIT_FAILURE;
        }

        // Remove the trailing newline from the input buffer
        buffer[strcspn(buffer, "\n")] = 0;

        // Exit condition
        if (strcmp(buffer, "exit") == 0) {
            // Free memory after memory usage.
            free(buffer);
            return EXIT_SUCCESS;
        } else {
            // Execute the command using command_tokenizer
            command_tokenizer(buffer, current_directory);
        }
    }

    free(buffer);
    printf("Closing NebuShell.\n");
    return EXIT_SUCCESS;
}
