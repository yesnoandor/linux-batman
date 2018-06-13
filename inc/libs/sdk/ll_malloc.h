


#ifndef __LL_MALLOC_H__
#define __LL_MALLOC_H__

#include "ll_headers.h"
/************************************************************************
����malloc/free, ���ڴ�ű仯��С�Ķ�̬�ڴ��
�ڴ沼������, item�����һ������
|item + mem|item + mem|...|p_end
************************************************************************/

struct mem_item_t
{
	struct mem_item_t		*prev;	// ָ��ǰһ��mem_item_t
	struct mem_item_t		*next;	// ָ���һ��mem_item_t
	int				used;	// ����Ƿ�ʹ��
};

struct mem_t
{
	u8			*bufptr;	// �����С
	int			buflen;		// ����ߴ�
	struct mem_item_t	*p_end;		// ָ�򻺴����
	struct mem_item_t	*p_free;	// ָ����û���
};

// ��ʼ��
int mem_init(struct mem_t *pmem, u8 *bufptr, int buflen);
// ��ӡ, ���ڵ���
void mem_print(struct mem_t *pmem);
// �����ڴ�
void *mem_malloc(struct mem_t *pmem, int sz);
// �ͷ��ڴ�
void mem_free(struct mem_t *pmem, void *ptr);


/************************************************************************
�̶��ߴ�Ķ�̬�ڴ�
�ڴ沼��: �ڱ��ڴ�Ϊ����ʱ���ָ����һ������ڴ��ָ��
************************************************************************/
struct fix_mem_t
{
	u8		*bufptr;	// ����ָ��
	int		blksize;	// ÿ���ڴ��Ĺ̶��ߴ�
	int		nblks;		// �ڴ��ĸ���

	void		*p_free;	// �����ڴ��ָ��
	int		nfreeblks;	// �����ڴ�����
};
// �̶�����س�ʼ��, blksize>=sizeof(void*), nblks>0
int fix_mem_init(struct fix_mem_t *fix_mem, u8 *bufptr, int blksize, int nblks);
// ��ӡ����, ���ڵ���
void fix_mem_print(struct fix_mem_t *fix_mem);
// ����һ���ڴ�
void *fix_mem_get(struct fix_mem_t *fix_mem);
// �ͷ��ڴ�
void fix_mem_put(struct fix_mem_t *fix_mem, void *ptr);


#endif // __LL_MALLOC_H__




