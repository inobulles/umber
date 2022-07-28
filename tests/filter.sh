#!/bin/sh
set -e

. tests/common.sh

# build the example we'll be testing on
# would have like to use some kind of dynamic variables thing but what the heck, ain't nobody got time to test this on the 50 different shells which may be linked to '/bin/sh'

src_path=tests/filter.c

comp_a=$(openssl rand -base64 6)
comp_b=$(openssl rand -base64 6)
comp_c=$(openssl rand -base64 6)

spec_a=$(openssl rand -base64 6)
spec_b=$(openssl rand -base64 6)
spec_c=$(openssl rand -base64 6)

cc -std=c99 $src_path -I src -L bin -lumber -o .testfiles/filter \
	-DFUNC_NAME=$func_name \
	-DCOMP_A=\"$comp_a\" -DSPEC_A=\"$spec_a\" \
	-DCOMP_B=\"$comp_b\" -DSPEC_B=\"$spec_b\" \
	-DCOMP_C=\"$comp_c\" -DSPEC_C=\"$spec_c\"

# clear relevant environment variables to make sure they don't contaminate our testing setup

unset UMBER_BLACKLIST
unset UMBER_WHITELIST

# so that we don't have to think about it each time again, export the log level once here

export UMBER_LVL=5

# expected output

printf "$bold$grey[VERBOSE $comp_a -> $src_path:5 -> $func_name]$regular$grey $spec_a$clear_
$bold$grey[VERBOSE $comp_b -> $src_path:9 -> $func_name]$regular$grey $spec_b$clear_
$bold$grey[VERBOSE $comp_c -> $src_path:13 -> $func_name]$regular$grey $spec_c$clear_
" > .testfiles/expected

# test no filtering

.testfiles/filter > .testfiles/got
diff .testfiles/expected .testfiles/got

# test blacklisting

UMBER_BLACKLIST=$comp_a .testfiles/filter > .testfiles/got
grep -v $comp_a .testfiles/expected | diff .testfiles/got -

UMBER_BLACKLIST=$comp_a:$comp_b .testfiles/filter > .testfiles/got
grep -v $comp_a'\|'$comp_b .testfiles/expected | diff .testfiles/got -

UMBER_BLACKLIST=$comp_a:$comp_b:$comp_c .testfiles/filter > .testfiles/got
grep -v $comp_a'\|'$comp_b'\|'$comp_c .testfiles/expected | diff .testfiles/got -

# test whitelisting

UMBER_WHITELIST=$comp_a .testfiles/filter > .testfiles/got
grep $comp_a .testfiles/expected | diff .testfiles/got -

UMBER_WHITELIST=$comp_a:$comp_b .testfiles/filter > .testfiles/got
grep $comp_a'\|'$comp_b .testfiles/expected | diff .testfiles/got -

UMBER_WHITELIST=$comp_a:$comp_b:$comp_c .testfiles/filter > .testfiles/got
grep $comp_a'\|'$comp_b'\|'$comp_c .testfiles/expected | diff .testfiles/got -

# test both at once (blacklist should take precedence)

UMBER_BLACKLIST=$comp_a:$comp_c .testfiles/filter > .testfiles/expected
UMBER_BLACKLIST=$comp_a:$comp_c UMBER_WHITELIST=$comp_b .testfiles/filter > .testfiles/got

diff .testfiles/expected .testfiles/got
