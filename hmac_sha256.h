#ifndef __HMAC_SHA256_H__
#define __HMAC_SHA256_H__

#include "sha256.h"

typedef struct {
    sha256_context ctx;
    U8 bufferB[64];
    U8 bufferL[32];
} hmac_sha256_context;

void hmac_sha256_starts(hmac_sha256_context *);
void hmac_sha256_update(hmac_sha256_context *, U32, const U8 *);
void hmac_sha256_finish(const hmac_sha256_context *, U8 *);
void hmac_sha256(U32, const U8 *, U32, const U8 *, U8 *);

#endif
