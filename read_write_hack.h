#include <stdio.h>

int read_from_file_stream(
        FILE *stream, unsigned char *data, size_t buf_capacity, size_t *dlen);

int write_to_file_stream(
        FILE *stream, const unsigned char *data, size_t dlen);

#define READ_FROM(filename, ok_block, ng_block)                             \
    do {                                                                    \
        unsigned char data[16384];                                          \
        size_t dlen = 0;                                                    \
        int error_code = -1;                                                \
        FILE *fp = fopen(filename, "r");                                    \
        if (fp) {                                                           \
            error_code = read_from_file_stream(fp, data, 16384, &dlen);     \
            fclose(fp);                                                     \
        }                                                                   \
        if (error_code) {                                                   \
            ng_block                                                        \
        } else {                                                            \
            ok_block                                                        \
        }                                                                   \
    } while (0)

#define WRITE_TO(filename, data, dlen, ok_block, ng_block)                  \
    do {                                                                    \
        int error_code = -1;                                                \
        FILE *fp = fopen(filename, "w");                                    \
        if (fp) {                                                           \
            error_code = write_to_file_stream(fp, data, dlen);              \
            fclose(fp);                                                     \
        }                                                                   \
        if (error_code) {                                                   \
            ng_block                                                        \
        } else {                                                            \
            ok_block                                                        \
        }                                                                   \
    } while (0)
