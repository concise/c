#include "hmac.h"

static int is_bad_sha_info(const sha_info_t *info)
{
    return !(
        info && info->sha_context && info->sha_starts && info->sha_update &&
        info->sha_finish && info->bufferB && info->bufferL &&
        info->B > 0 && info->L > 0 && info->L <= info->B
    );
}

void hmac_starts(const sha_info_t *info, int keylen, const unsigned char *key)
{
    int i;

    if (is_bad_sha_info(info) || keylen < 0 || (keylen > 0 && !key)) {
        return;
    }

    if (keylen > info->B) {
        (*info->sha_starts)(info->sha_context);
        (*info->sha_update)(info->sha_context, keylen, key);
        (*info->sha_finish)(info->sha_context, info->bufferB);
        for (i = info->L; i < info->B; ++i) {
            info->bufferB[i] = 0x00;
        }
    } else {
        for (i = 0; i < keylen; ++i) {
            info->bufferB[i] = key[i];
        }
        for (i = keylen; i < info->B; ++i) {
            info->bufferB[i] = 0x00;
        }
    }

    for (i = 0; i < info->B; ++i) {
        info->bufferB[i] ^= 0x36;
    }

    (*info->sha_starts)(info->sha_context);
    (*info->sha_update)(info->sha_context, info->B, info->bufferB);

    for (i = 0; i < info->B; ++i) {
        info->bufferB[i] ^= 0x6a;
    }
}

void hmac_update(const sha_info_t *info, int msglen, const unsigned char *msg)
{
    if (is_bad_sha_info(info) || msglen < 0 || (msglen > 0 && !msg)) {
        return;
    }

    if (msglen > 0) {
        (*info->sha_update)(info->sha_context, msglen, msg);
    }
}

void hmac_finish(const sha_info_t *info, unsigned char *out)
{
    if (is_bad_sha_info(info) || !out) {
        return;
    }

    (*info->sha_finish)(info->sha_context, info->bufferL);
    (*info->sha_starts)(info->sha_context);
    (*info->sha_update)(info->sha_context, info->B, info->bufferB);
    (*info->sha_update)(info->sha_context, info->L, info->bufferL);
    (*info->sha_finish)(info->sha_context, out);
}

void hmac(const sha_info_t *info,
          int keylen, const unsigned char *key,
          int msglen, const unsigned char *msg,
          unsigned char *out)
{
    if (is_bad_sha_info(info) || keylen < 0 || (keylen > 0 && !key) ||
        msglen < 0 || (msglen > 0 && !msg) || !out)
    {
        return;
    }

    hmac_starts(info, keylen, key);
    hmac_update(info, msglen, msg);
    hmac_finish(info, out);
}
