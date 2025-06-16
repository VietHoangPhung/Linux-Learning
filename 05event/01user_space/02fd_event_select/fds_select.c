#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>

int main() {
    fd_set readfds;                     // the fds set
    FD_ZERO(&readfds);                  // empty set
    FD_SET(STDIN_FILENO, &readfds);     // add STDIN ato fds set

    struct timeval timeout;
    // timeout in 5 secs.
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    printf("waiting for input on stdin\n");

    int temp = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);

    if (temp == -1) {
        perror("select()");
        exit(EXIT_FAILURE);
    } else if(temp) {
        printf("Data is available on stdin!\n");

        char buffer[100];
        read(STDIN_FILENO, buffer, sizeof(buffer));
        printf("You typed: %s\n", buffer);
    } else {
        printf("No input within 5 seconds. Timing out.\n");
    }

    return 0;
}

/** note begin ---
need a fd_set to indicate which fd(s) to inspect

FD_SET() add fd to fd_set, simpply set the <fd>th bit of the fd_set to 1

linearly scan the 

----- note end **/