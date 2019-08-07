#!/bin/sh
printf "\n"
uname -a;

flags="-std=gnu99 -pedantic -Wall -O3 -I/usr/local/include -I/usr/include $1"
linker=""

if [ -f /etc/centos-release ]; then
    linker="${linker}-lrt";
fi

gcc $flags pgs_test.c $linker -o pgs_test && \
gcc $flags rdr_test.c $linker -o rdr_test

 ./pgs_test && ./rdr_test

rm pgs_test &>/dev/null
rm rdr_test &>/dev/null

printf "\n"
