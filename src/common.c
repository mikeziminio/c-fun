#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_error_and_exit(int e, char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    fprintf(stderr, "\n%s\n", strerror(e));
    exit(1);
}