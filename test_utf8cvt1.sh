#! /bin/sh
set -x
mkdir tempwork
cc -o tempwork/test_utf8cvt1 test_utf8cvt1.c
tempwork/test_utf8cvt1 tempwork/all
iconv -f UTF32LE -t UTF8 tempwork/all.UTF32LE > tempwork/all.UTF8.iconv
echo "Expect cmp to tell are the same"
cmp tempwork/all.UTF8 tempwork/all.UTF8.iconv
ls -ls tempwork/all.*
echo "Sanity check, not zeroed files"
od -tx1 -N 16 tempwork/all.UTF8
od -tx1 -j 256 -N 16 tempwork/all.UTF8
od -tx4 -N 16 tempwork/all.UTF32LE
od -tx4 -j 256 -N 16 -j 256 tempwork/all.UTF32LE
echo "Can clean up by : rm -rf tempwork"


