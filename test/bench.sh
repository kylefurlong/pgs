#!/bin/sh
platforms=(centos alpine ubuntu)

cp ../pgs.c ./
for platform in ${platforms[*]}; do
    docker build -t pgs:$platform -f Dockerfile.$platform . 1>/dev/null
done
    
./run.sh
for platform in ${platforms[*]}; do
    printf "$platform: "
    docker run -it pgs:$platform
done
