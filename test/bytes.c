#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../pgs.c"

#define BLOCK 16777216

int main(int argc, char** argv) {
    setbuf(stdout, NULL);
    uint64_t* next = (uint64_t*)malloc(BLOCK);
    while (1) {
        for (int i = 0; i < BLOCK/8; i++) {
            *(next+i) = pgs();
        }
        write(1, next, BLOCK);
        usleep(0);
    }
    exit(0);
}
