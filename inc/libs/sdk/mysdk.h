#ifndef __MY_SDK_H__
#define __MY_SDK_H__

#include "MdvrSDK.h"
#include "LocalThread.h"
// 事件通知函数
typedef int (*sdkEventCallback)(int Type, int Subtype, int State, int State1, int State2, char *Data,int DataLen, void *args);

//简写宏定义--------------------------------------------------------------
#define WRITE_DBG_LOG(str...) 		CMySdk::Instance()->WriteLog(str)
#define WRITE_DBG_LOG_BUF(pbuf,len)	CMySdk::Instance()->WriteLogBuf(pbuf,len)
//------------------------------------------------------------------------------

class CMySdk: public CLocalThread
{
private:
	CMySdk();
	~CMySdk(){}	
public:	
	static CMySdk *Instance()
	{
		static CMySdk *g_instance = NULL;
		static CLocalMutex mutex;
		if ( g_instance == NULL )
		{
			mutex.Lock();
			if ( g_instance == NULL )
				g_instance = new CMySdk;
			mutex.Unlock();
		}
		return g_instance;
	}
			
protected:
	virtual void PthreadProc(); // 线程函数体
public:
	int Start(char *sdkname);
	int Stop();
	
	int RegisterEventCb(sdkEventCallback cbFunc, void *cbArgs);//注册报警回掉
	int InputAlarm(int type, int subtype, int State, int State1, int State2, char *alarmData, int alarmDataLen);
	static int OnAlarmCallback(int type, int subtype, int State, int State1, int State2, char *alarmData, int alarmDataLen, void *args);
	int OnAlarmCallback2(int type, int subtype, int State, int State1, int State2, char *alarmData, int alarmDataLen, void *args);
	
	int setState(int type, int subtype, int State, int State1=0, int State2=0, int init=0);
	int sendAlarm(int type, int subtype, int State, int State1=0, int State2=0, char *alarmData=NULL, int alarmDataLen=0);

	//写调试日志
	int WriteLog(const char * fmt, ...);
	int WriteLogBuf(const void *bufptr, int buflen);

	//透传部标视频协议
	int transBbAvProtocol(int id, char *data, int dataLen=0);
	
	int ctrlLED(MDVR_POWER_TYPE_E type, unsigned char state);//LED控制 
	
	void Video_Play_Ctrl(s32 ctrl_mode,s8* path,s32 offsettime,s32 layoutmode,s32 showpage);
    int Video_Get_PlayBack_Status(s32 ctrl_mode,s8* path,s32 offsettime,s32 layoutmode,s32 showpage);
	void Video_Layout_Mode_Ctrl(s32 ctrl_mode,s32 showpage);
	
	s32 Get_Video_Layout_Mode(void);
	
	Control_VideoSearch Control_Video_Search(s32 rec_disk,s32 message_type,s8* starttime,s8* endtime,s32 curpage);
	//void Control_Video_Export(s32 rec_disk,s8* starttime,s8* endtime,s8* export_path);
	void Control_CreateMultimediaFile(s32 filetype,s8 pic_size,s8* filename,s32 channelno,s32 timelong);
	void Front_Osd_Set();
	void Record_Osd_Set(char gpsInfo[32]);		//chrade add shenzheng 170716
	Param_CenterNetWork Get_CentterNetWork_Param(s32 network_type);
	
	void Center_NetWork_Set(s32 networkid,s8 * centerip,s32 contrlport);

	int Control_Video_File_OutPut(s8* rec_path,s8* export_path,s8* starttime,s8* endtime);

	void Phone_Contorl(s32 type,s32 volume);

	Param_Terminal_Info Get_Param_Terminal_Info(void);
	void Set_Param_Terminal_Info(Param_Terminal_Info info);
	Param_LocalNetWork Get_Param_LocalNetWork_Info(void);

	void Set_Param_LocalNetWork_Info(Param_LocalNetWork info);
	Param_RecordSet Get_Param_RecordSet_Info(void);
	void Set_Param_RecordSet_Info(Param_RecordSet info);
	Param_RecordCodeSet Get_Param_RecordCode_Info(s32 StreamType,s8 chn);
	void Set_Param_RecordCode_Info(Param_RecordCodeSet info);
	void Set_Param_CameraType(s32 CameraType);
	void Send_Av_Ctl_Protocol(char* data,int len);

	void Get_video_info(Info_video * info);
    int Set_Remote_Key(int type, int subtype, int State );
	mdvr_sdk::CmdvrSDK *getsdk(){return m_sdk;}

private:
	mdvr_sdk::CmdvrSDK *m_sdk;
	char m_sdkName[16];
	
};

#endif

