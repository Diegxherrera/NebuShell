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

#define MAX_DIRECTORY_LENGTH 1024
#define MAX_ARGS_LENGTH 1024
#define MIN_PATH_LENGTH 256

extern char **environ;

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
        perror("✘ nsh: Change directory failed. Error Code: 1003");
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

    // Flags for options
    int option_l = 0; // Long listing format
    int option_a = 0; // Include entries starting with '.'
    int option_f = 0; // Example flag for '-f' option
    // Add more option flags as needed

    // Array to store paths provided by the user (Dynamically allocated)
    char **paths = malloc(MIN_PATH_LENGTH * sizeof(char *));
    size_t path_size = MIN_PATH_LENGTH;
    int path_count = 0;

    // Memory allocation error
    if (paths == NULL) {
        perror("✘ nsh: Internal error. Memory allocation failed for paths.\n");
        return EXIT_FAILURE;
    }

    // Add the first path (currentDirectory) to the array of paths if none are specified
    if (path_count == 0) {
        paths[path_count++] = strdup(currentDirectory);  // strdup allocates memory and copies the string
    }

    // Copy args to a modifiable buffer
    char args_copy[MAX_ARGS_LENGTH];
    if (args != NULL && args[0] != '\0') {
        strncpy(args_copy, args, sizeof(args_copy));
        args_copy[sizeof(args_copy) - 1] = '\0';
    } else {
        args_copy[0] = '\0';
    }

    // Tokenize args
    char *token = strtok(args_copy, " ");
    while (token != NULL) {
        if (token[0] == '-') {
            // Process options
            for (int i = 1; token[i] != '\0'; i++) {
                switch (token[i]) {
                    case 'l':
                        option_l = 1;
                        break;
                    case 'a':
                        option_a = 1;
                        break;
                    case 'f':
                        option_f = 1;
                        break;
                    default:
                        printf("\033[0;31m✘ nsh: invalid option -- '%c'\n\033[0m", token[i]);
                        free(paths);
                        return EXIT_FAILURE;
                }
            }
        } else {
            // Assume it's a path
            if (path_count >= path_size) {
                path_size *= 2;  // Double the size of the paths array if it's full
                char **new_paths = realloc(paths, path_size * sizeof(char *));
                if (new_paths == NULL) {
                    perror("✘ nsh: Memory reallocation failed.\n");
                    free(paths);
                    return EXIT_FAILURE;
                }
                paths = new_paths;
            }
            paths[path_count++] = strdup(token);
        }
        token = strtok(NULL, " ");
    }

    // Process each path
    for (int p = 0; p < path_count; p++) {
        dp = opendir(paths[p]);  // Correctly pass individual paths
        if (dp == NULL) {
            fprintf(stderr, "✘ nsh: Failed opening directory '%s'\n", paths[p]);
            free(paths[p]); // Free the individual path string
            continue;
        }

        printf("┌ \e[1;93m%s\e[0m\n", paths[p]);
        while ((entry = readdir(dp)) != NULL) {
            // Skip entries starting with '.' unless option_a is set
            if (!option_a && entry->d_name[0] == '.') {
                continue;
            }

            if (option_l) {
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
        free(paths[p]);  // Free the individual path string after processing
        printf("└ \e[1;92m%s\n\e[0m", "✔ nsh: Directory listed successfully!");
    }

    free(paths);  // Free the array of pointers after processing all paths
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
    // TBA
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