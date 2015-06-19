#ifndef __SHA256_H__
#define __SHA256_H__

typedef unsigned char U8;       /* Make sure it can hold 0 ~ 0xff           */
typedef unsigned long int U32;  /* Make sure it can hold 0 ~ 0xffffffff     */
typedef struct {
    U32 runninghash[8];         /* The intermediate hash value (H0 ~ H7)    */
    U32 totalbitlen[2];         /* The bit length (l) of the input message  */
    U8 msgchunk[64];            /* The last unprocessed message chunk       */
    U8 msgchunklen;             /* The byte length of the unprocessed chunk */
} sha256_context;

void sha256_init(sha256_context *);
void sha256_feed(sha256_context *, U32, const U8 *);
void sha256_done(const sha256_context *, U8 *);
void sha256(U32, const U8 *, U8 *);

#endif
