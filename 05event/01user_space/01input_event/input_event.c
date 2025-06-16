#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    const char* device = (argc >= 2) ? argv[1] : "/dev/input/event12";   // my cutie custom keyboard as default
    int fd = open(device, O_RDONLY);
    if(fd < 0) {
        perror("open");
        return 1;
    }

    struct input_event ev;
    while (1) {
        ssize_t n = read(fd, &ev, sizeof(ev));
        if(n == -1) {
            perror("read");
            continue;
        }
        //perror("read");
        if(n == (ssize_t)sizeof(ev)) {
            if(ev.type == EV_KEY) {
                printf("\nKey %s (code %d) %s\n",
                    ev.code < 256 ? (ev.code < 30 ? "UNKNOWN" : "KEY") : "BTN",
                    ev.code,
                    ev.value == 1 ? "PRESSED" : ev.value == 0 ? "RELEASED" : "REPEATED");
            } else if(ev.type == EV_REL) {
                printf("Relative movement event code %d value %d\n", ev.code, ev.value);
            } else if(ev.type == EV_ABS) {
                printf("Absolute event code %d value %d\n", ev.code, ev.value);
            }
        }
    }

    close(fd);
    return 0;
}
