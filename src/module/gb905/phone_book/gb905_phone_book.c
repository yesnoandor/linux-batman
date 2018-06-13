/****************************************Copyright (c)**************************************************                         
** File name:			gb905_phone_book.c
** Descriptions:		���õ绰��(0x8401)
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

#include	"middleware/db/sqlite3/phone_book_sqlite3.h"

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/phone_book/gb905_phone_book.h"

#define		DEBUG_Y
#include	"libs/debug.h"



// �绰�����õ���Ϣ���ʽ
typedef struct{
    unsigned char number;						// ��ϵ������
    int content;								// �绰����ʼ�ֽ�
}__packed gb905_phone_book_t;


// �绰��������ݸ�ʽ
typedef struct{
	unsigned char flag;								// ��־:1:����;2:����;3:����/����
	char number[PHONE_NUMBER_MAX_LEN];				// �绰����
	char name[PNONE_NAME_MAX_LEN];					// ��ϵ��
}__packed phone_book_list_t;


static phone_book_list_t phone_book_list[PHONE_MEMBER_MAX_NUMBER];



/**
* @brief 	ƽ̨���õ绰���Ĵ���
* @param buf		���õ绰����Ϣ������ݵ�ַ
* @param len		���õ绰����Ϣ������ݳ���
*
* @return  �����ն�ͨ��Ӧ���״̬
*/
unsigned char gb905_set_phone_book_treat(unsigned char *buf,int len)
{
    gb905_phone_book_t * phone_book;
    
    int i;
	int offset;
	char *point;

	DbgFuncEntry();

	phone_book = (gb905_phone_book_t *)buf;
	point = (char *)&phone_book->content;  // ָ��ָ��绰������

	// �����ϵ��������Ϊ0����ɾ������������ϵ��
    if(0 == phone_book->number)
	{
		memset(phone_book_list,0x00,sizeof(phone_book_list));

		// ɾ���ն����ݿ��е�������ϵ��	
		phone_book_sqlite3_delete_record();
	}
	else
    {	
    	//ɾ��ԭ������ϵ����Ϣ
    	phone_book_sqlite3_delete_record();
		
    	for(i = 0;i<phone_book->number;i++)
		{
    		memset(&phone_book_list[i],0x00,sizeof(phone_book_list_t));

			phone_book_list[i].flag = *point;
			point++;
			offset = strlen((char *)point) + 1;// 1:'/0'
            
    		if(offset > PHONE_NUMBER_MAX_LEN)
    		{
    			memcpy(phone_book_list[i].number,point,PHONE_NUMBER_MAX_LEN - 1);
    		}
    		else
    		{
    			memcpy(phone_book_list[i].number,point,offset);
    		}

			point += offset;

			offset = strlen((char *)point) + 1; // 1:'/0'
			if(offset > PNONE_NAME_MAX_LEN)
    		{
    			memcpy(phone_book_list[i].name,point,PNONE_NAME_MAX_LEN - 1);
    		}
    		else
    		{
    			memcpy(phone_book_list[i].name,point,offset);
    		}

			//������ϵ����Ϣ�����ݿ�
			phone_book_sqlite3_insert_record((int)phone_book_list[i].flag,phone_book_list[i].number,phone_book_list[i].name);
			
    		point += offset;
    		
    	}

		
    }
	
	phone_book_sqlite3_restore_record();//for test
		
	//gb905_debug_phone_book();

	DbgFuncExit();

    return GB905_RESULT_OK;
}


/**
* @brief 	��ӡ�绰������Ϣ
*
*/
void gb905_debug_phone_book(void)
{
	int i = 0;

	DbgFuncEntry();
	
	for(i=0;i < PHONE_MEMBER_MAX_NUMBER;i++)
	{
		if(strlen(phone_book_list[i].number) == 0 || strlen(phone_book_list[i].name) == 0)
			break;
	
		DbgPrintf("phone flag 0x%02x \r\n",phone_book_list[i].flag);
		DbgPrintf("phone number %s \r\n",phone_book_list[i].number);
		DbgPrintf("phone name %s \r\n",phone_book_list[i].name);

	}

	DbgFuncExit();
} 

