//
//  PrettyGoodSeed
//  Copyright 2019 Kyle Furlong
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//  
//      http://www.apache.org/licenses/LICENSE-2.0
//  
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

#include <stdlib.h>
#include <time.h>
#include <unistd.h>

static uint64_t _pgsX = 0x7e86a8c1936a6c28;
static uint64_t _pgsA = 0xac87c8c81d99fcf5;

uint64_t pgs() {
    uint64_t sW = time(0) & 0xFF;
    sW = (sW << 0L) | (sW << 8L) |
         (sW << 16) | (sW << 24) |
         (sW << 32) | (sW << 40) |
         (sW << 48) | (sW << 56) ;

    uint64_t sH = (uint64_t)malloc(8);
    uint64_t sS = (uint64_t)&sH;

    sS =  sH << 32 | (sS & 0xFFFFFFFF);
    sS ^= sW; free((void*)sH);

    uint64_t sC = clock() & 0xFF;
    sC = (sC << 0L) | (sC << 8L) |
         (sC << 16) | (sC << 24) |
         (sC << 32) | (sC << 40) |
         (sC << 48) | (sC << 56) ;

    _pgsX ^= sS ^ sC;

    return _pgsX ^ _pgsA++;
}

// Test, run once on your platform then drop. Please
// contact kylefurlong at gmail com if you encounter
// any mix errors or repeats

#include <stdio.h>
#include <assert.h>

int main() {
    const size_t run = 100000;
    struct timespec ts;

    uint64_t* seeds = (uint64_t*)malloc(run*(sizeof(uint64_t)));

    for (int i = 0; i < run; i++) {
        seeds[i] = pgs();
        if (i % 25 == 0) printf("%06d 0x%08llx\n", i, seeds[i]);
        for (int k = 0; k < i; k++) {
            if (seeds[k] == seeds[i]) {
                printf("0x%08llx (%d == %d)\n", seeds[k], k, i);
                assert(seeds[k] != seeds[i]); // Uniqueness error
            }
        }
    }

    uint64_t tz = 0;
    for (int i = 0; i < run; i++) {
        uint64_t s = seeds[i];

        for (int k = 0; k < 64; k++) {
            tz += __builtin_ctzl(s >> k);
        }
    }

    tz = tz / run;

    printf("\navg tz: %llu\n\n", tz);
    assert(tz > 59 && tz < 69); // Mix error
}
