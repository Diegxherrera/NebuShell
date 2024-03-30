#include <stdio.h>
#include "history.h"

//
// Created by diegxherrera on 30/3/24.
//

void init_history() {
    FILE *fptr;
    fptr = fopen("/~/.nsh_history", "w");
}

void add_to_history(const char *command) {
    // Add the command to an in-memory data structure and possibly append to the history file
}

const char *get_last_command() {
    // Retrieve the last command entered
}

void save_history() {
    // Write the in-memory history to a file
}

void cleanup_history() {
    // Clean up resources used by the history functionality
}
