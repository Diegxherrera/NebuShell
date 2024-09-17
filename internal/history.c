#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

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
        perror("Environment variable HOME is not set.\n");
        return EXIT_FAILURE;
    }

    char path[1024];
    snprintf(path, sizeof(path), "%s/.nsh_history", home);

    fptr = fopen(path, "r");
    if (fptr == NULL) {
        // Create a new history file if none exists
        fptr = fopen(path, "w");
        if (fptr == NULL) {
            perror("Failed to create history file");
            return EXIT_FAILURE;
        }

        // Set the file permissions to read and write for the user
        if (chmod(path, S_IRUSR | S_IWUSR) != 0) {
            perror("Failed to set permissions on history file");
            fclose(fptr);
            return EXIT_FAILURE;
        }
    } else {
        // Read existing commands into history
        while (fgets(history[current_pos], MAX_COMMAND_LENGTH, fptr) && current_pos < MAX_HISTORY) {
            history[current_pos][strcspn(history[current_pos], "\n")] = 0;
            current_pos++;
        }
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
        freopen(filename, "w", input_file); // Reopen input file in write mode

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

void show_history() {
    FILE *fptr;
    char path[1024];
    char *home = getenv("HOME");
    if (home == NULL) {
        fprintf(stderr, "Environment variable HOME is not set.\n");
        return;
    }
    snprintf(path, sizeof(path), "%s/.nsh_history", home);

    fptr = fopen(path, "r");
    if (fptr == NULL) {
        perror("Failed to open history file");
        return;
    }

    char command[MAX_COMMAND_LENGTH];
    while (fgets(command, MAX_COMMAND_LENGTH, fptr)) {
        printf("%s", command);
    }

    fclose(fptr);
}

const char *get_last_command() {
    if (current_pos == 0 && strlen(history[MAX_HISTORY - 1]) == 0) {
        return NULL;
    }

    history_pos = (history_pos - 1 + MAX_HISTORY) % MAX_HISTORY;
    return history[history_pos];
}

void handle_key_up() {
    const char *last_command = get_last_command();
    if (last_command != NULL) {
        printf("\r%s", last_command);
        fflush(stdout);
    }
}