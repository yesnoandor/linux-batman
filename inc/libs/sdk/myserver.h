
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
	//写日志
	virtual void PutOutLog(const char * fmt, ...);

#if 1 //server接口实现
	//看门狗
	virtual int deal_Sdk_watchDog(Sdk_watchDog * pdata);

//-------第一阶段实现接口-----------------------------------------------------------------

	//b.系统操作
	virtual int deal_Control_system(Control_system * pdata);
	
//-------第二阶段实现接口-----------------------------------------------------------------

	//b.电话控制
	virtual int deal_Control_opTel(Control_opTel * pdata);

	//发短信
	virtual int deal_Control_sendSMS(Control_sendSMS * pdata);

	//信息发送
	virtual int deal_Control_sendInfo(Control_sendInfo * pdata);

	//控制云台的操作
	virtual int deal_Control_PTZProc(Control_PTZProc * pdata);

	//数据下行透传
	virtual int deal_Control_data_transit(Control_data_transit * pdata);

	//事件格式
	virtual int deal_SYSTEM_EVENT(SYSTEM_EVENT * pdata);

#endif
};

#endif

