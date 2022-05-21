#!/bin/sh
set -e

rm -rf bin
mkdir bin

echo "Building libraries ..."

# _GNU_SOURCE define for vasprintf on GNU (see feature_test_macros(7))
cc -std=c99 -D_GNU_SOURCE -fPIC -c src/umber.c -o bin/libumber.o -I src

ar rc bin/libumber.a bin/libumber.o
ranlib bin/libumber.a

cc -shared bin/libumber.o -o bin/libumber.so

echo "Running tests ..."

rm -rf .testfiles
mkdir .testfiles

for path in $(find -L tests -maxdepth 1 -type f -name "*.sh"); do
    echo -n "Running $path test ..."
    LD_LIBRARY_PATH=bin sh $path
    echo " ✅ Passed"
done

rm -r .testfiles

if [ $# -gt 0 ]; then
    exit 0
fi

echo "Installing libraries & headers (/usr/local) ..."

su_list="cp $(realpath src/umber.h) /usr/local/include"
su_list="$su_list && cp $(realpath bin/libumber.a) $(realpath bin/libumber.so) /usr/local/lib"

su -l root -c "$su_list"
