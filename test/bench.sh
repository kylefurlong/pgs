#!/bin/sh
platforms=(centos alpine ubuntu)

cp ../pgs.c ./
for platform in ${platforms[*]}; do
    docker build -t pgs:$platform -f Dockerfile.$platform . 1>/dev/null
done
    
for platform in ${platforms[*]}; do
    printf "$platform: "
    docker run -it -m 8g --memory-swappiness=0 pgs:$platform
done

./run.sh "-DNDEBUG -DPGS_TEST_BATCHES=3000 -DTR_ENABLE -DTR_BITS=1 -DTR_BYTES=2"
