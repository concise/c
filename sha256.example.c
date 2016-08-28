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
    sha256_context_t ctx;
    int c;
    uint8_t obuf[32];

    sha256_begin(&ctx);

    for (;;) {
        uint8_t tmpbuf[64];
        int nbytes = (int) fread(tmpbuf, 1, 64, stdin);
        if (nbytes == 0)
            break;
        sha256_update(&ctx, nbytes, tmpbuf);
    }

    sha256_output(&ctx, obuf);
    for (c = 0; c < 32; ++c) {
        printf("%02x", obuf[c]);
    }
    printf("\n");
}
