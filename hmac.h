#ifndef __HMAC_H__
#define __HMAC_H__

typedef struct {
    void *hash_context;
    void (*hash_starts)(void *);
    void (*hash_update)(void *, int, const unsigned char *);
    void (*hash_finish)(void *, unsigned char *);
    unsigned char *bufferB;
    unsigned char *bufferL;
    int B;
    int L;
} hash_info_t;

void hmac_starts(const hash_info_t *, int, const unsigned char *);
void hmac_update(const hash_info_t *, int, const unsigned char *);
void hmac_finish(const hash_info_t *, unsigned char *);
void hmac       (const hash_info_t *, int, const unsigned char *,
                                      int, const unsigned char *,
                                      unsigned char *);

#endif
