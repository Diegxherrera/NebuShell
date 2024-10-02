#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "alias.h"
#include "file_handler.h"

#ifdef _WIN32
#define ALIAS_FILE_PATHNAME NULL
    #include <windows.h>
#elif defined(__APPLE__) && defined(__MACH__)
#define ALIAS_FILE_PATHNAME "~/.zshrc"
#define ALIAS_FILE_PATHNAME_ALT "~/.bashrc"
#elif defined(__linux__)
#define ALIAS_FILE_PATHNAME "/usr/share/nsh/aliases"
#endif

char *find_alias(char *alias_name) {
    return EXIT_SUCCESS;
}

int show_aliases() {
    char* expanded_pathname = expand_tilde(ALIAS_FILE_PATHNAME);
    char* expanded_alt_pathname = expand_tilde(ALIAS_FILE_PATHNAME_ALT);

    if (open_file(expanded_pathname, "r", NULL) != 0) {
        if (open_file(expanded_alt_pathname, "r", NULL) != 0) {
            perror("✘ nsh: Error while opening alias file.");
        }
        free(expanded_pathname);
        free(expanded_alt_pathname);
        return EXIT_FAILURE;
    }

    free(expanded_pathname);
    free(expanded_alt_pathname);
    return EXIT_SUCCESS;
}

int alias(char *alias_name, char *aliased) {
    size_t alias_name_len = strlen(alias_name);
    size_t aliased_len = strlen(aliased);

    char *temp_alias = malloc(alias_name_len + 2); // +1 for '=' and +1 for '\0'
    if (temp_alias == NULL) {
        perror("Memory allocation failed for temp_alias");
        return EXIT_FAILURE;
    }
    strcpy(temp_alias, alias_name);
    strcat(temp_alias, "=");

    char *full_alias = malloc(alias_name_len + aliased_len + 2);
    if (full_alias == NULL) {
        perror("Memory allocation failed for full_alias");
        free(temp_alias);
        return EXIT_FAILURE;
    }
    strcpy(full_alias, temp_alias);
    strcat(full_alias, aliased);

    char* expanded_pathname = expand_tilde(ALIAS_FILE_PATHNAME);
    char* expanded_alt_pathname = expand_tilde(ALIAS_FILE_PATHNAME_ALT);

    // Try writing to the main alias file
    if (open_file(expanded_pathname, "a", full_alias) != 0) {  // Use append mode ("a")
        if (open_file(expanded_alt_pathname, "a", full_alias) != 0) {
            perror("✘ nsh: Error while writing to alias file.");
            free(temp_alias);
            free(full_alias);
            free(expanded_pathname);
            free(expanded_alt_pathname);
            return EXIT_FAILURE;
        }
    }

    // Reload the alias file to apply the alias in the current session
    if (access(expanded_pathname, F_OK) == 0) {
        char source_command[512];
        snprintf(source_command, sizeof(source_command), "source %s", expanded_pathname);
        system(source_command);
    } else if (access(expanded_alt_pathname, F_OK) == 0) {
        char source_command[512];
        snprintf(source_command, sizeof(source_command), "source %s", expanded_alt_pathname);
        system(source_command);
    } else {
        perror("✘ nsh: Alias file could not be reloaded.");
    }

    free(temp_alias);
    free(full_alias);
    free(expanded_pathname);
    free(expanded_alt_pathname);

    return EXIT_SUCCESS;
}

int set_up_aliases() {
    char* expanded_pathname = expand_tilde(ALIAS_FILE_PATHNAME);
    char* expanded_alt_pathname = expand_tilde(ALIAS_FILE_PATHNAME_ALT);

    if (access(expanded_pathname, F_OK) != 0 && access(expanded_alt_pathname, F_OK) != 0) {
        perror("✘ nsh: Alias file does not exist.");
        free(expanded_pathname);
        free(expanded_alt_pathname);
        return EXIT_FAILURE;
    }

    free(expanded_pathname);
    free(expanded_alt_pathname);
    return EXIT_SUCCESS;
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