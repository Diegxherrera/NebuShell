#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_handler.h"
#define MIN_BUFFER_LENGTH 128

char* expand_tilde(const char* filename) {
    if (filename[0] == '~') {
        const char* home = getenv("HOME");
        if (home) {
            size_t home_len = strlen(home);
            size_t filename_len = strlen(filename);

            char* expanded_filename = malloc(home_len + filename_len);
            if (expanded_filename == NULL) {
                perror("Memory allocation failed for expanded_filename");
                return NULL;
            }

            strcpy(expanded_filename, home);
            strcat(expanded_filename, filename + 1); // Skip the tilde (~)
            return expanded_filename;
        }
    }
    return strdup(filename); // If no tilde, return a duplicate of the original filename
}

int open_file(char *filename, const char *mode, char *optional_input) {
    if (filename == NULL) {
        perror("✘ nsh: Filename was not provided. Error Code: 901.");
        return EXIT_FAILURE;
    }

    // Expand the tilde (~) in the filename if necessary
    char* expanded_filename = expand_tilde(filename);
    if (expanded_filename == NULL) {
        return EXIT_FAILURE;
    }

    FILE *file = fopen(expanded_filename, mode);
    if (file == NULL) {
        perror("✘ nsh: Failed to open file");
        free(expanded_filename);
        return EXIT_FAILURE;
    }

    char *buffer = malloc(MIN_BUFFER_LENGTH * sizeof(char) + 1);
    if (buffer == NULL) {
        perror("Memory allocation failed for buffer");
        fclose(file);
        free(expanded_filename);
        return EXIT_FAILURE;
    }

    if (strcmp(mode, "r") == 0) {
        while (fgets(buffer, MIN_BUFFER_LENGTH, file) != NULL) {
            printf("%s", buffer);
        }
    } else if (strcmp(mode, "w") == 0 && optional_input != NULL) {
        fprintf(file, "%s\n", optional_input);
    }

    // Clean up
    fclose(file);
    free(buffer);
    free(expanded_filename);

    return EXIT_SUCCESS;
}
