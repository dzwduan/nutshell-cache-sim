# cache+mmu功能建模

参考nutshell设计，之后移植到NEMU，和Nutshell进行difftest

## TLB设计

修改NEMU原生设计，支持Sv39，参考[nutshell tlb](https://oscpu.github.io/NutShell-doc/%E5%8A%9F%E8%83%BD%E9%83%A8%E4%BB%B6/tlb.html)

配置 : 4-way ITLB + 64项 4-way DTLB

行为建模：

1. 检查读写行为和权限位

   1. ITLB权限不合法，需要等待cache清空后将例外信息传递.
   2. DTLB权限不合法，实现中会将例外信息缓存一拍发出.

2. TLB命中

   1. 得到实地址结果

3. TLB不命中

   1. 通过状态机方式访问页表，填充页表，阻塞后续指令进入TLB，三级页表需要三次访存

4. sfence.vma修改satp，需要将TLB内容设置无效

   



## cache设计

参考 [nutshell cache](https://oscpu.github.io/NutShell-doc/%E5%8A%9F%E8%83%BD%E9%83%A8%E4%BB%B6/cache.html)

配置：32KB Icache + 32KB DCache + 128KB L2 Cache，4路组相联，随即替换+写回策略， cache line 64B，tag 和 index均为实地址

行为建模：

1. 根据访存地址，获得tag，index, word index, byte offset
2. 读索引对应的cache组，检查hit/miss，如果miss则进行替换（原版本采用的随即替换，考虑改进为pLRU）
   1. hit，load直接返回数据，store根据写掩码选择的写数据和命中的原始数据进行拼接，并修改dirty=1
   2. miss, 读取该 miss 地址的一个Cacheline的数据填入victim Cache line，如果该victim cache line的数据被修改过，需要在读取数据之前将该 Cache 行的数据写入下一级存储，重填采用关键字优先
3. L2预取器，看文档是啥也不做


## TODO

修改 Nutshell cache 替换算法为 plru，随机的不方便difftest
