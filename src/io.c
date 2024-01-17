#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {

    int fd;
    char file_content[] = "hola a todos";
    char text[] = "The most interesting\n";
    void* mem = malloc(20);
    int rb;

    fd = open("text.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    
    if (fd == -1) {
        printf("Ошибка открытия файла\n");
    }

    write(fd, file_content, strlen(file_content));

    // Вывод в std out (файловый дескриптор #1)
    write(1, text, strlen(text));

    // Чтение из буффера
    while (1) {
        rb = read(0, mem, 20);
        if (rb > 0) break;
    }

    printf("Произошёл ввод \n");

    free(mem);
    close(fd);

    return 0;
}