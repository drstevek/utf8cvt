#! /bin/sh
set -x
test_utf8cvt1_errors.sh
test_utf8cvtn_errors.sh
test_utf8cvt1.sh
test_utf8cvtn.sh
rm -rf tempwork
