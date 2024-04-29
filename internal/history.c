#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_HISTORY 100
#define MAX_COMMAND_LENGTH 1024

static char history[MAX_HISTORY][MAX_COMMAND_LENGTH];
static int current_pos = 0;
static int history_pos = 0;

void init_history() {
    FILE *fptr;
    char *home = getenv("HOME");
    if (home == NULL) {
        fprintf(stderr, "Environment variable HOME is not set.\n");
        return;
    }

    char path[1024];
    snprintf(path, sizeof(path), "%s/.nsh_history", home);

    fptr = fopen(path, "r");
    if (fptr == NULL) {
        fptr = fopen(path, "w");
        if (fptr == NULL) {
            perror("Failed to create history file");
            return;
        }

        // Set the file permissions to read and write for the user
        if (chmod(path, S_IRUSR | S_IWUSR) != 0) {
            perror("Failed to set permissions on history file");
            fclose(fptr);
            return;
        }
    } else {
        while (fgets(history[current_pos], MAX_COMMAND_LENGTH, fptr) && current_pos < MAX_HISTORY) {
            history[current_pos][strcspn(history[current_pos], "\n")] = 0;
            current_pos++;
        }
    }

    fclose(fptr);
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

    // Add the command to the history array
    strncpy(history[current_pos], command, MAX_COMMAND_LENGTH - 1);
    history[current_pos][MAX_COMMAND_LENGTH - 1] = '\0';

    // Write the command to the file
    if (fprintf(fptr, "%s\n", command) < 0) {
        perror("Failed to write to history file");
    }

    // Close the file
    fclose(fptr);

    // Update position in the circular history array
    current_pos = (current_pos + 1) % MAX_HISTORY;
    history_pos = current_pos;
}

void show_history() {
    FILE *fptr;
    char path[1024];
    char *home = getenv("HOME");
    snprintf(path, sizeof(path), "%s/.nsh_history", home);

    fptr = fopen(path, "r");
    if (fptr == NULL) {
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
    if (last_command) {
        printf("\r%s", last_command);
        fflush(stdout);
    }
}