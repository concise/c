#ifndef LOADFILE_H__
#define LOADFILE_H__

#include <stdio.h>

const char *loadfile(
        const char *fname, unsigned char *buf, size_t blen, size_t *dlen);

#endif
