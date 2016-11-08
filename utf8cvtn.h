/* utf8cvtn.h */
/* Based on utf8cvt1, small utilities to convert multiple codepoints
 * to or from utf8 */
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

#ifndef UTF8CVTN_H_INCLUDED
#define UTF8CVTN_H_INCLUDED

#ifndef UTF8CVTN_EXTERN
#define UTF8CVTN_EXTERN
#endif

/* Error codes, in addition to the UTF8CVT1 error codes */
#define UTF8CVTN_INSUFFICIENT_SPACE -5
#define UTF8CVTN_INCOMPLETE_UTF8_SEQUENCE -6

/** Given a set of codepoints, compute the number of utf8 bytes
 *  need to represent those unicode characters.
 *  If non-NULL, the error_count will be filled in with the number
 *  of errors encountered (including from illegal codepoints).
 *  If non-NULL, and there are errors, first_error will be filled in
 *  with the first error code.
 *  If non-NULL, and there are errors, first_error_location will
 *  be pointed to the place of the first error. */
UTF8CVTN_EXTERN
void utf8cvtn_utf8_count_from_codepoints(unsigned codepoint_count,
                                         unsigned *codepoints,
                                         unsigned *utf8_count,
                                         unsigned *error_count,
                                         int      *first_error,
                                         unsigned **first_error_location);

/** Given a set of codepoints, convert these to utf8 bytes.
 *  utf8_count gives the maximum available byte count.
 *  If non-NULL, the error_count will be filled in with the number
 *  of errors encountered (including from illegal codepoints,
 *  or running out of space to write to).
 *  If non-NULL, and there are errors, first_error will be filled in
 *  with the first error code.
 *  If non-NULL, and there are errors, first_error_location will
 *  be pointed to the place of the first error. */
UTF8CVTN_EXTERN
void utf8cvtn_utf8_from_codepoints(unsigned codepoint_count,
                                   unsigned *codepoints,
                                   unsigned utf8_count,
                                   unsigned char *utf8,
                                   unsigned *error_count,
                                   int      *first_error,
                                   unsigned **first_error_location);

/** Given a set of utf8 bytes, compute and return the number of
 *  (legal) codepoints (i.e. characters).
 *  If non-NULL, the error_count will be filled in with the number
 *  of errors encountered (including from :illegal utf8 sequences,
 *  or from illegal codepoints).
 *  If non-NULL, and there are errors, first_error will be filled in
 *  with the first error code.
 *  If non-NULL, and there are errors, first_error_location will
 *  be pointed to the place of the first error. */
UTF8CVTN_EXTERN
void utf8cvtn_codepoint_count_from_utf8(unsigned utf8_count,
                                        unsigned char *utf8,
                                        unsigned *codepoint_count,
                                        unsigned *error_count,
                                        int      *first_error,
                                        unsigned char **first_error_location);

/** Give the set of utf8 bytes, convert this to 32-bit codepoints.
 *  codepoint_count tells the number of 4-byte slots in codepoints
 *  available for writing the results into.
 *  If non-NULL, the error_count will be filled in with the number
 *  of errors encountered (including from : illegal utf8 sequences,
 *  or from illegal codepoints, or not having enough space to write to).
 *  If non-NULL, and there are errors, first_error will be filled in
 *  with the first error code.
 *  If non-NULL, and there are errors, first_error_location will
 *  be pointed to the place of the first error. */
UTF8CVTN_EXTERN
void utf8cvtn_codepoints_from_utf8(unsigned utf8_count,
                                   unsigned char *utf8,
                                   unsigned codepoint_count,
                                   unsigned *codepoints,
                                   unsigned *error_count,
                                   int      *first_error,
                                   unsigned char **first_error_location);

#endif /*UTF8CVTN_H_INCLUDED*/

#ifdef UTF8CVTN_DO_IMPLEMENTATION
/*************************************************************/

#ifndef UTF8CVTN_DEFN
#define UTF8CVTN_DEFN
#endif

UTF8CVTN_DEFN void utf8cvtn_utf8_count_from_codepoints(unsigned codepoint_count,
                                                       unsigned *codepoints,
                                                       unsigned *utf8_count,
                                                       unsigned *error_count,
                                                       int      *first_error,
                                                       unsigned **first_error_location)
{
  unsigned lerror_count = 0;
  unsigned lutf8_count = 0;
  int lfirst_error = 0;
  for (int i_from = 0; i_from < codepoint_count; ++i_from) {
    unsigned codepoint = codepoints[i_from];
    int utf8_count_1 = utf8cvt1_utf8_count_from_codepoint(codepoint);
    if (utf8_count_1 < 0) {
      ++lerror_count;
      if (lfirst_error == 0) {
        lfirst_error = utf8_count_1;
        if (first_error_location) *first_error_location = codepoints + i_from;
      }
    } else {
      lutf8_count += utf8_count_1;
    }
  }
  if (error_count) *error_count = lerror_count;
  if (utf8_count)  *utf8_count = lutf8_count;
  if (first_error) *first_error = lfirst_error;
}

UTF8CVTN_DEFN void utf8cvtn_utf8_from_codepoints(unsigned codepoint_count,
                                                 unsigned *codepoints,
                                                 unsigned utf8_count,
                                                 unsigned char *utf8,
                                                 unsigned *error_count,
                                                 int      *first_error,
                                                 unsigned **first_error_location)
{
  unsigned lerror_count = 0;
  unsigned i_to = 0;
  int lfirst_error = 0;
  for (int i_from = 0; i_from < codepoint_count; ++i_from) {
    unsigned codepoint = codepoints[i_from];
    int utf8_count_1 = utf8cvt1_utf8_count_from_codepoint(codepoint);
    if (utf8_count_1 < 0) {
      ++lerror_count;
      if (lfirst_error == 0) {
        lfirst_error = utf8_count_1;
        if (first_error_location) *first_error_location = codepoints + i_from;
      }
    } else if (i_to + utf8_count_1 > utf8_count) {
      ++lerror_count;
      if (lfirst_error == 0) {
        lfirst_error = UTF8CVTN_INSUFFICIENT_SPACE;
        if (first_error_location) *first_error_location = codepoints + i_from;
      }      
      break;
    } else {
      utf8cvt1_utf8_from_codepoint(codepoint, utf8_count_1, utf8+i_to);
      i_to += utf8_count_1;
    }
  }
  if (error_count) *error_count = lerror_count;
  if (first_error) *first_error = lfirst_error;
}

UTF8CVTN_DEFN void utf8cvtn_codepoint_count_from_utf8(unsigned utf8_count,
                                                      unsigned char *utf8,
                                                      unsigned *codepoint_count,
                                                      unsigned *error_count,
                                                      int      *first_error,
                                                      unsigned char **first_error_location)
{
  int lerror_count = 0;
  int lcodepoint_count = 0;
  int lfirst_error = 0;
  for (int i_from = 0; i_from < utf8_count ; ) {
    int utf8_count_1 = utf8cvt1_utf8_count_from_utf8_lead(utf8[i_from]);
    if (utf8_count_1 < 0) {
      ++lerror_count;
      if (lfirst_error == 0) {
        lfirst_error = utf8_count_1;
        if (first_error_location) *first_error_location = utf8 + i_from;
      }
      ++i_from;
    } else if (i_from + utf8_count_1 > utf8_count) {
      ++lerror_count;
      if (lfirst_error == 0) {
        lfirst_error = UTF8CVTN_INCOMPLETE_UTF8_SEQUENCE;
        if (first_error_location) *first_error_location = utf8 + i_from;
      }
      break;
    } else {
      int codepoint = utf8cvt1_codepoint_from_utf8(utf8_count_1,
                                                  utf8+i_from);
      if (codepoint < 0) {
        ++lerror_count;
        if (lfirst_error == 0) {
          lfirst_error = codepoint;
          if (first_error_location) *first_error_location = utf8 + i_from;
        }
      } else {
        ++lcodepoint_count;
      } 
      i_from += utf8_count_1;
    }
  }
  if (error_count) *error_count = lerror_count;
  if (codepoint_count) *codepoint_count = lcodepoint_count;
  if (first_error) *first_error = lfirst_error;
}

UTF8CVTN_DEFN void utf8cvtn_codepoints_from_utf8(unsigned utf8_count,
                                                 unsigned char *utf8,
                                                 unsigned codepoint_count,
                                                 unsigned *codepoints,
                                                 unsigned *error_count,
                                                 int      *first_error,
                                                 unsigned char **first_error_location)
{
  int lerror_count = 0;
  int i_to = 0;
  int lfirst_error = 0;
  for (int i_from = 0; i_from < utf8_count ; ) {
    int utf8_count_1 = utf8cvt1_utf8_count_from_utf8_lead(utf8[i_from]);
    if (utf8_count_1 < 0) {
      ++lerror_count;
      if (lfirst_error == 0) {
        lfirst_error = utf8_count_1;
        if (first_error_location) *first_error_location = utf8 + i_from;
      }
      ++i_from;
    } else if (i_from + utf8_count_1 > utf8_count) {
      ++lerror_count;
      if (lfirst_error == 0) {
        lfirst_error = UTF8CVTN_INCOMPLETE_UTF8_SEQUENCE;
        if (first_error_location) *first_error_location = utf8 + i_from;
      }
      break;
    } else if (i_to >= codepoint_count) {
      ++lerror_count;
      if (lfirst_error == 0) {
        lfirst_error = UTF8CVTN_INSUFFICIENT_SPACE;
        if (first_error_location) *first_error_location = utf8 + i_from;
      }
      break;
    } else {
      int codepoint = utf8cvt1_codepoint_from_utf8(utf8_count_1,
                                                   utf8+i_from);
      if (codepoint < 0) {
        ++lerror_count;
        if (lfirst_error == 0) {
          lfirst_error = codepoint;
          if (first_error_location) *first_error_location = utf8 + i_from;
        }
      } else {
        codepoints[i_to++] = codepoint;
      } 
      i_from += utf8_count_1;
    }
  }
  if (error_count) *error_count = lerror_count;
  if (first_error) *first_error = lfirst_error;
}

/*************************************************************/
#endif /*UTF8CVTN_DO_IMPLEMENTATION*/
