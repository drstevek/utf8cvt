/* test_utf8cvt1.c */
/* test of utf8cvt1.h
 * writes out 2 files, one with all of the valid codepoints,
 * and another file with that translated to utf8,
 * also tests for each codepoint, that going from the codepoint
 * to utf8, and back, gives the original codepoint.
 * All that remains then is to validate the output utf8 file by using
 * another utility to do the conversion from the codepoint utf32le
 * file to the utf8 (or the reverse) and compare.
 */

/* cc -o test_utf8cvt1 test_utf8cvt1.c */

#define UTF8CVT1_DO_IMPLEMENTATION
#include "utf8cvt1.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int testOne(unsigned codepoint, int *count, unsigned char *bytes)
{
  int lcount1 = utf8cvt1_utf8_count_from_codepoint(codepoint);
  if (lcount1 <= 0) {
    fprintf(stderr,"utf8cvt1_utf8_count_from_codepoint() FAILED\n");
    return(0);
  }
  *count = lcount1;
  int lcount2 = utf8cvt1_utf8_from_codepoint(codepoint, lcount1, bytes);
  if (lcount2 <= 0) {
    fprintf(stderr,"utf8cvt1_utf8_from_codepoint() FAILED\n");
    return(0);
  }
  if (lcount2 != lcount1) {
    fprintf(stderr,"MISMATCHED count from utf8cvt1_utf8_from_codepoint()\n");
    return(0);
  }
  int lcount3 = utf8cvt1_utf8_count_from_utf8_lead(bytes[0]);
  if (lcount3 <= 0) {
    fprintf(stderr,"utf8cvt1_utf8_count_from_utf8_lead() FAILED\n");
    return(0);
  }
  if (lcount3 != lcount1) {
    fprintf(stderr,"MISMATCHED count from utf8cvt1_utf8_count_from_utf8_lead()\n");
    return(0);
  }
  int codepoint_out = utf8cvt1_codepoint_from_utf8(lcount3, bytes);
  if (codepoint_out < 0) {
    fprintf(stderr,"utf8cvt1_codepoint_from_utf8() FAILED\n");
    return(0);
  }
  if (codepoint_out != codepoint) {
    fprintf(stderr,"MISMATCHED codepoint from utf8cvt1_from_utf8\n");
    return(0);
  }
  return(1);
}

void writeBytes(FILE *stream, int count, unsigned char *bytes)
{
  for ( ; count > 0; --count) {
    fputc(*bytes++, stream);
  }
}

void unsigned2le_bytes(unsigned value, unsigned char *bytes)
{
  for (int count = 0; count < 4; ++count, value >>= 8) {
    *bytes++ = (0xff & value);
  }
}

char *string_join(const char *s1, const char *s2)
{
  char *result = malloc(strlen(s1) + strlen(s2) + 1);
  strcpy(result, s1);
  strcat(result, s2);
  return(result);
}

int main(int argc, char **argv)
{
  if (argc < 2) {
    printf("Usage: %s outfileprefix\n", argv[0]);
    return(0);
  }
  const char *outfileprefix = argv[1];
  char *fc_name = string_join(outfileprefix, ".UTF32LE");
  char *fu_name = string_join(outfileprefix, ".UTF8");
  FILE *fc = fopen(fc_name,"w");
  FILE *fu = fopen(fu_name,"w");
  unsigned char bytes[4];
  int count;
  for (unsigned codepoint = 0; codepoint <= 0x10FFFF; ++codepoint) {
    if (codepoint == 0xD800)  codepoint = 0xE000;
    if (!testOne(codepoint, &count, bytes)) {
      fprintf(stderr,"test_utf8cvt1 FAILED at codepoint=%d\n",codepoint);
      break;
    }
    writeBytes(fu, count, bytes);
    unsigned2le_bytes(codepoint, bytes);
    writeBytes(fc, 4, bytes);
  }
  fclose(fc);
  fclose(fu);
  printf("wrote %s\n", fc_name);
  printf("wrote %s\n", fu_name);
  return(0);
}
