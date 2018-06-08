#include "../../include/mempool.h"
#include "../../include/const.h"

CObjectPool::CObjectPool()
{
	m_max_array_index = MAX_ARRAY_INDEX;
	m_min_array_index = ARRAY_ALLOC_BASE;
	pthread_mutex_init(&m_mutex, NULL);
}

CObjectPool::CObjectPool(int maxindex,int minindex)
{
	m_max_array_index = maxindex;
	m_min_array_index = minindex;
	pthread_mutex_init(&m_mutex, NULL);
}

CObjectPool::~CObjectPool()
{
	bpool_clear();
	pthread_mutex_destroy(&m_mutex);
}

void CObjectPool::bpool_init (int maxcount, int bsize)
{
	memset (&m_bp, 0, sizeof(bpool));
	m_bp.maxcount = maxcount;
	m_bp.bsize = bsize;
}

void CObjectPool::bpool_clear ()
{
	int i;
	for (i=0; i<m_bp.nindex; i++) {
		if (m_bp.freeits[i]) free ((void*)m_bp.freeits[i]);    
	}
}

int CObjectPool::nlz(unsigned x)
{
	int n = 1;

	if (x == 0) return(32);
	if ((x >> 16) == 0) {n = n +16; x = x <<16;}
	if ((x >> 24) == 0) {n = n + 8; x = x << 8;}
	if ((x >> 28) == 0) {n = n + 4; x = x << 4;}
	if ((x >> 30) == 0) {n = n + 2; x = x << 2;}
	n = n - (x >> 31);
	return n;
}

void*  CObjectPool::bpool_locat_block (void *head[], int offset, int isize)
{
	//第几个索引数组,先除以基数，再取最高为1的位的位置
	//前加1是保证为0或全1时进位，后减1是为了从0开始取索引
	//对于freeits和allits阶梯数组，第i个数组有base*(i^2)个项

	//则对于第0ffset个，其梯级号是x
	int x = 32 - nlz((offset>>m_min_array_index) + 1) - 1;
	//数组内偏移, 减去前x个之和
	int y = offset - (((1<<x)-1) << m_min_array_index);

	char *arr = (char* )head[x];
	assert (x < m_max_array_index);
	assert (y < ((1<<x)<<m_min_array_index));
	return (void*)(arr+y*isize);
}

void* CObjectPool::bpool_alloc_block()
{
	int len = 0;
	int ncount = 0;
	void *node, **pret=NULL;
	char *mem = NULL;

	if (m_bp.nfreeblock > 0)
		goto __found;

	ncount = (1<<m_bp.nindex)<<m_min_array_index;
	if (m_bp.maxcount!=SIZE_AUTO_EXPAND && ncount > m_bp.maxcount - m_bp.nallblock)
		ncount = m_bp.maxcount-m_bp.nallblock;

	if (ncount<=0 || m_bp.nindex == m_max_array_index)
		return NULL;//到限制了

	//新增加数组
	mem = (char*)malloc ((sizeof(void*) + m_bp.bsize) * ncount);

	if (!mem)
		return NULL;

	len = sizeof(void*)*ncount;
	m_bp.freeits[m_bp.nindex] = (void**)mem;
	m_bp.allits[m_bp.nindex] = (void*)(mem+len);
	//只需要初始化freeits
	memset (m_bp.freeits[m_bp.nindex], 0, len);
	m_bp.nindex++;

	//freeits里有itemcount个未确定其真实空闲void内存的位置.为NULL
	//allits里有itemcount个未作任何挂接的位置
	//除了第一个要返回使用，其它空闲要加到free里
	//每次申请时再动态挂接上
	m_bp.nallblock += ncount;
	m_bp.nfreeblock += ncount;

__found:

	//目标是取me->nfreeblock--的位置
	m_bp.nfreeblock--;
	pret = (void**)bpool_locat_block ((void**)m_bp.freeits, m_bp.nfreeblock, sizeof(void*));

	if (!*pret)
		//原先未放有void内存，这个时候再挂接上去
		//挂谁上去？此时满足count+ not_mount =nallblock
		//例如第3次时，cnt=3，应挂第3个
		//如果归还过，则直到用完以前归还的item，才会进本分支
		*pret = (void*)bpool_locat_block ((void**)m_bp.allits, m_bp.nblock, m_bp.bsize);

	m_bp.nblock++;
	//拿走后要设置这个位置为空，否则若再扩展就会污染
	node = *pret;
	*pret = NULL;
	return node;
}

void CObjectPool::bpool_free_block (void *p)
{
	//直接将此void*挂接到freeits的最后可用结点上
	void **pret = (void**)bpool_locat_block ((void**)m_bp.freeits, m_bp.nfreeblock, sizeof(void*));
	assert (pret && m_bp.nfreeblock < m_bp.nallblock);

	*pret = p;
	m_bp.nblock--;
	m_bp.nfreeblock++;
}
