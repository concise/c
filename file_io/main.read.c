#include "hack.h"

int main(void)
{
    READ_FROM("/tmp/fifo", data, dlen, {
        printf("We have read %zu bytes\n", dlen);
        for (size_t i = 0; i < dlen; ++i) {
            printf("%02x", data[i]);
        }
        printf("\n");
    }, {
        printf("Fail\n");
    });

    return 0;
}
