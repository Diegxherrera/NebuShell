#include <stdio.h>
#include <stdlib.h>
#include "file_handler.h"

int open_file(char* filename, const char* mode) {
    FILE *file = fopen(filename, mode);

    if (filename == NULL) {
        perror("✘ nsh: Filename was not provided. Error Code: 901 ");
        return EXIT_FAILURE;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }

    fclose(file); // Close the file when done
    return EXIT_SUCCESS;
}

void create_file(char *file_name) {

}

