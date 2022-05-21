#!/bin/sh
set -e

rm -rf bin
mkdir bin

echo "Building libraries ..."

cc -std=c99 -fPIC -c src/umber.c -o bin/libumber.o -I src

ar rc bin/libumber.a bin/libumber.o
ranlib bin/libumber.a

cc -shared bin/libumber.o -o bin/libumber.so

echo "Installing libraries & headers (/usr/local) ..."

su_list="cp $(realpath src/umber.h) /usr/local/include"
su_list="$su_list && cp $(realpath bin/libumber.a) $(realpath bin/libumber.so) /usr/local/lib"

su -l root -c "$su_list"
