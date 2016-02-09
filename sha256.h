#ifndef SHA256_H__
#define SHA256_H__

#include <stddef.h>
#include <stdint.h>

#define SHA256_IBLOCK_SIZE 64
#define SHA256_OUTPUT_SIZE 32

typedef struct {
    uint32_t runninghash[8];    /* intermediate hash value (H0 ~ H7)    */
    uint32_t totalbitlen[2];    /* bit length (l) of the input message  */
    uint8_t msgchunk[64];       /* last unprocessed message chunk       */
    uint8_t msgchunklen;        /* byte length of the unprocessed chunk */
} sha256_context_t;

void sha256_begin(void *_ctx);
void sha256_update(void *_ctx, size_t ilen, const uint8_t *ibuf);
void sha256_output(void *_ctx, uint8_t *obuf);

#endif
