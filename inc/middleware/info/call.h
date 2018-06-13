/****************************************Copyright (c)**************************************************                         
** File name:			call.c
** Descriptions:		ͨ����Ϣ����
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


//AT+CLCC��ѯ��Ӧ�����ݽṹ(����)
typedef struct{
	int idx;							//�������ͣ��绰ʶ��
	int dir;							//0:�ƶ�̨����(MO)�ĵ绰;1:�ƶ�̨��ֹ(MO)�ĵ绰
	int stat;							//�绰��״̬(0:���ڽ���;1:����;2:����MO;3:����MO;4:����MT;5:�ȴ�MT)
	int mode;							//0:����;1:����;2:����;9:δ֪
	int mpty;							//0:�绰���Ƕ෽�Ự�еĳ�Ա;1:�绰�Ƕ෽�Ự�еĳ�Ա
	char phone[PHONE_NUMBER_MAX_LEN];	//�ַ��͵绰����
}__packed clcc_info_t;

//����ͨ����Ϣ�����ݽṹ
typedef struct{
	unsigned int start_time;			//ͨ����ʼʱ��(��1970�����ڵ�����)
	unsigned int total_time;			//����ͨ����ʱ��(��λ:��)
	unsigned int status;				//ͨ��״̬(1:����ͨ��;2:����ͨ��)
}__packed call_info_t;

void set_clcc_info(clcc_info_t *info);
void get_clcc_info(clcc_info_t *info);

void set_call_info(void);
call_info_t * get_call_info(void);


#ifdef __cplusplus
}
#endif

#endif

