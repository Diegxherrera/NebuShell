#ifndef NEBUSHELL_HISTORY_H
#define NEBUSHELL_HISTORY_H

int init_history();
void add_to_history(const char *command);
void show_history();
void clean_history(const char* filename);
int handle_history(char *cmd, char *arg1, char *arg2);

#endif // NEBUSHELL_HISTORY_H