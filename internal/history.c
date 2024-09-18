#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#include "../tools/nebula_tools.h"

#define MAX_HISTORY 1024
#define MAX_COMMAND_LENGTH 1024
#define MAX_LINES 1024
#define LINES_TO_REMOVE 100
#define MAX_LINE_LENGTH 1024

static char history[MAX_HISTORY][MAX_COMMAND_LENGTH];
static int current_pos = 0;
static int history_pos = 0;

int init_history() {
    FILE *fptr;
    char *home = getenv("HOME");
    if (home == NULL) {
        perror("\033[0;31m ✘ nsh: Environment variable HOME is not set.\n");
        return EXIT_FAILURE;
    }

    char *path = malloc(MIN_PATH_LENGTH * sizeof(char));
    size_t path_size = MIN_PATH_LENGTH;

    fptr = fopen(path, "r");
    if (fptr == NULL) {
        // Create a new history file if none exists
        fptr = fopen(path, "w");
        if (fptr == NULL) {
            perror("Failed to create history file");

            free(path);
            return EXIT_FAILURE;
        }

        // Set the file permissions to read and write for the user
        if (chmod(path, S_IRUSR | S_IWUSR) != 0) {
            perror("Failed to set permissions on history file");
            fclose(fptr);

            free(path);
            return EXIT_FAILURE;
        }
    } else {
        // Read existing commands into history
        while (fgets(history[current_pos], MAX_COMMAND_LENGTH, fptr) && current_pos < MAX_HISTORY) {
            history[current_pos][strcspn(history[current_pos], "\n")] = 0;
            current_pos++;
        }

        free(path);
        return EXIT_SUCCESS;
    }

    fclose(fptr);
}

void clean_history(const char *filename) {
    FILE *input_file = fopen(filename, "r");
    if (input_file == NULL) {
        perror("Unable to open the input file");
        return;
    }

    // Create a temporary file to store the modified content
    FILE *temp_file = tmpfile();
    if (temp_file == NULL) {
        perror("Unable to create a temporary file");
        fclose(input_file);
        return;
    }

    // Read and count the total number of lines
    char line[MAX_LINE_LENGTH];
    int total_lines = 0;
    while (fgets(line, sizeof(line), input_file) != NULL) {
        total_lines++;
    }

    // If the file has more than 1024 lines, remove the first 100
    if (total_lines > MAX_LINES) {
        rewind(input_file); // Return to the beginning of the input file

        // Skip the first 100 lines
        for (int i = 0; i < LINES_TO_REMOVE; i++) {
            if (fgets(line, sizeof(line), input_file) == NULL) {
                break;
            }
        }

        // Copy the remaining lines to the temporary file
        while (fgets(line, sizeof(line), input_file) != NULL) {
            fputs(line, temp_file);
        }

        // Write the contents of the temporary file back to the original file
        rewind(temp_file);       // Rewind the temp file to the beginning

        fclose(input_file);
        input_file = fopen(filename, "w");
        if (input_file == NULL) {
            perror("Unable to reopen the file for writing");
            fclose(temp_file);
            return;
        }

        while (fgets(line, sizeof(line), temp_file) != NULL) {
            fputs(line, input_file);
        }
    }

    // Close the files
    fclose(input_file);
    fclose(temp_file);
}

void add_to_history(const char *command) {
    FILE *fptr;
    char *home = getenv("HOME");
    if (home == NULL) {
        fprintf(stderr, "Environment variable HOME is not set.\n");
        return;
    }

    char path[1024];
    snprintf(path, sizeof(path), "%s/.nsh_history", home);

    // Open the history file in append mode
    fptr = fopen(path, "a");
    if (fptr == NULL) {
        perror("Failed to open history file");
        return;
    }

    // Check if the command is only whitespace or empty
    int is_whitespace = 1;
    for (int i = 0; command[i] != '\0'; i++) {
        if (!isspace((unsigned char)command[i])) {
            is_whitespace = 0;
            break;
        }
    }

    if (is_whitespace) {
        fclose(fptr);
        return;
    }

    // Add the command to the history array
    strncpy(history[current_pos], command, MAX_COMMAND_LENGTH - 1);
    history[current_pos][MAX_COMMAND_LENGTH - 1] = '\0';

    // Write the command to the file
    if (fprintf(fptr, "%s\n", command) < 0) {
        perror("Failed to write to history file");
    }

    // Close the file
    fclose(fptr);
    clean_history(path);

    // Update position in the circular history array
    current_pos = (current_pos + 1) % MAX_HISTORY;
    history_pos = current_pos;
}

int show_history(char *args) {
    FILE *fptr;
    char *path = malloc(MIN_PATH_LENGTH * sizeof (char));
    size_t path_length = MIN_PATH_LENGTH;
    char command[MAX_COMMAND_LENGTH];

    char *home = getenv("HOME");
    if (home == NULL) {
        fprintf(stderr, "Environment variable HOME is not set.\n");
        return EXIT_FAILURE;
    }

    int required_length = snprintf(NULL, 0, "%s/.nsh_history", home) + 1; // +1 for the null-terminator
    if (required_length > path_length) {
        char *new_path = realloc(path, required_length * sizeof(char));
        if (new_path == NULL) {
            fprintf(stderr, "Memory reallocation failed for path.\n");
            free(path); // Free the old path before returning
            return EXIT_FAILURE;
        }
        path = new_path;
        path_length = required_length; // Update path_length to reflect the new size
    }

    // Now that we have sufficient space, build the path
    snprintf(path, path_length, "%s/.nsh_history", home);

    fptr = fopen(path, "r");
    if (fptr == NULL) {
        perror("Failed to open history file");
        return EXIT_FAILURE;
    }

    if (strcmp(args, NULL) == 0) {
        while (fgets(command, MAX_COMMAND_LENGTH, fptr)) {
            printf("%s", command);
        }
    } else if (strcmp(args, "clean") == 0) {
        clean_history(path);
    } else if (strcmp(args, "-f") == 0) {
        // Do something else
    }

    free(path);
    fclose(fptr);
    return EXIT_SUCCESS;
}
