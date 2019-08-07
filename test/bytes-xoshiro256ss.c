#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../pgs.c"

#define BLOCK 16777216

static uint64_t s[4];

uint64_t xoshiro256(void) {
	const uint64_t result_starstar = rotl(s[1] * 5, 7) * 9;

	const uint64_t t = s[1] << 17;

	s[2] ^= s[0];
	s[3] ^= s[1];
	s[1] ^= s[2];
	s[0] ^= s[3];

	s[2] ^= t;

	s[3] = rotl(s[3], 45);

	return result_starstar;
}

int main(int argc, char** argv) {
    setbuf(stdout, NULL);
    s[0] = pgs(); s[1] = pgs(); s[2] = pgs(); s[3] = pgs();

    uint64_t* next = (uint64_t*)malloc(BLOCK);
    while (1) {
        for (int i = 0; i < BLOCK/8; i++) {
            *(next+i) = xoshiro256();
        }
        write(1, next, BLOCK);
        usleep(0);
    }
    exit(0);
}
