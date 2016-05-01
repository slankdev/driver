
#include "device.h"
#include <pgen2.h>

const char* path = "/dev/slank";

int main()
{
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



}
