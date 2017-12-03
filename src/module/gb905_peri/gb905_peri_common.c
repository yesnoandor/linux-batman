/****************************************Copyright (c)**************************************************                         
** File name:			gb905_peri_common.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-18
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

#include	"misc/bcd.h"
#include	"misc/endian.h"

#include	"module/gb905_peri/gb905_peri_common.h"


#define		DEBUG_Y
#include	"libs/debug.h"



#define		GB905_PERI_MAGIC_NUM		0x55AA


bool find_gb905_peri_magic_char(unsigned char *buf, int len,unsigned int * offset)
{
	unsigned short * p_magic;
	unsigned short magic;
	
	int index=0;
	bool ret = false;
	
	//gb905_meter_header_t * header;
	
	DbgFuncEntry();

	//header = (gb905_meter_header_t *)buf;
	
	*offset = 0;
	
	while(index < len)
	{
		p_magic = (unsigned short *)(buf+index);
		magic = EndianReverse16(*p_magic);
		
		if(magic == GB905_PERI_MAGIC_NUM)
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
* @brief	   解析出一条完整的GB905   外围设备的消息
* @param msg			buff  管理指针
*
* @return    完整协议的偏移地址
*/   
int gb905_peri_get_a_full_msg(double_buff_mgr_t *msg)
{
	bool ret;
	unsigned int head_offset = 0,tail_offset = 0;
	int offset;

	DbgFuncEntry();
	
	ret = find_gb905_peri_magic_char(msg->raw.buf,msg->raw.len,&head_offset);
	if(ret && (msg->raw.len - head_offset > sizeof(gb905_peri_header_t) + sizeof(gb905_peri_tail_t)))
	{
		ret = find_gb905_peri_magic_char(msg->raw.buf + head_offset + 2,msg->raw.len - head_offset - 2,&tail_offset);
		tail_offset += head_offset + 2;
		if(ret)
		{
			if(tail_offset - head_offset + 2 > sizeof(gb905_peri_header_t) + sizeof(gb905_peri_tail_t))
			{
				//DbgPrintf("head_offset = %d\r\n",head_offset);
				//DbgPrintf("tail_offset = %d\r\n",tail_offset);
			
				msg->raw.buf = (msg->raw.buf + head_offset);
				msg->raw.len = tail_offset - head_offset + 2;
				offset = tail_offset + 2;
			}
			else
			{
				msg->raw.len = 0;
				offset = head_offset;
			}
		}
		else
		{
			msg->raw.len = 0;
			offset = head_offset;
		}
	}
	else
	{
		msg->raw.len = 0;
		offset = head_offset;
	}

	//DbgPrintf("offset = %d\r\n",offset);
	
	DbgFuncExit();
	
	return offset;
}



void gb905_peri_build_header(gb905_peri_header_t* header,unsigned short cmd,unsigned short len)
{
	header->magic_start = EndianReverse16(GB905_PERI_MAGIC_NUM);
	header->len = EndianReverse16(len);
	header->cmd = EndianReverse16(cmd);
}

void gb905_peri_build_tail(gb905_peri_tail_t * tail,unsigned char xor)
{
	tail->xor = xor;
	tail->magic_end = EndianReverse16(GB905_PERI_MAGIC_NUM);
}


void gb905_peri_build_timestamp(gb905_peri_bcd_timestamp_t * timestamp)
{
	time_t timep;
	struct tm *p;
	
	time(&timep);
	p = localtime(&timep);

	decimal2bcd(p->tm_year + 1900,&timestamp->bcd[0],2);
	decimal2bcd(p->tm_mon+1,&timestamp->bcd[2],1);
	decimal2bcd(p->tm_mday,&timestamp->bcd[3],1);
	decimal2bcd(p->tm_hour,&timestamp->bcd[4],1);
	decimal2bcd(p->tm_min,&timestamp->bcd[5],1);
	decimal2bcd(p->tm_sec,&timestamp->bcd[6],1);
}

