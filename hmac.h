#ifndef __HMAC_H__
#define __HMAC_H__

typedef struct {
    void *sha_context;
    void (*sha_starts)(void *);
    void (*sha_update)(void *, int, const unsigned char *);
    void (*sha_finish)(void *, unsigned char *);
    unsigned char *bufferB;
    unsigned char *bufferL;
    int B;
    int L;
} hmac_info_t;

void hmac_starts(const hmac_info_t *, int, const unsigned char *);
void hmac_update(const hmac_info_t *, int, const unsigned char *);
void hmac_finish(const hmac_info_t *, unsigned char *);
void hmac       (const hmac_info_t *, int, const unsigned char *,
                                      int, const unsigned char *,
                                      unsigned char *);

#endif
