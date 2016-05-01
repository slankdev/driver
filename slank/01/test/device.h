
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


class device {
    private:
        int fd;
    public:
        
        ~device()
        {
            close();
        }

        void open(const char* path, int oflag)
        {
            fd = ::open(path, oflag);
            printf("open: %d is fd \n", fd);
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
