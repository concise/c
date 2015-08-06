#include "hmac.h"

static int is_bad_hash_info(const hmac_context_t *info)
{
    return !(
        info && info->hash_context && info->hash_begin && info->hash_update &&
        info->hash_output && info->workingBufferB && info->workingBufferL &&
        info->B > 0 && info->L > 0 && info->L <= info->B
    );
}

void hmac_begin(
    const hmac_context_t *info, int keylen, const unsigned char *key)
{
    int i;

    if (is_bad_hash_info(info) || keylen < 0 || (keylen > 0 && !key)) {
        return;
    }

    if (keylen > info->B) {
        (*info->hash_begin)(info->hash_context);
        (*info->hash_update)(info->hash_context, keylen, key);
        (*info->hash_output)(info->hash_context, info->workingBufferB);
        for (i = info->L; i < info->B; ++i) {
            info->workingBufferB[i] = 0x00;
        }
    } else {
        for (i = 0; i < keylen; ++i) {
            info->workingBufferB[i] = key[i];
        }
        for (i = keylen; i < info->B; ++i) {
            info->workingBufferB[i] = 0x00;
        }
    }

    for (i = 0; i < info->B; ++i) {
        info->workingBufferB[i] ^= 0x36;
    }

    (*info->hash_begin)(info->hash_context);
    (*info->hash_update)(info->hash_context, info->B, info->workingBufferB);

    for (i = 0; i < info->B; ++i) {
        info->workingBufferB[i] ^= 0x6a;
    }
}

void hmac_update(
    const hmac_context_t *info, int msglen, const unsigned char *msg)
{
    if (is_bad_hash_info(info) || msglen < 0 || (msglen > 0 && !msg)) {
        return;
    }

    if (msglen > 0) {
        (*info->hash_update)(info->hash_context, msglen, msg);
    }
}

void hmac_output(const hmac_context_t *info, unsigned char *out)
{
    if (is_bad_hash_info(info) || !out) {
        return;
    }

    (*info->hash_output)(info->hash_context, info->workingBufferL);
    (*info->hash_begin)(info->hash_context);
    (*info->hash_update)(info->hash_context, info->B, info->workingBufferB);
    (*info->hash_update)(info->hash_context, info->L, info->workingBufferL);
    (*info->hash_output)(info->hash_context, out);
}
