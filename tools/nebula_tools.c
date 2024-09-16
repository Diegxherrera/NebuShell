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
#include "file_handler.h"

#define MAX_DIRECTORY_LENGTH 1024
#define HELP_PAGES_DIRECTORY "/any/working/directory"

int change_directory(char *path, char *currentDirectory) {
    if (path == NULL || path[0] == '\0') {
        perror("✘ nsh: Path not specified. Error Code: 1001\n");
        return EXIT_FAILURE;
    }

    if (chdir(path) == 0) {
        if (getcwd(currentDirectory, MAX_DIRECTORY_LENGTH) == NULL) {
            perror("✘ nsh: Current Directory couldn't be updated: Error Code: 1002");
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    } else {
        perror("✘ nsh: change_directory failed. Error Code: 1003");
        return EXIT_FAILURE;
    }
}

int list_directory(char *currentDirectory, const char *args) {
    struct dirent *entry;
    DIR *dp = opendir(currentDirectory);
    int directories = 0;
    int regular_files = 0;
    int symbolic_links = 0;
    int local_sockets = 0;

    if (dp == NULL) {
        perror("✘ nsh: Failed opening directory. Error Code: 2001");
        return EXIT_FAILURE;
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
        return EXIT_FAILURE;
    } else {
        printf("└ \e[1;92m%s\n\e[0m", "✔ nsh: Directory listed successfully!");
        return EXIT_SUCCESS;
    }

    closedir(dp);
    return EXIT_SUCCESS;
}

int print_working_directory() {
    char buffer[1024];

    if (getcwd(buffer, sizeof(buffer)) == NULL) {
        perror("✘ nsh: Getcwd failed. Error Code: 4001");
        return EXIT_FAILURE;
    } else {
        printf("%s\n", buffer);
        return EXIT_SUCCESS;
    }
}

int who_am_i() {
    char *username = getenv("USER");
    if (username != NULL) {
        printf("%s\n", username);
        return EXIT_SUCCESS;
    } else {
        printf("✘ nsh: User environment variable not set. Error Code: 4001\n");
        return EXIT_FAILURE;
    }
}

int hostname() {
    char buffer[1024] = "";

    if (gethostname(buffer, sizeof(buffer)) == 0) {
        printf("%s\n", buffer);
        return EXIT_SUCCESS;
    } else {
        perror("✘ nsh: Hostname could not be found. Error Code: 5001");
        return EXIT_FAILURE;
    }
}

int echo(char *args) {
    if (args == NULL || args[0] == '\0') {
        printf("\n");
        return EXIT_SUCCESS;
    } else {
        while (*args == '"' || *args == '\'') {
            args++;
        }
        char *end = args + strlen(args) - 1;
        while (end > args && (*end == '"' || *end == '\'')) {
            end--;
        }
        *(end + 1) = 0;

        printf("%s\n", args);
        return EXIT_SUCCESS;
    }
}

void clear() {
#ifdef _WIN32
    system("cls");
#elif defined(__APPLE__) && defined(__MACH__) || defined(__linux__)
    system("clear");
#endif
}

int close_shell() {
    printf("Closing NebuShell...\n");
    return EXIT_SUCCESS;
}

int help_page(int page) {
    if (open_file(HELP_PAGES_DIRECTORY, (const char *) 'r') == 0) {
    return 1;
    }
    return 0;
}