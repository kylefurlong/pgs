#!/bin/sh
printf "\n"
uname -a;

flags="-std=gnu99 -pedantic -Wall -Os -I/usr/local/include -I/usr/include"
linker=""

if [ -f /etc/centos-release ]; then
    linker="${linker}-lrt";
fi

gcc $flags urs_test.c $linker -o urs_test && ./urs_test && rm urs_test
gcc $flags pgs_test.c $linker -o pgs_test && ./pgs_test && rm pgs_test
gcc $flags mtp_test.c $linker -o mtp_test && ./mtp_test && rm mtp_test

printf "\n"
