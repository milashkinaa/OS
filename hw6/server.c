#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main() {
    int* shm; // Указатель на разделяемую память
    int shmid;
    char name[] = "server.c";
    key_t key;
    
    // Генерация ключа для области разделяемой памяти
    if ((key = ftok(name, 0)) < 0) {
         printf("ftok error\n");
         exit(-1);
    }
    
    // Попытка создания памяти для эксклюзивного ключа
    // Если уже существует, то подвязываемся к ней
    if ((shmid = shmget(key, sizeof(int), 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
         if (errno != EEXIST) {
             printf("shmget error\n");
             exit(-1);
         }
         shmid = shmget(key, sizeof(int), 0);
    }
    
    // Привязка участка разделенной памяти к нашей программе
    if ((shm = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
        printf("shmat error\n");
        exit(-1);
    }
    
    // Если какое-то число было сгенерировано (ноль не может быть, поэтому такая проверка),
    // Выведем это число
    if (shm[0] != 0) {
        printf("%d\n", shm[0]);
        shm[0] = 0;
    }
    
    return 0;
}
