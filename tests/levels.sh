#!/bin/sh
set -e

# build the example we'll be testing on

component=$(openssl rand -base64 6)
src_path=tests/levels.c
func_name=_$(openssl rand -hex 6)

test_str=$(openssl rand -base64 6)
test_fmt="$test_str %s"
test_arg=$(openssl rand -base64 6)

cc -std=c99 $src_path -DCOMPONENT=\"$component\" -DFUNC_NAME=$func_name -DTEST_FMT=\""$test_fmt"\" -DTEST_ARG=\"$test_arg\" -I src -L bin -lumber -o .testfiles/levels

# test all the log levels

clear="\033[0m"
regular="\033[0;"
bold="\033[1;"

purple="35m"
red="31m"
yellow="33m"
green="32m"
blue="34m"
grey="37m"

echo -e "$bold$purple[FATAL $component -> $src_path -> $func_name -> 5]$regular$purple $test_str $test_arg$clear
$bold$red[ERROR $component -> $src_path -> $func_name -> 6]$regular$red $test_str $test_arg$clear
$bold$yellow[WARN $component -> $src_path -> $func_name -> 7]$regular$yellow $test_str $test_arg$clear
$bold$green[SUCCESS $component -> $src_path -> $func_name -> 8]$regular$green $test_str $test_arg$clear
$bold$blue[INFO $component -> $src_path -> $func_name -> 9]$regular$blue $test_str $test_arg$clear
$bold$grey[VERBOSE $component -> $src_path -> $func_name -> 10]$regular$grey $test_str $test_arg$clear" > .testfiles/expected

for lvl in $(seq 0 5); do
	expected_path=.testfiles/expected$lvl
	got_path=.testfiles/got$lvl

	head -n$(($lvl + 1)) .testfiles/expected > $expected_path
	UMBER_LVL=$lvl .testfiles/levels > $got_path 2>&1

	diff $expected_path $got_path
done