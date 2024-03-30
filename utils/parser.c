#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "parser.h"

//
// Created by diegxherrera on 29/3/24.
//

void commandTokenizer(char command[1024]) {
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

    if(strcmp(command,"") != 0){
        printf("\033[0;31m");
        printf("✘ nsh:");
        printf("\033[0;31m");
        printf(" command not found: ");
        printf("%s", command);
        printf("\n\033[0m");
    }
}