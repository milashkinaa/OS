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
    int serv_sock, clnt_sock1, clnt_sock2;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;
    char buffer[BUF_SIZE];
    int str_len;

    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) {
        perror("socket() error");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("bind() error");
        exit(1);
    }

    if (listen(serv_sock, 2) == -1) {
        perror("listen() error");
        exit(1);
    }

    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock1 = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
    clnt_sock2 = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);

    while (1) {
        str_len = read(clnt_sock1, buffer, BUF_SIZE);
        buffer[str_len] = '\0';
        write(clnt_sock2, buffer, str_len);

        if (strcmp(buffer, THE_END) == 0) {
            break;
        }
    }

    close(clnt_sock1);
    close(clnt_sock2);
    close(serv_sock);

    return 0;
}
