#include <stdbool.h>

//
// Created by diegxherrera on 30/3/24.
//

#ifndef NEBUSHELL_NEBULA_TOOLS_H
#define NEBUSHELL_NEBULA_TOOLS_H

void change_directory(char *path, char *currentDirectory);
void list_directory();
void print_working_directory();
void who_am_i();
void hostname();

#endif //NEBUSHELL_NEBULA_TOOLS_H
