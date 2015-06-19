#include "hmac.h"
#include "sha256.h"
#include <stdio.h>

int main(void)
{
    sha256_context sha256_ctx;
    sha_info_t hmac_sha256_ctx;
    unsigned char bufferB[64];
    unsigned char bufferL[32];
    unsigned char obuf[32];
    int i;

    hmac_sha256_ctx.sha_context = &sha256_ctx;
    hmac_sha256_ctx.sha_starts  = &sha256_starts;
    hmac_sha256_ctx.sha_update  = &sha256_update;
    hmac_sha256_ctx.sha_finish  = &sha256_finish;
    hmac_sha256_ctx.bufferB     = bufferB;
    hmac_sha256_ctx.bufferL     = bufferL;
    hmac_sha256_ctx.B           = 64;
    hmac_sha256_ctx.L           = 32;
    hmac(&hmac_sha256_ctx, 0, 0, 0, 0, obuf);

    for (i = 0; i < 32; ++i) {
        printf("%02x", obuf[i]);
    }
    printf("\n");

    return 0;
}
