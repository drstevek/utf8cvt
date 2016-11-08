#! /bin/sh
set -x
mkdir tempwork
cc -o tempwork/test_utf8cvt1 test_utf8cvt1.c
tempwork/test_utf8cvt1 tempwork/all
cc -o tempwork/test_utf8cvtn test_utf8cvtn.c
tempwork/test_utf8cvtn --utf8_to_utf32le < tempwork/all.UTF8 > tempwork/all.UTF32LE.cvtn
tempwork/test_utf8cvtn --utf32le_to_utf8 < tempwork/all.UTF32LE > tempwork/all.UTF8.cvtn
echo "Expect cmp to tell are the same"
cmp tempwork/all.UTF8 tempwork/all.UTF8.cvtn
cmp tempwork/all.UTF32LE tempwork/all.UTF32LE.cvtn
ls -ls tempwork/all.*
echo "Can clean up by : rm -rf tempwork"
