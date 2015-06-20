#ifndef __SHA256_H__
#define __SHA256_H__

#define SHA256_BLOCK_SIZE 64
#define SHA256_OUTPUT_SIZE 32

typedef unsigned long int U32;  /* Make sure it can hold 0 ~ 0xffffffff     */

typedef struct {
    U32 runninghash[8];         /* The intermediate hash value (H0 ~ H7)    */
    U32 totalbitlen[2];         /* The bit length (l) of the input message  */
    unsigned char msgchunk[64]; /* The last unprocessed message chunk       */
    unsigned char msgchunklen;  /* The byte length of the unprocessed chunk */
} sha256_context;

void sha256_starts(void *);
void sha256_update(void *, int, const unsigned char *);
void sha256_finish(void *, unsigned char *);
void sha256       (int, const unsigned char *, unsigned char *);

#endif
