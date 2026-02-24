这是一个定长内存池简易版 适合c++新手学习
主要功能是减少new和malloc的调用
主要包括四个变量：内存块的数目，大小 内存池起始地址 空闲内存块管理链表
主要包括三个函数：allocate分配函数 deallocate返回函数 initFreeList切割函数
