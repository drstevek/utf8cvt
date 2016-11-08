/* test_utf8cvt1_errors.c */
/* test error results from utf8cvt1.h */

#define UTF8CVT1_DO_IMPLEMENTATION
#include "utf8cvt1.h"

#include <stdio.h>

void expect(int truth, int line, const char *filename,
            int *fail_count, int *pass_count)
{
  if (truth) { ++(*pass_count); }
  else {
    ++(*fail_count);
    fprintf(stderr, "TEST FAILED at line %d in %s\n",
            line, filename);
  }
}

int test_utf8cvt1_errors()
{
  int fail_count = 0;
  int pass_count = 0;
  unsigned char utf8_dest[4] = {0,0,0,0,};
  unsigned char utf8_source[6] = {0,0,0,0,0,0};
  
  expect((1 == utf8cvt1_utf8_count_from_codepoint(0x31)),
         __LINE__, __FILE__, &fail_count, &pass_count);
  expect((4 == utf8cvt1_utf8_count_from_codepoint(0x10FFFF)),
         __LINE__, __FILE__, &fail_count, &pass_count);
  expect((UTF8CVT1_ILLEGAL_CODEPOINT
          == utf8cvt1_utf8_count_from_codepoint(0xD801)),
         __LINE__, __FILE__, &fail_count, &pass_count);
  expect((UTF8CVT1_ILLEGAL_CODEPOINT
          == utf8cvt1_utf8_count_from_codepoint(0x110000)),
         __LINE__, __FILE__, &fail_count, &pass_count);

  expect((UTF8CVT1_ILLEGAL_CODEPOINT
          == utf8cvt1_utf8_from_codepoint(0x110000, UTF8CVT1_ILLEGAL_CODEPOINT,
                                          utf8_dest)),
         __LINE__, __FILE__, &fail_count, &pass_count);
  expect((UTF8CVT1_ILLEGAL_BYTE_COUNT
          == utf8cvt1_utf8_from_codepoint(0x32, 5, utf8_dest)),
         __LINE__, __FILE__, &fail_count, &pass_count);

  expect((UTF8CVT1_ILLEGAL_CONTINUATION_AS_LEAD_BYTE
          == utf8cvt1_utf8_count_from_utf8_lead(0xBE)),
         __LINE__, __FILE__, &fail_count, &pass_count);
  expect((UTF8CVT1_ILLEGAL_LEAD_BYTE
          == utf8cvt1_utf8_count_from_utf8_lead(0xF8)),
         __LINE__, __FILE__, &fail_count, &pass_count);

  expect((UTF8CVT1_ILLEGAL_LEAD_BYTE
          == utf8cvt1_codepoint_from_utf8(UTF8CVT1_ILLEGAL_LEAD_BYTE,
                                          utf8_source)),
         __LINE__, __FILE__, &fail_count, &pass_count);
  expect((UTF8CVT1_ILLEGAL_BYTE_COUNT
          == utf8cvt1_codepoint_from_utf8(5, utf8_source)),
         __LINE__, __FILE__, &fail_count, &pass_count);

  fprintf(stdout, "%d TESTS_FAILED %d TESTS_PASSED\n",
          fail_count, pass_count);
  return((fail_count > 0)?(-1):(0));
}

int main()
{
  return(test_utf8cvt1_errors());
}
