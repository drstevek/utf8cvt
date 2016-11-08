/* test_utf8cvtn_errors.c */
/* test error results from utf8cvtn.h */

#define UTF8CVT1_DO_IMPLEMENTATION
#include "utf8cvt1.h"
#define UTF8CVTN_DO_IMPLEMENTATION
#include "utf8cvtn.h"

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

int test_utf8cvtn_errors()
{
  int fail_count = 0;
  int pass_count = 0;

  {
    unsigned codepoints[4] = { 0x32, 0x200000, 0x34, 0x200000 };
    unsigned utf8_count = 0;
    unsigned char utf8s[4] = {0,0,0,0};
    unsigned error_count = 0;
    int      first_error = 0;
    unsigned *first_error_location = NULL;
    utf8cvtn_utf8_count_from_codepoints(4, codepoints, &utf8_count,
                                        NULL, NULL, NULL);
    expect((2 == utf8_count),
           __LINE__, __FILE__, &fail_count, &pass_count);
    utf8_count = 0;
    utf8cvtn_utf8_count_from_codepoints(4, codepoints, &utf8_count,
                                        &error_count,
                                        &first_error,
                                        &first_error_location);
    expect((2 == utf8_count),
           __LINE__, __FILE__, &fail_count, &pass_count);
    expect((2 == error_count),
           __LINE__, __FILE__, &fail_count, &pass_count);
    expect((UTF8CVT1_ILLEGAL_CODEPOINT == first_error),
           __LINE__, __FILE__, &fail_count, &pass_count);
    expect((1 == (first_error_location - codepoints)),
           __LINE__, __FILE__, &fail_count, &pass_count);

    utf8_count = 2;
    error_count = 0;
    first_error = 0;
    first_error_location = NULL;
    utf8cvtn_utf8_from_codepoints(4, codepoints,
                                  utf8_count, utf8s,
                                  &error_count,
                                  &first_error,
                                  &first_error_location);
    expect((2 == error_count),
           __LINE__, __FILE__, &fail_count, &pass_count);
    expect((UTF8CVT1_ILLEGAL_CODEPOINT == first_error),
           __LINE__, __FILE__, &fail_count, &pass_count);
    expect((1 == (first_error_location - codepoints)),
           __LINE__, __FILE__, &fail_count, &pass_count);
    expect((codepoints[2] == (unsigned)(utf8s[1])),
           __LINE__, __FILE__, &fail_count, &pass_count);

    codepoints[1] = 0x33;
    utf8_count = 2;
    error_count = 0;
    first_error = 0;
    first_error_location = NULL;
    utf8cvtn_utf8_from_codepoints(4, codepoints,
                                  utf8_count, utf8s,
                                  &error_count,
                                  &first_error,
                                  &first_error_location);
    expect((1 == error_count),
           __LINE__, __FILE__, &fail_count, &pass_count);
    expect((UTF8CVTN_INSUFFICIENT_SPACE == first_error),
           __LINE__, __FILE__, &fail_count, &pass_count);
    expect((2 == (first_error_location - codepoints)),
           __LINE__, __FILE__, &fail_count, &pass_count);
    expect((codepoints[1] == (unsigned)(utf8s[1])),
           __LINE__, __FILE__, &fail_count, &pass_count);
    
  }

  {
    unsigned char utf8s1[4] = {0x32,0xBE,0x34,0xDF};
    unsigned codepoint_count = 0;
    unsigned codepoints[6] = {0,0,0,0,0,0};
    unsigned error_count = 0;
    int      first_error = 0;
    unsigned char *first_error_location = NULL;

    utf8cvtn_codepoint_count_from_utf8(4, utf8s1, &codepoint_count,
                                       &error_count, &first_error,
                                       &first_error_location);
    expect((2 == codepoint_count),
           __LINE__, __FILE__, &fail_count, &pass_count);
    expect((2 == error_count),
           __LINE__, __FILE__, &fail_count, &pass_count);
    expect((UTF8CVT1_ILLEGAL_CONTINUATION_AS_LEAD_BYTE
            == first_error),
           __LINE__, __FILE__, &fail_count, &pass_count);
    expect((1 == (first_error_location - utf8s1)),
           __LINE__, __FILE__, &fail_count, &pass_count);

    error_count = 0;
    first_error = 0;
    first_error_location = NULL;
    codepoint_count = 2;
    utf8cvtn_codepoints_from_utf8(4, utf8s1, codepoint_count,
                                  codepoints,
                                  &error_count, &first_error,
                                  &first_error_location);
    expect((2 == error_count),
           __LINE__, __FILE__, &fail_count, &pass_count);
    expect((UTF8CVT1_ILLEGAL_CONTINUATION_AS_LEAD_BYTE
            == first_error),
           __LINE__, __FILE__, &fail_count, &pass_count);
    expect((1 == (first_error_location - utf8s1)),
           __LINE__, __FILE__, &fail_count, &pass_count);
    expect((codepoints[1] == utf8s1[2]),
           __LINE__, __FILE__, &fail_count, &pass_count);

    utf8s1[1] = 0x33;
    utf8cvtn_codepoint_count_from_utf8(4, utf8s1, &codepoint_count,
                                       &error_count, &first_error,
                                       &first_error_location);
    expect((3 == codepoint_count),
           __LINE__, __FILE__, &fail_count, &pass_count);
    expect((1 == error_count),
           __LINE__, __FILE__, &fail_count, &pass_count);
    expect((UTF8CVTN_INCOMPLETE_UTF8_SEQUENCE
            == first_error),
           __LINE__, __FILE__, &fail_count, &pass_count);
    expect((3 == (first_error_location - utf8s1)),
           __LINE__, __FILE__, &fail_count, &pass_count);

    codepoint_count = 3;
    error_count = 0;
    first_error = 0;
    first_error_location = NULL;
    utf8cvtn_codepoints_from_utf8(4, utf8s1, codepoint_count,
                                  codepoints, 
                                  &error_count, &first_error,
                                  &first_error_location);
    expect((1 == error_count),
           __LINE__, __FILE__, &fail_count, &pass_count);
    expect((UTF8CVTN_INCOMPLETE_UTF8_SEQUENCE
            == first_error),
           __LINE__, __FILE__, &fail_count, &pass_count);
    expect((3 == (first_error_location - utf8s1)),
           __LINE__, __FILE__, &fail_count, &pass_count);
    expect((codepoints[2] == utf8s1[2]),
           __LINE__, __FILE__, &fail_count, &pass_count);

    codepoint_count = 2;
    error_count = 0;
    first_error_location = NULL;
    utf8cvtn_codepoints_from_utf8(4, utf8s1, codepoint_count,
                                  codepoints, 
                                  &error_count, &first_error,
                                  &first_error_location);
    expect((1 == error_count),
           __LINE__, __FILE__, &fail_count, &pass_count);
    expect((UTF8CVTN_INSUFFICIENT_SPACE
            == first_error),
           __LINE__, __FILE__, &fail_count, &pass_count);
    expect((2 == (first_error_location - utf8s1)),
           __LINE__, __FILE__, &fail_count, &pass_count);
  }           

  fprintf(stdout, "%d TESTS_FAILED %d TESTS_PASSED\n",
          fail_count, pass_count);
  return((fail_count > 0)?(-1):(0));
}

int main()
{
  return(test_utf8cvtn_errors());
}
