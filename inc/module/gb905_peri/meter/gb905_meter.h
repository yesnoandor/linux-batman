/****************************************Copyright (c)**************************************************                         
** File name:			gb905_meter.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-18
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef _GB905_METER_H
#define _GB905_METER_H

#include	"module/gb905/gb905_common.h"

#include	"module/gb905_peri/gb905_peri_common.h"


#ifdef __cplusplus
extern "C" {
#endif

// �Ƽ�����ǰ״̬
typedef union 
{
	unsigned char whole;
	
	struct
	{
		// low --> high
		unsigned char loading:1;	// =0,�ճ���=1,�س�	
		unsigned char login:1;		// =0,ǩ�ˣ�=1,��¼		
		unsigned char force_on:1;	// =0,������=1,ǿ�ƿ���
		unsigned char force_off:1;	// =0,������=1,ǿ�ƹػ�
		unsigned char :0;
    }flag;
}meter_state_t;




// ��BCD  ���ʾʱ������
typedef struct {
	unsigned char bcd[5];				// YYYY-MM-DD-hh
}__packed meter_bcd_time_threhold_t;

// ��BCD  ���ʾ��ǰʱ��
typedef struct{
	unsigned char bcd[6];				// YYYY-MM-DD-hh-mm
}__packed gb905_meter_timestamp_t;




//�Ƽ���״̬��ѯ���ݽṹ(ISU -->  METER)
typedef struct{
	gb905_peri_header_t header;
	gb905_peri_bcd_timestamp_t timestamp;
	gb905_peri_tail_t tail;
}__packed gb905_meter_query_state_t;

// �Ƽ���״̬��Ϣ�����ݽṹ		(METER --> ISU)
typedef struct{
	uuid_device_id_t	uuid_device_id;				// �豸���
	unsigned char  		hw_version;					// Ӳ���汾��
	unsigned char 		sw_major_version;			// ������汾��		
	unsigned char		sw_minor_version;			// ����ΰ汾��	
	unsigned char		device_state;				// �豸״̬
	unsigned char		running_state;				// �Ƽ�������״̬
	unsigned char		license_plate[6];			// ���ƺ�
	unsigned char 		company_license_number[16];	// ��˾��Ӫ���֤��(ASCII��,  ����16  �ֽڣ��Ҳ�0)
	unsigned char 		driver_license_number[19];	// ��ʻԱ��ҵ�ʸ�֤��(ASCII��,  ����19  �ֽڣ��Ҳ�0)
	unsigned int		operation_conut;			// ��Ӫ�˴���
}__packed gb905_meter_state_body_t;


// �Ƽ���������ѯ���ݽṹ(ISU -->  METER)
typedef struct{
	gb905_peri_header_t header;
	gb905_peri_tail_t tail;
}__packed gb905_meter_query_param_t;

// �Ƽ���������Ϣ�����ݽṹ		(METER <--> ISU)
typedef struct{
	meter_bcd_time_threhold_t enable_time;			// ��������ʱ��( BCD[5] : YYYY-MM-DD-hh)
	unsigned char price_return_per_day_bcd[2];		// ������������( BCD[2] : xx.xx yuan/km)
	unsigned char price_return_per_night_bcd[2];	// ҹ����������( BCD[2] : xx.xx yuan/km)
	unsigned char price_oneway_per_day_bcd[2];		// ���쵥�̵���( BCD[2] : xx.xx yuan/km)
	unsigned char price_oneway_per_night_bcd[2];	// ҹ�䵥�̵���( BCD[2] : xx.xx yuan/km)
	unsigned char price_subsidy_per_day_bcd[2];		// ������ο�������( BCD[2] : xx.xx yuan/km)
	unsigned char price_subsidy_per_night_bcd[2];	// ҹ����ο�������( BCD[2] : xx.xx yuan/km)
	unsigned char price_start_per_day_bcd[2];		// �����𲽼�( BCD[2] : xx.xx yuan/km)
	unsigned char price_start_per_night_bcd[2];		// ҹ���𲽼�( BCD[2] : xx.xx yuan/km)

	unsigned char onword_kilometre_bcd[2];			// ���������( BCD[2] : xx.xx km)
	unsigned char start_kilometre_bcd[2];			// ���̹���( BCD[2] : xx.xx km)
	unsigned char oneway_kilometre_bcd[2];			// ���̹���( BCD[2] : xx.xx km)
	unsigned char empty_subsidy_kilometre_bcd[2];	// ���ο�������( BCD[2] : xx.xx km)
	
	unsigned char price_wait_per_day_bcd[2];		// ����Ⱥ򵥼�( BCD[2] : xx.xx yuan)
	unsigned char price_wait_per_night_bcd[2];		// ҹ��Ⱥ򵥼�( BCD[2] : xx.xx yuan)
	
	unsigned char time_free_wait_bcd[2];			// ��ѵȺ�ʱ��( BCD[2] : xxxx min_sec)
	unsigned char time_increase_price_bcd[2];		// �Ӽ�ʱ��( BCD[2] : xxxx min_sec)
	unsigned char start_time_per_night_bcd[2];		// ҹ�俪ʼʱ��( BCD[2] : xxxx hour_min)
	unsigned char end_time_per_night_bcd[2];		// ҹ�����ʱ��( BCD[2] : xxxx hour_min)
	unsigned char rfu[22];							// ϵͳԤ��
	unsigned char vendor_reserved[64];				// �����Զ��������
}__packed gb905_meter_parameter_body_t;

// �Ƽ��������������ݽṹ(ISU -->  METER)
typedef struct{
	gb905_peri_header_t header;
	gb905_meter_parameter_body_t body;
	gb905_peri_tail_t tail;
}__packed gb905_meter_set_param_t;


// �Ƽ�����������Ӧ����Ϣ�����ݽṹ
typedef struct{
	unsigned char ack_result;						// �������
	meter_bcd_time_threhold_t enable_time;			// ����ʱ��
}__packed gb905_meter_set_param_ack_body_t;



// �Ƽ���������Ϣ�����ݽṹ
typedef struct{
	meter_state_t meter_state;
	unsigned char company_license_number[16];	// ��˾��Ӫ���֤��(ASCII��,  ����16  �ֽڣ��Ҳ�0)
	unsigned char driver_license_number[19];	// ��ʻԱ��ҵ�ʸ�֤��(ASCII��,  ����19  �ֽڣ��Ҳ�0)
}__packed gb905_meter_heart_beat_body_t;


//�Ƽ�������Ӧ����Ϣ�����ݽṹ
typedef struct{
	unsigned short terminal_state;						// �ն�����״̬
	meter_bcd_time_threhold_t time_limit;				// ʱ������
	unsigned char number_of_times_limit;				// ��������
	unsigned short rfu;
}__packed gb905_meter_heart_beat_ack_body_t;

//�Ƽ�������Ӧ�����ݽṹ
typedef struct{
	gb905_peri_header_t header;
	gb905_meter_heart_beat_ack_body_t body;
	gb905_peri_tail_t tail;
}__packed gb905_meter_heart_beat_ack_t;







//�Ƽ�������/  �ػ�Ӧ����Ϣ�����ݽṹ
typedef struct{
	unsigned char company_license_number[16];			// ��˾��Ӫ���֤��(ASCII��,  ����16  �ֽڣ��Ҳ�0)
	unsigned char driver_license_number[19];			// ��ʻԱ��ҵ�ʸ�֤��(ASCII��,  ����19  �ֽڣ��Ҳ�0)
	unsigned char plate_number[6];						// ���ƺ�(ASCII ��)
	gb905_meter_timestamp_t swipe_timestamplit;			// ˢ��ʱ��
	unsigned short terminal_status;						// ISU ״̬
	unsigned char time_limit[5];						// ʱ������: YYYYMMDDhh, ȫ0   ����������
	unsigned char number_of_times_limit[2];				// ��������: 0000 ��ʾ������
	unsigned char result;								// �������
}__packed gb905_meter_loginout_ack_body_t;

//�Ƽ�������/  �ػ�Ӧ�����ݽṹ
typedef struct{
	gb905_peri_header_t header;
	gb905_meter_loginout_ack_body_t ack_body;
	gb905_peri_tail_t tail;
}__packed gb905_meter_loginout_ack_t;

#if 0
typedef struct{
	unsigned char name[12];
	unsigned char card_number[18];
	unsigned char business_type[48];
	unsigned char card_validity[8];
	unsigned char server_company[40];
	unsigned char star;
	unsigned char driver_number[6];
	unsigned char company_number[5];
	unsigned char return_result[2];
		
}__packed gb905_tsm_driver_info_t;
#endif

//�Ƽ��������ɹ���Ϣ�����ݽṹ(METER  -->  ISU)
typedef struct{
	unsigned char company_license_number[16];		// ��˾��Ӫ���֤��(ASCII��,  ����16  �ֽڣ��Ҳ�0)
	unsigned char driver_license_number[19];		// ��ʻԱ��ҵ�ʸ�֤��(ASCII��,  ����19  �ֽڣ��Ҳ�0)
	unsigned char plate_number[6];					// ���ƺ�(ASCII ��)
	gb905_meter_timestamp_t open_timestamplit;	// �Ƽ�������ʱ��
	unsigned int total_operation_number;			// ��Ӫ�˴���
	unsigned char result;							// �������
}__packed gb905_meter_open_success_body_t;


// �Ƽ����ػ��ɹ���Ϣ�����ݽṹ(METER  -->  ISU)
typedef struct{
	unsigned char company_license_number[16];		// ��˾��Ӫ���֤��(ASCII��,  ����16  �ֽڣ��Ҳ�0)
	unsigned char driver_license_number[19];		// ��ʻԱ��ҵ�ʸ�֤��(ASCII��,  ����19  �ֽڣ��Ҳ�0)
	unsigned char plate_number[6];					// ���ƺ�(ASCII ��)
	
	unsigned char pulse[2];							// �Ƽ���K   ֵ
	gb905_meter_timestamp_t open_timestamplit;	// ���࿪��ʱ��
	gb905_meter_timestamp_t close_timestamplit;	// ����ػ�ʱ��
	
	unsigned char duty_mileage[3];					// �������(xxxxx.x km)
	unsigned char driver_operation_km[3];			// ����Ӫ�����
	unsigned char driver_num[2];					// ����
	unsigned char conut_time[3];					// ��ʱʱ��
	unsigned char total_money[3];					// �ܼƽ��
	unsigned char card_money[3];					// ���ս��
	unsigned char card_num[2];						// ����
	unsigned char night_km[2];						// ������
	unsigned char total_km[4];						// �ܼ����
	unsigned char total_operation_km[4];			// ��Ӫ�����
	unsigned char unit_price[2];					// ����
	unsigned int total_operation_num;				// ��Ӫ�˴���
}__packed gb905_meter_close_success_body_t;


// �Ƽ�������ʱ������ѯ���ݽṹ
typedef gb905_meter_query_state_t	gb905_meter_calibration_time_t;



// �Ƽ���������Ϣ�����ݽṹ
typedef struct{
	unsigned char vendor_id;
	unsigned char hw_version;
	unsigned char main_sw_version;
	unsigned char vice_sw_version;
}__packed gb905_meter_update_body_t;

// �Ƽ����������������ݽṹ
typedef struct{
	gb905_peri_header_t header;
	gb905_meter_update_body_t body;
	gb905_peri_tail_t tail;
}__packed gb905_meter_update_t;


//  �Ƽ���Ӫ�����ݽṹ(METER  -->  ISU)
typedef struct{
	unsigned char plate_number[6];					// ���ƺ�(ASCII ��)
	unsigned char company_license_number[16];		// ��˾��Ӫ���֤��(ASCII��,  ����16  �ֽڣ��Ҳ�0)
	unsigned char driver_license_number[19];		// ��ʻԱ��ҵ�ʸ�֤��(ASCII��,  ����19  �ֽڣ��Ҳ�0)
	unsigned char get_on_timestamp[5];				// �ϳ�ʱ��(BCD��YY-MM-DD-hh-mm)
	unsigned char get_off_timestamp[2];				// �³�ʱ��(BCD��hh-mm)
	unsigned char distance[3];						// �Ƴ̹���(BCD��xxxxx.x km)
	unsigned char empty_distance[2];				// ��ʻ����(BCD��xxx.x km)
	unsigned char surcharge[3];						// ���ӷ�(BCD��xxxxx.x Ԫ)
	unsigned char waiting_time[2];					// �ȴ���ʱʱ��(BCD��hh-mm)
	unsigned char transaction_amount[3];			// ���׽��(BCD��xxxxx.x Ԫ)
	unsigned int  car_number;						// ��ǰ����	
	unsigned char transaction_type;					// ��������
}__packed gb905_meter_operation_t;


void gb905_meter_query_state(void);
void gb905_meter_query_parameter(void);
void gb905_meter_set_parameter(void);

bool gb905_meter_open_close_ack(void);
void gb905_meter_update(void);


int gb905_meter_protocol_ayalyze(unsigned char * buf,int len);

void gb905_meter_transparent(unsigned char *msg_buf,unsigned short msg_len);

void gb905_meter_heart_beat_treat(void);

#ifdef __cplusplus
}
#endif
	
#endif



