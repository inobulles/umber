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

# Build the example we'll be testing on.
# Would have like to use some kind of dynamic variables thing but what the heck, ain't nobody got time to test this on the 50 different shells which may be linked to '/bin/sh'.

src_path=tests/filter/filter.c

class_a=$(openssl rand -hex 6)-A
class_b=$(openssl rand -hex 6)-B
class_c=$class_b.$(openssl rand -hex 6)-C

spec_a=$(openssl rand -base64 6)
spec_b=$(openssl rand -base64 6)
spec_c=$(openssl rand -base64 6)

if [ "$(uname)" = "Darwin" ]; then
	rpath="-rpath /usr/local"
fi

cc -std=c99 $src_path $rpath -lumber -I/usr/local/include -o filter \
	-DCLASS_A=\"$class_a\" -DSPEC_A=\"$spec_a\" \
	-DCLASS_B=\"$class_b\" -DSPEC_B=\"$spec_b\" \
	-DCLASS_C=\"$class_c\" -DSPEC_C=\"$spec_c\"

# Expected output.

printf "$bold$grey[V $class_a $src_path:8]$regular$grey $spec_a$clear_
$bold$grey[V $class_b $src_path:9]$regular$grey $spec_b$clear_
$bold$grey[V $class_c $src_path:10]$regular$grey $spec_c$clear_
" > expected

# Test no filtering.

UMBER_LVL= ./filter > got
diff expected got

# Test blacklisting.

UMBER_LVL=$class_a=never ./filter > got
grep -v $class_a expected | diff got -

UMBER_LVL=$class_a=never,$class_c=never ./filter > got
grep -v $class_a'\|'$class_c expected | diff got -

UMBER_LVL=$class_a=never,$class_b*=never ./filter > got
grep -v $class_a'\|'$class_b\ '\|'$class_c expected | diff got -

UMBER_LVL=$class_a=never,$class_b=never ./filter > got
grep -v $class_a'\|'$class_b\  expected | diff got -

# Test whitelisting.

UMBER_LVL=*=never,$class_a=verbose ./filter > got
grep $class_a expected | diff got -

UMBER_LVL=*=never,$class_a=verbose,$class_b=verbose ./filter > got
grep $class_a'\|'$class_b\  expected | diff got -

UMBER_LVL=*=never,$class_a=verbose,$class_b*=verbose ./filter > got
grep $class_a'\|'$class_b\ '\|'$class_c expected | diff got -

UMBER_LVL=*=never,$class_a=verbose,$class_c=verbose ./filter > got
grep $class_a'\|'$class_c expected | diff got -
