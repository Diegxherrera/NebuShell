#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "parser.h"
#include "nebula_tools.h"

//
// Created by diegxherrera on 29/3/24.
//

void command_tokenizer(char command[1024]) { // TODO: CT should return a characters 2D-array with command + arguments
    char* workingDirectory[1024];
    int start = 0;
    int end = strlen(command) - 1;

    while (command[start] != '\0' && isspace((unsigned char)command[start])) {
        start++;
    }

    while (end >= start && isspace((unsigned char)command[end])) {
        end--;
    }

    for (int i = start; i <= end; i++) {
        command[i - start] = command[i];
    }

    if (strcmp(command, "pwd") == 0) {
        print_working_directory(workingDirectory);
    } else if(strcmp(command, "ls") == 0) {
        list_directory(workingDirectory); //TODO: Code the list directory function.
    } else if (strcmp(command, "cd") == 0){
        change_directory(workingDirectory); // TODO: Replace with trimmed argument (Handle relative & absolute paths)
    } else {
        printf("\033[0;31m");
        printf("✘ nsh:");
        printf(" command not found: ");
        printf("%s", command);
        printf("\n\033[0m");
    }
}