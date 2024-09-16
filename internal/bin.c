#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <termios.h>

int runBinary(char* bin) {
    fflush(stdout);
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        execlp(bin, bin, (char *)NULL);

        perror("execlp failed");
        _exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);

        if (!WIFEXITED(status)) {
            printf("Child terminated abnormally\n");
        }
    }

    return 0;
}
