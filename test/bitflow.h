typedef struct BitFlow BitFlow;
struct BitFlow {
    uint8_t* data;
    uintmax_t bit;
    size_t end;
};

static inline uint32_t _bf_mod8(uint32_t n) {
    return n & 0x7; 
}

static inline uint32_t _bf_div8(uint32_t n) {
    return n >> 3; 
}

static inline uint8_t _bf_cb(BitFlow* _bf) {
    return *(_bf->data+(_bf_div8(_bf->bit)));
}

void bf_init(BitFlow* _bf, void* data, size_t len) {
    _bf->data = data;
    _bf->end = len;
    _bf->bit = 0;
}

uint64_t bf_next(BitFlow* _bf, size_t n) {
    uint64_t out = 0;

    for (int i = 0; i < n; i++) {
        size_t wit = _bf_mod8(_bf->bit);
        out |= ((_bf_cb(_bf) & (0x1 << wit)) >> (wit-i));
        _bf->bit = _bf->bit + 1;
    }

    return out;
}
