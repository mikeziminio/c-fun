#include "common.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define print_error(msg) fprintf(stderr, "%s\n%s\n", msg, strerror(errno))

int main()
{
    int r;

    // Получаем файловый дескриптор серверного сокета
    int client_socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_socket_fd == -1) {
        print_error_and_exit(errno, "Не удалось открыть сокет");
        exit(1);
    }

    const char* host = "127.0.0.1";
    const in_port_t port = 8188;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(host);
    addr.sin_port = port;

    // Коннектимся к серверному сокету
    r = connect(client_socket_fd, (struct sockaddr*)&addr, sizeof(addr));
    if (r == -1) {
        print_error_and_exit(errno, "Не удалось подсоединиться к серверному сокету по адресу %s:%d", host, port);
    }

    fprintf(stdout, "Удалось подключиться к серверному сокету по адресу %s:%d\n", host, port);

    close(client_socket_fd);
    if (r == -1) {
        print_error_and_exit(errno, "Не удалось закрыть клиентский сокет");
    }

    char* buf = malloc(1000);
    read(client_socket_fd, buf, 1000);
    puts(buf);
    free(buf);

    return 0;
}
