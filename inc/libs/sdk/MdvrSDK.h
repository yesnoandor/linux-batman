
/********************************************
  File Name    : MdvrSDK.h
  Author        : chenguangde
  Created      : 2015/01/01
  
sdkʹ�÷���:
ʹ��ʱ������ͷ�ļ��Ϳ��
1),ʵ����:ʵ����ʱ��sdkȡ������(С��16�ַ�,����ʹ����չӦ�ó�����)
CmdvrSDK* mysdk = CmdvrSDK::Instance();

2),����sdk
mysdk->StartSdk();

3),ֹͣsdk
mysdk->StopSdk();

4),����Ҫ���մ��������򱨾��¼���Ϣ����ע��ص�����
mysdk->RegisterEventInfoCall( (sdk_pdealEventInfoCallBack)sdk_alarm_cb, NULL);

5),�������ݵĽӿ���Ҫ�������������ѡ��ʹ��
Param_System_Info system_Info;//������������
mysdk->getData(&system_Info);//��ȡ����(�з���,0--�ɹ�,-1--ʧ��,10s��ʱ)
mysdk->setData(&system_Info);//��������(�з���,10s��ʱ)
mysdk->sendData(&system_Info);//��������(�޷���,�����ö�����͵Ĳ���ʱ,ǰ��������sendData,���һ����setData�����棬�ɼӿ���Ӧ�ٶ�)

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
		
		//����serverip,���ⲿ���������ã����س���Ĭ��ʹ��127.0.0.1:9961
		void SetServerIP(char *serverIp="127.0.0.1", int serverPort = 9961);

		//����sdk
		int StartSdk(char* sdkname = NULL);
		
		//ֹͣsdk
		int StopSdk();

		//��server�㲥��ǰsdk�¼�,server�յ����������sdk�㲥���¼�������Ҫ����Ӧ����
		int sendEvent(SYSTEM_EVENT * event);

		//��ȡSDK�汾��
		int GetSdkVer(char *SdkVer );

		//ע��  �¼�����ص��ӿ�
		int RegisterEventInfoCall(sdk_pdealEventInfoCallBack cbFunc, void *cbArgs);
		
		//ע���¼�����ص��ӿ�
		int UnregisterEventInfoCall(sdk_pdealEventInfoCallBack cbFunc, void *cbArgs);
		
		//����Server�������ӱ�־
		void SetLookFlag(int lookFlag);
		
		//sdk�ӿں궨��
		/*
			�������ݾ�ʹ�����½ӿ�
			int getDate(structName *pdata);//��ȡ��������Ϣ���з��ؽ��
			int setDate(structName *pdata);//���ò���,�з���ִ�н��
			int sendDate(structName *pdata);//������Ϣ,�޷��ؽ��,�����ķ��ؽ��ʱ,���ô˽ӿ�ִ�нϿ�,ǰ������������server���ؽ����ʱ.
			�˲��ֽӿڴ������sdk_data_def.h�ɴ����������Զ����ɣ���Ҫ�ֶ��༭
		*/
		__mdvrsdk_h_define_

		int getDate(const int struct_id, void *pdata, int timeout = 10, int showXml = 0);
		int setDate(const int struct_id, void *pdata, int timeout = 10, int showXml = 0);
		int sendDate(const int struct_id, void *pdata, int timeout = 10, int showXml = 0);
		//�ṹ��ת��xml
		int structToXml(const char *structName, MESSAGE_TYPE_E msgType, void *pdata, char *xmlstr);
		//xmlת���ṹ��
		int xmlToStruct(const char *structName, char *xmlstr, void *pdata);
		//ִ��xml,���Ҳ���ص�xml��
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


