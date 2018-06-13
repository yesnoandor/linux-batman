/****************************************Copyright (c)**************************************************                         
** File name:			gb905_phone_book.h
** Descriptions:		设置电话本
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
#ifndef _GB905_PHONE_BOOK_H
#define _GB905_PHONE_BOOK_H

#ifdef __cplusplus
extern "C" {
#endif



#define 	PHONE_MEMBER_MAX_NUMBER		100		// 最多联系人成员
#define 	PHONE_NUMBER_MAX_LEN		20		// 电话号码字符串最大长度
#define 	PNONE_NAME_MAX_LEN			10		// 电话联系人字符串最大长度



unsigned char gb905_set_phone_book_treat(unsigned char *buf,int len);

#ifdef __cplusplus
}
#endif
	
#endif

