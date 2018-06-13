/******************************************************************************
  File Name     : LocalMutex.h
  Version       : Initial Draft
  Author        : 
  Created       : 2010/08/30
  Last Modified :
  Description   : ʵ�ֻ��������ܽӿ�
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
�ȴ����ʵ��, �������糬ʱ�ȴ�
************************************************************************/
typedef struct
{
	sem_t		sem;		// ��ʱ�ź���
	u16		seq;		// �������
	
	int		buflen;		// �û����ݳ���
	u8		bufptr[2048];	// �û�����
}ze_sem_item_t;

class CZeWaitList
{
public:
	CZeWaitList();
	~CZeWaitList();
	
	// ��ʼ��������ʼ��
	int InitSemItem(int num);
	int UninitSemItem();

	// ��λ, �����еȴ������������б���
	int Reset();
	
	// ��ȡ���ͷ��ź���
	ze_sem_item_t *GetSemItem(u16 seq);
	int ReleaseSemItem(ze_sem_item_t *item);
	// ��ʱ�ȴ��ź���, ����0��ʾ�ɹ�
	int WaitSemItem(ze_sem_item_t *item, int wait_ms);
	
	// �����ź���
	int UpdateSemItem(u16 seq, const void *bufptr, int buflen);
	
protected:
	
	std::list<ze_sem_item_t *>		m_freeList;
	std::map<u16, ze_sem_item_t*>		m_useList;
	CLocalMutex				m_listLock;
};


/************************************************************************
����ģʽ�Ķ���(�̰߳�ȫ/��Ч�汾)
ע:.cpp�ļ���ʵ�ֹ��캯��
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
