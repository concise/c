#include "sha256.h"
#include <stdio.h>

void super_sha256(void)
{
    unsigned long int i;
    unsigned char obuf[32];
    sha256_context_t ctx;
    unsigned char test_vector[64] = "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmno";

    /* The input length is 1 GiB */
    sha256_starts(&ctx);
    for (i = 1; i <= 16777216; ++i) {
        sha256_update(&ctx, 64, test_vector);
    }
    sha256_finish(&ctx, obuf);

    for (i = 0; i < 32; ++i) {
        printf("%02x", obuf[i]);
    }
    printf("\n");
}

void test_sha256(void)
{
    int c;
    unsigned char obuf[32];
    sha256_context_t ctx;

    sha256_starts(&ctx);
    while ((c = fgetc(stdin)) != EOF) {
        sha256_update(&ctx, 1, (const unsigned char *) &c);
    }
    sha256_finish(&ctx, obuf);

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
