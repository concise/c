#include <stdio.h>

static const char *readfile(
        const char *fname, unsigned char *buf, size_t blen, size_t *dlen);

static const char *writefile(
        const char *fname, const unsigned char *buf, size_t blen);

static const char *readfilestream(
        FILE *f, unsigned char *buf, size_t blen, size_t *dlen);

static const char *writefilestream(
        FILE *f, const unsigned char *buf, size_t blen);

static void test_read(void);

static void test_write(void);

int main(void)
{
    test_write();
    test_read();
}

static const char *readfile(
        const char *fname, unsigned char *buf, size_t blen, size_t *dlen)
{
    FILE *f;
    const char *error;

    f = fopen(fname, "r");

    if (f == NULL)
        return "cannot open file";

    error = readfilestream(f, buf, blen, dlen);

    fclose(f);

    return error;
}

static const char *writefile(
        const char *fname, const unsigned char *buf, size_t blen)
{
    FILE *f;
    const char *error;

    f = fopen(fname, "w");

    if (f == NULL)
        return "cannot open file";

    error = writefilestream(f, buf, blen);

    fclose(f);

    return error;
}

static const char *readfilestream(
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

static const char *writefilestream(
        FILE *f, const unsigned char *buf, size_t blen)
{
    size_t nbytes_written;

    if (f == NULL || (buf == NULL && blen > 0))
        return "some input argument is obviously wrong";

    nbytes_written = fwrite(buf, 1, blen, f);

    if (nbytes_written == blen)
        return NULL;
    else
        return "some unknown error occurs";
}

static void test_read(void)
{
    size_t dlen;
    unsigned char buf[4096];
    const char *error;

    error = readfile("/tmp/testfile.txt", buf, 4096, &dlen);

    if (error)
        printf("Error: %s\n", error);
    else
        printf("Successfully read %zu bytes from /tmp/testfile.txt\n", dlen);
}

static void test_write(void)
{
    unsigned char buf[12] = "Hello world!";
    const char *error;

    error = writefile("/tmp/testfile.txt", buf, 12);

    if (error)
        printf("Error: %s\n", error);
    else
        printf("Successfully write some data to /tmp/testfile.txt\n");
}
