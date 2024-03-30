#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>

//
// Created by diegxherrera on 29/3/24.
//

bool findBinary(const char *path, const char *filename) {
    struct dirent *entry;
    bool found = false;
    DIR *dir = opendir(path);

    if (dir == NULL) {
        return false;
    }

    while ((entry = readdir(dir)) != NULL) {
        char fullPath[1024];
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }
            sprintf(fullPath, "%s/%s", path, entry->d_name);
        } else if (entry->d_type == DT_REG) {
            if (strcmp(entry->d_name, filename) == 0) {
                found = true;
            }
        }
        if (found) break;
    }

    closedir(dir);
    return found;
}

void binariesSearch(char binaryName[1024]) {
    if (findBinary("/bin", binaryName)) {

    } else if (findBinary("/opt/brew", binaryName)){

    }
}