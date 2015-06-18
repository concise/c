static unsigned char hexcodec_encoding_translation(unsigned char n)
{
    return "0123456789abcdef"[n % 16];
}

static unsigned char hexcodec_ENCODING_translation(unsigned char n)
{
    return "0123456789ABCDEF"[n % 16];
}

static unsigned char hexcodec_decoding_translation(unsigned char d)
{
    return ((d >= '0' && d <= '9') ? (d - '0'     ) :
            (d >= 'A' && d <= 'F') ? (d - 'A' + 10) :
            (d >= 'a' && d <= 'f') ? (d - 'a' + 10) : 0);
}

/* Assume the size of ibuf buffer is at least n bytes
 * Assume the size of obuf buffer is at least (n * 2) bytes */
void hexcodec_encode(int n, const void *ibuf, void *obuf)
{
    const unsigned char *i = ibuf;
    unsigned char *o = obuf;

    if (!(n > 0 && i && o)) return;

    while (n--) {
        o[n * 2    ] = hexcodec_encoding_translation(i[n] / 16);
        o[n * 2 + 1] = hexcodec_encoding_translation(i[n] % 16);
    }
}

/* Assume the size of ibuf buffer is at least n bytes
 * Assume the size of obuf buffer is at least (n * 2) bytes */
void hexcodec_Encode(int n, const void *ibuf, void *obuf)
{
    const unsigned char *i = ibuf;
    unsigned char *o = obuf;

    if (!(n > 0 && i && o)) return;

    while (n--) {
        o[n * 2    ] = hexcodec_ENCODING_translation(i[n] / 16);
        o[n * 2 + 1] = hexcodec_ENCODING_translation(i[n] % 16);
    }
}

/* Assume the size of ibuf buffer is at least n bytes
 * Assume the size of obuf buffer is at least floor((n + 1) / 2) bytes */
void hexcodec_decode(int n, const void *ibuf, void *obuf)
{
    const unsigned char *i = ibuf;
    unsigned char *o = obuf;

    if (!(n > 0 && i && o)) return;

    while (n--) {
        o[n / 2] = hexcodec_decoding_translation(i[n]);
        if (n--) {
            o[(n + 1) / 2] += hexcodec_decoding_translation(i[n]) * 16;
        } else {
            break;
        }
    }
}
