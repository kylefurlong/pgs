# pgs
A random seed for platforms without a robust entropy source

Sample test on macOS: `ns` is `nanoseconds/seed`; `tz` is `for i in 63 { tz += ctzl(seed >> i) }`

    Darwin Tantalus.local 18.6.0 Darwin Kernel Version 18.6.0: Thu Apr 25 23:16:27 PDT 2019; root:xnu-4903.261.4~2/RELEASE_X86_64 x86_64

    Baseline with urandom

    000000 0xe99aa5c6ccfcd08a
    020000 0x7a985ebaa59c7b3c
    040000 0x54cbc998e568f09d
    060000 0x5f49ad4d0d545549
    080000 0x8838a4fc512b25e7
    100000 0x85c0c7bcac074732

    avg ns: 224
    avg tz: 60

    Testing pgs

    000000 0x495db16e11ef44f1
    020000 0x95816db2cd3029cd
    040000 0xcfdb37e8976aa1b7
    060000 0xe7f31fc0bf42f7ff
    080000 0x9f8b67b8c73b5da7
    100000 0x4155b96619e53599

    avg ns: 9
    avg tz: 61

    Testing MT19937-64 seeded with pgs

    000000 0x53a6c6eb34bac61c
    020000 0x2322236a07d3dbc0
    040000 0xff243f039bf1d851
    060000 0xb169563c4b2bc371
    080000 0xa860ddc080e6c5c0
    100000 0x3e97eb16d99d4b1e

    avg ns: 3
    avg tz: 60
