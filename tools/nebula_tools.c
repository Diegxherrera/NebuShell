#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include "nebula_tools.h"

void change_directory(char *path, char *currentDirectory) { // cd command
    if (path == NULL || path[0] ==  '\0') {
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

void list_directory(char *currentDirectory, const char *args) {
    struct dirent *entry;
    DIR *dp = opendir(currentDirectory);
    int directories = 0;
    int regular_files = 0;
    int symbolic_links = 0;
    int local_sockets = 0;

    if (dp == NULL) {
        perror("opendir failed");
        return;
    }

    // Flag to indicate if the argument is recognized
    int recognized = 0;

    if (args == NULL || strcmp(args, "") == 0 || strcmp(args, "|") == 0) {
        recognized = 1;
        printf("┌ \e[1;93m%s\e[0m\n", currentDirectory);
        while ((entry = readdir(dp)) != NULL) {
            printf("├ %s\n", entry->d_name);
        }
    } else if (strstr(args, "-l") != NULL) {
        recognized = 1;
        printf("┌ \e[1;93m%s\e[0m\n", currentDirectory);
        while ((entry = readdir(dp)) != NULL) {
            switch (entry->d_type) {
                case DT_DIR:
                    printf("├ \e[1;34m%s\n\e[0m", entry->d_name); // Directory
                    directories++;
                    break;
                case DT_REG:
                    printf("├ %s\n", entry->d_name); // Regular file
                    regular_files++;
                    break;
                case DT_LNK:
                    printf("├ \e[1;36m%s\n\e[0m", entry->d_name); // Symbolic Link
                    symbolic_links++;
                    break;
                case DT_SOCK:
                    printf("├ \e[1;38m%s\n\e[0m", entry->d_name); // Local Domain Socket
                    local_sockets++;
                    break;
                default:
                    printf("├ %s\n", entry->d_name); // Other types without specific formatting
                    break;
            }
        }
    }

    // If the argument was not recognized, print a message
    if (!recognized) {
        printf("\033[0;31m✘ nsh: bad option: %s\n\033[0m", args);
    } else {
        printf("└ \e[1;92m%s\n\e[0m", "Directory listed successfully!");
    }

    closedir(dp);
}

void print_working_directory() { // pwd command
    char buffer[1024];

    if (getcwd(buffer, sizeof(buffer)) == NULL) {
        perror("getcwd() error");
    }

    if (strcmp(buffer, "") != 0 ) {
        printf("%s\n", getcwd(buffer, 100));
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

void echo(char *args) {
    char *end;

    if (args == NULL || args[0] == '\0') {
        printf("\n");
    } else {
        while(*args == '"' || *args == '\'') args++;
        end = args + strlen(args) - 1;
        while(end > args && *end == '"' || *end == '\'') end--;
        *(end + 1) = 0;

        printf("%s\n", args);
    }
}

void clear() {
    printf("\033[2J");
}

int closeShell() {
    printf("Closing NebuShell.\n");
    return EXIT_SUCCESS;
}