/*
 * This file I provide an example of combining the wrapper interface of hmac.c
 * and the hash implementation of sha256.c into an HMAC_SHA256 implementation.
 */



/* ----- hmac_sha256.h (C header file for developers) ----- */

#include "hmac.h"
#include "sha256.h"

typedef struct {
    hmac_context_t hmac_ctx;
    sha256_context_t sha256_ctx;
    unsigned char ibuf[SHA256_IBLOCK_SIZE];
    unsigned char obuf[SHA256_OUTPUT_SIZE];
} hmac_sha256_context_t;

void hmac_sha256_begin(hmac_sha256_context_t *, int, const unsigned char *);
void hmac_sha256_update(hmac_sha256_context_t *, int, const unsigned char *);
void hmac_sha256_output(hmac_sha256_context_t *, unsigned char *);



/* ----- hmac_sha256.c (implementation details) ----- */

void hmac_sha256_begin(
        hmac_sha256_context_t *ctx, int keylen, const unsigned char *key)
{
    if (!ctx) {
        return;
    }

    ctx->hmac_ctx.hash_context   = &ctx->sha256_ctx;
    ctx->hmac_ctx.hash_begin     = &sha256_begin;
    ctx->hmac_ctx.hash_update    = &sha256_update;
    ctx->hmac_ctx.hash_output    = &sha256_output;
    ctx->hmac_ctx.B              = SHA256_IBLOCK_SIZE;
    ctx->hmac_ctx.L              = SHA256_OUTPUT_SIZE;
    ctx->hmac_ctx.workingBufferB = ctx->ibuf;
    ctx->hmac_ctx.workingBufferL = ctx->obuf;

    hmac_begin(&ctx->hmac_ctx, keylen, key);
}

void hmac_sha256_update(
        hmac_sha256_context_t *ctx, int msglen, const unsigned char *msg)
{
    if (!ctx) {
        return;
    }

    hmac_update(&ctx->hmac_ctx, msglen, msg);
}

void hmac_sha256_output(
        hmac_sha256_context_t *ctx, unsigned char *out)
{
    if (!ctx) {
        return;
    }

    hmac_output(&ctx->hmac_ctx, out);
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

void example_usage_1(void);

void example_usage_2(void);

int main(void)
{
    example_usage_1();
    example_usage_2();

    // HMAC-SHA-256("", "") is
    // b613679a0814d9ec772f95d778c35fc5ff1697c493715653c6c712144292c5ad

    return 0;
}

void example_usage_1(void)
{
    unsigned char result[SHA256_OUTPUT_SIZE];

    hmac_sha256_context_t hmac_sha256_ctx;
    hmac_sha256_begin(&hmac_sha256_ctx, 0, 0);
    hmac_sha256_update(&hmac_sha256_ctx, 0, 0);
    hmac_sha256_output(&hmac_sha256_ctx, result);

    dump_buffer(SHA256_OUTPUT_SIZE, result);
}

void example_usage_2(void)
{
    unsigned char result[SHA256_OUTPUT_SIZE];

    {
        hmac_context_t hmac_ctx;
        sha256_context_t sha256_ctx;
        unsigned char ibuf[SHA256_IBLOCK_SIZE];
        unsigned char obuf[SHA256_OUTPUT_SIZE];

        hmac_ctx.hash_context   = &sha256_ctx;
        hmac_ctx.hash_begin     = &sha256_begin;
        hmac_ctx.hash_update    = &sha256_update;
        hmac_ctx.hash_output    = &sha256_output;
        hmac_ctx.B              = SHA256_IBLOCK_SIZE;
        hmac_ctx.L              = SHA256_OUTPUT_SIZE;
        hmac_ctx.workingBufferB = ibuf;
        hmac_ctx.workingBufferL = obuf;

        hmac_begin(&hmac_ctx, 0, 0);
        hmac_update(&hmac_ctx, 0, 0);
        hmac_output(&hmac_ctx, result);
    }

    dump_buffer(SHA256_OUTPUT_SIZE, result);
}
