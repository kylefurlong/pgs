#!/bin/sh
platforms=(centos alpine ubuntu)

for platform in ${platforms[*]}; do
    docker build -t pgs:$platform -f Dockerfile.$platform . 1>/dev/null
done
    
./test.sh
for platform in ${platforms[*]}; do
    printf "$platform: "
    docker run -it pgs:$platform
done
