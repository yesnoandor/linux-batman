/****************************************Copyright (c)**************************************************                         
** File name:			fleety_pppd.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-08
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef _FLEETY_PPPD_H
#define	_FLEETY_PPPD_H

#ifdef __cplusplus
extern "C" {
#endif

#define		PPPD_DAIL_TIMEOUT		60

void pppd_dail_first(void);

void pppd_dail_check(void);

#ifdef __cplusplus
}
#endif

#endif

