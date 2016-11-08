/* utf8cvt1.h */
/* Small utilities to convert between utf8 and 4-byte unsigned code points.
 * Specification is directly RFC 3629, which defines how to convert
 * unsigned values between 0 and 0x10FFFF to or from 1-4 byte values.
 */
/* Copyright (c) 2016, Stephen A. Kukolich
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef UTF8CVT1_H_INCLUDED
#define UTF8CVT1_H_INCLUDED

#ifndef UTF8CVT1_EXTERN
#define UTF8CVT1_EXTERN
#endif

/* Error codes */
#define UTF8CVT1_ILLEGAL_CODEPOINT -1
#define UTF8CVT1_ILLEGAL_LEAD_BYTE -2
#define UTF8CVT1_ILLEGAL_CONTINUATION_AS_LEAD_BYTE -3
#define UTF8CVT1_ILLEGAL_BYTE_COUNT -4

/** Given a 4-byte unsigned code point, return the number of bytes
 *  needed to represent it in utf8, 1 to 4, or < 0 for not legal. */
UTF8CVT1_EXTERN int utf8cvt1_utf8_count_from_codepoint(unsigned codepoint);

/** Given a 4-byte unsigned code point, convert it into 
 *  into the utf8 bytes; utf8_count must be what
 *  utf8cvt1_utf8_count_from_codepoint  returned, and  dest must be large
 *  enough to receive utf8_count bytes, utf8_count is returned.  */
UTF8CVT1_EXTERN int utf8cvt1_utf8_from_codepoint(unsigned codepoint,
                                                 int utf8_count,
                                                 unsigned char *dest);

/** Given a lead utf8 byte, return the total number of utf8 bytes
 *  for the represented code point */
UTF8CVT1_EXTERN int utf8cvt1_utf8_count_from_utf8_lead(unsigned char lead_byte);

/** Given some utf8 bytes, and the count returned by
 *  utf8cvt1_utf8_count_from_utf8_lead(), return the 4-byte codepoint value,
 *  or < 0 if utf8_count is bad. */
UTF8CVT1_EXTERN int utf8cvt1_codepoint_from_utf8(int utf8_count,
                                                 unsigned char *source);

#endif /*UTF8CVT1_H_INCLUDED*/

#ifdef UTF8CVT1_DO_IMPLEMENTATION
/*************************************************************/
/* RFC 3629 defines the mapping as follows :
 *
 * Char. number range  |        UTF-8 octet sequence
 *    (hexadecimal)    |              (binary)
 * --------------------+---------------------------------------------
 * 0000 0000-0000 007F | 0xxxxxxx
 * 0000 0080-0000 07FF | 110xxxxx 10xxxxxx
 * 0000 0800-0000 FFFF | 1110xxxx 10xxxxxx 10xxxxxx
 * 0001 0000-0010 FFFF | 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
 *
 * Encoding a character to UTF-8 proceeds as follows:
 *
 * 1.  Determine the number of octets required from the character number
 *     and the first column of the table above.  It is important to note
 *     that the rows of the table are mutually exclusive, i.e., there is
 *     only one valid way to encode a given character.
 *
 * 2.  Prepare the high-order bits of the octets as per the second
 *     column of the table.
 *
 * 3.  Fill in the bits marked x from the bits of the character number,
 *     expressed in binary.  Start by putting the lowest-order bit of
 *     the character number in the lowest-order position of the last
 *     octet of the sequence, then put the next higher-order bit of the
 *     character number in the next higher-order position of that octet,
 *     etc.  When the x bits of the last octet are filled in, move on to
 *     the next to last octet, then to the preceding one, etc. until all
 *     x bits are filled in.
 *
 * The definition of UTF-8 prohibits encoding character numbers between
 * U+D800 and U+DFFF,...
 */


#ifndef UTF8CVT1_DEFN
#define UTF8CVT1_DEFN
#endif /*UTF8CVT1_DEFN*/

UTF8CVT1_DEFN int utf8cvt1_utf8_count_from_codepoint(unsigned codepoint)
{
  if (codepoint <= 0x7F) {
    return(1);
  } else if (codepoint <= 0x7FF) {
    return(2);
  } else if (codepoint <= 0xFFFF) {
    if ((0xD800 <= codepoint)&&(codepoint <= 0xDFFF)) {
      return(UTF8CVT1_ILLEGAL_CODEPOINT);
    }
    return(3);
  } else if (codepoint <= 0x10FFFF) {
    return(4);
  } else {
    return(UTF8CVT1_ILLEGAL_CODEPOINT);
  }
}

UTF8CVT1_DEFN int utf8cvt1_utf8_from_codepoint(unsigned codepoint,
                                               int utf8_count,
                                               unsigned char *dest)
{
  switch(utf8_count) {
  case 4:
    dest[3] = 0x80 | (codepoint & 0x3f);
    codepoint >>= 6;
    codepoint |= (0xf0 << 12); /*set lead byte bits, w/ reversed remaining shifts*/
    /* fall through */
  case 3:
    dest[2] = 0x80 | (codepoint & 0x3f);
    codepoint >>= 6;
    codepoint |= (0xe0 << 6); /*set lead byte bits, w/ reversed remaining shifts*/
    /* fall through */
  case 2:
    dest[1] = 0x80 | (codepoint & 0x3f);
    codepoint >>= 6;
    codepoint |= (0xc0); /* set lead byte bits */
    /* fall through */
  case 1:
    dest[0] = codepoint;
    break;
  default:
    if (utf8_count < 0) { return utf8_count; }
    return(UTF8CVT1_ILLEGAL_BYTE_COUNT);
    break;
  }
  return(utf8_count);
}

UTF8CVT1_DEFN int utf8cvt1_utf8_count_from_utf8_lead(unsigned char lead_byte)
{
  if      (lead_byte <= 0x7F /* 01111111 */) return(1);
  else if (lead_byte <= 0xBF /* 10111111 */) return(UTF8CVT1_ILLEGAL_CONTINUATION_AS_LEAD_BYTE);
  else if (lead_byte <= 0xDF /* 11011111 */) return(2);
  else if (lead_byte <= 0xEF /* 11101111 */) return(3);
  else if (lead_byte <= 0xF7 /* 11110111 */) return(4);
  else                                       return(UTF8CVT1_ILLEGAL_LEAD_BYTE);
}

UTF8CVT1_DEFN int utf8cvt1_codepoint_from_utf8(int count,
                                               unsigned char *source)
{
  switch (count) {
  case 1:
    return( (unsigned)(source[0]));
  case 2:
    return( (((unsigned)(source[0] & 0x1F)) << 6)
            | ((unsigned)(source[1] & 0x3F)) );
  case 3:
    return( (((unsigned)(source[0] & 0x0F)) << 12)
            | (((unsigned)(source[1] & 0x3F)) << 6)
            | ((unsigned)(source[2] & 0x3F)) );
  case 4:
    return( (((unsigned)(source[0] & 0x07)) << 18)
            | (((unsigned)(source[1] & 0x3F)) << 12)
            | (((unsigned)(source[2] & 0x3F)) << 6)
            | ((unsigned)(source[3] & 0x3F)) );
  default:
    if (count < 0) return(count);
    return(UTF8CVT1_ILLEGAL_BYTE_COUNT);
  }
}

/*************************************************************/
#endif /*UTF8CVT1_DO_IMPLEMENTATION*/
