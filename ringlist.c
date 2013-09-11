//
//  ringlist.c
//
//
//  Created by oneoo on 13-9-11.
//
//

#include "ringlist.h"

uint32_t fnv1_32 ( const char *data, uint32_t len )
{
    uint32_t rv = 2166136261;
    uint32_t i;

    for ( i = 0; i < len; i++ ) {
        rv += (rv<<1) + (rv<<4) + (rv<<7) + (rv<<8) + (rv<<24);
        rv = rv ^ data[i];
    }

    return rv;
}

void *rl_create(uint32_t n)
{
    if(n<1) {
        return NULL;
    }

    void *rt = malloc(sizeof(uint32_t)+sizeof(uint32_t)+n*2*sizeof(void*));

    if(rt) {
        memcpy(rt, &n, sizeof(uint32_t));
        memset(rt+sizeof(uint32_t), 0, sizeof(uint32_t)+n*2*sizeof(void*));
    }

    return rt;
}

uint32_t rl_counts(rl_t *rl)
{
    uint32_t *rt = rl+sizeof(uint32_t);
    return *rt;
}

rl_node_t *rl_add(rl_t *rl, uint32_t k)
{
    rl_node_t *rln = malloc(sizeof(rl_node_t));
    memset(rln, 0, sizeof(rl_node_t));

    uint32_t *r = rl;
    uint32_t *c = rl+sizeof(uint32_t);
    *c+=1;

    rln->_k = (k%*r);

    void **l = rl+sizeof(uint32_t)+*r*sizeof(void*)+sizeof(uint32_t)+(k%*r)*sizeof(void*); // last

    if(*l) {
        rl_node_t *n = (rl_node_t*)*l;
        n->next = rln;
        rln->uper = *l;
        *l = rln;
    } else {
        void **f = rl+sizeof(uint32_t)+sizeof(uint32_t)+(k%*r)*sizeof(void*); // first

        *f = rln;
        *l = rln;
    }

    return rln;
}

void rl_del(rl_t *rl, rl_node_t *n)
{
    if(!n) {
        return;
    }

    if(n->uper) {
        ((rl_node_t*)n->uper)->next = n->next;
    }
    if(n->next) {
        ((rl_node_t*)n->next)->uper = n->uper;
    }

    uint32_t *r = rl;
    uint32_t *c = rl+sizeof(uint32_t);
    *c-=1;

    void **f = rl+sizeof(uint32_t)+sizeof(uint32_t)+n->_k*sizeof(void*); // first
    void **l = rl+sizeof(uint32_t)+*r*sizeof(void*)+sizeof(uint32_t)+n->_k*sizeof(void*); // last

    if(*f == n) {
        *f = n->next;
    }
    if(*l == n) {
        *l = n->uper;
    }


    free(n);
}

static rl_node_t *__rl_p = NULL;
rl_node_t *rl_each(rl_t* rl, uint32_t k)
{
    if(rl) {
        __rl_p = NULL;
        uint32_t *r = rl;
        void **f = rl+sizeof(uint32_t)+sizeof(uint32_t)+(k%*r)*sizeof(void*); // first

        if(*f) {
            __rl_p = *f;
        }
    } else {
        __rl_p = __rl_p->next;
    }

    return __rl_p;
}

void *rl_pop(rl_t *rl, uint32_t k)
{
    uint32_t *r = rl;
    void **f = rl+sizeof(uint32_t)+sizeof(uint32_t)+(k%*r)*sizeof(void*); // first

    if(*f) {
        void *ptr = ((rl_node_t*)*f)->ptr;
        rl_del(rl, *f);
        return ptr;
    }

    return NULL;
}

void *rl_shift(rl_t *rl, uint32_t k)
{
    uint32_t *r = rl;
    void **l = rl+sizeof(uint32_t)+sizeof(uint32_t)+*r*sizeof(void*)+(k%*r)*sizeof(void*); // last

    if(*l) {
        void *ptr = ((rl_node_t*)*l)->ptr;
        rl_del(rl, *l);
        return ptr;
    }

    return NULL;
}

rl_node_t *rl_find(rl_t *rl, uint32_t k, rl_cmp_h *f, void *d)
{
    rl_node_t *n = rl_each(rl, k);
    while(n) {
        if(f(n, d)) {
            return n;
        }
        n = n->next;
    }

    return NULL;
}

void rl_destory(rl_t *rl)
{
    uint32_t *r = rl;
    uint32_t i = 0;

    void **f = NULL;
    rl_node_t *n = NULL, *d = NULL;

    for(i=0; i<*r; i++) {
        f = rl+sizeof(uint32_t)+sizeof(uint32_t)+(i%*r)*sizeof(void*); // first
        n = *f;
        while(n) {
            d = n;
            n = n->next;
            free(d);
        }
    }

    free(rl);
}
