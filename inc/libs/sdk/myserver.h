
#ifndef __SERVER_INTERFACE_H__ 
#define	__SERVER_INTERFACE_H__ 

#include "MdvrServerInterface.h"
#include "LocalThread.h"

using namespace mdvr_sdk; 

class CMyServer: public CMdvrServerInterface, public CLocalThread
{
private :
	CMyServer(){};
	~CMyServer(){};
	
public:	
	static CMyServer *Instance()
	{
		static CMyServer *g_instance = NULL;
		static CLocalMutex mutex;
		if ( g_instance == NULL )
		{
			mutex.Lock();
			if ( g_instance == NULL )
				g_instance = new CMyServer;
			mutex.Unlock();
		}
		return g_instance;
	}
public:
	int Start(char* server_Name);
	int Stop();
	
public:
	//д��־
	virtual void PutOutLog(const char * fmt, ...);

#if 1 //server�ӿ�ʵ��
	//���Ź�
	virtual int deal_Sdk_watchDog(Sdk_watchDog * pdata);

//-------��һ�׶�ʵ�ֽӿ�-----------------------------------------------------------------

	//b.ϵͳ����
	virtual int deal_Control_system(Control_system * pdata);
	
//-------�ڶ��׶�ʵ�ֽӿ�-----------------------------------------------------------------

	//b.�绰����
	virtual int deal_Control_opTel(Control_opTel * pdata);

	//������
	virtual int deal_Control_sendSMS(Control_sendSMS * pdata);

	//��Ϣ����
	virtual int deal_Control_sendInfo(Control_sendInfo * pdata);

	//������̨�Ĳ���
	virtual int deal_Control_PTZProc(Control_PTZProc * pdata);

	//��������͸��
	virtual int deal_Control_data_transit(Control_data_transit * pdata);

	//�¼���ʽ
	virtual int deal_SYSTEM_EVENT(SYSTEM_EVENT * pdata);

#endif
};

#endif

