
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <pgen2.h>

const char* path = "/dev/slank";

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


int main()
{
    size_t res;
    uint8_t buf[10];
    device dev;
    
    dev.open(path, O_RDWR);

    memset(buf, 0xaa, sizeof buf);
    dev.write(buf, sizeof(buf));
    printf("write aa  ");
    pgen::hex(buf, sizeof buf);

    memset(buf, 0xbb, sizeof buf);
    dev.write(buf, sizeof(buf));
    printf("write bb  ");
    pgen::hex(buf, sizeof buf);

    memset(buf, 0xcc, sizeof buf);
    dev.write(buf, sizeof(buf));
    printf("write cc  ");
    pgen::hex(buf, sizeof buf);

    memset(buf, 0, sizeof buf);
    res = dev.read(buf, sizeof(buf));
    pgen::hex(buf, res);

    memset(buf, 0, sizeof buf);
    res = dev.read(buf, sizeof(buf));
    pgen::hex(buf, res);

}
