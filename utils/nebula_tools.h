#include <stdbool.h>

//
// Created by diegxherrera on 30/3/24.
//

#ifndef NEBUSHELL_NEBULA_TOOLS_H
#define NEBUSHELL_NEBULA_TOOLS_H

void change_directory(char path[1024][1024]);
void list_directory(char *path[1024]);
void print_working_directory(char *path[1024]);
void who_am_i();
void hostname();

#endif //NEBUSHELL_NEBULA_TOOLS_H
