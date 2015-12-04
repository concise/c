#include <stdio.h>

int read_until_eof(unsigned char *buf, int buf_capacity, FILE *stream);

int read_until_eof(unsigned char *buf, int buf_capacity, FILE *stream)
{
        int nbytes_read = 0;
        if (!buf || buf_capacity < 0 || !stream) {
                return -1; // some argument is invalid
        }
        for (;;) {
                int thisbyte = getc(stream);
                if (thisbyte != EOF) {
                        if (nbytes_read < buf_capacity) {
                                buf[nbytes_read] = (unsigned char) thisbyte;
                                nbytes_read += 1;
                        } else {
                                return -2; // the buffer is too small
                        }
                } else {
                        if (feof(stream)) {
                                return nbytes_read;
                        } else if (ferror(stream)) {
                                return -3; // the file stream has errors occurred
                        } else {
                                return -4; // the file stream has unknown errors occurred
                        }
                }
        }
}

int main(void)
{
        FILE *fp = fopen("/tmp/test.txt", "r");
        if (!fp) {
                printf("open file error\n");
                return -1;
        }

        unsigned char buf[16];
        int nbytes = read_until_eof(buf, 16, fp);
        if (nbytes > 0) {
                printf("%d bytes\n", nbytes);
        } else if (nbytes == -1) {
                printf("argument error\n");
        } else if (nbytes == -2) {
                printf("buffer too small error\n");
        } else if (nbytes == -3) {
                printf("file stream error\n");
        } else if (nbytes == -4) {
                printf("file stream unknown error\n");
        }

        return 0;
}
