/*
 * This file provides a 64-bit C implementation of SHA-256, assuming we have
 * the "unsigned long" and "unsigned long long" C data types directly.
 *
 * The algorithm of SHA-256 is specified in FIPS 180-4 published by NIST
 * http://csrc.nist.gov/publications/fips/fips180-4/fips-180-4.pdf
 */

#include "sha256.h"

static const unsigned long K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b,
    0x59f111f1, 0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01,
    0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7,
    0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 0x983e5152,
    0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
    0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc,
    0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819,
    0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116, 0x1e376c08,
    0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f,
    0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

void sha256_init(sha256_context *ctx)
{
    if (!ctx) {
        return;
    }

    ctx->H[0] = 0x6a09e667;
    ctx->H[1] = 0xbb67ae85;
    ctx->H[2] = 0x3c6ef372;
    ctx->H[3] = 0xa54ff53a;
    ctx->H[4] = 0x510e527f;
    ctx->H[5] = 0x9b05688c;
    ctx->H[6] = 0x1f83d9ab;
    ctx->H[7] = 0x5be0cd19;

    ctx->nbytes = 0;
}

static void sha256_step(sha256_context *ctx)
{
    // TODO message schedule
    // TODO initialize 8 working variables
    // TODO repeat some statements 64 times
    // TODO update the H0 ~ H7 intermediate hash value
}

void sha256_feed(
        sha256_context *ctx, unsigned long long ilen, const void *ibuf)
{
    const unsigned char *msg_chunk_ptr = ibuf;
    unsigned long long i;

    if (!ctx || !ibuf) {
        return;
    }

    for (i = 0; i < ilen && ctx->nbytes < 0x1fffffffffffffffull; ++i) {
        ctx->msgbuffer[ctx->nbytes % 64] = msg_chunk_ptr[i];
        ctx->nbytes += 1;
        if (ctx->nbytes % 64 == 0) {
            sha256_step(ctx);
        }
    }
}

void sha256_done(sha256_context *ctx, void *obuf)
{
    unsigned char *output_buffer = (unsigned char *) obuf;

    if (!ctx || !obuf) {
        return;
    }

    // TODO now we know the bit length of the input
    // TODO add padding bytes after the last inputted message block
    // TODO execute one more step
    // TODO return H0 ~ H7
}
