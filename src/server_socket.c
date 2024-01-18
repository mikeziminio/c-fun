#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define print_error(msg) fprintf(stderr, "%s\n%s\n", msg, strerror(errno))

int main()
{
    int r;

    // Получаем файловый дескриптор серверного сокета
    int server_socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket_fd == -1) {
        print_error("Не удалось открыть сокет");
        exit(1);
    }

    const int enabled = 1;

    // Устанавливаем опции серверного сокета
    r = setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &enabled,
        sizeof(enabled));
    if (r == -1) {
        print_error("Не удалось установить опцию сокета SO_REUSEADDR");
        exit(1);
    }

    // Добавить SO_REUSEPORT

    const char* host = "127.0.0.1";
    const in_port_t port = 8099;

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(host);
    server_addr.sin_port = port;

    // Привязываем этот файловый дескриптор к адресу
    r = bind(server_socket_fd, (struct sockaddr*)&server_addr,
        sizeof(server_addr));
    if (r == -1) {
        char buf[200];
        sprintf((char*)&buf, "Не удалось связать серверный сокет с адресом %s:%d",
            host, port);
        print_error((char*)&buf);
        exit(1);
    }

    fprintf(stdout, "Удалось связать серверный сокет с адресом %s:%d", host, port);

    struct sockaddr_in client_addr;
    socklen_t client_addr_len;

    // Ожидаем подключение клиента и в случае успеха - получаем сокет соединения
    int connection_socket_fd = accept(server_socket_fd, (struct sockaddr*)&client_addr, &client_addr_len);

    return 0;
}
