#include "common.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

int main()
{
    int r;

    // Получаем файловый дескриптор серверного сокета
    int server_socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket_fd == -1) {
        print_error_and_exit(errno, "Не удалось открыть сокет");
    }

    const int enabled = 1;

    // Устанавливаем опции серверного сокета
    r = setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &enabled, sizeof(enabled));
    if (r == -1) {
        print_error_and_exit(errno, "Не удалось установить опцию сокета %s", "SO_REUSEADDR");
    }

    // Добавить SO_REUSEPORT

    const char* host = "127.0.0.1";
    const in_port_t port = 8088;

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(host);
    server_addr.sin_port = port;

    // Привязываем этот файловый дескриптор к адресу
    r = bind(server_socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (r == -1) {
        print_error_and_exit(errno, "Не удалось связать серверный сокет с адресом %s:%d", host, port);
    }

    fprintf(stdout, "Серверный сокет связан с адресом %s:%d\n", host, port);

    r = listen(server_socket_fd, 100);
    if (r == -1) {
        print_error_and_exit(errno, "Не удалось установить серверный сокет в режим прослушивания");
    }

    fprintf(stdout, "Серверный сокет установлен в режим прослушивания\n");

    while (1) {

        struct sockaddr_in client_addr;
        socklen_t client_addr_len;

        // Ожидаем подключение клиента и в случае успеха - получаем сокет соединения
        int conn_socket_fd = accept(server_socket_fd, (struct sockaddr*)&client_addr, &client_addr_len);
        if (conn_socket_fd == -1) {
            print_error_and_exit(errno, "Не удалось установить клиентское подключение\n");
        }

        char* client_host = inet_ntoa(client_addr.sin_addr);
        in_port_t client_port = client_addr.sin_port;

        fprintf(stdout, "Установлено клиентское подключение с адреса %s:%d\n", client_host, client_port);
    }

    return 0;
}
