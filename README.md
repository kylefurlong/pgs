# pgs

An entropy source needing only a 32bit hardware cycle counter and 64bit operations. Weak entropy from the cycle counter avalanches nonlinearly through a dynamic nop loop, and finally passes through the ** mixer of David Blackman and Sebastiano Vigna. Pull requests for interesting platforms and other bitnesses are heartily welcomed!

Thanks to Walter Bright for the clean rdtsc asm, and for his long years of service in the C/C++/D community

### Benchmarking

pgs passes PractRand, ENT, and homebrew tests mostly on the strength of **. One starts to think of pgs as a xoshiro style generator where the state space is dynamically sized to the entropy basis of the cycle counter, at the added latency cost of the cycle counter itself. Not being an academic themself, the author does not make any attempt to calculate the period of pgs as generator

Practically speaking, pgs provides a replacement to urandom and rdrand either on systems where these are not supported, or where their throughput, latency, or soundness does not meet a certain project's requirements. Note that on recent Intel, pgs outperforms rdrand by an order of magnitude. Comparisons with most urandom implementations are similarly favorable, only to a lesser degree

#### Homebrew

Testing pgs 🔬

  Verifying uniqueness

    000000 0x01e91c1851f792a1
    020000 0x7a047eb12690b2f2
    040000 0xd58b4eb9dc96f292
    060000 0x9ceee78a2cc8aaff
    080000 0x1e1c1bf5c217d511
    100000 0x2ef46cd6ed9a6a0a

  Each value unique  ✓

  Benchmarking latency

    ns_avg | 8

  Validating run lengths

    zr_max | 32
    zr_avg | 3

  Validating bit count ratio

    bcr_avg | 1.0000173

  Validating bit pattern noncorrelation

    trie_depth | 00 [1199991527, 1200008473] 1.0000
    trie_depth | 01 [599989502, 600018971] 1.0000
    trie_depth | 02 [299978121, 300028435] 1.0002
    trie_depth | 03 [149981906, 150024904] 1.0003
    trie_depth | 04 [74982427, 75018949] 1.0005
    trie_depth | 05 [37482405, 37515605] 1.0009
    trie_depth | 06 [18735808, 18762923] 1.0014
    trie_depth | 07 [9366566, 9383156] 1.0018
    trie_depth | 08 [4680840, 4694341] 1.0029
    trie_depth | 09 [2337993, 2349104] 1.0048
    trie_depth | 10 [1167970, 1175888] 1.0068
    trie_depth | 11 [582581, 588563] 1.0103
    trie_depth | 12 [290969, 294829] 1.0133
    trie_depth | 13 [144697, 147947] 1.0225
    trie_depth | 14 [72160, 74303] 1.0297
    trie_depth | 15 [35757, 37448] 1.0473


Baselining with rdrand 🔬

  Verifying uniqueness

    000000 0x449548512efdc0a6
    020000 0xffdaeef4800b54cf
    040000 0x9f47d40863613b59
    060000 0xb053e73312b003f3
    080000 0xe1cc4483059d1aab
    100000 0x813e72cf68b690cb

  Each value unique  ✓

  Benchmarking latency

    ns_avg | 100

  Validating run lengths

    zr_max | 31
    zr_avg | 3

  Validating bit count ratio

    bcr_avg | 0.9999772

  Validating bit pattern noncorrelation

    trie_depth | 00 [1199960744, 1200039256] 1.0001
    trie_depth | 01 [599941062, 600020100] 1.0001
    trie_depth | 02 [299943387, 300020591] 1.0003
    trie_depth | 03 [149948320, 150016287] 1.0005
    trie_depth | 04 [74973322, 75016672] 1.0006
    trie_depth | 05 [37485673, 37518946] 1.0009
    trie_depth | 06 [18737796, 18760076] 1.0012
    trie_depth | 07 [9364956, 9384283] 1.0021
    trie_depth | 08 [4681709, 4694630] 1.0028
    trie_depth | 09 [2338047, 2349022] 1.0047
    trie_depth | 10 [1167370, 1176153] 1.0075
    trie_depth | 11 [583248, 588948] 1.0098
    trie_depth | 12 [291033, 295227] 1.0144
    trie_depth | 13 [144994, 148091] 1.0214
    trie_depth | 14 [72186, 74421] 1.0310
    trie_depth | 15 [35865, 37386] 1.0424


#### PractRand

    RNG_test using PractRand version 0.94
    RNG = RNG_stdin64, seed = unknown
    test set = core, folding = standard (64 bit)

    rng=RNG_stdin64, seed=unknown
    length= 512 megabytes (2^29 bytes), time= 3.4 seconds
      no anomalies in 226 test result(s)

    rng=RNG_stdin64, seed=unknown
    length= 1 gigabyte (2^30 bytes), time= 7.2 seconds
      no anomalies in 243 test result(s)

    rng=RNG_stdin64, seed=unknown
    length= 2 gigabytes (2^31 bytes), time= 14.3 seconds
      no anomalies in 261 test result(s)

    rng=RNG_stdin64, seed=unknown
    length= 4 gigabytes (2^32 bytes), time= 27.6 seconds
      no anomalies in 277 test result(s)

    rng=RNG_stdin64, seed=unknown
    length= 8 gigabytes (2^33 bytes), time= 54.3 seconds
      no anomalies in 294 test result(s)

    rng=RNG_stdin64, seed=unknown
    length= 16 gigabytes (2^34 bytes), time= 107 seconds
      no anomalies in 310 test result(s)

#### ENT

    ent for urandom

    Entropy = 1.000000 bits per bit.

    Optimum compression would reduce the size
    of this 8614019344 bit file by 0 percent.

    Chi square distribution for 8614019344 samples is 0.92, and randomly
    would exceed this value 33.81 percent of the times.

    Arithmetic mean value of data bits is 0.5000 (0.5 = random).
    Monte Carlo value for Pi is 3.141480279 (error 0.00 percent).
    Serial correlation coefficient is -0.000012 (totally uncorrelated = 0.0).


    ent for pgs

    Entropy = 1.000000 bits per bit.

    Optimum compression would reduce the size
    of this 8614019344 bit file by 0 percent.

    Chi square distribution for 8614019344 samples is 1.12, and randomly
    would exceed this value 28.97 percent of the times.

    Arithmetic mean value of data bits is 0.5000 (0.5 = random).
    Monte Carlo value for Pi is 3.141543491 (error 0.00 percent).
    Serial correlation coefficient is -0.000000 (totally uncorrelated = 0.0).
