#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utils/signal_handler.h"
#include "utils/binaries_execution.h"

#define MAX_DIRECTORY_LENGTH 1024

int main() {
    char *username = getenv("USER");
    char buffer[1024] = "";
    char currentDirectory[MAX_DIRECTORY_LENGTH] = "";

    setupSignalHandler();

    FILE *fp = popen("pwd", "r");
    if (fgets(currentDirectory, sizeof(currentDirectory), fp) == NULL) {
        printf("Failed to read current directory.\n");
        pclose(fp);
        return EXIT_FAILURE;
    }
    pclose(fp);
    currentDirectory[strcspn(currentDirectory, "\n")] = 0;

    while (!exitSignal) {
        // Current Directory Check
        if (getcwd(currentDirectory, sizeof(currentDirectory)) == NULL) {
            perror("getcwd() error");
            return EXIT_FAILURE;
        }

        // NebuShell line-by-line formatting
        printf("%s - %s /> ", username, currentDirectory);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            if (feof(stdin)) {
                printf("\nCtrl-D detected! Exiting...\n");
                break;
            }
            continue;
        }

        buffer[strcspn(buffer, "\n")] = 0;

        if (strcmp(buffer, "exit") == 0) {
            break;
        } else {
            binariesSearch(buffer);
        }
    }

    printf("Exiting program.\n");
    return EXIT_SUCCESS;
}
