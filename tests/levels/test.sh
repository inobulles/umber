#!/bin/sh
set -e

clear_="\033[0m"
regular="\033[0;"
bold="\033[1;"

purple="35m"
red="31m"
yellow="33m"
green="32m"
blue="34m"
grey="37m"

func_name=_$(openssl rand -hex 6)

# build the example we'll be testing on

component=$(openssl rand -base64 6)
src_path=levels.c

test_str=$(openssl rand -base64 6)
test_fmt="$test_str %s"
test_arg=$(openssl rand -base64 6)

cc -std=c99 $src_path -DCOMPONENT=\"$component\" -DFUNC_NAME=$func_name -DTEST_FMT=\""$test_fmt"\" -DTEST_ARG=\"$test_arg\" -I. -lumber -o levels

# test all the log levels

printf "$bold$purple[FATAL $component -> $src_path:5 -> $func_name]$regular$purple $test_str $test_arg$clear_
$bold$red[ERROR $component -> $src_path:6 -> $func_name]$regular$red $test_str $test_arg$clear_
$bold$yellow[WARN $component -> $src_path:7 -> $func_name]$regular$yellow $test_str $test_arg$clear_
$bold$green[SUCCESS $component -> $src_path:8 -> $func_name]$regular$green $test_str $test_arg$clear_
$bold$blue[INFO $component -> $src_path:9 -> $func_name]$regular$blue $test_str $test_arg$clear_
$bold$grey[VERBOSE $component -> $src_path:10 -> $func_name]$regular$grey $test_str $test_arg$clear_
" > expected

for lvl in $(seq 0 5); do
	expected_path=expected$lvl
	got_path=got$lvl

	head -n$(($lvl + 1)) expected > $expected_path
	UMBER_LVL=$lvl ./levels > $got_path 2>&1

	diff $expected_path $got_path || echo "Level $1 got unexpected output"
done
