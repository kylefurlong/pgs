#include "pgs.c"
#define SEED (pgs())
#include "test.h"
#include <stdio.h>
#include <assert.h>

int main() {
    printf("\nTesting pgs%s", "");
    test();
}
