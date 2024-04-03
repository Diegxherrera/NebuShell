#ifndef NEBUSHELL_HISTORY_H
#define NEBUSHELL_HISTORY_H

void init_history();

// Add a command to history
void add_to_history(const char *command);

// Get the last command from history
const char *get_last_command();

// Save history to file
void save_history();

// Clean up history on exit
void cleanup_history();

#endif // NEBUSHELL_HISTORY_H