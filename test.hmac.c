#include "hmac.h"
#include "sha256.h"
#include <stdio.h>

int main(void)
{
    sha256_context sha256_ctx;
    unsigned char bufferB[64];
    unsigned char bufferL[32];
    sha_info_t sha_info;
    unsigned char obuf[32];
    int i;

    sha_info.sha_context = &sha256_ctx;
    sha_info.sha_starts  = &sha256_starts;
    sha_info.sha_update  = &sha256_update;
    sha_info.sha_finish  = &sha256_finish;
    sha_info.bufferB     = bufferB;
    sha_info.bufferL     = bufferL;
    sha_info.B           = 64;
    sha_info.L           = 32;


    hmac(&sha_info, 0, 0, 0, 0, obuf);

    for (i = 0; i < 32; ++i) {
        printf("%02x", obuf[i]);
    }
    printf("\n");

    return 0;
}
