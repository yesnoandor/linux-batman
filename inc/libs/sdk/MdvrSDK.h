
/********************************************
  File Name    : MdvrSDK.h
  Author        : chenguangde
  Created      : 2015/01/01
  
sdk使用方法:
使用时包含此头文件和库后
1),实例化:实例化时给sdk取个名字(小于16字符,建议使用扩展应用程序名)
CmdvrSDK* mysdk = CmdvrSDK::Instance();

2),启动sdk
mysdk->StartSdk();

3),停止sdk
mysdk->StopSdk();

4),如需要接收处理主程序报警事件信息的需注册回调函数
mysdk->RegisterEventInfoCall( (sdk_pdealEventInfoCallBack)sdk_alarm_cb, NULL);

5),交互数据的接口主要有三个，看情况选择使用
Param_System_Info system_Info;//定义数据类型
mysdk->getData(&system_Info);//获取数据(有返回,0--成功,-1--失败,10s超时)
mysdk->setData(&system_Info);//设置数据(有返回,10s超时)
mysdk->sendData(&system_Info);//发送数据(无返回,在设置多个类型的参数时,前几个可用sendData,最后一个用setData来保存，可加快相应速度)

********************************************/

#ifndef  __MDVR_SDK_H__
#define __MDVR_SDK_H__

#include<stdio.h>

#include "sdk_data_def.h"

#ifdef WIN32
#ifdef BB_CPP_SDK_EXPORTS
#define BB_CPP_SDK_API __declspec(dllexport)
#else
#define BB_CPP_SDK_API __declspec(dllimport)
#endif
#else
#define BB_CPP_SDK_API
#endif

#ifdef __cplusplus
extern "C"
{
#endif

namespace mdvr_sdk
{
	
	typedef int (*sdk_pdealEventInfoCallBack)(int Type, int Subtype, int State, int State1, int State2, char *Data,int DataLen, void *args);
	typedef struct
	{
		sdk_pdealEventInfoCallBack cbFunc;
		void *cbArgs;
	}EVENT_CALLBACK_S;

	class BB_CPP_SDK_API CmdvrSDK
	{
	public :
		CmdvrSDK();
		~CmdvrSDK();
		static CmdvrSDK* Instance();
		
		//设置serverip,仅外部访问需设置，本地程序默认使用127.0.0.1:9961
		void SetServerIP(char *serverIp="127.0.0.1", int serverPort = 9961);

		//启动sdk
		int StartSdk(char* sdkname = NULL);
		
		//停止sdk
		int StopSdk();

		//向server广播当前sdk事件,server收到后会向所有sdk广播该事件并按需要做对应处理
		int sendEvent(SYSTEM_EVENT * event);

		//读取SDK版本号
		int GetSdkVer(char *SdkVer );

		//注册  事件处理回调接口
		int RegisterEventInfoCall(sdk_pdealEventInfoCallBack cbFunc, void *cbArgs);
		
		//注销事件处理回调接口
		int UnregisterEventInfoCall(sdk_pdealEventInfoCallBack cbFunc, void *cbArgs);
		
		//设置Server工作监视标志
		void SetLookFlag(int lookFlag);
		
		//sdk接口宏定义
		/*
			所有数据均使用如下接口
			int getDate(structName *pdata);//读取参数或信息，有返回结果
			int setDate(structName *pdata);//设置参数,有返回执行结果
			int sendDate(structName *pdata);//发送信息,无返回结果,不关心返回结果时,调用此接口执行较快,前两个需阻塞等server返回结果或超时.
			此部分接口代码放于sdk_data_def.h由代码生成器自动生成，不要手动编辑
		*/
		__mdvrsdk_h_define_

		int getDate(const int struct_id, void *pdata, int timeout = 10, int showXml = 0);
		int setDate(const int struct_id, void *pdata, int timeout = 10, int showXml = 0);
		int sendDate(const int struct_id, void *pdata, int timeout = 10, int showXml = 0);
		//结构体转换xml
		int structToXml(const char *structName, MESSAGE_TYPE_E msgType, void *pdata, char *xmlstr);
		//xml转换结构体
		int xmlToStruct(const char *structName, char *xmlstr, void *pdata);
		//执行xml,结果也返回到xml里
		int executeXml(int msgType, char *xmlStr, int timeout = 10, int showXml = 0);

	private :
		static CmdvrSDK *m_pInstance;
		void* m_myClient;
		char m_sdkName[16];
	};
}

#ifdef __cplusplus
}
#endif


#endif


