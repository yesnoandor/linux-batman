/****************************************Copyright (c)**************************************************                         
** File name:			version.c
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

#include	"middleware/info/product.h"


#define		DEBUG_Y
#include	"libs/debug.h"

//#define		TEST_HW_VERSION			"v1.0.0.0"
//#define		TEST_HW_VERSION			"570LL"
//#define		TEST_SW_VERSION			"v1.0.0.0"

void get_hw_version(char * buff)
{
	product_params_t product_params;
	
	get_product_params((unsigned char *)&product_params);

	memset(buff,0x00,MAX_HW_VERSION_SIZE);
	strncpy((void *)buff,(void *)product_params.hardware_version,MAX_HW_VERSION_SIZE);
}

/* 
获取程序编译时间, 用于自动生成版本号很好 
 __DATE__  : "Mar 25 2013" 
          	   "Mar  1 2013" 
 
__TIME__  :  "21:42:20" 
*/
void get_sw_version(char * buff)
{
	//memset(buff,0x00,MAX_SW_VERSION_SIZE);
	//memcpy(buff,TEST_SW_VERSION,strlen(TEST_SW_VERSION));char date[MAX_CMD_CHARS_SIZE];
	char date[MAX_CMD_CHARS_SIZE];
	char time[MAX_CMD_CHARS_SIZE];
	int year,month,mday;
	int hour,mintue,second;
	int i;
	
	char * monthes[] =    
    {    
        "Jan",    
        "Feb",    
        "Mar",    
        "Apr",    
        "May",    
        "Jun",    
        "Jul",    
        "Aug",    
        "Sep",    
        "Oct",    
        "Nov",    
        "Dec",    
    };

	strcpy(date,__DATE__);
	strcpy(time,__TIME__);

	DbgPrintf("date = %s\r\n",date);
	DbgPrintf("time = %s\r\n",time);

	year = strtol(date+strlen(date)-4, 0, 10);  

	month = 0;    
    for(i = 0; i < 12; i++)
	{
    	if(strstr(date,monthes[i]))    
        {
            month = i + 1;
            break;    
        }    
    }     
	
	date[6] = '\0';
	mday = strtol(date+4, 0, 10); 
	
	DbgPrintf("year = %d\r\n",year);
	DbgPrintf("month = %d\r\n",month);
	DbgPrintf("mday = %d\r\n",mday);

	time[2] = time[5] = '\0';
	hour = strtol(time, 0, 10);
	mintue = strtol(time + 3, 0, 10);
	second = strtol(time + 6, 0, 10);
	
	DbgPrintf("hour = %d\r\n",hour);
	DbgPrintf("mintue = %d\r\n",mintue);
	DbgPrintf("second = %d\r\n",second);
	
	sprintf(buff,"1.0.%04d%02d%02d.%02d%02d%02d",year, month, mday, hour,mintue,second); 
}

