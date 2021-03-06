#include "hmac.h"
#include "hmacsha256.h"
#include <stdio.h>

static void dump_buffer(int length, const unsigned char *ptr)
{
    int i;
    for (i = 0; i < length; ++i) {
        printf("%02x", ptr[i]);
    }
    printf("\n");
}

static void example_usage_1(
    int keylen, const unsigned char *key,
    int msglen, const unsigned char *msg,
    unsigned char *out)
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

    hmac_begin(&hmac_ctx, keylen, key);
    hmac_update(&hmac_ctx, msglen, msg);
    hmac_output(&hmac_ctx, out);
}

static void example_usage_2(
    int keylen, const unsigned char *key,
    int msglen, const unsigned char *msg,
    unsigned char *out)
{
    hmacsha256_context_t hmacsha256_ctx;
    hmacsha256_init(&hmacsha256_ctx);
    hmac_begin(&hmacsha256_ctx, keylen, key);
    hmac_update(&hmacsha256_ctx, msglen, msg);
    hmac_output(&hmacsha256_ctx, out);

    //
    // OR:
    //

    // hmacsha256_context_t hmacsha256_ctx;
    // hmacsha256_init(&hmacsha256_ctx);
    // hmac_begin(&hmacsha256_ctx.hmac_ctx, keylen, key);
    // hmac_update(&hmacsha256_ctx.hmac_ctx, msglen, msg);
    // hmac_output(&hmacsha256_ctx.hmac_ctx, out);
}

static void example_usage_3(
    int keylen, const unsigned char *key,
    int msglen, const unsigned char *msg,
    unsigned char *out)
{
    hmacsha256(keylen, key, msglen, msg, out);
}

int main(void)
{
    unsigned char result[SHA256_OUTPUT_SIZE];

    example_usage_1(0, 0, 0, 0, result);
    dump_buffer(SHA256_OUTPUT_SIZE, result);
    // b613679a0814d9ec772f95d778c35fc5ff1697c493715653c6c712144292c5ad

    example_usage_2(0, 0, 0, 0, result);
    dump_buffer(SHA256_OUTPUT_SIZE, result);
    // b613679a0814d9ec772f95d778c35fc5ff1697c493715653c6c712144292c5ad

    example_usage_3(0, 0, 0, 0, result);
    dump_buffer(SHA256_OUTPUT_SIZE, result);
    // b613679a0814d9ec772f95d778c35fc5ff1697c493715653c6c712144292c5ad

    example_usage_3(
        20, (const unsigned char *) &(struct { unsigned char _[20]; }){
            "\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b"
            "\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b" },
        8, (const unsigned char *) &(struct { unsigned char _[8]; }){
            "Hi There" },
        result
    );
    dump_buffer(SHA256_OUTPUT_SIZE, result);
    // b0344c61d8db38535ca8afceaf0bf12b881dc200c9833da726e9376c2e32cff7

    return 0;
}

