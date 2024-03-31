#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "nebula_tools.h"

//
// Created by diegxherrera on 30/3/24.
//

void change_directory(char path[1024][1024]) { // cd command
}

void list_directory(char *path[1024]) { // ls command
}

void print_working_directory(char *path[1024]) { // pwd command
    char buffer[1024];

    if (getcwd(buffer, sizeof(buffer)) == NULL) {
        perror("getcwd() error");
    }

    if (strcmp(buffer, "") != 0 ) {
        printf("%s\n", getcwd(buffer, 100));
    } else {

    }
}

void who_am_i() {
    char *username = getenv("USER");
    printf("Current user is: %s\n",username);
}

void hostname() {
    char hostname[1024];

    if (gethostname(hostname, sizeof(hostname)) == 0) {
        printf("%s\n", hostname);
    } else {
        perror("gethostname");
    }
}

void echo(char *content[1024][1024]) {
    printf("%s\n", content[0][0]);
}