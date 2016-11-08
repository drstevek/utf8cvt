#! /bin/sh
set -x
mkdir tempwork
cc -o tempwork/test_utf8cvt1_errors test_utf8cvt1_errors.c
tempwork/test_utf8cvt1_errors
echo "Can clean up by : rm -rf tempwork"


