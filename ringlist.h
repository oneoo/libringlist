//
//  ringlist.h
//
//
//  Created by oneoo on 13-9-11.
//
//

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#ifndef _ringlist_h
#define _ringlist_h

#define rl_t void

typedef struct rl_node_s {
    void *uper;         //private
    void *next;         //private
    uint64_t _k;        //private
    void *ptr;
} rl_node_t;

uint32_t fnv1_32 ( const char *data, uint32_t len );

void *rl_create(uint32_t n);
uint32_t rl_counts(rl_t *rl);
rl_node_t *rl_add(rl_t *rl, uint32_t k);
void rl_del(rl_t *rl, rl_node_t *n);
rl_node_t *rl_each(rl_t* rl, uint32_t k);
void *rl_pop(rl_t *rl, uint32_t k);
void *rl_shift(rl_t *rl, uint32_t k);
typedef int rl_cmp_h(rl_node_t *n, void *d);
rl_node_t *rl_find(rl_t *rl, uint32_t k, rl_cmp_h *f, void *d);
void rl_destory(rl_t *rl);

#endif
