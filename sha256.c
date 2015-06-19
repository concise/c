/*
 * This file provides a 64-bit C implementation of SHA-256, assuming we have
 * the "unsigned long" and "unsigned long long" C data types directly.
 *
 * The algorithm of SHA-256 is specified in FIPS 180-4 published by NIST
 * http://csrc.nist.gov/publications/fips/fips180-4/fips-180-4.pdf
 */

/*
 * TODO do not use "unsigned long long"
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

void sha256_init(sha256_context *x)
{
    if (!x) {
        return;
    }

    x->H[0] = 0x6a09e667;
    x->H[1] = 0xbb67ae85;
    x->H[2] = 0x3c6ef372;
    x->H[3] = 0xa54ff53a;
    x->H[4] = 0x510e527f;
    x->H[5] = 0x9b05688c;
    x->H[6] = 0x1f83d9ab;
    x->H[7] = 0x5be0cd19;

    x->L = 0;
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

static void sha256_step(sha256_context *x, const unsigned char *M)
{
    unsigned long W[64];
    unsigned long a, b, c, d, e, f, g, h, T1, T2;
    int t;

    if (!x) {
        return;
    }

    /* 1. Prepare the message schedule */
    for (t = 0; t <= 15; ++t) {
        W[t] = (M[t * 4    ] << 24) | (M[t * 4 + 1] << 16) |
               (M[t * 4 + 2] <<  8) | (M[t * 4 + 3]      );
    }
    for (t = 16; t <= 63; ++t) {
        W[t] = s1(W[t - 2]) + W[t - 7] + s0(W[t - 15]) + W[t - 16];
    }

    /* 2. Initialize the eight working variables */
    a = x->H[0];
    b = x->H[1];
    c = x->H[2];
    d = x->H[3];
    e = x->H[4];
    f = x->H[5];
    g = x->H[6];
    h = x->H[7];

    /* 3. repeat some statements 64 times */
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

    /* 4. update the H0 ~ H7 intermediate hash value */
    x->H[0] += a;
    x->H[1] += b;
    x->H[2] += c;
    x->H[3] += d;
    x->H[4] += e;
    x->H[5] += f;
    x->H[6] += g;
    x->H[7] += h;
}

void sha256_feed(sha256_context *x, unsigned long long ilen, const void *ibuf)
{
    const unsigned char *msg_chunk_ptr = ibuf;
    unsigned long long i;

    if (!x || !ibuf) {
        return;
    }

    for (i = 0; i < ilen && x->L < 0x1fffffffffffffffull; ++i) {
        x->M[x->L % 64] = msg_chunk_ptr[i];
        x->L += 1;
        if (x->L % 64 == 0) {
            sha256_step(x, x->M);
        }
    }
}

void sha256_done(sha256_context *x, void *obuf)
{
    /* TODO Refactor to accept a (const sha256_context *) pointer */

    unsigned char *output_buffer = obuf;
    unsigned char pending_nbytes;
    int i;

    if (!x || !obuf) {
        return;
    }

    pending_nbytes = x->L % 64;

    if (pending_nbytes >= 56) {
        x->M[pending_nbytes] = 0x80;
        pending_nbytes += 1;
        while (pending_nbytes < 64) {
            x->M[pending_nbytes] = 0x00;
            pending_nbytes += 1;
        }
        sha256_step(x, x->M);
        pending_nbytes = 0;
    } else {
        x->M[pending_nbytes] = 0x80;
        pending_nbytes += 1;
    }

    while (pending_nbytes < 56) {
        x->M[pending_nbytes] = 0x00;
        pending_nbytes += 1;
    }
    x->M[pending_nbytes    ] = ((8 * x->L) >> 56) % 256;
    x->M[pending_nbytes + 1] = ((8 * x->L) >> 48) % 256;
    x->M[pending_nbytes + 2] = ((8 * x->L) >> 40) % 256;
    x->M[pending_nbytes + 3] = ((8 * x->L) >> 32) % 256;
    x->M[pending_nbytes + 4] = ((8 * x->L) >> 24) % 256;
    x->M[pending_nbytes + 5] = ((8 * x->L) >> 16) % 256;
    x->M[pending_nbytes + 6] = ((8 * x->L) >>  8) % 256;
    x->M[pending_nbytes + 7] = ((8 * x->L)      ) % 256;
    sha256_step(x, x->M);

    /* return MSB-first 32-byte result { H0, H1, ..., H7 } */
    for (i = 0; i <= 7; ++i) {
        output_buffer[8 * i    ] = (x->H[i] >> 24);
        output_buffer[8 * i + 1] = (x->H[i] >> 16);
        output_buffer[8 * i + 2] = (x->H[i] >>  8);
        output_buffer[8 * i + 3] = (x->H[i]      );
    }
}
