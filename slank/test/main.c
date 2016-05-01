
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

const char* path = "/dev/slank0";
/* const char* path = "/dev/tty"; */

int main()
{
    int fd = open(path, O_RDWR);
    printf("fd: %d \n", fd);
    if (fd < 0) {
        perror("open");
        return -1;
    }

    close(fd);
}
