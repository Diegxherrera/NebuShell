#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "list_directory.h"
#include "../internal/parser.h"
#include "../constants.h"

int list_directory(int argc, char *argv[], char *currentDirectory) {
    struct dirent *entry;
    DIR *dp = NULL;
    int directories = 0;
    int regular_files = 0;
    int symbolic_links = 0;
    int local_sockets = 0;

    // Allocate memory for the path string (Dynamically allocated)
    char *path = malloc(MAX_PATH_LENGTH * sizeof(char) + 1);
    size_t path_size = MAX_PATH_LENGTH;

    // Memory allocation error
    if (path == NULL) {
        perror("✘ nsh: Internal error. Memory allocation failed for path.\n");
        return EXIT_FAILURE;
    }

    // Parse options and determine directory path
    Options opts;
    char *directory = NULL;
    parse_options(argc, argv, &opts, &directory);

    // If no directory is specified, use currentDirectory
    if (directory == NULL) {
        snprintf(path, path_size, "%s", currentDirectory);
    } else {
        size_t token_length = strlen(directory);
        if (token_length >= path_size) {
            // Reallocate the path if it's not large enough
            char *new_path = realloc(path, (token_length + 1) * sizeof(char));
            if (new_path == NULL) {
                perror("✘ nsh: Memory reallocation failed.\n");
                free(path);
                return EXIT_FAILURE;
            }
            path = new_path;
            path_size = token_length + 1;
        }
        snprintf(path, path_size, "%s", directory);
    }

    // Open the directory with the given path
    dp = opendir(path);
    if (dp == NULL) {
        fprintf(stderr, "✘ nsh: Failed opening directory '%s'\n", path);
        free(path); // Free the dynamically allocated path
        return EXIT_FAILURE;
    }

    // Print the directory contents
    printf("┌ \e[1;93m%s\e[0m\n", path);
    while ((entry = readdir(dp)) != NULL) {
        // Skip entries starting with '.' unless option_a is set
        if (!opts.option_a && entry->d_name[0] == '.') {
            continue;
        }

        if (opts.option_l) {
            // Detailed listing
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
                    printf("├ %s\n", entry->d_name); // Other types
                    break;
            }
        } else {
            // Simple listing
            printf("├ %s\n", entry->d_name);
        }
    }
    closedir(dp);

    printf("└ \e[1;92m%s\n\e[0m", "✔ nsh: Directory listed successfully!");

    // Free the dynamically allocated path
    free(path);
    return EXIT_SUCCESS;
}
