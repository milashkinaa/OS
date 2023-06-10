#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SWR "/semaphore_writer"
#define SR "/semaphore_reader"

int main() {
    sem_t *sem_wr;
    sem_t *sem_r;
    FILE *buffer;
    char buffered_text[6];

    if ((sem_wr = sem_open("/writersem", O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED) {
        perror("Couldn't open semaphore writer");
        exit(1);
    }

    if ((sem_r = sem_open("/readersem", O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED) {
        perror("Couldn't open semaphore reader");
        exit(1);
    }
    while (1) {
        sem_wait(sem_r);
        if ((buffer = fopen("buffer.txt", "r")) < 0) {
            perror("Can't open file for reading");
            exit(1);
        }
        fread(buffered_text, 1, 6, buffer);
        if (strcmp(buffered_text, "exit\n") == 0) {
            fclose(buffer);
            sem_post(sem_wr);
            sem_close(sem_wr);
            sem_close(sem_r);
            break;
        }
        fprintf(stdout, "Reader read %s\n", buffered_text);
	for (size_t j = 0; j <= 5; ++j) {
	    buffered_text[j] = '\0';
	}
        fclose(buffer);
        sem_post(sem_wr);
    }
}
