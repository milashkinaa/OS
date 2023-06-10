#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    int fd;
    if (argc != 2) {
        printf("You should input name of the buffer file via command line arguments");
    }
    char* name = argv[1];

    mkfifo(name, 0666);

    char arr1[80], arr2[80];
    while (1)
    {
        // Открывает файл и читает, что там написано (в данном случае ждет,
        // пока там что нибудь напишут)
        fd = open(name, O_RDONLY);
        read(fd, arr1, 80);
        if (fd < 0) {
            printf("Something went wrong\n");
            return 0;
        }

        // Выводит, какое сообщение получил от первой программы
        printf("User1: %s\nWhat can you answer? ", arr1);
        close(fd);

        // Открывает в режиме писателя
        fd = open(name, O_WRONLY);
        if (fd < 0) {
            printf("Something went wrong\n");
            return 0;
        }
        
        // Получает от пользователя ответ с консоли и выводит его в файл
        fgets(arr2, 80, stdin);
        write(fd, arr2, strlen(arr2) + 1);
        close(fd);
    }
    return 0;
}
