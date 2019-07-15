#include "pgs.c"
#define SEED (mt19937_rand64())
#include "test.h"
#include <stdio.h>
#include <assert.h>

int main() {
    printf("\nTesting MT19937-64 seeded with pgs%s", "");
    mt19937_srand(pgs());
    test();
}
