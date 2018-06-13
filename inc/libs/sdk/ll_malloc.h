


#ifndef __LL_MALLOC_H__
#define __LL_MALLOC_H__

#include "ll_headers.h"
/************************************************************************
类似malloc/free, 用于存放变化大小的动态内存块
内存布局如下, item会组成一个链表
|item + mem|item + mem|...|p_end
************************************************************************/

struct mem_item_t
{
	struct mem_item_t		*prev;	// 指向前一个mem_item_t
	struct mem_item_t		*next;	// 指向后一个mem_item_t
	int				used;	// 标记是否被使用
};

struct mem_t
{
	u8			*bufptr;	// 缓存大小
	int			buflen;		// 缓存尺寸
	struct mem_item_t	*p_end;		// 指向缓存结束
	struct mem_item_t	*p_free;	// 指向可用缓存
};

// 初始化
int mem_init(struct mem_t *pmem, u8 *bufptr, int buflen);
// 打印, 用于调试
void mem_print(struct mem_t *pmem);
// 申请内存
void *mem_malloc(struct mem_t *pmem, int sz);
// 释放内存
void mem_free(struct mem_t *pmem, void *ptr);


/************************************************************************
固定尺寸的动态内存
内存布局: 在本内存为空闲时存放指向下一块空闲内存的指针
************************************************************************/
struct fix_mem_t
{
	u8		*bufptr;	// 缓存指针
	int		blksize;	// 每个内存块的固定尺寸
	int		nblks;		// 内存块的个数

	void		*p_free;	// 空闲内存块指针
	int		nfreeblks;	// 空闲内存数量
};
// 固定缓存池初始化, blksize>=sizeof(void*), nblks>0
int fix_mem_init(struct fix_mem_t *fix_mem, u8 *bufptr, int blksize, int nblks);
// 打印函数, 用于调试
void fix_mem_print(struct fix_mem_t *fix_mem);
// 申请一块内存
void *fix_mem_get(struct fix_mem_t *fix_mem);
// 释放内存
void fix_mem_put(struct fix_mem_t *fix_mem, void *ptr);


#endif // __LL_MALLOC_H__




