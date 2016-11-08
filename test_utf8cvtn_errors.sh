#! /bin/sh
set -x
mkdir tempwork
cc -o tempwork/test_utf8cvtn_errors test_utf8cvtn_errors.c
tempwork/test_utf8cvtn_errors
echo "Can clean up by : rm -rf tempwork"


