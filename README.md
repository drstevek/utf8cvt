utf8cvt1.h provides very simple C functions (which can also be used in
C++), to convert single characters from a 32-bit unicode "codepoint"
value to the corresponding 1 to 4 utf8 bytes, or do the reverse.

utf8cvtn.h provides for converting multiple characters, in either
direction.

In the interests of simplicity and generality, no memory allocation is
included.  You would usually first use one function to determine the
amount of memory space needed for output, then allocate the space
needed (or use a buffer you already have one that is large enough),
then call another function to do the actual conversion.

To use these, just copy one or both .h files into your project,
and include them where you need them.

Remember to create an implementation, in one .c or .cpp file, by also
defining the corresponding preprocessor constant that forces a
definition.  The included test examples (test_utf8cvt1.c and
test_utf8cvtn.c) show examples of this.  test_utf8cvtn.c has :
```C
    #define UTF8CVT1_DO_IMPLEMENTATION
    #include "utf8cvt1.h"
    #define UTF8CVTN_DO_IMPLEMENTATION
    #include "utf8cvtn.h"
```

Note that these .h files can be used in other ways as well, including
putting them inside C++ namespaces, or using as file static functions
in only one file.  Each function is preceeded by a preprocessor
constant, e.g. UTF8CVTN_EXTERN, that defaults to empty, but could
easily be defined to be something like "static", or "inline", or some
form of "extern", before the corresponding .h file is included.

The test examples files test_utf8cvt1.c and test_utf8cvtn.c serve both
as examples, and tests.  Though to fully run the shell script tests,
you will need some unix utilities, and iconv.
However, you only need the one or two .h files to use this code.
No other files are necessary.

The motivation for creating these files is : C and C++ programmers
have needed simple functions to do these crucial conversions for over
two decades.  However, not until last year, was there a gcc compiler
version (gcc 5) that included support for the part of the C++11
standard that gives library support to do these conversions.  Not
everybody is able yet to switch away from slightly older compilers,
like gcc 4, that do NOT support these conversions via the standard
C++ library.

My guess is that mixing up the essential utf8 conversion functions
with the highly non-portable "wide" character conversion functions
(which the C++ standards committee did), contributed to the gcc team's
(or libc++ library team's) delays in implementing this.

This has left programmers with the choices of : ignore full utf-8
handling, or use a large messy library (like libiconv or icu), or
write their own utf8 to/from 32-bit unicode conversion functions.

A problem with the libiconv and icu libraries is that they are quite
large, and nontrivial to either find, or get to compile, on different
platforms.  They also offer an interesting mess of licenses.  libiconv
is LGPL, which is usually okay to link to, but then it is unclear if
you can pick and choose source code pieces from the library, instead
of taking the whole thing, without the danger of GPL infection.

This, really tiny solution, allows you to just drop two small .h files
in your project, and carry on.  The BSD license on these files means
that your project can be free or GPL or commercial, this BSD license
is compatible with all of them.

