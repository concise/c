#ifndef __SHA256_H__
#define __SHA256_H__

typedef struct {
    unsigned long H[8];     /* Intermediate hash value (8 32-bit words) */
    unsigned long long L;   /* Byte length of the message we are hashing */
    unsigned char M[64];    /* The last unprocessed message chunk */
} sha256_context;

void sha256_init(sha256_context *);
void sha256_feed(sha256_context *, unsigned long long, const void *);
void sha256_done(sha256_context *, void *);

#endif
