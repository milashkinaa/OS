#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int *shm;
    int shmid;
    char name[] = "server.c";
    key_t key;
    
    if ((key = ftok(name, 0)) < 0) {
        printf("ftok error\n");
        exit(-1);
    }
    
    if ((shmid = shmget(key, sizeof(int), 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
        if (errno != EEXIST) {
            printf("shmget error\n");
            exit(-1);
        }
        shmid = shmget(key, sizeof(int), 0);
    }
    
    if ((shm = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
        printf("shmat error\n");
        exit(-1);
    }
    
    srand(time(NULL));
    shm[0] = rand() % 10 + 1;
    
    if (shmdt(shm) < 0) {
        printf("shmdt error\n");
        exit(-1);
    }
    
    return 0;
}
