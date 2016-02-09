#include "sha256.h"
#include <stdio.h>

void test_sha256(void);
void super_sha256(void);

int main(void)
{
    test_sha256();
    /* super_sha256(); */
    return 0;
}

void test_sha256(void)
{
    int c;
    uint8_t obuf[32];
    sha256_context_t ctx;

    sha256_begin(&ctx);
    while ((c = fgetc(stdin)) != EOF) {
        sha256_update(&ctx, 1, (const uint8_t *) &c);
    }
    sha256_output(&ctx, obuf);

    for (c = 0; c < 32; ++c) {
        printf("%02x", obuf[c]);
    }
    printf("\n");
}

void super_sha256(void)
{
    uint32_t i;
    uint8_t obuf[32];
    sha256_context_t ctx;
    uint8_t test_vector[64] = "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmno";

    /* The input length is 1 GiB */
    sha256_begin(&ctx);
    for (i = 1; i <= 16777216; ++i) {
        sha256_update(&ctx, 64, test_vector);
    }
    sha256_output(&ctx, obuf);

    for (i = 0; i < 32; ++i) {
        printf("%02x", obuf[i]);
    }
    printf("\n");
}
