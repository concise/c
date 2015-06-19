#include "sha256.h"
#include <stdio.h>

void super_sha256(void)
{
    /* The input length is 1 GiB */
    U32 i;
    U8 obuf[32];
    sha256_context ctx;
    U8 test_vector[64] = "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmno";

    sha256_init(&ctx);
    for (i = 1; i <= 16777216; ++i) {
        sha256_feed(&ctx, 64, test_vector);
    }
    sha256_done(&ctx, obuf);

    for (i = 0; i < 32; ++i) {
        printf("%02x", obuf[i]);
    }
    printf("\n");
}

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
        printf("%02x", obuf[c]);
    }
    printf("\n");
}

int main(void)
{
    test_sha256();
    /* super_sha256(); */
    return 0;
}
