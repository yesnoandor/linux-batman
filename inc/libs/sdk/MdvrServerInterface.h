
#ifndef _MDVR_SERVER_INTERFACE_H_
#define _MDVR_SERVER_INTERFACE_H_

#include<stdio.h>
#include<stdarg.h>

#include "sdk_data_def.h"

namespace mdvr_sdk
{
	class	CMdvrServerInterface
	{
	public:
		CMdvrServerInterface();
		virtual ~CMdvrServerInterface();
		
		//����serverPort,�����˿��г�ͻʱ�����ã�Ĭ��ʹ��:9961
		void SetServerPort(int serverPort = 9961);
		
		//����server
		//serverName-server���ƣ��ⷢ����ʱ�����ڱ���Դ�����ϸ����ƣ��Ա�ʶ�𱨾�Դ��
		//maxDealNum-����ҵ�����߳�����ǰ(maxDealNum-1)���ⲿAppҵ�������ɶ����̴߳������������и���(maxDealNum)���̴߳������֧��16��ҵ���߳�
		int StartServer(char* serverName = NULL, int maxDealNum = 3);
		
		//ֹͣserver
		int StopServer();
		
		//�ⷢ�¼���Ϣ
		//Type �¼����ͣ���:MDVR_ALARM_TYPE 
		int sendEventInfo(int Type, int Subtype, int State, int State1 = 0, int State2 = 0, char *Data = NULL, int DataLen = 0);

		//����xmlָ��
		int dealXmlCommand(char * commandXml, unsigned int commandLen, char *resultXml,unsigned int *resultLen);

		//����Server�������ӱ�־,�����ڲ��շ�xmlָ��ʱ�ɴ򿪼���
		void SetLookFlag(int lookFlag);

		//д��־
		virtual void PutOutLog(const char * LogStr){return ;}

		//server�ӿں궨��
		/*   �˺궨���оٳ�����server��Ҫʵ�ֵĽӿ�
			�˲��ֽӿڴ������sdk_data_def.h�ɴ����������Զ����ɣ���Ҫ�ֶ��༭ */
		__MdvrServerInterface_h_define_
		
		private :
			static char m_serverName[16];
	};
}
#endif




