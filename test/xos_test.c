#include <stdint.h>

#define SEED ((xoshiro256()))

#include "../pgs.c"

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

#include "test.h"

int main() {
    s[0] = pgs(); s[1] = pgs(); s[2] = pgs(); s[3] = pgs();
    printf("\n\nTesting xoshiro256** with pgs seeds%s", "🔬");
    return test();
}
