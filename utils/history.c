#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HISTORY 100
#define MAX_COMMAND_LENGTH 1024

static char history[MAX_HISTORY][MAX_COMMAND_LENGTH];
static int current_pos = 0;
static int history_pos = 0;

void init_history() {
    FILE *fptr;
    char *home = getenv("HOME");
    char path[1024];
    snprintf(path, sizeof(path), "%s/.nsh_history", home);

    fptr = fopen(path, "r");
    if (fptr == NULL) {
        return;
    }

    while (fgets(history[current_pos], MAX_COMMAND_LENGTH, fptr) && current_pos < MAX_HISTORY) {
        history[current_pos][strcspn(history[current_pos], "\n")] = 0;
        current_pos++;
    }

    fclose(fptr);
}

void add_to_history(const char *command) {
    strncpy(history[current_pos], command, MAX_COMMAND_LENGTH - 1);
    history[current_pos][MAX_COMMAND_LENGTH - 1] = '\0';
    current_pos = (current_pos + 1) % MAX_HISTORY;
    history_pos = current_pos;
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