#ifndef USTRING_H
#define USTRING_H 1

#include <stddef.h>

/** \brief rune is a type large enough to hold any Unicode code point */
typedef int rune;

/** \brief byte is a type with 8 bits */
typedef unsigned char byte;

/**
 * \brief utf_decode finds and returns the value of the first UTF-8 code point in buf.
 *
 * utf_decode will stop at the first invalid byte and return the Unicode replace character (U+FFFD).
 *
 * To prevent an overrun, ensure that buf ends with a NULL byte.
 *
 * \param buf the buffer to read a code point from
 * \param size if not NULL, the number of bytes used by the code point will be returned.
 */
rune utf8_decode(byte *buf, size_t *size);

/**
 * \brief utf8_len returns the number of bytes in a UTF-8 encoded code point according to the first byte of the encoded text.
 *
 * If the byte is not valid a length of 0 is returned.
 *
 * \param b the first byte of the encoded text.
 */
size_t utf8_len(byte b);

#endif
