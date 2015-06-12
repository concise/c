#include <stdio.h> // printf fflush stdout
#include "myreadline.h"

int main(void)
{
    int ret;
    unsigned char buf[10];

loop:
    printf(">>> ");
    fflush(stdout);

read_more:
    // READ
    ret = myreadline(10, buf);
    if (ret < 0) goto end;

    // EVAL & PRINT
    if (buf[ret - 1] != '\n') {
        printf("(%d \"%*.*s\") ", ret, ret, ret, buf);
        goto read_more;
    }
    printf("(%d \"%*.*s\")\n", ret - 1, ret - 1, ret - 1, buf);

    // LOOP
    goto loop;

end:
    printf("\n");
    return 0;
}
