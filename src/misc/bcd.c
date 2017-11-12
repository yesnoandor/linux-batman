/****************************************Copyright (c)**************************************************                         
** File name:			bcd.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-04
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


#define		DEBUG_Y
#include	"libs/debug.h"


unsigned int power(int base, int times) 
{
	int i;
	unsigned int rusult = 1;
	
	for(i=0;i < times; i++)
		rusult *= base;

	return rusult;
}

/*
* 十进制转化为bcd  码
* @dec : 十进制的值
* @buf : 存放bcd  的缓存
* @len : bu  存放bcd缓存的长度
* @return : buf  中的有效数据开始位置
*/
int decimal2bcd(unsigned int dec,unsigned char *buf, int len)
{
	int i;
	unsigned temp;

	//DbgPrintf("dec = %d\r\n",dec);
	//DbgPrintf("len = %d\r\n",len);

	memset(buf,0x00,len);
	
	for(i=len-1;i>=0;i--)
	{
		if(dec == 0)
			break;
			
		temp = dec % 100;		
		buf[i] = ((temp/10) << 4) | ((temp % 10) & 0x0F);   
		dec /= 100;


		//DbgPrintf("temp = %d\r\n",temp);
		//DbgPrintf("dec = %d\r\n",dec);
		//DbgPrintf("val = %d\r\n",buf[i]);

	}
	
	return i+1;
}

/*
* bcd码转化为十进制
* @buf : 存放bcd的缓存
* @len : 存放bcd缓存的长度
* @return : 返回十进制的数据
*/
unsigned int bcd2decimal(unsigned char *buf, int len)
{
	int	i;
	unsigned int tmp;
	unsigned int dec = 0;
	
	for(i=0;i<len;i++)
	{
		tmp = ((buf[i] >> 4) & 0x0F) * 10 + (buf[i]&0x0F);
		dec += tmp * power(100, len-1-i);    
	}

	return dec;
}

bool is_bcd(unsigned char *buf, int len)
{
	int i;
	
	for(i=0; i<len; i++)
	{
		if((((buf[i] >> 4) & 0x0F) > 0x09) || ((buf[i]  & 0x0F) > 0x09))
			return false;
	}

	return true;
}

void bcdtime_to_char(unsigned char *bcd_time,unsigned char* str_time)
{
	
	DbgFuncEntry();
	unsigned int year,mon,day,hour,min,sec;
	
	year = bcd2decimal(bcd_time++,1);
	mon  = bcd2decimal(bcd_time++,1);
	day  = bcd2decimal(bcd_time++,1);
	hour = bcd2decimal(bcd_time++,1);
	min  = bcd2decimal(bcd_time++,1);
	sec  = bcd2decimal(bcd_time++,1);
	
	sprintf((char*)str_time,"%d-%02d-%02d %02d:%02d:%02d",year+2000,mon,day,hour,min,sec);
	DbgFuncExit();
	return;
}

