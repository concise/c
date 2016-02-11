#ifndef SHA256_H__
#define SHA256_H__

#include <stddef.h>
#include <stdint.h>

#define SHA256_IBLOCK_SIZE 64   // input block size in bytes
#define SHA256_OUTPUT_SIZE 32   // output size in bytes

typedef struct {
    uint32_t runninghash[8];    // intermediate hash value (H0 ~ H7)
    uint32_t totalbitlen[2];    // bit length (l) of the input message
    uint8_t buffer[64];         // buffer for unprocessed input message
    uint32_t bufferlen;         // byte length of unprocessed input message
} sha256_context;

void sha256_starts(sha256_context *ctx);
void sha256_update(sha256_context *ctx, const uint8_t *input, size_t ilen);
void sha256_finish(const sha256_context *ctx, uint8_t *output);

#endif
