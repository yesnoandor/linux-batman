/****************************************Copyright (c)**************************************************                         
** File name:			mcu_eval.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2016-11-29
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/


#ifndef _MCU_EVAL_H
#define	_MCU_EVAL_H

#ifdef __cplusplus
extern "C" {
#endif

enum{
	EVAL_NONE,
	EVAL_OK,
	EVAL_SOSO,
	EVAL_BAD,
	EVAL_COMPLAINT,
};

unsigned char mcu_get_evaluate_treat(unsigned char *buf,int len);



#ifdef __cplusplus
}
#endif

#endif

