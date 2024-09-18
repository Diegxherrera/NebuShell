#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <limits.h>


int run_binary(char* bin) {
    fflush(stdout);
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork failed");
        return EXIT_FAILURE;
    } else if (pid == 0) {
        if (execlp(bin, bin, (char *)NULL) == -1) {
            perror("✘ nsh: Execution of the binary failed");
            return EXIT_FAILURE;
        }

        _exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);

        if (!WIFEXITED(status)) {
            printf("✘ nsh: Child terminated abnormally\n");
        }
    }

    return EXIT_SUCCESS;
}

int is_a_binary(const char *cmd) {
    if (strchr(cmd, '/')) {
        // If cmd contains '/', check it directly
        return access(cmd, X_OK) == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
    }

    char *path_env = getenv("PATH");
    if (!path_env) return EXIT_FAILURE;

    char *path_dup = strdup(path_env);
    if (!path_dup) return EXIT_FAILURE;

    char *dir = strtok(path_dup, ":");
    while (dir) {
        char fullpath[PATH_MAX];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dir, cmd);
        if (access(fullpath, X_OK) == 0) {
            free(path_dup);
            return EXIT_SUCCESS;
        }
        dir = strtok(NULL, ":");
    }
    free(path_dup);
    return EXIT_FAILURE;
}