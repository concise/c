#include <stdio.h>

static const char *filestream2buf(
        FILE *f, unsigned char *buf, size_t blen, size_t *dlen);

static const char *loadfile(
        const char *fname, unsigned char *buf, size_t blen, size_t *dlen);

int main(void)
{
    size_t dlen;
    unsigned char buf[4096];
    const char *error;

    error = loadfile("/tmp/testfile.txt", buf, 4096, &dlen);

    if (error)
        printf("Error: %s\n", error);
    else
        printf("Successfully read %zu bytes from /tmp/testfile.txt\n", dlen);

    return 0;
}

static const char *filestream2buf(
        FILE *f, unsigned char *buf, size_t blen, size_t *dlen)
{
    if (f == NULL || buf == NULL || !(blen > 0) || dlen == NULL)
        return "some input argument is obviously wrong";

    *dlen = fread(buf, 1, blen, f);

    if (feof(f))
        return NULL;
    else if (*dlen == blen)
        return "the buffer is too small to hold the content of a file";
    else if (ferror(f))
        return "some file stream error occurs";
    else
        return "some unknown error occurs";
}

static const char *loadfile(
        const char *fname, unsigned char *buf, size_t blen, size_t *dlen)
{
    FILE *f;
    const char *error;

    f = fopen(fname, "r");

    if (f == NULL)
        return "cannot open file";

    error = filestream2buf(f, buf, blen, dlen);

    fclose(f);

    return error;
}
