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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Date and time data
 * @see nmea_time_now
 */
typedef struct
{
    int     year;       /**< Years since 1900 */
    int     mon;        /**< Months since January - [0,11] */
    int     day;        /**< Day of the month - [1,31] */
    int     hour;       /**< Hours since midnight - [0,23] */
    int     min;        /**< Minutes after the hour - [0,59] */
    int     sec;        /**< Seconds after the minute - [0,59] */
    int     hsec;       /**< Hundredth part of second - [0,99] */
} utc_t;

typedef struct
{
    utc_t	utc;            			//  UTC of position 

	int		fixed;						// �Ƿ�λ
	unsigned int latitude;				// γ�� (1/10000 ��)
	unsigned int longitude;				// ���� (1/10000 ��)
	unsigned short speed;				// �ٶ� (1/10KM/H)
	unsigned char direction;			// ���� (0~178,�̶�=2  ��,����Ϊ0,˳ʱ��)
}gps_info_t,*p_gps_info_t;


void gps_get_info(gps_info_t * gps_info);
void gps_set_info(gps_info_t * gps_info);


#ifdef __cplusplus
}
#endif



#endif





