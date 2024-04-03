#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include "nebula_tools.h"
#include "../main.h"

void change_directory(char *path, char *currentDirectory) {
    if (path == NULL || path[0] == '\0') {
        path = getenv("HOME");
        if (path == NULL) {
            fprintf(stderr, "change_directory: HOME environment variable not set.\n");
            return;
        }
    }

    if (chdir(path) == 0) {
        if (getcwd(currentDirectory, 1024) == NULL) {
            perror("getcwd failed");
        }
    } else {
        perror("change_directory");
    }
}

void list_directory(char *currentDirectory) { // ls command
    struct dirent *entry;
    DIR *dp;
    int counter = 0;

    dp = opendir(currentDirectory);
    if (dp == NULL) {
        perror("opendir failed");
        return;
    }

    printf("┌ \e[1;93m%s\e[0m\n", currentDirectory);

    while ((entry = readdir(dp)) != NULL) {
        if (entry->d_name[0] != '.') {
            printf("├ %s\n", entry->d_name);
        }
        counter++;
    }

    printf("└ \e[1;92m%s\n\e[0m", "Directory listed successfully!");

    closedir(dp);
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
    char buffer[1024];

    if (gethostname(buffer, sizeof(buffer)) == 0) {
        printf("%s\n", buffer);
    } else {
        perror("gethostname");
    }
}

void clear() {

}

int closeShell() {
    printf("Closing NebuShell.\n");
    return EXIT_SUCCESS;
}