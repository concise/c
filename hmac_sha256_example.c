/*
 * This file I provide an example of combining the wrapper interface of hmac.c
 * and the hash implementation of sha256.c into an HMAC_SHA256 implementation.
 */



/* ----- hmac_sha256.h (C header file for developers) ----- */

#include "hmac.h"
#include "sha256.h"

typedef struct {
    hash_info_t hash_info;
    sha256_context_t sha256_ctx;
    unsigned char bufferBarr[SHA256_BLOCK_SIZE];
    unsigned char bufferLarr[SHA256_OUTPUT_SIZE];
} hmac_sha256_context_t;

void hmac_sha256_starts(hmac_sha256_context_t *, int, const unsigned char *);
void hmac_sha256_update(hmac_sha256_context_t *, int, const unsigned char *);
void hmac_sha256_finish(hmac_sha256_context_t *, unsigned char *);
void hmac_sha256       (hmac_sha256_context_t *, int, const unsigned char *,
                                                 int, const unsigned char *,
                                                 unsigned char *);



/* ----- hmac_sha256.c (implementation details) ----- */

void hmac_sha256_starts(
        hmac_sha256_context_t *ctx, int keylen, const unsigned char *key)
{
    if (!ctx) {
        return;
    }

    ctx->hash_info.hash_context = &ctx->sha256_ctx;
    ctx->hash_info.hash_starts  = &sha256_starts;
    ctx->hash_info.hash_update  = &sha256_update;
    ctx->hash_info.hash_finish  = &sha256_finish;
    ctx->hash_info.bufferB      = ctx->bufferBarr;
    ctx->hash_info.bufferL      = ctx->bufferLarr;
    ctx->hash_info.B            = SHA256_BLOCK_SIZE;
    ctx->hash_info.L            = SHA256_OUTPUT_SIZE;

    hmac_starts(&ctx->hash_info, keylen, key);
}

void hmac_sha256_update(
        hmac_sha256_context_t *ctx, int msglen, const unsigned char *msg)
{
    if (!ctx) {
        return;
    }

    hmac_update(&ctx->hash_info, msglen, msg);
}

void hmac_sha256_finish(
        hmac_sha256_context_t *ctx, unsigned char *out)
{
    if (!ctx) {
        return;
    }

    hmac_finish(&ctx->hash_info, out);
}

void hmac_sha256(
        hmac_sha256_context_t *ctx,
        int keylen, const unsigned char *key,
        int msglen, const unsigned char *msg,
        unsigned char *out)
{
    hmac_sha256_starts(ctx, keylen, key);
    hmac_sha256_update(ctx, msglen, msg);
    hmac_sha256_finish(ctx, out);
}



/* ----- main.c (example usage) ----- */

#include <stdio.h>

static void dump_buffer(int length, const unsigned char *ptr)
{
    int i;
    for (i = 0; i < length; ++i) {
        printf("%02x", ptr[i]);
    }
    printf("\n");
}

int main(void)
{
    hmac_sha256_context_t hmac_sha256_ctx;
    unsigned char obuf[SHA256_OUTPUT_SIZE];

    /*
     * Compute HMAC_SHA256(key, msg) directly
     */
    hmac_sha256(&hmac_sha256_ctx, 0, 0, 0, 0, obuf);
    dump_buffer(SHA256_OUTPUT_SIZE, obuf);

    /*
     * Three-stage style:
     *
     *      (1) Start with key
     *      (2) Update with message chunks
     *      (3) Output
     */
    hmac_sha256_starts(&hmac_sha256_ctx, 0, 0);
    hmac_sha256_update(&hmac_sha256_ctx, 0, 0);
    hmac_sha256_finish(&hmac_sha256_ctx, obuf);
    dump_buffer(SHA256_OUTPUT_SIZE, obuf);

    /*
     * The results will be printed to stdout and both of them should be
     * b613679a0814d9ec772f95d778c35fc5ff1697c493715653c6c712144292c5ad
     */

    return 0;
}
