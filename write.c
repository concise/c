#include "read_write_hack.h"

int main(void)
{
    unsigned char data[6] = {'H', 'e', 'l', 'l', 'o', '!'};

    WRITE_TO("/tmp/fifo", data, 6, {
        printf("write ok\n");
    }, {
        printf("write ng\n");
    });

    return 0;
}
