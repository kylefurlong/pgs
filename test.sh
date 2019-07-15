#!/bin/sh
printf "\n"
uname -a;

flags="-std=gnu99 -pedantic -Wall -Os -I/usr/local/include -I/usr/include"
linker=""

if [ -f /etc/centos-release ]; then
    linker="${linker}-lrt";
fi

gcc $flags pgs.c $linker -o pgs_test && ./pgs_test && rm pgs_test
