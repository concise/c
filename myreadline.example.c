#include <stdio.h> // printf fflush stdout
#include "myreadline.h"

int main(void)
{
    const unsigned char *buf;
    int blen;
    int ret;

loop:
    // READ
    printf(">>> ");
    fflush(stdout);
    ret = myreadline(&buf, &blen);

    if (ret != 0) {
        printf("\n(myreadline returns nonzero value)\n");
        goto end;
    }

    // EVAL & PRINT
    printf("(%d \"%*.*s\")\n", blen, blen, blen, buf);

    // LOOP
    goto loop;

end:
    printf("\n");
    return 0;
}
