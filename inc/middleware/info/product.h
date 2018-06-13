/****************************************Copyright (c)**************************************************                         
** File name:			product.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2018-03-10
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef _PRODUCT_H
#define	_PRODUCT_H


#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
	unsigned int	meter_vendor_id;			// �Ƽ�������ID
	unsigned int	toplight_vendor_id;			// ���Ƴ���ID
	unsigned int	tsm_vendor_id;				// TSM ����ID
		
	unsigned char 	hardware_version[32];		// Ӳ���汾
}product_params_t;

void init_product_params(void);
void get_product_params(unsigned char *buf);

void debug_product_params(void);

#ifdef __cplusplus
}
#endif

#endif



