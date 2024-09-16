//
// Created by Diego Herrera Redondo on 5/4/24.
//

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "jobs.h"

void process_status(char *args) {
    if (strcmp(args, "-e") == 0) {

    } else if (strcmp(args, "-f") == 0) {

    } else if (strcmp(args, "-ef") == 0) {

    } else if (strcmp(args, "-C") == 0) {

    } else if (strcmp(args, "-p") == 0) {

    } else if (strcmp(args, "-u") == 0) {

    } else if (strcmp(args, "-") == 0) {

    }
}

int new_process(char *cmd, char *argv[]) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork fail");
        return EXIT_FAILURE;
    } else if (pid == 0) {
        // Child process
        execvp(cmd, argv);
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    }
}
int kill_process() {

}