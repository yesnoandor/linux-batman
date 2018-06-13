/******************************************************************************
  File Name     : LocalMutex.h
  Version       : Initial Draft
  Author        : 
  Created       : 2010/08/30
  Last Modified :
  Description   : 实现互斥锁功能接口
  Function List :
  History       :
  1.Date        : 2010/08/30
    Author      : 
    Modification: Created file

******************************************************************************/

#ifndef _LOCAL_MUTEX_H_
#define _LOCAL_MUTEX_H_

#include "ll_headers.h"
#include "ll_evbuf.h"
#include <map>
#include <list>

class CLocalMutex
{
	public:
		CLocalMutex();
		virtual ~CLocalMutex();
	public:
		void Lock();
		void Unlock();
		int Trylock();
		
	private:
		pthread_mutex_t m_mutex;
};

class CLocalAutoLock
{
	public:
		CLocalAutoLock(CLocalMutex *pMutex);
		virtual ~CLocalAutoLock();

	private:
		CLocalMutex *m_pMutex;
};


/************************************************************************
等待项的实现, 用于网络超时等待
************************************************************************/
typedef struct
{
	sem_t		sem;		// 超时信号量
	u16		seq;		// 查找序号
	
	int		buflen;		// 用户数据长度
	u8		bufptr[2048];	// 用户数据
}ze_sem_item_t;

class CZeWaitList
{
public:
	CZeWaitList();
	~CZeWaitList();
	
	// 初始化及反初始化
	int InitSemItem(int num);
	int UninitSemItem();

	// 复位, 将所有等待对象放入空闲列表中
	int Reset();
	
	// 获取和释放信号量
	ze_sem_item_t *GetSemItem(u16 seq);
	int ReleaseSemItem(ze_sem_item_t *item);
	// 超时等待信号量, 返回0表示成功
	int WaitSemItem(ze_sem_item_t *item, int wait_ms);
	
	// 更新信号量
	int UpdateSemItem(u16 seq, const void *bufptr, int buflen);
	
protected:
	
	std::list<ze_sem_item_t *>		m_freeList;
	std::map<u16, ze_sem_item_t*>		m_useList;
	CLocalMutex				m_listLock;
};


/************************************************************************
单例模式的定义(线程安全/高效版本)
注:.cpp文件需实现构造函数
************************************************************************/
#define SINGLETON_DECLARE(class_name)	\
public:		\
	static class_name *Instance()	\
	{	\
		static class_name *g_instance = NULL;	\
		static CLocalMutex mutex;	\
		if ( g_instance == NULL )	\
		{	\
			mutex.Lock();	\
			if ( g_instance == NULL )	\
				g_instance = new class_name;	\
			mutex.Unlock();		\
		}	\
		return g_instance;	\
	}	\
private:	\
	class_name();	\
	~class_name(){}		\
	void operator=(const class_name&){}
			
//class_name(const class_name&){} 	\

#endif
