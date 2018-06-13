/****************************************Copyright (c)**************************************************                         
** File name:			phone_book.h
** Descriptions:		记录电话薄信息
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-12
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef _PHONE_BOOK_H
#define _PHONE_BOOK_H

#ifdef __cplusplus
extern "C" {
#endif

#define	PHONE_BOOK_MAX_NUM		100


// 电话薄信息结构
typedef struct{
	unsigned char flag;										// 标志
	unsigned char phone_num[PHONE_NUMBER_MAX_LEN];			// 电话号码
	unsigned char name[PNONE_NAME_MAX_LEN];					//联系人
}__packed msg_phone_book_t;

// 电话薄信息保存结构
typedef struct{
	unsigned int num;										//电话薄信息数目
	msg_phone_book_t phone_book_msg[PHONE_BOOK_MAX_NUM];	// 电话薄信息结构
}__packed save_phone_book_t;


void phone_book_set_info(save_phone_book_t *phone_book_info);
void phone_book_get_info(save_phone_book_t *phone_book_info);
void phone_book_info_debug(void);


#ifdef __cplusplus
}
#endif
	
#endif

