#include <stdio.h>
#include "read_write_hack.h"

int main(void)
{
        unsigned char data[6] = {'H', 'e', 'l', 'l', 'o', '!'};
        FILE *fp;
        int status;

        fp = fopen("/tmp/fifo", "w");
        if (!fp) {
                printf("open file error\n");
                return -1;
        }

        status = write_to_file_stream(fp, data, 6);
        if (status < 0) {
                printf("error\n");
                return -1;
        }

        fclose(fp);

        return 0;
}
