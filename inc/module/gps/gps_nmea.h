/****************************************Copyright (c)**************************************************                         
** File name:			gps_nmea.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-23
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef _GPS_NMEA_H
#define _GPS_NMEA_H

#ifdef __cplusplus
extern "C" {
#endif

void gps_nmea_init(void);

int gps_nmea_protocol_ayalyze(unsigned char * buf,int len);


#ifdef __cplusplus
}
#endif



#endif





