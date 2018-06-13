
/************************************************************************
�����߳����ʵ��
ʹ�ü�����Ĳ��Դ���
************************************************************************/

#ifndef __CPP_THREAD_H__
#define __CPP_THREAD_H__

#include "ll_headers.h"
#include "LocalMutex.h"
#include <list>

#define THREAD_DEBUG 0

#ifdef __unix__
typedef void* (*thread_proc)(void *args);
#else
typedef unsigned (__stdcall *thread_proc)(void *args);
#endif

#define CONNECT_STR(x, y)				x##y
#define THREAD_PROC_NAME(class_name, proc_name)		CONNECT_STR(thread_proc_##class_name, _##proc_name)

#ifdef __unix__
#define THREAD_PROC(class_name, proc_name)			\
static void *THREAD_PROC_NAME(class_name, proc_name)(void *args)	\
{								\
	class_name *pthis = (class_name *)args;			\
	pthis->proc_name();					\
	pthread_exit(NULL);					\
}
#else
#define THREAD_PROC(class_name, proc_name)			\
static unsigned __stdcall THREAD_PROC_NAME(class_name, proc_name)(void *args)	\
{								\
	class_name *pthis = (class_name *)args;			\
	pthis->proc_name();					\
	_endthreadex(0);					\
	return 0;						\
}
#endif

class CLocalThread
{
public:
	CLocalThread();
	virtual ~CLocalThread();

	int Start(const char *threadName);
	int Start(thread_proc threadProc, void *args, const char *threadName);
	int Stop();
	int GetStatus(); //0=stopped, 1=running, 2=paused
	void SetStatus(int status);
	int ExitSelf(); //�Զ������߳�, �������̺߳������е���

	virtual void PthreadProc(); //�̺߳�����

	static void PrintInfo();

	void SetUserData(int idx, void *data)	{ m_userData[idx] = data; }
	void *GetUserData(int idx) { return m_userData[idx]; }

private:
#ifdef __unix__
	pthread_t m_threadID;
	pid_t m_pid;
#else
	HANDLE m_threadID;
	DWORD m_pid;
#endif
	int m_runStatus; //�߳�����״̬, 0=ֹͣ, 1=����, 2=��ͣ
	char m_threadName[64]; //�߳�����, ���ڵ���

	void *m_userData[4];

	CLocalMutex m_startLock; //�����̻߳�����
	CLocalMutex m_stopLock; //ֹͣ�̻߳�����

	void Initialize();
	

#if THREAD_DEBUG	
	static std::list<CLocalThread *> m_threadList;
	static CLocalMutex m_threadListLock;
#endif

};


/************************************************************************
C���ӿ�
************************************************************************/
typedef struct __LocalThread	LocalThread;
typedef void (*CstyleProc)(LocalThread *lt, void *args);

LocalThread *LocalThread_New();
//int LocalThread_Init(LocalThread *lt);
int LocalThread_Delete(LocalThread *lt);
int LocalThread_Start(LocalThread *lt, CstyleProc procFunc, void *procArgs, const char *threadName);
int LocalThread_Stop(LocalThread *lt);
int LocalThread_GetStatus(LocalThread *lt);
void LocalThread_SetStatus(LocalThread *lt, int status);
int LocalThread_ExitSelf(LocalThread *lt); //�Զ������߳�, �������̺߳������е���

void LocalThread_SetUserData(LocalThread *lt, int idx, void *data);
void *LocalThread_GetUserData(LocalThread *lt, int idx);

void LocalThread_Print();


#endif //__CPP_THREAD_H__


