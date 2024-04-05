#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "parser.h"
#include "../utils/nebula_tools.h"
#include "bin.h"

#define MAX_ARGS 1024

void command_tokenizer(char command[1024], char *currentDirectory) {
    char *args[MAX_ARGS] = {NULL};
    int argCount = 0;

    // Trim leading and trailing whitespace from the command
    char *trimmedCommand = command;
    while (isspace((unsigned char)*trimmedCommand)) trimmedCommand++;
    char *end = trimmedCommand + strlen(trimmedCommand) - 1;
    while (end > trimmedCommand && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';

    // Look for a pipe symbol, ignoring spaces around it
    char *pipePos = strchr(trimmedCommand, '|');
    if (pipePos != NULL) {
        // Split the command at the pipe, handling spaces
        *pipePos = '\0';
        char *commandBeforePipe = trimmedCommand;
        char *commandAfterPipe = pipePos + 1;
        while (isspace((unsigned char)*commandAfterPipe)) commandAfterPipe++;

        command_tokenizer(commandBeforePipe, currentDirectory);
        command_tokenizer(commandAfterPipe, currentDirectory);
        return;
    }

    char *cmd = strtok(trimmedCommand, " ");
    if (!cmd) return;

    // Extract arguments
    char *token;
    while ((token = strtok(NULL, " ")) != NULL) {
        args[argCount++] = token;
        if (argCount >= MAX_ARGS) break;
    }

    // Call the appropriate function based on the command
    if (strcmp(cmd, "pwd") == 0) {
        print_working_directory();
    } else if (strcmp(cmd, "ls") == 0) {
        list_directory(currentDirectory, args[0] ? args[0] : "");
    } else if (strcmp(cmd, "cd") == 0) {
        change_directory(args[0], currentDirectory);
    } else if (strcmp(cmd, "whoami") == 0) {
        who_am_i();
    } else if (strcmp(cmd, "hostname") == 0) {
        hostname();
    } else if (strcmp(cmd, "echo") == 0) {
        echo(args[0]);
    } else if (strcmp(cmd, "test") == 0) {
        runBinary(args[0]);
    } else {
        printf("\033[0;31m✘ nsh: command not found: %s\n\033[0m", cmd);
    }
}
