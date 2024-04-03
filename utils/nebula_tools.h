#include <stdbool.h>

#ifndef NEBUSHELL_NEBULA_TOOLS_H
#define NEBUSHELL_NEBULA_TOOLS_H

void change_directory(char *path, char *currentDirectory);
void list_directory(char *currentDirectory);
void print_working_directory();
void who_am_i();
void hostname();
int closeShell();

#endif //NEBUSHELL_NEBULA_TOOLS_H
