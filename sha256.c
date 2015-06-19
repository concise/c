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

#define ROTL(n, x)      (((x) << (n)) | ((x) >> (32 - (n))))
#define ROTR(n, x)      (((x) >> (n)) | ((x) << (32 - (n))))
#define SHR(n, x)       ((x) >> (n))
#define Ch(x, y, z)     (((x) & (y)) ^ (~(x) & (z)))
#define Maj(x, y, z)    (((x) & (y)) ^ ((x) ^ (z)) ^ ((y) & (z)))
#define S0(x)           (ROTR(2, (x)) ^ ROTR(13, (x)) ^ ROTR(22, (x)))
#define S1(x)           (ROTR(6, (x)) ^ ROTR(11, (x)) ^ ROTR(25, (x)))
#define s0(x)           (ROTR(7, (x)) ^ ROTR(18, (x)) ^ SHR(3, (x)))
#define s1(x)           (ROTR(17, (x)) ^ ROTR(19, (x)) ^ SHR(10, (x)))

static void sha256_step(sha256_context *ctx)
{
    unsigned long W[64];
    unsigned long a, b, c, d, e, f, g, h, T1, T2;
    unsigned long *H;
    unsigned char *M;
    int t;

    if (!ctx) {
        return;
    }

    H = ctx->H;         // uint32_t[8]
    M = ctx->msgbuffer; // uint8_t[64]

    // 1. Prepare the message schedule
    for (t = 0; t <= 15; ++t) {
        W[t] = (M[t * 4    ] << 24) |
               (M[t * 4 + 1] << 16) |
               (M[t * 4 + 2] <<  8) |
               (M[t * 4 + 3]      );
    }
    for (t = 16; t <= 63; ++t) {
        W[t] = s1(W[t - 2]) + W[t - 7] + s0(W[t - 15]) + W[t - 16];
    }

    // 2. Initialize the eight working variables
    a = H[0];
    b = H[1];
    c = H[2];
    d = H[3];
    e = H[4];
    f = H[5];
    g = H[6];
    h = H[7];

    // 3. repeat some statements 64 times
    for (t = 0; t < 64; ++t) {
        T1 = h + S1(e) + Ch(e, f, g) + K[t] + W[t];
        T2 = S0(a) + Maj(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + T1;
        d = c;
        c = b;
        b = a;
        a = T1 + T2;
    }

    // 4. update the H0 ~ H7 intermediate hash value
    H[0] += a;
    H[1] += b;
    H[2] += c;
    H[3] += d;
    H[4] += e;
    H[5] += f;
    H[6] += g;
    H[7] += h;
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
    unsigned char *output_buffer = obuf;
    unsigned char pending_nbytes;

    if (!ctx || !obuf) {
        return;
    }

    pending_nbytes = ctx->nbytes % 64;

    if (pending_nbytes <= 55) {
        // Put one 0x80 byte and (55 - pending_nbytes) 0x00 bytes
        // Increase pending_nbytes 1 by 1
    } else {
        // Put one 0x80 byte and (119 - pending_nbytes) 0x00 bytes
        // Increase pending_nbytes 1 by 1
        // Invoke sha256_step() when pending_nbytes is zero after %= 64
    }
    // Put 8 bytes that is MSB-first 64-bit integer of (8 * ctx->nbytes)
    // Invoke sha256_step()

    // return MSB first 32-byte buffer { H0, H1, ..., H7 }
    (void) output_buffer;
}
