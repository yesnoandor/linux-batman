/****************************************Copyright (c)**************************************************                         
** File name:			fifo.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2016-01-22
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef __FIFO_H
#define __FIFO_H

#ifdef __cplusplus
extern "C" {
#endif


enum{
	FIFO_NORMAL = 0,
	FIFO_FULL = 1,
	FIFO_EMPTY = 2,
};


typedef struct 
{
	unsigned char * buff;
		
	int ptr_in;       
	int ptr_out;

	int size;
	int	len;
	int count;
}fifo_t;   

void fifo_init(fifo_t * fifo_ptr,void *pbuf,int size,int len);

void fifo_in(fifo_t * fifo_ptr, void * data);
void fifo_out(fifo_t * fifo_ptr, void * data);

int	fifo_count(fifo_t * fifo_ptr);
int fifo_status(fifo_t * fifo_ptr);



#ifdef __cplusplus
}
#endif



#endif

