
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

const char* path = "/dev/slank";

class device {
    private:
        int fd;
    public:
        void open(const char* path, int oflag)
        {
            fd = ::open(path, oflag);
            if (fd < 0) {
                perror("open");
                exit(-1);
            }
        }
        void close()
        {
            if (fd >= 0) {
                ::close(fd);
            }
        }
        void write(const void* buf, size_t nbyte)
        {
            ssize_t writelen = ::write(fd, buf, nbyte);
            if (writelen < 0) {
                perror("write");
                exit(-1);
            }
        }
        ssize_t read(void* buf, size_t nbyte)
        {
            ssize_t readlen = ::read(fd, buf, nbyte);
            if (readlen < 0) {
                perror("read");
                exit(-1);
            }
        }
};


int main()
{
    int fd = open(path, O_RDWR);
    printf("open: %d is fd \n", fd);
    if (fd < 0) {
        perror("open");
        return -1;
    }

    uint8_t buf[10];
    memset(buf, 0xff, sizeof buf);
    ssize_t res = write(fd, buf, sizeof(buf));
    printf("write: %zd byte wrote \n", res);
    if (res < 0) {
        perror("write");
        exit(-1);
    }

    memset(buf, 0, sizeof buf);
    res = read(fd, buf, sizeof(buf));
    printf("read: %zd byte read \n", res);
    if (res < 0) {
        perror("read");
        exit(-1);
    }


    close(fd);
}
