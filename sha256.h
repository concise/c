#ifndef __SHA256_H__
#define __SHA256_H__

typedef struct {
    unsigned long H[8];
    unsigned long long nbytes;
    unsigned char msgbuffer[64];
} sha256_context;

void sha256_init(sha256_context *);
void sha256_feed(sha256_context *, unsigned long long, const void *);
void sha256_done(sha256_context *, void *);

#endif
