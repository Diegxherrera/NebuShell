#ifdef _WIN32
    #define HELP_PAGES_DIRECTORY NULL
    #include <windows.h>
#elif defined(__APPLE__) && defined(__MACH__)
    #include <unistd.h>
    #define HELP_PAGES_DIRECTORY "/usr/local/share/nsh/help_"
#elif defined(__linux__)
    #include <unistd.h>
    #define HELP_PAGES_DIRECTORY "/usr/share/nsh/help:"
#endif
#include "help.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_handler.h"

int help_page(char *option, char *command) {
    char help_page_path[1024];

    if (option == NULL || strcmp(option, "") == 0) {
        // No option provided, display the default help page
        snprintf(help_page_path, sizeof(help_page_path), "%sdefault", HELP_PAGES_DIRECTORY);
    } else if (strcmp(option, "-p") == 0) {
        if (command == NULL || strcmp(command, "") == 0) {
            fprintf(stderr, "✘ nsh: No page specified after -p option.\n");
            return EXIT_FAILURE;
        }

        snprintf(help_page_path, sizeof(help_page_path), "%s%s", HELP_PAGES_DIRECTORY, command);
    } else {
        // Option provided is actually the command name
        snprintf(help_page_path, sizeof(help_page_path), "%s%s", HELP_PAGES_DIRECTORY, option);
    }

    // Check if the help page exists
    if (access(help_page_path, F_OK) != 0) {
        fprintf(stderr, "✘ nsh: Help page not found: %s\n", help_page_path);
        return EXIT_FAILURE;
    }

    // Read and display the help page
    if (open_file(help_page_path, "-r", NULL) != EXIT_SUCCESS) {
        fprintf(stderr, "✘ nsh: Could not display help page: %s\n", help_page_path);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int init_help_pages() {

}