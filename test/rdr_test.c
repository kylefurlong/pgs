#include <stdint.h>

#define SEED (rdrand())
static uint64_t __attribute__((__target__("rdrnd"))) rdrand() {
    unsigned long long r;
    __builtin_ia32_rdrand64_step(&r);
    return (uint64_t)r;
}
#include "test.h"

int main() {
    printf("\n\nBaselining with rdrand %s", "🔬");
    return test();
}
