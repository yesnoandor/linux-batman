/****************************************Copyright (c)**************************************************                         
** File name:			gps.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-01
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef _GPS_H
#define _GPS_H

#include	"libs/nmea.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	nmeaTIME	utc;            		//  UTC of position 

	int		fixed;						// �Ƿ�λ
	unsigned int latitude;				// γ�� (1/10000 ��)
	unsigned int longitude;				// ���� (1/10000 ��)
	unsigned short speed;				// �ٶ� (1/10KM/H)
	unsigned char direction;			// ���� (0~178,�̶�=2  ��,����Ϊ0,˳ʱ��)

	int number;							// ����
}gps_info_t,*p_gps_info_t;


void gps_get_info(gps_info_t * gps_info);
void gps_set_info(gps_info_t * gps_info);


#ifdef __cplusplus
}
#endif



#endif





