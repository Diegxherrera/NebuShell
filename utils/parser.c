#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include "parser.h"
#include "nebula_tools.h"
#include "history.h"

#define MAX_ARGS 1024

void command_tokenizer(char command[1024], char *currentDirectory) {
    char *args[MAX_ARGS] = {NULL};
    int argCount = 0;

    char *cmd = strtok(command, " \t");
    if (!cmd) return;

    // Extract arguments
    char *token;
    while ((token = strtok(NULL, " \t")) != NULL) {
        args[argCount++] = token;
        if (argCount >= MAX_ARGS) break;
    }

    // Built-in shell commands
    if (strcmp(cmd, "pwd") == 0) {
        print_working_directory();
    } else if (strcmp(cmd, "ls") == 0) {
        list_directory(currentDirectory);
    } else if (strcmp(cmd, "cd") == 0) {
        change_directory(args[0], currentDirectory);
    } else if (strcmp(cmd, "whoami") == 0) {
        who_am_i();
    } else if (strcmp(cmd, "hostname") == 0) {
        hostname();
    } else if (strcmp(cmd, "history") == 0) {

    } else {
        // Command not found
        printf("\033[0;31m✘ nsh: command not found: %s\n\033[0m", cmd);
    }
}
