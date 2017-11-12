/****************************************Copyright (c)**************************************************                         
** File name:			endian.h
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
**------------------------------------------------------------------------------------------------------
** Remark         : 
**	TCP/IP  : big-endian
** 	intel/arm : little-endian
********************************************************************************************************/

#ifndef _ENDIANS_H
#define	_ENDIANS_H		

#ifdef __cplusplus
extern "C" {
#endif



#define EndianReverse16(val)	((((unsigned short)(val) & 0xff00) >> 8) | (((unsigned short)(val) & 0x00ff) << 8))
							
#define EndianReverse32(val)	((((unsigned int)(val) & 0xff000000) >> 24) | \
								 (((unsigned int)(val) & 0x00ff0000) >> 8) | \
								 (((unsigned int)(val) & 0x0000ff00) << 8) | \
								 (((unsigned int)(val) & 0x000000ff) << 24))



bool get_cpu_endian(void);

#ifdef __cplusplus
}
#endif

#endif
