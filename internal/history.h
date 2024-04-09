#ifndef NEBUSHELL_HISTORY_H
#define NEBUSHELL_HISTORY_H

void init_history();
void add_to_history(const char *command);
const char *get_last_command();
void show_history();
void save_history();
void cleanup_history();

#endif // NEBUSHELL_HISTORY_H