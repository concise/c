#include "rfc6979sha256p256csprng.h"
#include <stdio.h>

void hexdump(unsigned char *k, int len);
void hexdump(unsigned char *k, int len)
{
    int i;
    for (i = 0; i < len; ++i) {
        printf("%02x", k[i]);
    }
    printf("\n");
}

int main(void)
{
    int i;
    unsigned char k[32];
    unsigned char tmp[64];

    rfc6979sha256p256csprng_init(tmp,
        (const unsigned char *)
        "\x18\x1c\x4c\xc7\x33\x6c\xfd\x21\x3f\xe0\x4b\x88\x3e\x1c\xa5\x6b\xaf\xc9\xf2\x58\x7b\x17\x8f\x84\x33\x07\xc5\x38\x04\xa3\xde\x11"
        ,
        32
    );

    for (i = 0; i < 10; ++i) {
        rfc6979sha256p256csprng_gen(tmp, k);
        hexdump(k, 32);
    }
    // 17d9e867ee0f3888f0ac113cd228e9be5032d68e71f3116d0452853144b38a4b
    // 4fbff99419551ef022a12e6baa448cf60ca2cc2fb3fc76a18cd18acfe579e886
    // 86c3f4fc6cbffce51743bfb69cce8d481ea0fd9d9547687c959b2d6383fe6907
    // dcd8a2ae2c750d0efcad3daa3be78ed528ce304cc273cfda014d1a67bb295685
    // 742492fc78b5b68a9a9491e7da91d70979258e48f0c0bee9f21675971b44cf1d
    // 37fc879f0fbf1e6db6e4ea74135cb02e81c8871de344c6c8d88ea7126892bfea
    // b4b721e8c4a491c703aa0ea05c90d0363f54af1421d3e672291c6f17982b55f4
    // 24c854fc2cea5cb185d4857c12a2664271c2bf7deca8c4d7064b100a2efbb0a0
    // 667f7d63074efa3a22b2ed3deaa080d9a3460b45791582b3de67dc51056cb61b
    // 5347eea28aec1695b23a1fb03de2b07746374e5900fc8c3d2d36fbb0545e68b5
}
