#include <sys/types.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

char buf[1];

void child(int* pd, int semid, int semid2) {
    struct sembuf parent_buf = {.sem_num = 0, .sem_op = 1, .sem_flg = 0};
    struct sembuf child_buf = {.sem_num = 0, .sem_op = -1, .sem_flg = 0};
    if (semop(semid2, &child_buf, 1) < 0) {
    	fprintf(stderr, "Can\'t sub 1 from semaphore reader\n");
    	exit(0);
    }
    char child_read[1];
    read(pd[0], child_read, 1);
    fprintf(stdout, "Reader read %s\n", child_read);
    if (semop(semid, &parent_buf, 1) < 0) {
    	fprintf(stderr, "Can\'t add 1 to semaphore writer\n");
    	exit(0);
    }
    exit(0);
}


void parent(int* pd, int semid, int semid2) {
    struct sembuf parent_buf = {.sem_num = 0, .sem_op = -1, .sem_flg = 0};
    struct sembuf child_buf = {.sem_num = 0, .sem_op = 1, .sem_flg = 0};
    if (semop(semid, &parent_buf, 1) < 0) {
    	fprintf(stderr, "Can\'t sub 1 from semaphore writer\n");
    	exit(0);
    }
    char generated = (char)('0' + rand() % 10);
    buf[0] = generated;
    write(pd[1], &buf, 1);
    fprintf(stdout, "Parent wrote %d\n", (int)(generated - '0'));
    if (semop(semid2, &child_buf, 1) < 0) {
    	fprintf(stderr, "Can\'t add 1 to semaphore reader\n");
    }
}

int main() {
    srand(time(NULL));
    int i = 0;
    int fd[2];
    if (pipe(fd) < 0) {
    	fprintf(stderr, "Error while creating pipe\n");
    	exit(0);
    }
    
    char pathname[] = "semaphore_writer.c";
    char pathname2[] = "semaphore_reader.c";
    key_t key = ftok(pathname, 0);
    key_t key2 = ftok(pathname2, 0);
    int semid, semid2;
    struct sembuf mybuf;
    struct sembuf mybuf2;
    
    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
    	fprintf(stderr, "Can\'t create semaphore\n");
    	exit(0);
    }
    
    if ((semid2 = semget(key2, 1, 0666 | IPC_CREAT)) < 0) {
    	fprintf(stderr, "Can\'t create semaphore\n");
    	exit(0);
    }
    
    semctl(semid, 0, SETVAL, 0);
    semctl(semid2, 0, SETVAL, 1);
    
    while (i < 10) {
    	++i;
    	pid_t pid = fork();
    	if (pid < 0) {
            fprintf(stderr, "Fork error\n");
            exit(0);
    	} else if (pid == 0) {
    	    child(fd, semid, semid2);
    	    if (i == 10) {
    	    	close(fd[0]);
    	    	close(fd[1]);
    	    }
            exit(0);
    	} else {
    	    parent(fd, semid, semid2);
    	}
    	wait(0);
    }
    
    
    if (close(fd[0]) != 0 || close(fd[1]) != 0) {
        fprintf(stderr, "Error while deleting pipe\n");
        exit(0);
    }
    
    semctl(semid, 0, IPC_RMID, 0);
    semctl(semid2, 0, IPC_RMID, 0);
    
    printf("The end\n");
    
    return 0;
}
