#include <stdio.h> // getc

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
