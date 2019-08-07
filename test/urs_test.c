#define SEED (urs())

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static FILE* urfd;
void* r = NULL;
static uint64_t urs() {
    if (r && urfd) {
        if (fread(r, 8, 1, urfd)) {
            return *(uint64_t*)r;
        }
    }
    return 0;
}

#include "test.h"

int main() {
    r = malloc(8);
    urfd = fopen("/dev/urandom", "r");
    if (!urfd) {
        printf("\nNo urandom on this platform%s", "");
        exit(0);
    } else {
        printf("\n\nBaselining with urandom %s", "🔬");
        return test();
    }
}
