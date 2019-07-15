#include <inttypes.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>

static FILE* urfd;
uint64_t urs() {
    void* r = malloc(8);
    if (r && urfd) {
        if (fread(r, 8, 1, urfd)) {
            return *(uint64_t*)r;
        }
    }
    return 0;
}

//
//  mt19937-64 PRNG
//
//  retrieved from http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/VERSIONS/C-LANG/mt19937-64.c
//  on 2019-07-10, and modified for use with fastlist.h 
//      - remove unneeded functions
//      - identifiers changed to avoid collisions
//      - minor formatting tweaks
//
//  my thanks cannot add to Matsumoto-shi and Nishimura-shi 's
//      accolades, nevertheless, thanks I give
//

/* 
   A C-program for MT19937-64 (2004/9/29 version).
   Coded by Takuji Nishimura and Makoto Matsumoto.

   This is a 64-bit version of Mersenne Twister pseudorandom number
   generator.

   Before using, initialize the state by using init_genrand64(seed)  
   or init_by_array64(init_key, key_length).

   Copyright (C) 2004, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.                          

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote 
        products derived from this software without specific prior written 
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   References:
   T. Nishimura, ``Tables of 64-bit Mersenne Twisters''
     ACM Transactions on Modeling and 
     Computer Simulation 10. (2000) 348--357.
   M. Matsumoto and T. Nishimura,
     ``Mersenne Twister: a 623-dimensionally equidistributed
       uniform pseudorandom number generator''
     ACM Transactions on Modeling and 
     Computer Simulation 8. (Jan. 1998) 3--30.

   Any feedback is very welcome.
   http://www.math.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove spaces)
*/

#define FL_MT_NN 312
#define FL_MT_MM 156
#define FL_MT_MATRIX_A 0xB5026F5AA96619E9ULL
#define FL_MT_UM 0xFFFFFFFF80000000ULL /* Most significant 33 bits */
#define FL_MT_LM 0x7FFFFFFFULL /* Least significant 31 bits */

/* The array for the state vector */
static uint64_t _fl_mt[FL_MT_NN]; 
/* mti==NN+1 means mt[NN] is not initialized */
static int _fl_mti=FL_MT_NN+1; 

/* initializes mt[NN] with a seed */
void mt19937_srand(uint64_t seed)
{
    _fl_mt[0] = seed;
    for (_fl_mti=1; _fl_mti<FL_MT_NN; _fl_mti++) 
        _fl_mt[_fl_mti] = (6364136223846793005ULL * (_fl_mt[_fl_mti-1] ^ (_fl_mt[_fl_mti-1] >> 62)) + _fl_mti);
}

/* generates a random number on [0, 2^64-1]-interval */
uint64_t mt19937_rand64(void)
{
    int i;
    uint64_t x;
    static uint64_t mag01[2]={0ULL, FL_MT_MATRIX_A};

    if (_fl_mti >= FL_MT_NN) { /* generate NN words at one time */
        for (i=0;i<FL_MT_NN-FL_MT_MM;i++) {
            x = (_fl_mt[i]&FL_MT_UM)|(_fl_mt[i+1]&FL_MT_LM);
            _fl_mt[i] = _fl_mt[i+FL_MT_MM] ^ (x>>1) ^ mag01[(int)(x&1ULL)];
        }
        for (;i<FL_MT_NN-1;i++) {
            x = (_fl_mt[i]&FL_MT_UM)|(_fl_mt[i+1]&FL_MT_LM);
            _fl_mt[i] = _fl_mt[i+(FL_MT_MM-FL_MT_NN)] ^ (x>>1) ^ mag01[(int)(x&1ULL)];
        }
        x = (_fl_mt[FL_MT_NN-1]&FL_MT_UM)|(_fl_mt[0]&FL_MT_LM);
        _fl_mt[FL_MT_NN-1] = _fl_mt[FL_MT_MM-1] ^ (x>>1) ^ mag01[(int)(x&1ULL)];

        _fl_mti = 0;
    }
  
    x = _fl_mt[_fl_mti++];

    x ^= (x >> 29) & 0x5555555555555555ULL;
    x ^= (x << 17) & 0x71D67FFFEDA60000ULL;
    x ^= (x << 37) & 0xFFF7EEE000000000ULL;
    x ^= (x >> 43);

    return x;
}

void test() {
    const size_t run = 100000;
    uint64_t* seeds = (uint64_t*)malloc(run*(sizeof(uint64_t)));

    setbuf(stdout, NULL);
    printf("\n");

    for (int i = 3; i > 0; i--) {
        printf("\r%d", i);
        sleep(1); // Let clocks run so we ensure no bias in run length
    }
    printf("\r%s\n", " ");

    for (int i = 0; i <= run; i++) {
        seeds[i] = SEED;
        if (i % 25 == 0) printf("\r%06d 0x%08jx", i, (uintmax_t)seeds[i]);
        if (i % 20000 == 0) printf("\n");
        for (int k = 0; k < i; k++) {
            if (seeds[k] == seeds[i]) {
                printf("0x%08jx (%d == %d)\n", (uintmax_t)seeds[k], k, i);
                assert(seeds[k] != seeds[i]); // Uniqueness error
            }
        }
    }

    printf("\n");

#ifdef CLOCK_PROCESS_CPUTIME_ID

    struct timespec ts;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
    uint64_t t = (ts.tv_sec * 1000000000) + ts.tv_nsec;
    for (int i = 0; i <= run; i++) {
        seeds[i] = SEED;
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
    t = (ts.tv_sec * 1000000000) + ts.tv_nsec - t;

    printf("avg ns: %d\n", (uint32_t)(uintmax_t)(t / run));

#endif

    int tz = 0;
    for (int i = 0; i < run; i++) {
        uint64_t s = seeds[i];

        for (int k = 0; k < 62; k++) {
            tz += __builtin_ctzl(s >> k);
        }
    }
    tz = tz / run;

    printf("avg tz: %d\n", tz);
    assert(tz > 54 && tz < 74); // Mix error
}
