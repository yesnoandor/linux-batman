/****************************************Copyright (c)**************************************************                         
** File name:			call.c
** Descriptions:		通话信息保存
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-22
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef _CALL_H
#define _CALL_H

#ifdef __cplusplus
extern "C" {
#endif

#define		CALL_INFO_FILE		"call_info.xml"
#define		CALL_INFO_FILE_BAK	"call_info.bak"


//AT+CLCC查询的应答数据结构(部分)
typedef struct{
	int idx;							//整数类型，电话识别
	int dir;							//0:移动台发起(MO)的电话;1:移动台终止(MO)的电话
	int stat;							//电话的状态(0:正在进行;1:保持;2:拨号MO;3:振铃MO;4:来电MT;5:等待MT)
	int mode;							//0:语音;1:数据;2:传真;9:未知
	int mpty;							//0:电话不是多方会话中的成员;1:电话是多方会话中的成员
	char phone[PHONE_NUMBER_MAX_LEN];	//字符型电话号码
}__packed clcc_info_t;

//保存通话信息的数据结构
typedef struct{
	unsigned int start_time;			//通话开始时间(从1970到现在的秒数)
	unsigned int total_time;			//当月通话总时长(单位:秒)
	unsigned int status;				//通话状态(1:正在通话;2:不在通话)
}__packed call_info_t;

void set_clcc_info(clcc_info_t *info);
void get_clcc_info(clcc_info_t *info);

void set_call_info(void);
call_info_t * get_call_info(void);


#ifdef __cplusplus
}
#endif

#endif

