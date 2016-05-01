
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

const char* path = "/dev/slank";

int main()
{
    int fd = open(path, O_RDWR);
    if (fd < 0) {
        perror("open");
        return -1;
    }

    close(fd);
}
