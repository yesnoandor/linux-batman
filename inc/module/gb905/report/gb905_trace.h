#ifndef _GB905_TRACE_H
#define	_GB905_TRACE_H

#ifdef __cplusplus
extern "C" {
#endif

enum{
	LOCATION_TRACE_TIME_POLICY = 0x00,
	LOCATION_TRACE_DISTANCE_POLICY = 0x11,
	LOCATION_TRACE_TIME_DISTANCE_POLICY = 0x01,
	LOCATION_TRACE_DISTANCE_TIME_POLICY = 0x10,
	LOCATION_TRACE_STOP_POLICY = 0xFF,
};

typedef struct 
{	
	// ���ٲ���
	// 		= 0x00,ʱ����������ʱ��
	//		= 0x11,����������������
	//		= 0x01,ʱ��������������
	//		= 0x10,������������ʱ��
	//		= 0xFF,ֹͣ��ǰ����
	unsigned char policy;
	unsigned short interval;		// ʱ��(s) /������(m)
	unsigned int threhold;			// ����ʱ��(s) /���(m)
}__packed location_trace_params_t;

// ����
typedef struct 
{
	// =True,λ�ø�������
	// =False,λ�ø���ֹͣ
	bool location_trace_flag;

	// λ�ø��ٵĲ���
	location_trace_params_t location_trace_params;
}location_trace_t;

