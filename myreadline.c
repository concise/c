#include <stdio.h> // fgetc EOF feof ferror
#include <string.h> // memset

// myfgets
//
//      Reads at most (count - 1) bytes from the given file stream and stores
//      them in str.  We assume the capacity of str is count, and it will be
//      cleared out with 0x00 before any data is read from stream.  Parsing
//      stops if end-of-file occurs or a newline character is found, in which
//      case str will contain that newline character.
//
// Parameters
//
//      str     - buffer (of count bytes) to read the characters to
//      count   - the length of str, should be an integer greater than 1
//      stream  - file stream to read the data from
//
// Return value
//
//      Number of bytes (positive integer) read from stream,
//      -1 when some of the input parameters is invalid,
//      -2 when no byte is read because of end-of-file condition,
//      -3 when no byte is read because of some other I/O error,
//      -4 when no byte is read because of some other unknown error.
//
// Example
//
//      unsigned char buffer[16];
//      int ret = myfgets(buffer, 16, stdin);
//      if (ret > 0) {
//          // Totally ret bytes were read into buffer
//          if (buffer[ret - 1] == '\n') {
//              // A complete line was read into buffer
//          }
//      }

static int myfgets(unsigned char *str, int count, FILE *stream)
{
    int thisbyte = 0;
    int nbytes = 0;

    if (!str)
        return -1;
    if (count < 2)
        return -1;
    if (!stream)
        return -1;

    memset(str, 0, count);

    for (;;) {
        thisbyte = fgetc(stream);
        if (thisbyte == EOF)
            break;

        str[nbytes] = thisbyte;
        nbytes += 1;

        if (thisbyte == '\n' || nbytes == count - 1)
            return nbytes;
    }

    if (nbytes > 0)
        return nbytes;
    else if (feof(stream))
        return -2;
    else if (ferror(stream))
        return -3;
    else
        return -4;
}

#define SWAP_CAPACITY 16384

int myreadline(const unsigned char **pbufptr, int *pbufsize)
{
    static unsigned char swap[SWAP_CAPACITY];

    if (!(pbufptr && pbufsize)) {
        return 1;
    }

    // Invoke myfgets() one or more times
    // until a complete line is read or
    // until the swap size is not enough or
    // until an I/O error occurs

    int total_size = 0;
    int chunk_size;

    for (;;) {
        chunk_size = myfgets(
                swap + total_size, SWAP_CAPACITY - total_size, stdin);
        if (chunk_size <= 0) {
            return 2;
        }
        if (swap[total_size + chunk_size - 1] == '\n') {
            total_size += chunk_size - 1;
            *pbufptr = swap;
            *pbufsize = total_size;
            return 0;
        }
        total_size += chunk_size;
    }
}
