#include <stdlib.h>
#include <stdio.h>
#include "alias.h"
#include "file_handler.h"

#define FULL_ALIAS_LENGTH 256
#ifdef _WIN32
    #define ALIAS_FILE_PATHNAME NULL
    #include <windows.h>
#elif defined(__APPLE__) && defined(__MACH__)
    #include <string.h>
    #define ALIAS_FILE_PATHNAME "~/.zshrc"
    #define ALIAS_FILE_PATHNAME_ALT "~/.bashrc"
#elif defined(__linux__)
    #include <unistd.h>
    #define ALIAS_FILE_PATHNAME "/usr/share/nsh/aliases"
#endif


char *find_alias(char *alias_name) {
    return EXIT_SUCCESS;
}

int show_aliases() {
    if (open_file(ALIAS_FILE_PATHNAME, "-r", NULL) != 0) {
        if (open_file(ALIAS_FILE_PATHNAME_ALT, "-r", NULL) != 0) {
            perror("✘ nsh: Error while opening alias file.");
        }
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int alias(char *alias_name, char *aliased) {
    char *full_alias = malloc(FULL_ALIAS_LENGTH * sizeof(char) + 1);
    char *temp_alias = malloc(FULL_ALIAS_LENGTH * sizeof(char) + 1);
    size_t full_alias_length = FULL_ALIAS_LENGTH;

    temp_alias = strcat(alias_name, "=");
    full_alias = strcat(temp_alias, aliased);

    if (open_file(ALIAS_FILE_PATHNAME, "-w", full_alias) != 0) {
        if (open_file(ALIAS_FILE_PATHNAME_ALT, "-w", full_alias) != 0) {
            perror("✘ nsh: Error while opening alias file.");
            return EXIT_FAILURE;
        }
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int set_up_aliases() {
    if(open_file(ALIAS_FILE_PATHNAME, "-r", NULL) != 0) {
        perror("✘ nsh: Error while opening alias file.");
        return EXIT_FAILURE;
    }
    return 0;
}

int handle_alias(char *arg1, char *optional_arg2) {
    if (arg1 == NULL || strcmp(arg1, "") == 0) {
        show_aliases();
    } else if (arg1 != NULL && optional_arg2 == NULL) {
        perror("✘ nsh: Alias needs another argument to alias successfully.");
    } else if (arg1 != NULL && optional_arg2 != NULL) {
        alias(arg1, optional_arg2);
    }
}