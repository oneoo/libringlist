//
//  test.c
//
//
//  Created by oneoo on 13-9-11.
//
//

#include <stdio.h>

#include "ringlist.h"


int main(int argc, char *argv[])
{
    rl_t *rl = rl_create(10);

    rl_node_t *rn1 = rl_add(rl, 0);
    rn1->ptr = (void*)101;
    rl_node_t *rn2 = rl_add(rl, 0);
    rn2->ptr = (void*)102;
    rl_node_t *rn3 = rl_add(rl, 0);
    rn3->ptr = (void*)103;

    rl_node_t *n = NULL;

    n = rl_each(rl, 0);
    while(n) {
        printf("%ld\n", (long)n->ptr);
        n = n->next;
    }

    void *v = rl_shift(rl, 0);
    if(v)
        printf("pop %ld\n", (long)v);

    n = rl_each(rl, 0);
    while(n) {
        printf("%ld\n", (long)n->ptr);
        n = n->next;
    }

    return 0;
}
