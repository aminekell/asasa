#include "get_next_line.h"
#include <libc.h>

int main()
{
    int fd = open("file.txt", O_RDWR);

    for (int i = 0; i < 7; i++)
        printf("%s", get_next_line(fd));
    return (0);
}