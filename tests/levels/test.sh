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

# Build the example we'll be testing on.

class_name=$(openssl rand -hex 6)
src_path=tests/levels/levels.c

test_str=$(openssl rand -hex 6)
test_fmt="$test_str %s"
test_arg=$(openssl rand -hex 6)

if [ "$(uname)" = "Darwin" ]; then
	rpath="-rpath /usr/local"
fi

cc -std=c99 $src_path $rpath -lumber -I/usr/local/include -o levels \
	-DCLASS_NAME=\"$class_name\" \
	-DTEST_FMT=\""$test_fmt"\" -DTEST_ARG=\"$test_arg\"

# Test all the log levels.

printf "
$bold$purple[F $class_name $src_path:9]$regular$purple $test_str $test_arg$clear_
$bold$red[E $class_name $src_path:10]$regular$red $test_str $test_arg$clear_
$bold$yellow[W $class_name $src_path:11]$regular$yellow $test_str $test_arg$clear_
$bold$blue[I $class_name $src_path:12]$regular$blue $test_str $test_arg$clear_
$bold$grey[V $class_name $src_path:13]$regular$grey $test_str $test_arg$clear_
" > expected

for lvl in $(seq 0 5); do
	expected_path=expected$lvl
	got_path=got$lvl

	head -n$(($lvl + 1)) expected > $expected_path
	UMBER_LVL=*=$lvl ./levels > $got_path 2>&1

	diff $expected_path $got_path || echo "Level $lvl got unexpected output"
done
