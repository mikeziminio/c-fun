#include "common.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <unistd.h>

int main()
{
    int r;

    // poll - опрос
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        print_error_and_exit(errno, "Не удалось создать файловый дескриптор epoll");
    }

    fprintf(stdout, "Создан файловый дескриптор через epoll_create1\n");

    struct epoll_event event, events[5];

    event.events = EPOLLIN;
    event.data.fd = 0; // stdin

    r = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, 0, &event);
    if (r == -1) {
        print_error_and_exit(errno, "Не удалось добавить дескриптор %d в epoll", 0);
    }
    printf("Дескриптор %d добавлен в epoll\n", 0);

    while (1) {
        printf("Waiting...\n");
        int event_count = epoll_wait(epoll_fd, events, 5, -1);
        if (event_count == -1) {
            print_error_and_exit(errno, "Вызов epoll_wait не удался");
        }
        printf("epoll_wait вернул %d событий\n", event_count);

        char buf[200 + 1];

        for (int i = 0; i < event_count; i++) {
            ssize_t size = read(events[0].data.fd, buf, 200);
            if (size == -1) {
                print_error_and_exit(errno, "Не удалось");
            }
            buf[size] = '\0'; // Делаем строку
            printf("Прочитано %ld байт: %s\n", size, buf);
        }
    }

    r = close(epoll_fd);
    if (r == -1) {
        print_error_and_exit(errno, "Не удалось закрыть файловый дескриптор epoll");
    }
}