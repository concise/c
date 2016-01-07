#ifndef LOADFILE_H__
#define LOADFILE_H__

#include <stdio.h>

const char *load_all_bytes_in_a_file_stream(
        FILE *f, unsigned char *buf, size_t blen, size_t *dlen);

const char *loadfile(
        const char *fname, unsigned char *buf, size_t blen, size_t *dlen);

#endif
