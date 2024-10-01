#include <stdio.h>
#ifdef _WIN32
    #define HELP_PAGES_DIRECTORY NULL
    #include <windows.h>
#elif defined(__APPLE__) && defined(__MACH__)
    #include <unistd.h>
    #define HELP_PAGES_DIRECTORY "/usr/local/share/nsh/help_"
#elif defined(__linux__)
    #include <unistd.h>
    #define HELP_PAGES_DIRECTORY "/usr/share/nsh/help_"
#endif
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include "nebula_tools.h"
#include "alias.h"
#include "../constants.h"


extern char **environ;

int change_directory(char *path, char *currentDirectory) {
    char *home_path = getenv("HOME");
    if (path == NULL || path[0] == '\0') {
        perror("✘ nsh: Path not specified. Error Code: 1001\n");
        return EXIT_FAILURE;
    }

    if (chdir(path) == 0) {
        if (getcwd(currentDirectory, MAX_DIRECTORY_LENGTH) == NULL) {
            perror("✘ nsh: Current Directory couldn't be updated: Error Code: 1002");
            return EXIT_FAILURE;
        }

        if (strcmp(path, "~") == 0) {
            if (chdir(home_path) == 0) {
                return EXIT_SUCCESS;
            } else {
                perror("✘ nsh: Home directory couldn't be updated: Error Code: 1002");
                return EXIT_FAILURE;
            }
        }
        return EXIT_SUCCESS;
    } else if (chdir(find_alias(path)) == 0) {

    }

    else {
        perror("✘ nsh: Change directory failed. Error Code: 1003");
        return EXIT_FAILURE;
    }
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
    // TBD
}

int close_shell() {
    printf("Closing NebuShell...\n");
    return EXIT_SUCCESS;
}

int environment_variables(const char *args) {
    if (args == NULL) {
        char **env = environ;
        while (*env != NULL) {
            printf("%s\n", *env);
            env++;
        }
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}

int where(const char *args) {

}

int redirect_output(const char *stdout, const char *stdin) {

}