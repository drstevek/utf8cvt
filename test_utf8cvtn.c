/* test_utf8cvtn.c */
/* test of utf8cvtn.h */

#define UTF8CVT1_DO_IMPLEMENTATION
#include "utf8cvt1.h"

#define UTF8CVTN_DO_IMPLEMENTATION
#include "utf8cvtn.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void writeBytes(FILE *stream, int count, unsigned char *bytes)
{
  for ( ; count > 0; --count) {
    fputc(*bytes++, stream);
  }
}

/** If this is a big endian machine, endian flip the 4-byte entries.*/
void maybe_endian_flip(unsigned byte_count, unsigned char *data)
{
  unsigned val = 0x04030201;
  if (0x01 == *(unsigned char *)(&val)) {
    /* little endian, skip */
    return;
  }
  byte_count -= byte_count & 3;
  for (int i = 0; i < byte_count; i+= 4) {
    unsigned char temp = data[0];
    data[0] = data[3];
    data[3] = temp;
    temp = data[1];
    data[1] = data[2];
    data[2] = temp;
  }
}

void read_all_in_data(FILE *stream, unsigned *in_data_size, unsigned char **in_data)
{
  unsigned allocated = 2;
  unsigned lin_data_size = 0;
  unsigned char *lin_data = (unsigned char*)malloc(allocated);
  int cin;
  while ((cin = getchar()) != EOF) {
    if (lin_data_size >= allocated) {
      unsigned new_allocated = allocated << 1;
      unsigned char *new_lin_data = (unsigned char*)malloc(new_allocated);
      memcpy(new_lin_data, lin_data, lin_data_size);
      free(lin_data);
      allocated = new_allocated;
      lin_data = new_lin_data;
    }
    lin_data[lin_data_size++] = cin;
  }
  *in_data_size = lin_data_size;
  *in_data = lin_data;
}

int main(int argc, char **argv)
{
  int from_utf8 = 0;
  int to_utf8 = 0;
  if (argc == 2) {
    if (0==strcmp("--utf8_to_utf32le", argv[1])) { from_utf8 = 1; }
    else if (0==strcmp("--utf32le_to_utf8", argv[1])) { to_utf8 = 1; }
  }
  if (!from_utf8 && !to_utf8) {
    printf("Usage: %s [--utf8_to_utf32le|--utf32le_to_utf8] < infile > outfile\n", argv[0]);
    return(0);
  }
  unsigned in_data_size = 0;
  unsigned char *in_data = NULL;
  unsigned out_data_size = 0;
  unsigned char *out_data = NULL;
  unsigned error_count = 0;
  read_all_in_data(stdin, &in_data_size, &in_data);
  if (to_utf8) {
    maybe_endian_flip(in_data_size, in_data);
    unsigned utf8_count = 0;
    utf8cvtn_utf8_count_from_codepoints(in_data_size >> 2,
                                        (unsigned*)in_data,
                                        &utf8_count, &error_count,
                                        NULL, NULL);
    out_data_size = utf8_count;
    out_data = malloc(utf8_count);
    utf8cvtn_utf8_from_codepoints(in_data_size >> 2,
                                  (unsigned*)in_data,
                                  utf8_count, out_data, &error_count,
                                  NULL, NULL);
  } else if (from_utf8) {
    unsigned codepoint_count = 0;
    utf8cvtn_codepoint_count_from_utf8(in_data_size, in_data,
                                       &codepoint_count, &error_count,
                                       NULL, NULL);
    out_data_size = codepoint_count << 2;
    out_data = malloc(out_data_size);
    utf8cvtn_codepoints_from_utf8(in_data_size, in_data,
                                  codepoint_count, (unsigned*)out_data,
                                  &error_count, NULL, NULL);
    maybe_endian_flip(out_data_size, out_data);
  }
  writeBytes(stdout, out_data_size, out_data);
}
