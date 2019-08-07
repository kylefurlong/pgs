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

static uint64_t _pgsX = 0x7e86a8c1936a6c28;

static inline uint64_t rotl(uint64_t x, uint16_t r) {
    return (x << r) | (x >> (64 - r));
}

uint64_t pgs() {
    uint32_t cL, cH;
    __asm__ __volatile__ ("rdtsc" : "=a" (cL), "=d" (cH));

    uint64_t sC = (cL >> 2) & 0xFFFF;
    sC = (sC << 0L) | (sC << 16) |
         (sC << 32) | (sC << 48) ;

    uint32_t iM = (cL & 0x3) | 0x1;
    while(iM--) {
        __asm__ ("nop");
    }

    _pgsX = rotl((_pgsX ^ sC) * 5, 7) * 9;

    return _pgsX;
}
