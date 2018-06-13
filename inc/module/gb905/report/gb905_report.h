/****************************************Copyright (c)**************************************************                         
** File name:			gb905_report.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-04
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef _GB905_REPORT_H
#define _GB905_REPORT_H

#ifdef __cplusplus
extern "C" {
#endif

// ������־����
typedef union 
{
	unsigned whole;
	
	struct
	{
		unsigned emergency:1;				// ��������
		unsigned early_warning:1;			// Ԥ��
		unsigned gps_fault:1;				// GPS ģ�����
		unsigned gps_antenna_cut_fault:1;	// GPS ����δ�ӻ򱻼���
		unsigned gps_antenna_short_fault:1;	// GPS ���߶�·
		unsigned under_voltage:1;			// ��ԴǷѹ
		unsigned power_down:1;				// ��Դ����
		unsigned lcd_device_fault:1;		// ��ʾ�ն˹���
		unsigned tts_fault:1;				// tts ģ�����
		unsigned camera_fault:1;			// ����ͷ����
		unsigned meter_fault:1;				// �Ƽ�������
		unsigned eval_fault:1;				// ��������������
		unsigned led_adver_fault:1;			// LED ���������
		unsigned lcd_module_fault:1;		// LCD ��ʾ������
		unsigned tsm_fault:1;				// ��ȫģ�����
		unsigned led_light_fault:1;			// LED ���ƹ���
		unsigned overspeed:1;				// ���ٱ���
		unsigned driving_timeout:1;			// ������ʻ��ʱ
		unsigned day_driving_timeout:1;		// �����ۼƼ�ʻ��ʱ
		unsigned parking_timeout:1;			// ��ʱͣ��
		unsigned out_of_area:1;				// ��������/·��
		unsigned road_driving:1;			// ·����ʻʱ�䲻��/����
		unsigned prohibited_area:1;			// ����·����ʻ
		unsigned sersor_fault:1;			// ���ٴ���������
		unsigned illegal_ignition:1;		// �����Ƿ����
		unsigned illegal_displacement:1;	// �����Ƿ�λ��
		unsigned abnormal_storage:1;		// �ն˴洢�쳣
		unsigned :0;
    }flag;
}report_alarm_t;


// ״̬��־����
typedef union 
{
	unsigned whole;
	
	struct
	{
		unsigned fix:1;					// =0,�����Ƕ�λ;=1,δ���Ƕ�λ
		unsigned lat:1;					// =0,��γ;=1,��γ
		unsigned lon:1;					// =0,����;=1,����
		unsigned operating:1;			// =0,Ӫ��״̬;=1,ͣ��״̬
		unsigned reservation:1;			// =0,δԤԼ;=1,ԤԼ
		unsigned weighting:1;			// =0,Ĭ��;=1, ��ת��
		unsigned emptying:1;			// =0,Ĭ��;=1, ��ת��
		unsigned reserved:1;
		unsigned acc:1;					// =0,ACC ��;=1,ACC ��
		unsigned loading:1;				// =0,�ճ�;=1,�س�
		unsigned oil:1;					// =0,������·����;=1,������·�Ͽ�
		unsigned circuit:1;				// =0,������·����;=1,������·�Ͽ�
		unsigned door_lock:1;			// =0,���Ž���;=1, ���ż���
		unsigned car_lock:1;			// =0,��������;=1, ��������
		unsigned :0;
    }flag;
}report_status_t;



// λ�û㱨����Ϣ����Ϣ���ݸ�ʽ
typedef struct {
	report_alarm_t	alarm;				// ������־
	report_status_t status;				// ״̬
	
	unsigned int latitude;				// γ�� (1/10000 ��)
	unsigned int longitude;				// ���� (1/10000 ��)
	unsigned short speed;				// �ٶ� (1/10KM/H)
	unsigned char direction;			// ���� (0~178,�̶�=2  ��,����Ϊ0,˳ʱ��)

	gb905_timestamp_t timestamp;		// ʱ�� (BCD�� YY-MM-DD-hh-mm-ss)
}__packed report_body_t;

// λ�û㱨��������Ϣ���ݸ�ʽ
typedef  struct
{
	unsigned char start_magic_id;
	gb905_header_t header;
	report_body_t report_body;
	unsigned char xor;
	unsigned char end_magic_id;
} __packed gb905_report_t;

// λ�ò�ѯӦ�����Ϣ����Ϣ���ݸ�ʽ
typedef struct {
	unsigned short ack_serial_number;
	report_body_t report;
}__packed report_ack_body_t;

// λ�ò�ѯӦ���������Ϣ���ݸ�ʽ
typedef  struct
{
    unsigned char start_magic_id;
    gb905_header_t header;
    report_ack_body_t report_ack_body;
    unsigned char xor;
    unsigned char end_magic_id;
} __packed gb905_report_ack_t;



void gb905_report_send(void);
void gb905_report_ack_send(unsigned short seq_number);

void gb905_build_report_body(report_body_t * report_body);

#ifdef __cplusplus
}
#endif
	
#endif

