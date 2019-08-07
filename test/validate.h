#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "bitflow.h"

#ifndef TR_BITS
#define TR_BITS 1
#endif

#ifndef TR_BYTES
#define TR_BYTES 1
#endif

typedef struct MaxSum MaxSum;
struct MaxSum {
    uintmax_t sum;
    uint32_t max;
};

typedef struct TrieNode TrieNode;
struct TrieNode {
    TrieNode** spans;
    uint64_t  num;
};

typedef struct Trie Trie;
struct Trie {
    TrieNode* nil;
    size_t    bps;
    size_t    nsp;
};

typedef void(*TrieVisitor)(void* st, size_t d, uint64_t n);

typedef struct TrieWalker TrieWalker;
struct TrieWalker {
    void* state;
    TrieVisitor visitor;
};

TrieNode* _trie_node_init(const Trie* _tr) {
    assert(_tr);
    assert(_tr->nsp);

    TrieNode* _tr_node = (TrieNode*)calloc(1, sizeof(TrieNode));
    assert(_tr_node);

    _tr_node->spans = (TrieNode**)calloc(_tr->nsp, sizeof(TrieNode*));
    assert(_tr_node->spans);

    _tr_node->num = 1;

    assert(_tr_node && _tr_node->spans && _tr_node->num == 1);
    return _tr_node;
}

Trie* trie_init(size_t bps) {
    assert(bps > 0);
    assert(bps < 65);

    Trie* _tr = (Trie*)calloc(1, sizeof(Trie));
    assert(_tr);

    _tr->bps = bps;
    _tr->nsp = 0x1 << bps;
    _tr->nil = _trie_node_init(_tr);

    return _tr;
}

void trie_absorb(const Trie* _tr, void* data, size_t len) {
    assert(data);
    assert(_tr);
    assert(_tr->bps > 0);
    assert(len >= _tr->bps/8);
    assert(_tr->nil->spans);

#ifndef NDEBUG
    bool l = false;
    if (((uint64_t)data) % 10001 == 0) {
        l = true;
        printf("\rtrie_absorb | ");
    }
#endif

    size_t bps = _tr->bps;
    size_t bits = len * 8;
    uint64_t span = 0;
    TrieNode* _cur = _tr->nil;
    BitFlow _bf;
    bf_init(&_bf, data, len);

    do {
        bits -= bps; span = bf_next(&_bf, bps);

#ifndef NDEBUG
        if (l) printf("%hhx ", (uint8_t)span);
#endif

        TrieNode* _nxt = _cur->spans[span];

        if (!_nxt) {
            _nxt = _trie_node_init(_tr);
            _cur->spans[span] = _nxt;
        } else {
            _nxt->num++;
            assert(_nxt->num);
        }

        _cur = _nxt;
    } while (bits >= bps);
} 

void _trie_walk(const Trie* _tr, const TrieNode* _tr_node, const TrieWalker* _tr_walker, size_t d) {
    assert(_tr);
    assert(_tr_node);
    assert(_tr_node->spans);
    assert(_tr_walker);
    assert(_tr_walker->visitor);

    _tr_walker->visitor(_tr_walker->state, d, _tr_node->num);

    for (int i = 0; i < _tr->nsp; i++) {
        TrieNode* _n = _tr_node->spans[i];
        if (_n) {
            assert(_n->num);
            _trie_walk(_tr, _n, _tr_walker, d+1);
        }
    }
}

TrieWalker trie_walk(const Trie* _tr, const TrieVisitor visitor, void* state) {
    assert(_tr);
    assert(_tr->nil->spans);
    assert(visitor);

    TrieWalker walker;

    walker.state = state;
    walker.visitor = visitor;

    TrieNode* _nil = _tr->nil;

    _trie_walk(_tr, _nil, &walker, 0);

    return walker;
}

typedef struct ValidDistState ValidDistState;
struct ValidDistState {
    uint32_t* min;
    uint32_t* max;
};

void valid_dist_visitor(void* state, size_t d, uint64_t num) {
    assert(state);
    assert(d >= 0);
    assert(num > 0);

    ValidDistState* s = (ValidDistState*)state;

    if (d > 0) {
        if (s->max[d-1] < num) {
            s->max[d-1] = num;
            assert(s->max[d-1] > 0);
        }

        if (s->min[d-1] > num) {
            s->min[d-1] = num;
        }
    }
}

void valid_dist_by_trie(const Trie* _tr, size_t d) {
    assert(_tr);
    assert(_tr->nil->spans);
    assert(d > 0);

    ValidDistState state;

    state.min = (uint32_t*)calloc(d, sizeof(uint32_t));
    state.max = (uint32_t*)calloc(d, sizeof(uint32_t));

    for (int i = 0; i < d; i++) {
        state.min[i] = UINT32_MAX;
    }

    trie_walk(_tr, &valid_dist_visitor, &state);

    for (int i = 0; i < d; i++) {
        float balance = (float)state.max[i] / (float)state.min[i];
        if (i == 0) printf("\r");
        printf("    trie_depth | %02d [%d, %d] %.04f\n", i, state.min[i], state.max[i], balance);
    }

}

const size_t _tr_bits = TR_BITS;
const size_t _tr_bytes = TR_BYTES;

const Trie* _tr = NULL;

void validate_pre(uint64_t* seeds, size_t num, volatile int* ending) {
    assert(seeds);
    assert(num > 0);

    if (!_tr) _tr = trie_init(_tr_bits);

    for (int i = 0; i < num * 8; i++) {
        trie_absorb(_tr, (((char*)seeds)+i), _tr_bytes);
        if (*ending) break;
    }
}

void validate_commit() {
    valid_dist_by_trie(_tr, _tr_bytes*8 / _tr_bits);
}

void validate_zr(uint64_t* seeds, size_t num, MaxSum* _ms) {
    uintmax_t zrs = 0;
    uint32_t zrm = 0;

    BitFlow _bf;
    bf_init(&_bf, seeds, num);

    uint32_t rz = 0;
    uint8_t cur = 1;

    for (int i = 0; i < num*8; i++) {
        uint8_t nxt = bf_next(&_bf, 1);
        if (nxt != cur) {
            zrs += rz;
            if (rz > zrm) zrm = rz;
            rz = 0;
        } else {
            rz++;
        }
        cur = nxt;
    }

    _ms->sum = zrs;
    _ms->max = zrm;
}

double validate_bc(uint64_t* seeds, size_t num) {
    uintmax_t c0 = 0;
    uintmax_t c1 = 0;

    BitFlow _bf;
    bf_init(&_bf, seeds, num);

    for (int i = 0; i < num*8; i++) {
        if (bf_next(&_bf, 1)) {
            c1++;
        } else {
            c0++;
        }
    }

    return (double)c0 / (double)c1;
}
