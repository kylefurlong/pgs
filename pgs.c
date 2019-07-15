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

#include <stdint.h>
#include <unistd.h>

static uint64_t _pgsX = 0x7e86a8c1936a6c28;
static uint64_t _pgsA = 0xa8c7ca831d99fbf5;

uint64_t pgs() {
    uint32_t cL, cH;
    __asm__ __volatile__ ("rdtsc" : "=a" (cL), "=d" (cH));

    uint64_t sC = cL & 0xFF;
    sC = (sC << 0L) | (sC << 8L) |
         (sC << 16) | (sC << 24) |
         (sC << 32) | (sC << 40) |
         (sC << 48) | (sC << 56) ;

    uint64_t sS = (uint64_t)&sC;

    sS  = sS << 32 | (sS & 0xFFFFFFFF);

    size_t iM = ((sS >> (cL & 0x3F)) & 0x3) | 0x1;
    while (iM--) { // Foil timing attacks
        __asm__ ("nop");
    }

    sS ^= sC;

    _pgsX ^= sS;

    return _pgsX ^ _pgsA++;
}
