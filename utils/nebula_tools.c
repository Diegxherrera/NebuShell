#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "nebula_tools.h"

//
// Created by diegxherrera on 30/3/24.
//

bool change_directory(char *path[1024]) { // cd command
    if (chdir(path) == -1) {
        perror("chdir failed");
        return false;
    } else {
        return true;
    }
}

bool list_directory(char *path[1024]) { // ls command
    return true;
}

bool print_working_directory(char *path[1024]) { // pwd command
    char buffer[1024];
    if (strcmp(buffer, "") != 0 ) {
        printf("%s\n", getcwd(buffer, 100));
        return true;
    } else {
        return false;
    }
}