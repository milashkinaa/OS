#include <sys/types.h>
#include <sys/mman.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd;
    char *ch;
    
    if ((fd = shm_open("shared", O_RDWR | O_CREAT, 0777)) == -1) {
        perror("Can't open shared memory\n");
        return 0;
    }
    
    if (ftruncate(fd, 1) == -1) {
        perror("Can't set the size\n");
        return 0;
    }
    
    ch = mmap(NULL, 1, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ch == MAP_FAILED) {
        perror("Can't mmap objects\n");
        return 0;
    }
    sleep(1);
    for (int i = 0; i < 5; ++i) {
        printf("Server read %d\n", ch[0]);
        sleep(2);
    }

    if (shm_unlink("shared") == -1) {
        perror("Can't unlink shared memory\n");
        return 0;
    }
}
