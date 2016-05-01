
#include "device.h"
#include <pgen2.h>

const char* path = "/dev/slank";


int main()
{
    size_t res;
    uint8_t buf[10000];
    device dev;
    
    dev.open(path, O_RDWR);

    memset(buf, 0, sizeof buf);
    res = dev.read(buf, sizeof(buf));
    pgen::hex(buf, res);

}
