#include "sha256.h"
#include "hexcodec.h"
#include <stdio.h>

void foo(void)
{
    int c = 0;
    sha256_context ctx;
    unsigned char obuf[32];
    char readable_obuf[64];
    unsigned char tmp;

    sha256_init(&ctx);

    while ((c = fgetc(stdin)) != EOF) {
        tmp = c;
        sha256_feed(&ctx, 1, &tmp);
    }

    sha256_done(&ctx, obuf);

    hexcodec_encode(32, obuf, readable_obuf);
    printf("%64.64s\n", readable_obuf);
}

int main(void)
{
    foo();
    return 0;
}
