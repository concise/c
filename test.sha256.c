#include "sha256.h"
#include <stdio.h>

void test_sha256(void)
{
    int c;
    U8 obuf[32];
    sha256_context ctx;

    sha256_init(&ctx);
    while ((c = fgetc(stdin)) != EOF) {
        sha256_feed(&ctx, 1, (const U8 *) &c);
    }
    sha256_done(&ctx, obuf);

    for (c = 0; c < 32; ++c) {
        printf("%02x", obuf[c] & 0xff);
    }
    printf("\n");
}

int main(void)
{
    test_sha256();
    return 0;
}
