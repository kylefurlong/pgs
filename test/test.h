#include <assert.h>
#include <inttypes.h>
#include <time.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "validate.h"

static volatile int ending = 0;
void stop(int unused) {
    ending = 1;
}

int test() {
    signal(SIGINT, stop);

    setbuf(stdout, NULL);

    printf("\n");

    const int vbatches = PGS_TEST_BATCHES;
    const int batch = 100000;

    uint64_t* seeds = (uint64_t*)malloc(batch*(sizeof(uint64_t)));

    for (int i = 2; i > 0; i--) {
        printf("\r%d", i);
        sleep(1); // Let clocks run so we ensure no bias in run length
    }
    printf("\r%s\n", " ");

    printf("  Verifying uniqueness\n\n");

    for (int i = 0; i <= batch; i++) {
        seeds[i] = SEED;
        if (i % 25 == 0) printf("\r    %06d 0x%016jx", i, (uintmax_t)seeds[i]);
        if (i % 20000 == 0) printf("\n");
        for (int k = 0; k < i; k++) {
            if (seeds[k] == seeds[i]) {
                printf("0x%016jx (%d == %d)\n", (uintmax_t)seeds[k], k, i);
                assert(seeds[k] != seeds[i]); // Uniqueness error
                exit(1);
            }
        }
    }
    printf("\n");

    printf("  Each value unique  ✓\n\n");

#ifdef CLOCK_PROCESS_CPUTIME_ID
    printf("  Benchmarking latency\n\n");

    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    uint64_t t = (ts.tv_sec * 1000000000) + ts.tv_nsec;
    for (int i = 0; i <= batch; i++) {
        seeds[i] = SEED;
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    t = (ts.tv_sec * 1000000000) + ts.tv_nsec - t;

    printf("    ns_avg | %d\n", (uint32_t)(uintmax_t)(t / batch));

#endif

    printf("\n  Validating run lengths\n\n");

    uintmax_t zrs = 0;
    uint32_t zrm = 0;

    for (int k = 1; k <= vbatches; k++) {
        for (int i = 0; i <= batch; i++) {
            seeds[i] = SEED;
        }

        MaxSum zr;

        validate_zr(seeds, batch, &zr);

        if (zr.max > zrm) zrm = zr.max;
        zrs += zr.sum;

        printf("\r    zr_batch | %03u [%04d of %04d]", zr.max, k, vbatches);

        if (ending) {
            break;
        }
    }

    printf("\r    zr_max | %u                           ", zrm);
    printf("\n    zr_avg | %u\n", (uint32_t)(zrs / (vbatches*batch)));

    printf("\n  Validating bit count ratio\n\n");

    double bcrs = 0.0;
    for (int k = 1; k <= vbatches; k++) {
        for (int i = 0; i <= batch; i++) {
            seeds[i] = SEED;
        }

        double bcr = validate_bc(seeds, batch);
        bcrs += bcr;

        printf("\r    bcr_batch | %0.2f [%04d of %04d]", bcr, k, vbatches);

        if (ending) {
            break;
        }
    }

    printf("\r    bcr_avg | %0.7f                        \n", bcrs/(double)vbatches);


#ifdef TR_ENABLE
    printf("\n  Validating bit pattern noncorrelation\n\n");

    for (int k = 1; k <= vbatches; k++) {
        for (int i = 0; i <= batch; i++) {
            seeds[i] = SEED;
        }

        validate_pre(seeds, batch, &ending);

        printf("\r    trie_batch | 00 [%04d of %04d]", k, vbatches);

        if (ending) {
            break;
        }
    }

    printf("\r                                      ");

    if (!ending) validate_commit();
#endif

    return ending;
}
