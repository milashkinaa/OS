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
        fd = open(name, O_WRONLY);
        if (fd < 0) {
            printf("Something went wrong\n");
            return 0;
        }

	// Получает какой-то текст от пользователя на вход
	printf("What do you want to answer? ");
        fgets(arr2, 80, stdin);

        // Записывает этот текст в файл
        write(fd, arr2, strlen(arr2)+1);
        close(fd);

        // Oткрывыает файл для записи
        fd = open(name, O_RDONLY);
        if (fd < 0) {
            printf("Something went wrong\n");
            return 0;
        }

        // Читает что-то из файла
        read(fd, arr1, sizeof(arr1));

        // Выводит информацию о сообщении от второй программы
        printf("Programm 2: %s\n", arr1);
        close(fd);
    }
    return 0;
}
