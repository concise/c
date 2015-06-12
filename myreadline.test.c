#include <stdio.h> // printf fflush stdout
#include "myreadline.h"

static void prompt(void)
{
    printf(">>> ");
    fflush(stdout);
}

int main()
{
    int ret;
    unsigned char buf[1024];

    prompt();

    while ((ret = myreadline(1024, buf)) >= 0) {
        printf("(%d \"%s\")\n", ret, buf);
        prompt();
    }

    printf("\n");
}
