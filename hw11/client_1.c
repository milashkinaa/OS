#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUF_SIZE 1024
#define THE_END "The End"

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[BUF_SIZE];

    if (argc != 3) {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket() error");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("connect() error");
        exit(1);
    }

    while (1) {
        printf("Введите сообщение: ");
        fgets(buffer, BUF_SIZE, stdin);
        buffer[strlen(buffer) - 1] = '\0'; // Удаляем символ новой строки

        write(sock, buffer, strlen(buffer));

        if (strcmp(buffer, THE_END) == 0) {
            break;
        }
    }

    close(sock);

    return 0;
}
