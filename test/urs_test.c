#define SEED (urs())
#include "test.h"

int main() {
    urfd = fopen("/dev/urandom", "r");
    if (!urfd) {
        printf("\nNo urandom on this platform%s", "");
        exit(0);
    } else {
        printf("\nBaseline with urandom%s", "");
        test();
    }
}
