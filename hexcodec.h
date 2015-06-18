#ifndef __HEXCODEC_H__
#define __HEXCODEC_H__

void hexcodec_encode(int, const void *, void *);
void hexcodec_Encode(int, const void *, void *);
void hexcodec_decode(int, const void *, void *);
unsigned char is_hexcodec_encoded(int, const void *);

#endif
