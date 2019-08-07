#define SEED (pgr())
#include "pgr.c"

#include "test.h"

int main() {
    printf("\n\nTesting pgr %s", "🔬");
    pgr_reseed();
    return test();
}
