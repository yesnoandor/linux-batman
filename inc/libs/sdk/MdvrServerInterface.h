
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
		
		//设置serverPort,仅当端口有冲突时需设置，默认使用:9961
		void SetServerPort(int serverPort = 9961);
		
		//启动server
		//serverName-server名称，外发报警时，会在报警源里填上该名称，以便识别报警源。
		//maxDealNum-启动业务处理线程数，前(maxDealNum-1)个外部App业务请求由独立线程处理，超过的则集中给第(maxDealNum)个线程处理，最大支持16个业务线程
		int StartServer(char* serverName = NULL, int maxDealNum = 3);
		
		//停止server
		int StopServer();
		
		//外发事件信息
		//Type 事件类型，见:MDVR_ALARM_TYPE 
		int sendEventInfo(int Type, int Subtype, int State, int State1 = 0, int State2 = 0, char *Data = NULL, int DataLen = 0);

		//处理xml指令
		int dealXmlCommand(char * commandXml, unsigned int commandLen, char *resultXml,unsigned int *resultLen);

		//设置Server工作监视标志,调试内部收发xml指令时可打开监视
		void SetLookFlag(int lookFlag);

		//写日志
		virtual void PutOutLog(const char * LogStr){return ;}

		//server接口宏定义
		/*   此宏定义列举出所有server需要实现的接口
			此部分接口代码放于sdk_data_def.h由代码生成器自动生成，不要手动编辑 */
		__MdvrServerInterface_h_define_
		
		private :
			static char m_serverName[16];
	};
}
#endif




