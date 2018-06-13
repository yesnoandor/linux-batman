/****************************************Copyright (c)**************************************************                         
** File name:			gb905_question.c
** Descriptions:		�����·�(0x8302),����Ӧ��(0x0302)
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2017-12-29
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#include	"common.h"
#include	"misc/endian.h"

#include	"middleware/socket/fleety_socket.h"

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/question/gb905_question.h"
#include	"module/gb905_ex/ui/ui_question.h"

#define		DEBUG_Y
#include	"libs/debug.h"

#define 	QUESTION_MAX_LEN    100

#define 	ANSWER_MAX_LEN      20
#define		ANSWER_MAX_NUM      30


//������Ϣ�·���־
typedef union 
{
	unsigned char whole;
	
	struct
	{
		// low --> high
		unsigned char urgent:1;		        // bit0 : ����
		unsigned char reserved1:1;			// bit1 : Ԥ��
		unsigned char reserved2:1;          // bit2 : Ԥ��
		unsigned char tts:1;                // bit3 : �����ϳɲ���
		unsigned char adv:1;                // bit4 : �������ʾ
		unsigned char :0;                   // bit5~bit7 : Ԥ��
    }flag;
}gb905_question_flag_t;

// ������Ϣ�·��ṹ
typedef struct{
    gb905_question_flag_t flag;				// ������Ϣ��־
    unsigned int id;						// ����ID
    int content;							// ������Ϣ�ĵ�ַ(������Ϣ����:��������,��ѡ���б�)
}__packed gb905_question_issue_t;


// ����Ϣ�ṹ������һ�δ𰸵���Ϣ
typedef struct{
    unsigned char id;                        // ��ID
    unsigned char content[ANSWER_MAX_LEN];   // ������
}__packed answer_info_t;


// ������Ϣ�ṹ������һ���������Ϣ
typedef struct{
    gb905_question_flag_t flag;					// ������Ϣ��־
    unsigned int id;							// ����ID
    unsigned char content[QUESTION_MAX_LEN];	// ������Ϣ�����ֽ�(������Ϣ����:��������,��ѡ���б�)
}__packed question_info_t;




// ����Ӧ����Ϣ������ݸ�ʽ
typedef struct{
    unsigned int question_id;                       // ����ID
    unsigned char answer_id;                        // ��ID
}__packed gb905_question_ack_body_t;


// ����Ӧ���������Ϣ���ݸ�ʽ
typedef struct{
	unsigned char start_magic_id;
	gb905_header_t header;
	gb905_question_ack_body_t question_ack_body;
	unsigned char xor;
	unsigned char end_magic_id;
}__packed gb905_question_ack_t;


static question_info_t question_info;
static answer_info_t answer_info_list[ANSWER_MAX_NUM];

/**
* @brief 	��ӡ�·������ʺʹ𰸵���Ϣ
*
*/
static void gb905_debug_question(void)
{
	int i = 0;

	DbgFuncEntry();

	DbgPrintf("question flag 0x%x \r\n",question_info.flag.whole);
	DbgPrintf("question id %d \r\n",EndianReverse32(question_info.id));
	DbgPrintf("question content %s \r\n",question_info.content);
	
	for(i=0;i < ANSWER_MAX_NUM;i++)
	{
		if(answer_info_list[i].id == 0)
			break;
		
		DbgPrintf("answer id %d \r\n",answer_info_list[i].id);
		DbgPrintf("answer content %s \r\n",answer_info_list[i].content);
	}

	DbgFuncExit();
} 


/** 
* @brief 	������������Ӧ������ݽṹ
* @param question_ack 	��������Ӧ�����ݽṹ��ַ
* @param question_id 		Ӧ�������ID
* @param answer_id 		    Ӧ��Ĵ�ID
*
*/
static void gb905_build_question_ack(gb905_question_ack_t *question_ack,unsigned int question_id,unsigned char answer_id)
{
    DbgFuncEntry();

    gb905_build_header(&question_ack->header,MESSAGE_QUESTION_ACK,sizeof(gb905_question_ack_t));

    question_ack->question_ack_body.question_id = EndianReverse32(question_id);
    question_ack->question_ack_body.answer_id = answer_id;
    
    DbgFuncExit();
}


/*
* @brief			�����·��Ĵ���
* @param buf		�����·���Ϣ������ݵ�ַ
* @param len		�����·���Ϣ������ݳ���
*
* @return			�����ն�ͨ��Ӧ���״̬
*/
unsigned char gb905_question_issue_treat(unsigned char *buf,int len)
{
    gb905_question_issue_t *question;
   // unsigned char question_content[QUESTION_MAX_LEN]={0};
	
    int size;
    int answer_num =0 ;
    int answer_list_len = 0;
    int one_answer_len = 0;
    int data_len = 0;
    char *point;
    
    DbgFuncEntry();

    question = (gb905_question_issue_t *)buf;
    
    point = (char *)buf;		
    size = strlen(point) + 1;			// 1 : '/0'

	memset(&question_info,0x00,sizeof(question_info));
    memcpy(&question_info,point,size + sizeof(question->flag) + sizeof(question->id));

	// ָ��ָ����б�
    point += sizeof(question->flag) + sizeof(question->id) + size;

    // �������б�ĳ���
    answer_list_len = len - (size + sizeof(question->flag) + sizeof(question->id));

	answer_num = 0;
    while(data_len < answer_list_len)
    {
        answer_info_list[answer_num].id = *point;
		point++;
		one_answer_len =strlen(point) + 1;	// 1:'/0'

        // �����𰸳��ȴ�����󳤶�(20)�Ĵ���
		if(one_answer_len >= ANSWER_MAX_LEN)
		{
			data_len = one_answer_len + 1;// 1:'len of answer_id'
			point += one_answer_len;
			continue;
		}
		
		memset((void *)answer_info_list[answer_num].content,0x00,ANSWER_MAX_LEN);
		strncpy((void *)answer_info_list[answer_num].content,(void *)point,one_answer_len);
		point += one_answer_len;
		data_len += one_answer_len + 1;	// 1:'len of answer_id'
		answer_num++;
    }

	ui_send_question_info(buf,len);
	gb905_debug_question();
  
    DbgFuncExit();

    return GB905_RESULT_OK;
}


/*
* @brief 	��������Ӧ�𵽷�����
* @param question_id	����ID
* @param answer_id	��ID
*
* @return  �����ն�ͨ��Ӧ���״̬
*/
void gb905_question_ack_treat(unsigned int question_id,unsigned char answer_id)
{
    gb905_question_ack_t question_ack;
    
    DbgFuncEntry();

    gb905_build_question_ack(&question_ack,question_id,answer_id);
    gb905_send_data(MAIN_SOCKET,(unsigned char *)&question_ack,sizeof(gb905_question_ack_t));
    
    DbgFuncExit();
}

