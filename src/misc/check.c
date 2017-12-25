/****************************************Copyright (c)**************************************************                         
** File name:			check.c
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




unsigned short crc_computer(const unsigned char * ptr,int count)
{
	unsigned short crc = 0 ;
	char ii;
	
	while(--count >= 0)
	{
		crc = crc ^ (int)*ptr++ << 8;
		ii = 8;
		do
		{
			if(crc & 0x8000)
			{
				crc = crc << 1 ^ 0x1021;
			}
			else
			{
				crc = crc << 1;
			}
		}while(--ii);
	}
	
	return(crc);
}

unsigned short crc16_compute(const unsigned char * p_data, unsigned int size, const unsigned short * p_crc)
{
    unsigned int i;
    unsigned short crc = (p_crc == NULL) ? 0xffff : *p_crc;

    for (i = 0; i < size; i++)
    {
        crc = (unsigned char)(crc >> 8) | (crc << 8);
        crc ^= p_data[i];
        crc ^= (unsigned char)(crc & 0xff) >> 4;
        crc ^= (crc << 8) << 4;
        crc ^= ((crc & 0xff) << 4) << 1;
    }
    
    return crc;
}


unsigned char xor8_computer(unsigned char * p_data, unsigned int len)
{
	unsigned char xor = 0;
	unsigned int i;

	for(i=0;i<len;i++)
	{
		xor ^= p_data[i];
	}
		
	return xor;
}

unsigned char sum8_compute(unsigned char * p_data, unsigned int len)
{
	unsigned char sum = 0;
	unsigned int i;

	for(i=0;i<len;i++)
	{
		sum += p_data[i];
		sum =sum&0xff;
	}
		
	return sum;
}

unsigned short sum16_compute(unsigned char * p_data, unsigned int len)
{
	unsigned short sum=0;
	unsigned int i=0;

	for(i=0; i<len; i++)
	{
		sum += (unsigned short)p_data[i];
	}

	return sum;
}



/**
* @brief 		检查目录是否存在
* @param 	path		目录路径
* 
* @return		返还是否存在
*	= false,  不存在
*	= true,   存在
*/
bool is_folder_exist(const char* path)
{
    DIR *dp;
    if ((dp = opendir(path)) == NULL)
    {
        return false;
    }

    closedir(dp);
	
    return true;
}

/**
* @brief 		检查文件是否存在
* @param 	path		目录路径
* 
* @return		返还是否存在
*	= false,  不存在
*	= true,   存在 
*/
bool is_file_exist(const char* path)
{
    return !access(path, F_OK);
}

