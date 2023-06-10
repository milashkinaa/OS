#include "stdio.h"
#include "stdlib.h"
#include "signal.h"
#include "unistd.h"

int is_received = 0;

void sighandler(int nsig) {
    if (nsig == SIGUSR2) {
        is_received = 1;
    }
}

int main() {
    pid_t receiver_pid;

    signal(SIGUSR2, sighandler);

    printf("Sender pid: %d\nInput receiver pid: ", getpid());
    scanf("%d", &receiver_pid);
    printf("Input decimal integer: ");
    int d;
    scanf("%d", &d);

    while (is_received == 0) {
        usleep(100);
    }
    
    for (int i = 0; i < 32; ++i) {
        is_received = 0;
        if ((d >> i) & 1) {
            kill(receiver_pid, SIGUSR2);
        } else {
            kill(receiver_pid, SIGUSR1);
        }
        while (is_received == 0) {
            usleep(100);
        }
    }
    kill(receiver_pid, SIGINT);
}
