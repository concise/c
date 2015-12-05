#include <stdio.h>
#include "read_write_hack.h"

int main(void)
{
        unsigned char data[16384];
        size_t dlen;
        FILE *fp;
        int status;

        fp = fopen("/tmp/fifo", "r");
        if (!fp) {
                printf("open file error\n");
                return -1;
        }

        status = read_from_file_stream(fp, data, 16384, &dlen);
        if (status < 0) {
                printf("error\n");
                return -1;
        }

        printf("%zu bytes\n", dlen);

        fclose(fp);

        return 0;
}
