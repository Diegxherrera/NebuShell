#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fnmatch.h>
#include "nebula_tools.h"
#include "../main.h"

//
// Created by diegxherrera on 30/3/24.
//

void change_directory(char *path, char *currentDirectory) {
    if (path == NULL || path[0] == '\0') {
        path = getenv("HOME");
        if (path == NULL) {
            fprintf(stderr, "change_directory: HOME environment variable not set.\n");
            return;
        }
    }

    if (chdir(path) == 0) {
        strncpy(currentDirectory, path, 1024);
        currentDirectory[1024 - 1] = '\0';
    } else {
        perror("change_directory");
    }
}

void list_directory() { // ls command
    struct dirent *entry;
    int count = 0;
    char *pattern = ".*";
    DIR *dp;

    dp = opendir(currentDirectory);

    if (dp == NULL) {
        printf("\033[0;31m✘ nsh: No such file or directory. \n\033[0m");
        return;
    }

    while ((entry = readdir(dp)) != NULL) {
        count++;
        if (fnmatch(pattern, entry->d_name, 0) != 0) {
            printf("%s\n", entry->d_name);
        }
    }

    closedir(dp);
    printf("\n");
}

void print_working_directory() { // pwd command
    char buffer[1024];

    if (getcwd(buffer, sizeof(buffer)) == NULL) {
        perror("getcwd() error");
    }

    if (strcmp(buffer, "") != 0 ) {
        printf("%s\n", getcwd(buffer, 100));
    } else {

    }
}

void who_am_i() { // whoami comaand
    char *username = getenv("USER");
    printf("%s\n",username);
}

void hostname() { // hostname command
    char hostname[1024];

    if (gethostname(hostname, sizeof(hostname)) == 0) {
        printf("%s\n", hostname);
    } else {
        perror("gethostname");
    }
}