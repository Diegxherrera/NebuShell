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
    if (open_file(ALIAS_FILE_PATHNAME, "r", NULL) != 0) {
        if (open_file(ALIAS_FILE_PATHNAME_ALT, "r", NULL) != 0) {
            perror("✘ nsh: Error while opening alias file.");
        }
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int alias(char *alias_name, char *aliased) {
    size_t alias_name_len = strlen(alias_name);
    size_t aliased_len = strlen(aliased);

    // Allocate memory for temp_alias ("alias_name=")
    char *temp_alias = malloc(alias_name_len + 2); // +1 for '=' and +1 for '\0'
    if (temp_alias == NULL) {
        perror("Memory allocation failed for temp_alias");
        return EXIT_FAILURE;
    }
    strcpy(temp_alias, alias_name);
    strcat(temp_alias, "=");

    // Allocate memory for full_alias ("alias_name=aliased")
    char *full_alias = malloc(alias_name_len + aliased_len + 2); // +2 for '=' and '\0'
    if (full_alias == NULL) {
        perror("Memory allocation failed for full_alias");
        free(temp_alias);
        return EXIT_FAILURE;
    }
    strcpy(full_alias, temp_alias);
    strcat(full_alias, aliased);

    // Use full_alias as needed
    // For example, write full_alias to a file
    if (open_file(ALIAS_FILE_PATHNAME, "w", full_alias) != 0) {
        if (open_file(ALIAS_FILE_PATHNAME_ALT, "w", full_alias) != 0) {
            perror("✘ nsh: Error while opening alias file.");
            free(temp_alias);
            free(full_alias);
            return EXIT_FAILURE;
        }
    }

    // Free allocated memory
    free(temp_alias);
    free(full_alias);

    return EXIT_SUCCESS;
}

int set_up_aliases() {
    if (open_file(ALIAS_FILE_PATHNAME, "r", NULL) != 0) {
        if (open_file(ALIAS_FILE_PATHNAME_ALT, "r", NULL) != 0) {
            perror("✘ nsh: Error while opening alias file.");
        }
        return EXIT_FAILURE;
    }
    return 0;
}

int handle_alias(char *arg1, char *optional_arg2) {
    if (arg1 == NULL || strcmp(arg1, "") == 0) {
        if (show_aliases() != EXIT_SUCCESS) {
            perror("✘ nsh: Alias could not be shown right now.");
        }
    } else if (arg1 != NULL && optional_arg2 == NULL) {
        perror("✘ nsh: Alias needs another argument to alias successfully.");
    } else if (arg1 != NULL && optional_arg2 != NULL) {
        if (alias(arg1, optional_arg2) != EXIT_SUCCESS) {
            perror("✘ nsh: Alias setting not available.");
        }
    }
}