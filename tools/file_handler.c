#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_handler.h"

int open_file(char *filename, const char *mode, char *optional_input) {
    FILE *file = fopen(filename, mode);

    if (filename == NULL) {
        perror("✘ nsh: Filename was not provided. Error Code: 901.");
        return EXIT_FAILURE;
    }

    char buffer[256]; //TODO: Allocate manually
    if (strcmp(mode, "-r") == 0){
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            printf("%s", buffer);
        }
    } else if (strcmp(mode, "-w") == 0) {

    }

    fclose(file); // Close the file when done
    return EXIT_SUCCESS;
}
