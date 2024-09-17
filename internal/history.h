#ifndef NEBUSHELL_HISTORY_H
#define NEBUSHELL_HISTORY_H

int init_history();
void add_to_history(const char *command);
const char *get_last_command();
void show_history();
void clean_history(const char* filename);

#endif // NEBUSHELL_HISTORY_H