#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <termios.h>

int runBinary(char *args) {
    fflush(stdout);  // Flush stdout to ensure all output is printed before forking
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        execlp(args, args, (char *)NULL);

        perror("execlp failed");
        _exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("Child exited with status %d\n", WEXITSTATUS(status));
        } else {
            printf("Child terminated abnormally\n");
        }
    }

    return 0;
}
