#include <stdbool.h>

#ifndef NEBUSHELL_BIN_H
#define NEBUSHELL_BIN_H

bool findBinary(const char *path, const char *filename);

void binariesSearch(char *binaryName[1024]);

#endif //NEBUSHELL_BIN_H
