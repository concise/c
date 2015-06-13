#include <stdio.h> // fgetc EOF feof ferror
#include <string.h> // memset

int myfgets(char *str, int count, FILE *stream);

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
//      Number of bytes (non-negative integer) read from stream,
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

int myfgets(char *str, int count, FILE *stream)
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

// Read at most (ocap - 1) bytes from stdin, and put all the bytes that are
// successfully read and one extra 0x00 byte at the end into the buffer pointed
// by obuf.  The number of bytes that are successfully read from stdin is
// returned.  The read always stops if a '\n' character is reached.
//
//      ocap    shall be an int value in { 2, 3, 4, ..., INT_MAX }
//      obuf    shall point to a buffer of at least ocap bytes
//      return  an int value in { 0, 1, 2, ..., ocap - 1 }
//
//      if (ret < 0) {
//
//          // Check the error
//
//      } else if (ret == 0) {
//
//          // The EOF is reached so that we cannot read anything
//
//      } else if (obuf[ret - 1] == '\n') {
//
//          // A complete line is read into obuf
//          // The length of this line including '\n' is ret
//          // Note that obuf[ret] should be 0x00 here
//
//      } else {
//
//          // No '\n' found.   Need to continue read further bytes
//          // Note that obuf[ret] should be 0x00 here
//
//      }
//
//  N  >=  1    read N bytes
//  N  ==  0    read nothing because of an unknown reason
//  N  == -1    read nothing because argument error
//  N  == -2    read nothing because stdin error
//  N  == -3    read nothing because stdin closed
//
int myreadline(int ocap, unsigned char *obuf)
{
    int c = 0;
    int nbytes = 0;

    if (!(ocap > 1 && obuf)) {
        return -1;
    }

    while (c != '\n' && nbytes < ocap - 1 && (c = getc(stdin)) != EOF) {
        obuf[nbytes] = c;
        nbytes += 1;
    }
    obuf[nbytes] = 0x00;

    if (c != EOF) {
        return nbytes;
    } else if (nbytes > 0) {
        return nbytes;
    } else if (ferror(stdin)) {
        return -2;
    } else if (feof(stdin)) {
        return -3;
    } else {
        return 0;
    }
}

// TODO: Refactor myreadline to use myfgets, malloc, realloc, free...
