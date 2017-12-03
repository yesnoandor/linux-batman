/****************************************Copyright (c)**************************************************                         
** File name:			gb905_trace.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-13
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef _GB905_TRACE_H
#define	_GB905_TRACE_H

#ifdef __cplusplus
extern "C" {
#endif

// λ�ø��ٲ���
enum{
	TIME_POLICY = 0x00,				// = 0x00,ʱ����������ʱ��
	TIME_DISTANCE_POLICY = 0x01,	// = 0x01,ʱ��������������	
	DISTANCE_TIME_POLICY = 0x10,	// = 0x10,������������ʱ��
	DISTANCE_POLICY = 0x11,			// = 0x11,����������������
	TRACE_STOP_POLICY = 0xFF,		// = 0xFF,ֹͣ����
};

// λ�ø��ٵ���Ϣ����Ϣ���ݸ�ʽ
typedef struct 
{
	unsigned char policy;			// ���ٲ���
	unsigned short interval;		// ʱ��(s) /������(m)
	unsigned int threhold;			// ����ʱ��(s) /����(m)
}__packed trace_body_t;



void gb905_trace_send(void);
unsigned char  gb905_trace_treat(unsigned char *buf,int len);

#ifdef __cplusplus
}
#endif
	
#endif

