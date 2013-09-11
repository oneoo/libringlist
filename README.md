RingList Library
===

基础用例
---

	rl_t *rl = rl_create(1); //创建1个 list 的 ring 集合
    
    rl_node_t *rn1 = rl_add(rl, 0);
    // rl_add(rl_t *, uint32_t k)
    // 参数 k 指在第几个 list 添加 node（k%listcount）
    // rl_add 以追加的形式加入 node
    
    rn1->ptr = (void*)100; //把数据存储到 list 的 node 上
    
    rl_node_t *rn2 = rl_add(rl, 0);
    rn2->ptr = (void*)102;
    
    rl_node_t *rn3 = rl_add(rl, 0);
    
    // 遍历某个 list
    rl_node_t *n = NULL;
    
    n = rl_each(rl, 0);
    while(n){
    	// n->ptr;
    	n = n->next;
    }
    
    long k = 0;
    // 从 list 头部弹出1个 node，注意 node 弹出即被释放，只返回 ptr 数据
    k = rl_pop(rl, 0);
    
    // 从 list 尾部弹出1个 node
    k = rl_shift(rl, 0);
    
    // 获取整个 ring 上的 node 数量
    printf("%ld\n", rl_counts(rl));

使用场景
===
超时检测队列
---

	rl_t *rl = rl_create(60); // 创建60个 list 的 ring 集合
    
    // timeout = now+38;
    rl_node_t *rn = rl_add(rl, now+38);
    rn->ptr = now+38;
    // 考虑到多种超时时间，因 ring 大小有限，会出现冲突，
    // 而导致 list 中存在并未到期的 node
    // 比如 now+98 到期的记录会跟 now+38 放在同一个 list 中
    // 所以我们要在 ptr 中写入超时时间以做判断
    
    /// 以下为每秒定时执行的程序片段
    { 
    	rl_node_t *n = rl_each(rl, now);
        while(n){
        	if(now >= n->ptr)
	        	rl_del(rl, n);
            // 注意 rl_del 只是在 list 中删除
            // 并未释放 ptr 数据
        }
    }

Word Counter
---

    int cmp(rl_node_t *n, void *d)
    {
        if(n->ptr == d)return 1;
        return 0;
    }
    
    {
        rl_t *rl = rl_create(4096); // 创建超大的 ring 集合
        
        char *key = "key1";
        int key_len = 4;
        rl_node_t *rn = rl_add(rl, fnv1_32(key, key_len));
        rn->ptr = key;
        
        rl_node_t *n = rl_find(rl, fnv1_32(key, key_len), cmp, key);
        if(n){
        	printf("found\n");
        }
    }
