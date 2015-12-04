#include <stdio.h>

int read_from_file_stream(FILE *stream, unsigned char *data, size_t buf_capacity, size_t *dlen);
int write_to_file_stream(FILE *stream, const unsigned char *data, size_t dlen);

int read_from_file_stream(FILE *stream, unsigned char *data, size_t buf_capacity, size_t *dlen)
{
        if (!data || !stream || !dlen) {
                return -1; // some argument is invalid
        }
        *dlen = 0;
        for (;;) {
                int thisbyte = getc(stream);
                if (thisbyte != EOF) {
                        if (*dlen < buf_capacity) {
                                data[*dlen] = (unsigned char) thisbyte;
                                *dlen += 1;
                        } else {
                                return -2; // the buffer is too small
                        }
                } else {
                        if (feof(stream)) {
                                return 0;
                        } else if (ferror(stream)) {
                                return -3; // the file stream has errors occurred
                        } else {
                                return -4; // the file stream has unknown errors occurred
                        }
                }
        }
}

int write_to_file_stream(FILE *stream, const unsigned char *data, size_t dlen)
{
        size_t fwrite_status;
        if (!data || !stream) {
                return -1; // some argument is invalid
        }
        if (dlen > 0) {
                fwrite_status = fwrite(data, 1, dlen, stream);
                if (fwrite_status != dlen) {
                        if (ferror(stream)) {
                                return -2; // the file stream has errors occurred
                        } else {
                                return -3; // the file stream has unknown errors occurred
                        }
                }
        }
        return 0;
}

int main(void)
{
        unsigned char data[6] = {'H', 'e', 'l', 'l', 'o', '!'};
        FILE *fp;
        int status;

        fp = fopen("/tmp/test.txt", "w");
        if (!fp) {
                printf("open file error\n");
                return -1;
        }

        status = write_to_file_stream(fp, data, 6);
        if (status < 0) {
                printf("error\n");
                return -1;
        }

        fclose(fp);

        return 0;
}
