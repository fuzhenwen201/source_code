#ifndef _TCP_H_
#define _TCP_H_

#include "log.h"
#include "const.h"
#include <cstring>
#include <iostream>
#include <string.h>
#include <malloc.h>
#include <assert.h>

typedef struct bpool_t 
{
	int maxcount;                           //最大块数支持
	int nblock;                             //使用中的块数
	int nfreeblock;                         //空闲块数
	int nallblock;                          //已申请块数
	int nindex;                             //已使用到的梯级号
	int bsize;                              //每块字节数

	void **freeits[MAX_ARRAY_INDEX];        //空闲指针阶梯数组
	void *allits[MAX_ARRAY_INDEX];          //块所在的阶梯数组
}bpool, *pbpool;

class CObjectPool
{
	public:
		bpool              m_bp;                                 //对象池信息
		int                m_max_array_index;                    //最大阶数
		int                m_min_array_index;                    //第一阶梯数组的块
		int                m_maxcount;                           //最大长度
		int                m_size;                               //每块长度
		int                m_nindex;                             //当前阶梯号
		int                m_nallblock;                          //已申请块数
		int                m_nfreeblock;                         //空闲块数

		void**             m_freeits[MAX_ARRAY_INDEX];           //空闲指针阶梯数组
		void*              m_allits[MAX_ARRAY_INDEX];            //块所在的阶梯数组
		pthread_mutex_t    m_mutex;                              //互斥量

		CObjectPool();
		~CObjectPool();
		CObjectPool(int maxindex,int minindex);

		int nlz(unsigned x);
		void bpool_clear ();
		void *bpool_alloc_block();
		void bpool_free_block (void *p);
		void bpool_init (int maxcount, int bsize);
		void *bpool_locat_block (void *head[], int offset, int isize);
};
#endif
