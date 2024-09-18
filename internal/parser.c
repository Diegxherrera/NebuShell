#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "parser.h"
#include "../tools/nebula_tools.h"
#include "bin.h"
#include "history.h"
#include "jobs.h"

#define MAX_ARGS 1024
#define MAX_PIPES 1024

bool illegal_characters_check(const char *str, const char illegal_chars[], int arrSize) {
    for (int i = 0; i < arrSize; i++) {
        if (strchr(str, illegal_chars[i]) != NULL) {
            return true;
        }
    }
    return false;
}

int command_tokenizer(char command[1024], char *currentDirectory) {
    // Initialize argument pointers and other variables
    char *args[MAX_ARGS] = {NULL};
    int argCount = 0;
    char illegal_characters[] = {
            ' ', '\t', '\n', '\r', // Whitespace and control characters
            '!', '"', '#', '$', '%', '&', // Special symbols used in shell
            '\'', '(', ')', '*', '+', ',', // More symbols
            '/', ':', ';', '=', '?', // And even more symbols
            '@', '[', '\\', ']', '^', '`', // Special paths or operations
            '{', '|', '}', '~' // Braces and common special characters
    };

    // Trim leading and trailing whitespace from the command
    char *trimmedCommand = command;
    while (isspace((unsigned char)*trimmedCommand)) {
        trimmedCommand++;
    }
    char *end = trimmedCommand + strlen(trimmedCommand) - 1;
    while (end > trimmedCommand && isspace((unsigned char)*end)) {
        end--;
    }
    *(end + 1) = '\0';

    // Tokenize the command using '|' as the delimiter
    char *pipeCommands[MAX_PIPES] = {NULL};
    int pipeCount = 0;
    char *pipeToken = strtok(trimmedCommand, "|");
    while (pipeToken != NULL && pipeCount < MAX_PIPES - 1) {
        // Trim leading and trailing whitespace from each pipeToken
        while (isspace((unsigned char)*pipeToken)) {
            pipeToken++;
        }
        char *pipeEnd = pipeToken + strlen(pipeToken) - 1;
        while (pipeEnd > pipeToken && isspace((unsigned char)*pipeEnd)) {
            pipeEnd--;
        }
        *(pipeEnd + 1) = '\0';

        pipeCommands[pipeCount++] = pipeToken;
        pipeToken = strtok(NULL, "|");
    }

    if (pipeToken != NULL) {
        printf("\033[0;31m✘ nsh: Too many pipe commands. Only the first %d will be used.\n\033[0m", MAX_PIPES);
    }

    // Process each command sequentially
    for (int i = 0; i < pipeCount; i++) {
        char *currentCommand = pipeCommands[i];

        // Tokenize the current command and extract the main command
        char *cmd = strtok(currentCommand, " ");
        if (!cmd) continue;

        add_to_history(cmd); // Add the command to history after trimming

        // Reset arguments for the current command
        argCount = 0;
        memset(args, 0, sizeof(args));

        // Extract arguments safely and prevent out-of-bounds errors
        char *token;
        while ((token = strtok(NULL, " ")) != NULL) {
            if (argCount < MAX_ARGS - 1) {
                args[argCount++] = token;
            } else if (argCount >= MAX_ARGS){
                printf("\033[0;31m✘ nsh: Too many arguments. Only the first %d will be used.\n\033[0m", MAX_ARGS);
                break;
            }
        }


        char *argv[MAX_ARGS + 2]; // +2 for cmd and NULL terminator
        argv[0] = cmd;
        for (int j = 0; j < argCount; j++) {
            argv[j + 1] = args[j];
        }
        argv[argCount + 1] = NULL;

        // Call the appropriate function based on the command
        if (strcmp(cmd, "pwd") == 0) {
            print_working_directory();
        } else if (strcmp(cmd, "ls") == 0) {
            list_directory(currentDirectory, argCount > 0 ? args[0] : NULL);
        } else if (strcmp(cmd, "cd") == 0) {
            change_directory(args[0], currentDirectory);
        } else if (strcmp(cmd, "whoami") == 0) {
            who_am_i();
        } else if (strcmp(cmd, "hostname") == 0) {
            hostname();
        } else if (strcmp(cmd, "echo") == 0) {
            echo(argCount > 0 ? args[0] : NULL);
        } else if (strcmp(cmd, "history") == 0) {
            show_history(argCount > 0 ? args[0] : NULL);
        } else if (strcmp(cmd, "clear") == 0) {
            clear();
        } else if (strcmp(cmd, "help") == 0) {
            help_page(argCount > 0 ? args[0] : NULL, argCount > 1 ? args[1] : NULL);
        } else if (strcmp(cmd, "exit") == 0) {
            close_shell();
        } else if (strcmp(cmd, "env") == 0) {
            environment_variables(NULL);
        } else if (illegal_characters_check(cmd, illegal_characters, sizeof(illegal_characters))) {
            printf("\033[0;31m✘ nsh: illegal character found: %s%s\n\033[0m", cmd, ". For further information type help");
        } else if (is_a_binary(cmd) == EXIT_SUCCESS) {
            if (new_process(cmd, argv) != 0) {
                return EXIT_FAILURE;
            }

            return EXIT_SUCCESS;
        } else {
            printf("\033[0;31m✘ nsh: command not found: %s\n\033[0m", cmd);
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}