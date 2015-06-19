typedef unsigned char U8;
typedef unsigned long U32;
typedef struct {
    U32 runninghash[8];    /* The intermediate hash value (H0, ..., H7) */
    U32 totalbitlen[2];    /* The bit length of the input message (l)   */
    U8 msgchunk[64];       /* The last unprocessed message chunk        */
    U8 msgchunklen;        /* The byte length of the unprocessed chunk  */
} sha256_context;

void sha256_init(sha256_context *);
void sha256_feed(sha256_context *, U32, const U8 *);
void sha256_done(const sha256_context *, U8 *);
void sha256_hash(U32, const U8 *, U8 *);

#define ROTL(a, b)      (((a) << (b)) | ((a) >> (32-(b))))
#define ROTR(a, b)      (((a) >> (b)) | ((a) << (32-(b))))
#define Ch(x, y, z)     (((x) & (y)) ^ (~(x) & (z)))
#define Maj(x, y, z)    (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define S0(x)           (ROTR(x,  2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define S1(x)           (ROTR(x,  6) ^ ROTR(x, 11) ^ ROTR(x, 25))
#define s0(x)           (ROTR(x,  7) ^ ROTR(x, 18) ^ ((x) >>  3))
#define s1(x)           (ROTR(x, 17) ^ ROTR(x, 19) ^ ((x) >> 10))

static const U32 K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
    0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
    0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
    0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
    0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
    0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

static void copy_context_from_to(
        const sha256_context *src, sha256_context *dst)
{
    U8 i;
    dst->runninghash[0] = src->runninghash[0];
    dst->runninghash[1] = src->runninghash[1];
    dst->runninghash[2] = src->runninghash[2];
    dst->runninghash[3] = src->runninghash[3];
    dst->runninghash[4] = src->runninghash[4];
    dst->runninghash[5] = src->runninghash[5];
    dst->runninghash[6] = src->runninghash[6];
    dst->runninghash[7] = src->runninghash[7];
    dst->totalbitlen[0] = src->totalbitlen[0];
    dst->totalbitlen[1] = src->totalbitlen[1];
    for (i = 0; i < 64; ++i) {
        dst->msgchunk[i] = src->msgchunk[i];
    }
    dst->msgchunklen = src->msgchunklen;
}

static void process_one_block(sha256_context *ctx, const U8 *data)
{
    U32 a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];

    for (i = 0, j = 0; i < 16; ++i, j += 4) {
        m[i] = (data[j    ] << 24) |
               (data[j + 1] << 16) |
               (data[j + 2] <<  8) |
               (data[j + 3]      );
    }
    for ( ; i < 64; ++i) {
        m[i] = (s1(m[i -  2]) + m[i -  7] +
                s0(m[i - 15]) + m[i - 16]) & 0xffffffff;
    }

    a = ctx->runninghash[0];
    b = ctx->runninghash[1];
    c = ctx->runninghash[2];
    d = ctx->runninghash[3];
    e = ctx->runninghash[4];
    f = ctx->runninghash[5];
    g = ctx->runninghash[6];
    h = ctx->runninghash[7];

    for (i = 0; i < 64; ++i) {
        t1 = (h + S1(e) + Ch(e,f,g) + K[i] + m[i]) & 0xffffffff;
        t2 = (S0(a) + Maj(a, b, c)) & 0xffffffff;
        h = g;
        g = f;
        f = e;
        e = (d + t1) & 0xffffffff;
        d = c;
        c = b;
        b = a;
        a = (t1 + t2) & 0xffffffff;
    }

    ctx->runninghash[0] = (a + ctx->runninghash[0]) & 0xffffffff;
    ctx->runninghash[1] = (b + ctx->runninghash[1]) & 0xffffffff;
    ctx->runninghash[2] = (c + ctx->runninghash[2]) & 0xffffffff;
    ctx->runninghash[3] = (d + ctx->runninghash[3]) & 0xffffffff;
    ctx->runninghash[4] = (e + ctx->runninghash[4]) & 0xffffffff;
    ctx->runninghash[5] = (f + ctx->runninghash[5]) & 0xffffffff;
    ctx->runninghash[6] = (g + ctx->runninghash[6]) & 0xffffffff;
    ctx->runninghash[7] = (h + ctx->runninghash[7]) & 0xffffffff;
}

static void add(U32 *bignumber, U32 incr)
{
    /*
     *          high bits           low bits
     *      +--------------+    +--------------+
     *      | bignumber[0] |    | bignumber[1] |
     *      +--------------+    +--------------+
     */
    U32 t = 0xffffffff - bignumber[1];
    if (incr < t) {
        bignumber[1] = bignumber[1] + incr;
    } else {
        bignumber[1] = (bignumber[1] + incr) & 0xffffffff;
        bignumber[0] = (bignumber[0] + 1   ) & 0xffffffff;
    }
}

void sha256_init(sha256_context *ctx)
{
    if (!ctx) {
        return;
    }
    ctx->runninghash[0] = 0x6a09e667;
    ctx->runninghash[1] = 0xbb67ae85;
    ctx->runninghash[2] = 0x3c6ef372;
    ctx->runninghash[3] = 0xa54ff53a;
    ctx->runninghash[4] = 0x510e527f;
    ctx->runninghash[5] = 0x9b05688c;
    ctx->runninghash[6] = 0x1f83d9ab;
    ctx->runninghash[7] = 0x5be0cd19;
    ctx->totalbitlen[0] = 0x00000000;
    ctx->totalbitlen[1] = 0x00000000;
    ctx->msgchunklen = 0x00;
}

void sha256_feed(sha256_context *ctx, U32 ilen, const U8 *ibuf)
{
    U32 i;

    if (!ctx || (ilen && !ibuf)) {
        return;
    }

    for (i = 0; i < ilen; ++i) {
        ctx->msgchunk[ctx->msgchunklen] = ibuf[i];
        ctx->msgchunklen++;
        if (ctx->msgchunklen == 64) {
            process_one_block(ctx, ctx->msgchunk);
            add(ctx->totalbitlen, 512);
            ctx->msgchunklen = 0;
        }
    }
}

void sha256_done(const sha256_context *ctx, U8 *obuf)
{
    sha256_context ctx_;
    U8 i;

    if (!ctx || !obuf) {
        return;
    }

    /* Create another working context, not polluting the original one */
    copy_context_from_to(ctx, &ctx_);

    /* Append one 0x80 byte */
    i = ctx_.msgchunklen;
    ctx_.msgchunk[i++] = 0x80;

    /* Append some number of 0x00 bytes */
    if (i >= 57) {
        while (i < 64) {
            ctx_.msgchunk[i++] = 0x00;
        }
        process_one_block(&ctx_, ctx_.msgchunk);
        i = 0;
    }
    while (i < 56) {
        ctx_.msgchunk[i++] = 0x00;
    }

    /* Append eight bytes representing the message bit length (big-endian) */
    add(ctx_.totalbitlen, ctx_.msgchunklen * 8);
    ctx_.msgchunk[56] = ctx_.totalbitlen[0] >> 24;
    ctx_.msgchunk[57] = ctx_.totalbitlen[0] >> 16;
    ctx_.msgchunk[58] = ctx_.totalbitlen[0] >>  8;
    ctx_.msgchunk[59] = ctx_.totalbitlen[0] >>  0;
    ctx_.msgchunk[60] = ctx_.totalbitlen[1] >> 24;
    ctx_.msgchunk[61] = ctx_.totalbitlen[1] >> 16;
    ctx_.msgchunk[62] = ctx_.totalbitlen[1] >>  8;
    ctx_.msgchunk[63] = ctx_.totalbitlen[1] >>  0;
    process_one_block(&ctx_, ctx_.msgchunk);

    /* Dump the resulting 32-byte hash value */
    for (i = 0; i < 8; ++i) {
        obuf[4 * i + 0] = ctx_.runninghash[i] >> 24;
        obuf[4 * i + 1] = ctx_.runninghash[i] >> 16;
        obuf[4 * i + 2] = ctx_.runninghash[i] >>  8;
        obuf[4 * i + 3] = ctx_.runninghash[i] >>  0;
    }
}

void sha256_hash(U32 ilen, const U8 *ibuf, U8 *obuf)
{
    sha256_context ctx_;

    if ((ilen && !ibuf) || !obuf) {
        return;
    }

    sha256_init(&ctx_);
    if (ilen) {
        sha256_feed(&ctx_, ilen, ibuf);
    }
    sha256_done(&ctx_, obuf);
}
