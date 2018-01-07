/****************************************Copyright (c)**************************************************                         
** File name:			gb905_nmea.c
** Descriptions:		λ�ø���(0x8202/0x0202)   ��Э�����
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
#include	"common.h"

#include	"misc/misc.h"

#include	"middleware/info/gps.h"

#include	"libs/nmea.h"

//#define		DEBUG_Y
#include	"libs/debug.h"


static nmeaINFO nmea_info;
static nmeaPARSER nmea_parser;



/** 
* @brief 	����NMEA  Э��Ŀ�ͷħ����
* @param buf	�յ����ݵĻ����ַ
* @param len	�յ����ݵĻ��峤��
* @param offset  	���ҵ���ͷħ���ֵ�ƫ�Ƶ�ַ
*
* @return	�Ƿ���ҵ�
*		=true, ���ҵ�
*		=false, û�в��ҵ�
*/
static bool gps_nmea_find_magic_head_char(unsigned char *buf, int len,unsigned int * offset)
{
	unsigned char magic;
	int index=0;
	bool ret = false;
	
	DbgFuncEntry();

	*offset = 0;
	while(index <= len)
	{
		magic = *(buf + index);
		if(magic == '$')
		{
			ret = true;
			*offset = index;
			break;
		}
		index++;
	}
	DbgFuncExit();

	return ret;
}

/** 
* @brief 	����NMEA  Э��Ľ�βħ����
* @param buf	�յ����ݵĻ����ַ
* @param len	�յ����ݵĻ��峤��
* @param offset  	���ҵ���βħ���ֵ�ƫ�Ƶ�ַ
*
* @return	�Ƿ���ҵ�
*		=true, ���ҵ�
*		=false, û�в��ҵ�
*/
static bool gps_nmea_find_magic_tail_char(unsigned char *buf, int len,unsigned int * offset)
{
	unsigned char magic;
	unsigned char magic2;
	int index=0;
	bool ret = false;
	
	DbgFuncEntry();

	*offset = 0;
	while(index < len)
	{
		magic = *(buf + index);
		magic2 = *(buf + index + 1);
		if((magic == '\r')&&(magic2 == '\n'))
		{
			ret = true;
			*offset = index;
			break;
		}
		index++;
	}
	
	DbgFuncExit();

	return ret;
}



/** 
* @brief	   ������һ��������GPS  NMEA   ��Ϣ
* @param msg		����GPS  NMEA  ���ݵ�buff  ����ָ��
*
* @return    ����Э���ƫ�Ƶ�ַ
*/
static int gps_nmea_get_a_full_msg(buff_mgr_t *msg)
{
	bool ret;
	unsigned int head_offset = 0,tail_offset = 0;
	int offset;

	DbgFuncEntry();
	
	ret = gps_nmea_find_magic_head_char(msg->buf,msg->len,&head_offset);
	if(ret && (msg->len - head_offset > 9)) // least : $GPGGA,\r\n
	{
		ret = gps_nmea_find_magic_tail_char(msg->buf + head_offset + 7,msg->len - head_offset - 7,&tail_offset);
		tail_offset += head_offset + 7;
		if(ret)
		{
			DbgPrintf("head_offset = %d\r\n",head_offset);
			DbgPrintf("tail_offset = %d\r\n",tail_offset);
			msg->buf = (msg->buf + head_offset);
			msg->len = tail_offset - head_offset + 2;
			offset = tail_offset + 2;
		}
		else
		{
			offset = head_offset;
		}
	}
	else
	{
		offset = head_offset;
	}
	
	//DbgPrintf("offset = %d\r\n",offset);
	
	DbgFuncExit();
	
	return offset;
}

/*
*  @brief 		����һ��������GPS   NMEA    ��Ϣ
*  @param msg 	���������Ϣ�����buf
* 
*/
static void gps_nmea_parse_a_full_msg(buff_mgr_t *msg)
{
	int i;
	gps_info_t gps_info;
	char time1[60] = {0};

	static int set_time_status = 0;

	DbgFuncEntry();
	
#if 0
	DbgPrintf("parse full gps msg :\r\n");


	for(i = 0;i < msg->len;i++)
	{
		DbgPrintf("%c",msg->buf[i]);
	}
	DbgPrintf("\r\n\r\n");
#endif
	
#if 1
	for(i = 0;i < msg->len;i++)
	{
		DbgPrintf(" 0x%02x ",msg->buf[i]);
	}
	DbgPrintf("\r\n");
#endif

	static const char *pheads[] = {
		   "$GNGGA",
		   "$GPGGA",
		   "$GNRMC",
		   "$GPRMC",
	};
	
	//if((0 == memcmp((const char*)msg->buf,pheads[1],6)) || (0 == memcmp((const char*)msg->buf,pheads[3],6)))
	//{
	for(i=0;i<ARRAY_SIZE(pheads);i++)
	{
		if(0 == memcmp((const char*)msg->buf,pheads[i],6))
		{
			nmea_parse(&nmea_parser,(const char*)msg->buf,(int)msg->len,&nmea_info);
			
			DbgPrintf("---- info->utc.year = %d \r\n",nmea_info.utc.year);
			DbgPrintf("---- info->utc.mon = %d \r\n",nmea_info.utc.mon);
			DbgPrintf("---- info->utc.day = %d \r\n",nmea_info.utc.day);
			DbgPrintf("---- info->utc.hour = %d \r\n",nmea_info.utc.hour);
			DbgPrintf("---- info->utc.min = %d \r\n",nmea_info.utc.min);
			DbgPrintf("---- info->utc.sec = %d \r\n",nmea_info.utc.sec);
			DbgPrintf("---- info->utc.hsec = %d \r\n",nmea_info.utc.hsec);
			DbgPrintf("---- info->smask = %d \r\n",nmea_info.smask);
			DbgPrintf("---- info->sig = %d \r\n",nmea_info.sig);
			DbgPrintf("---- info->fix = %d \r\n",nmea_info.fix);
			DbgPrintf("---- info->PDOP = %lf \r\n",nmea_info.PDOP);
			DbgPrintf("---- info->HDOP = %lf \r\n",nmea_info.HDOP);
			DbgPrintf("---- info->VDOP = %lf \r\n",nmea_info.VDOP);
			DbgPrintf("---- info->lat = %lf\r\n",nmea_info.lat); 
			DbgPrintf("---- info->lon = %lf \r\n",nmea_info.lon); 
			DbgPrintf("---- info->elv = %lf \r\n",nmea_info.elv);
			DbgPrintf("---- info->speed = %lf \r\n",nmea_info.speed);
			DbgPrintf("---- info->direction = %lf\r\n",nmea_info.direction);
			DbgPrintf("---- info->declination = %lf \r\n",nmea_info.declination);
			DbgPrintf("---- info->satinfo.inuse = %d \r\n",nmea_info.satinfo.inuse);
			DbgPrintf("---- info->satinfo.inview = %d \r\n",nmea_info.satinfo.inview);

			gps_info.utc = nmea_info.utc;
			gps_info.utc.mon += 1;
			gps_info.utc.hour += 8;
			gps_info.latitude = (unsigned int)((((unsigned int)nmea_info.lat)/100)*60*10000 + (((unsigned int)nmea_info.lat)%100)*10000 + ((unsigned int)(nmea_info.lat*10000))%10000);
			gps_info.longitude = (unsigned int)((((unsigned int)nmea_info.lon)/100)*60*10000 + (((unsigned int)nmea_info.lon)%100)*10000 + ((unsigned int)(nmea_info.lon*10000))%10000);
			gps_info.speed = (unsigned short)(nmea_info.speed*10);
			gps_info.direction = (unsigned char)nmea_info.direction;

			gps_info.number = 4;		// default
		
			if(nmea_info.fix >=2)
			{
				gps_info.fixed = 0;
				if(!set_time_status)
				{
					sprintf(time1, "date -s \"%d-%d-%d %d:%d:%d\"",
					1900 + gps_info.utc.year, gps_info.utc.mon, gps_info.utc.day, 
					gps_info.utc.hour,gps_info.utc.min,gps_info.utc.sec);
					system(time1);
					set_time_status=1;
				}
				DbgGood("GPS location OK!!! \r\n");
			}
			else
			{
				gps_info.fixed = 1;
				DbgWarn("GPS location filed!!! \r\n");
			}
			
			DbgPrintf("gps_info.latitude = %ld \r\n",gps_info.latitude);
			DbgPrintf("gps_info.longitude = %ld \r\n",gps_info.longitude);
			DbgPrintf("gps_info.speed = %d \r\n",gps_info.speed);
			DbgPrintf("gps_info.direction = %d \r\n",gps_info.direction);
			DbgPrintf("gps_info.fixed = %d \r\n",gps_info.fixed);
			DbgPrintf("gps_info.utc %d-%d-%d %d-%d-%d-%d \r\n",gps_info.utc.year,gps_info.utc.mon,gps_info.utc.day,gps_info.utc.hour,gps_info.utc.min,gps_info.utc.sec,gps_info.utc.hsec);

			gps_set_info(&gps_info);
			
		}
		//}
	}
	
	DbgFuncExit();
}


//-----
/** 
* @brief 	GPS  NMEA  Э�����
* @param buf	 	���NMEA   ��Ϣ����
* @param len		���NMEA   ��Ϣ����
*
* @return 		�����������ݳ���(   ���Դӻ��������Ƴ�)
*/
int gps_nmea_protocol_ayalyze(unsigned char * buf,int len)
{
	unsigned char * msg_buf;
	int msg_len;
	int offset;
	buff_mgr_t gps_buf;
	
	DbgFuncEntry();
	
	msg_buf = buf;
	msg_len = len;

	do{
		memset(&gps_buf,0,sizeof(buff_mgr_t));
		gps_buf.buf = msg_buf;
		gps_buf.len = msg_len;
		offset = gps_nmea_get_a_full_msg(&gps_buf);
		if(offset && gps_buf.len)
		{
			gps_nmea_parse_a_full_msg(&gps_buf);
		}
		msg_buf += offset;
		msg_len -= offset;
	}while(offset && gps_buf.len && msg_len > 0);

	DbgFuncExit();
	
	return (len - msg_len);
}


void gps_nmea_init(void)
{
	nmea_zero_INFO(&nmea_info);
	nmea_parser_init(&nmea_parser);
}

