#include <stdio.h>
#ifdef _WIN32
    #include <windows.h>
#elif defined(__APPLE__) && defined(__MACH__)
    #include <unistd.h>
#elif defined(__linux__)
    #include <unistd.h>
#endif
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include "nebula_tools.h"

void change_directory(char *path, char *currentDirectory) {
    if (path == NULL || path[0] == '\0') {
        fprintf(stderr, "change_directory: Path not specified or HOME environment variable not set.\n");
        return;
    }

    if (chdir(path) == 0) {
        if (getcwd(currentDirectory, sizeof(currentDirectory)) == NULL) {
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

void print_working_directory() {
    char buffer[1024];

    if (getcwd(buffer, sizeof(buffer)) == NULL) {
        perror("getcwd() error");
    } else {
        printf("%s\n", buffer); // Print once, no need for a second `getcwd`
    }
}

void who_am_i() {
    char *username = getenv("USER");
    if (username != NULL) {
        printf("%s\n", username);
    } else {
        printf("USER environment variable not set.\n");
    }
}

void hostname() {
    char buffer[1024] = ""; // Initialize buffer to an empty string

    if (gethostname(buffer, sizeof(buffer)) == 0) {
        printf("%s\n", buffer);
    } else {
        perror("gethostname");
    }
}

void echo(char *args) {
    if (args == NULL || args[0] == '\0') {
        printf("\n");
    } else {
        // Correct parentheses grouping
        while (*args == '"' || *args == '\'') {
            args++;
        }
        char *end = args + strlen(args) - 1;
        while (end > args && (*end == '"' || *end == '\'')) {
            end--;
        }
        *(end + 1) = 0;

        printf("%s\n", args);
    }
}

void clear() {
#ifdef _WIN32
    system("cls");
#elif defined(__APPLE__) && defined(__MACH__) || defined(__linux__)
    system("clear");
#endif
}

int closeShell() {
    printf("Closing NebuShell.\n");
    return EXIT_SUCCESS;
}
