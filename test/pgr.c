#include "pgs.c"

static uint64_t _pgr_xsx = 0x21313568715d164a; 
static uint64_t _pgr_xsy = 0xfd765a9b857e4ca5;
static uint64_t _pgr_xsz = 0xe7b59edabbd414b7; 

void pgr_reseed() {
    _pgr_xsx = pgs(); _pgr_xsy = pgs(); _pgr_xsz = pgs();
}

uint64_t pgr() {
    if (_pgr_xsx & 0xFFFF) {
        uint64_t tmp = (_pgr_xsx^(_pgr_xsx<<15));
        _pgr_xsx = _pgr_xsy; _pgr_xsy = _pgr_xsz;
        _pgr_xsz = (_pgr_xsz^(_pgr_xsz>>21))^(tmp^(tmp>>4));
    } else {
        pgr_reseed();
    }
    return _pgr_xsz;
} // G. Marsaglia's xorshift, reseeded with pgs periodically
