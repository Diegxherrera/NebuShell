#include <stdbool.h>

//
// Created by diegxherrera on 30/3/24.
//

#ifndef NEBUSHELL_NEBULA_TOOLS_H
#define NEBUSHELL_NEBULA_TOOLS_H

bool change_directory(char *path[1024]);
bool list_directory(char *path[1024]);
bool print_working_directory(char *path[1024]);

#endif //NEBUSHELL_NEBULA_TOOLS_H
