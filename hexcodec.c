static unsigned char hexcodec_encoding_translation(unsigned char n)
{
    return "0123456789abcdef"[n % 16];
}

static unsigned char hexcodec_ENCODING_translation(unsigned char n)
{
    return "0123456789ABCDEF"[n % 16];
}

static unsigned char hexcodec_decoding_translation(unsigned char x)
{
    return ((x >= '0' && x <= '9') ? (x - '0'     ) :
            (x >= 'A' && x <= 'F') ? (x - 'A' + 10) :
            (x >= 'a' && x <= 'f') ? (x - 'a' + 10) : 0);
}

// Assume the size of ibuf buffer is at least n       bytes
// Assume the size of obuf buffer is at least (n * 2) bytes
void hexcodec_encode(int n, const unsigned char *ibuf, unsigned char *obuf)
{
    int i;

    if (!(n > 0 && ibuf && obuf)) return;

    for (i = 0; i < n; ++i) {
        obuf[i * 2    ] = hexcodec_encoding_translation(ibuf[i] / 16);
        obuf[i * 2 + 1] = hexcodec_encoding_translation(ibuf[i] % 16);
    }
}

// Assume the size of ibuf buffer is at least n       bytes
// Assume the size of obuf buffer is at least (n * 2) bytes
void hexcodec_Encode(int n, const unsigned char *ibuf, unsigned char *obuf)
{
    int i;

    if (!(n > 0 && ibuf && obuf)) return;

    for (i = 0; i < n; ++i) {
        obuf[i * 2    ] = hexcodec_ENCODING_translation(ibuf[i] / 16);
        obuf[i * 2 + 1] = hexcodec_ENCODING_translation(ibuf[i] % 16);
    }
}

// Assume the size of ibuf buffer is at least n             bytes
// Assume the size of obuf buffer is at least ((n + 1) / 2) bytes
void hexcodec_decode(int n, const unsigned char *ibuf, unsigned char *obuf)
{
    if (!(n > 0 && ibuf && obuf)) return;

    while (n--) {
        obuf[n / 2] = hexcodec_decoding_translation(ibuf[n]);
        if (n--) {
            obuf[(n + 1) / 2] += hexcodec_decoding_translation(ibuf[n]) * 16;
        } else {
            break;
        }
    }

    //  n       ->  6
    //  ibuf    ->  {  'a', 'b',   'c', 'd',  'e', 'f'  }
    //  obuf    ->  {      0xab,       0xcd,      0xef  }
    //
    //  n       ->  5
    //  ibuf    ->  {       'b',   'c', 'd',  'e', 'f'  }
    //  obuf    ->  {      0x0b,       0xcd,      0xef  }
}
