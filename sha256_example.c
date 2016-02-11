#include "sha256.h"
#include <stdio.h>

static void test_sha256(void);

int main(void)
{
    test_sha256();
    return 0;
}

static void test_sha256(void)
{
    sha256_context ctx;
    int c;
    uint8_t obuf[32];

    sha256_starts(&ctx);

    for (;;) {
        uint8_t tmpbuf[64];
        size_t nbytes = fread(tmpbuf, 1, 64, stdin);
        if (nbytes == 0)
            break;
        sha256_update(&ctx, tmpbuf, nbytes);
    }

    sha256_finish(&ctx, obuf);
    for (c = 0; c < 32; ++c) {
        printf("%02x", obuf[c]);
    }
    printf("\n");
}
