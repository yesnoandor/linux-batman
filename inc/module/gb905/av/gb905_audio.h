/****************************************Copyright (c)**************************************************                         
** File name:			gb905_audio.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2018-02-03
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef _GB905_AUDIO_H
#define _GB905_AUDIO_H
			
#ifdef __cplusplus
extern "C" {
#endif
	

void gb905_audio_search_record_treat(unsigned char* buf,int len);
void gb905_audio_upload_record_treat(int audio_id,int offset,unsigned short reason);

		
#ifdef __cplusplus
}
#endif
	
#endif


