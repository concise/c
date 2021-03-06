#include <stdio.h>

static const char *readfile(
        const char *fname, void *buf, size_t blen, size_t *dlen);

static const char *writefile(
        const char *fname, const void *buf, size_t blen);

static const char *readfile(
        const char *fname, void *buf, size_t blen, size_t *dlen)
{
    FILE *f;
    const char *errormsg;
    if (fname == NULL || buf == NULL || blen == 0 || dlen == NULL)
        return "some input argument is obviously wrong";
    f = fopen(fname, "r");
    if (f == NULL)
        return "cannot open file";
    *dlen = fread(buf, 1, blen, f);
    if (feof(f))
        errormsg = NULL;
    else if (*dlen == blen)
        errormsg = "the buffer is too small to hold the content of a file";
    else
        errormsg = "some error occurs during reading a file stream";
    fclose(f);
    return errormsg;
}

static const char *writefile(
        const char *fname, const void *buf, size_t blen)
{
    FILE *f;
    size_t nbytes_written;
    const char *errormsg;
    if (fname == NULL || (buf == NULL && blen > 0))
        return "some input argument is obviously wrong";
    f = fopen(fname, "w");
    if (f == NULL)
        return "cannot open file";
    nbytes_written = fwrite(buf, 1, blen, f);
    if (nbytes_written == blen)
        errormsg = NULL;
    else
        errormsg = "some unknown error occurs";
    fclose(f);
    return errormsg;
}

int main(void)
{
    {
        unsigned char buf[12] = "Hello world!";
        const char *error;

        error = writefile("/tmp/testfile.txt", buf, 12);
        if (error)
            printf("Error: %s\n", error);
        else
            printf("Successfully write some data\n");
    }

    {
        unsigned char buf[4096];
        size_t dlen;
        const char *error;

        error = readfile("/tmp/testfile.txt", buf, 4096, &dlen);
        if (error)
            printf("Error: %s\n", error);
        else
            printf("Successfully read %zu bytes\n", dlen);
    }
}
