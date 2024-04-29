#ifndef NEBUSHELL_NEBULA_TOOLS_H
#define NEBUSHELL_NEBULA_TOOLS_H

void change_directory(char *path, char *currentDirectory);
void list_directory(char *currentDirectory, const char *args);
void print_working_directory();
void echo(char *args);
void who_am_i();
void hostname();
void clear();

#endif //NEBUSHELL_NEBULA_TOOLS_H
