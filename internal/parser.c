#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parser.h"
#include "../tools/nebula_tools.h"
#include "bin.h"
#include "history.h"
#include "jobs.h"
#include "../tools/alias.h"

void parse_options(int argc, char *argv[], Options *opts, char **directory) {
    // Initialize options to 0 (off)
    opts->option_l = 0;
    opts->option_a = 0;
    opts->option_f = 0;
    *directory = NULL;

    // Loop through arguments
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            // Process options starting with '-'
            for (int j = 1; argv[i][j] != '\0'; j++) {
                switch (argv[i][j]) {
                    case 'l':
                        opts->option_l = 1;
                        break;
                    case 'a':
                        opts->option_a = 1;
                        break;
                    case 'f':
                        opts->option_f = 1;
                        break;
                    default:
                        printf("\033[0;31m✘ nsh: invalid option -- '%c'\n\033[0m", argv[i][j]);
                        exit(EXIT_FAILURE); // Exit on invalid option
                }
            }
        } else {
            // Assume it's the directory path
            *directory = argv[i];
        }
    }
}

bool illegal_characters_check(const char *str, const char illegal_chars[], int arrSize) {
    for (int i = 0; i < arrSize; i++) {
        if (strchr(str, illegal_chars[i]) != NULL) {
            return true;
        }
    }
    return false;
}

int command_tokenizer(char *command, char *currentDirectory) {
    // Add command to history
    add_to_history(command);

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

        // Custom tokenizer that respects quotes
        char *cmd = NULL;
        argCount = 0;
        memset(args, 0, sizeof(args));
        bool in_quotes = false;
        char quote_char = '\0';

        char *token_start = currentCommand;
        char *ptr = currentCommand;

        while (*ptr != '\0') {
            if (!in_quotes && (*ptr == '\'' || *ptr == '"')) {
                // Start of quoted string
                in_quotes = true;
                quote_char = *ptr;
                token_start = ptr + 1;
            } else if (in_quotes && *ptr == quote_char) {
                // End of quoted string
                in_quotes = false;
                *ptr = '\0';  // Null-terminate the quoted string
                args[argCount++] = token_start;
            } else if (!in_quotes && isspace((unsigned char)*ptr)) {
                // End of a token outside of quotes
                *ptr = '\0';  // Null-terminate the token
                if (token_start != ptr) {
                    args[argCount++] = token_start;
                }
                token_start = ptr + 1;
            }
            ptr++;
        }

        // Add last token if there's any remaining
        if (token_start != ptr && *token_start != '\0') {
            args[argCount++] = token_start;
        }

        // Check if we have a command
        if (argCount == 0) {
            continue;
        }

        // Set the command as the first argument
        cmd = args[0];

        // Prepare arguments for execution
        char *argv[MAX_ARGS + 2];
        argv[0] = cmd;
        for (int j = 1; j < argCount; j++) {
            argv[j] = args[j];
        }
        argv[argCount] = NULL;

        // Options structure
        Options opts;
        char *directory = NULL;
        parse_options(argCount, argv, &opts, &directory);

        // Call the appropriate function based on the command
        if (strcmp(cmd, "pwd") == 0) {
            print_working_directory();
        } else if (strcmp(cmd, "ls") == 0) {
            list_directory(argCount, argv, currentDirectory);
        } else if (strcmp(cmd, "cd") == 0) {
            change_directory(directory ? directory : currentDirectory, currentDirectory);
        } else if (strcmp(cmd, "whoami") == 0) {
            who_am_i();
        } else if (strcmp(cmd, "hostname") == 0) {
            hostname();
        } else if (strcmp(cmd, "echo") == 0) {
            echo(argCount > 1 ? argv[1] : NULL);
        } else if (strcmp(cmd, "history") == 0) {
            handle_history(cmd, argCount > 1 ? argv[1] : NULL, argCount > 2 ? argv[2] : NULL);
        } else if (strcmp(cmd, "clear") == 0) {
            clear();
        } else if (strcmp(cmd, "help") == 0) {
            help_page(argCount > 1 ? argv[1] : NULL, argCount > 2 ? argv[2] : NULL);
        } else if (strcmp(cmd, "exit") == 0) {
            close_shell();
        } else if (strcmp(cmd, "env") == 0) {
            environment_variables(NULL);
        } else if (strcmp(cmd, "alias") == 0 || strcmp(cmd, "unalias") == 0){
            handle_alias(argCount > 1 ? argv[1] : NULL, argCount > 2 ? argv[2] : NULL);
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