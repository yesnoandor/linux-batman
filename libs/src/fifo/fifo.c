/****************************************Copyright (c)**************************************************                         
** File name:			fifo.c
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
#include	<stdlib.h>
#include	<string.h>

#include	"fifo.h"

void fifo_init(fifo_t * fifo_ptr,void *pbuf,int size,int len)
{
   
	fifo_ptr->buff = (unsigned char *)pbuf;
	fifo_ptr->ptr_in = fifo_ptr->ptr_out = 0;
	
	fifo_ptr->size = size;
	fifo_ptr->len = len;
	fifo_ptr->count = 0;
}

void fifo_in(fifo_t * fifo_ptr, void * data)
{
	//fifo_ptr->buff[fifo_ptr->ptr_in] = data;
	memcpy(&fifo_ptr->buff[fifo_ptr->ptr_in],data,fifo_ptr->size);
	
	fifo_ptr->ptr_in = (fifo_ptr->ptr_in + fifo_ptr->size) % (fifo_ptr->size * fifo_ptr->len);

	fifo_ptr->count++;
}


void fifo_out(fifo_t * fifo_ptr, void * data)
{
	//* data_ptr = fifo_ptr->buff[fifo_ptr->ptr_out];
	memcpy(data,&fifo_ptr->buff[fifo_ptr->ptr_out],fifo_ptr->size);
	
	fifo_ptr->ptr_out = (fifo_ptr->ptr_out + fifo_ptr->size) % (fifo_ptr->size * fifo_ptr->len);

	fifo_ptr->count--;
}

int	fifo_count(fifo_t * fifo_ptr)
{
	//return (fifo_ptr->size + fifo_ptr->ptr_in - fifo_ptr->ptr_out) % fifo_ptr->size;
	return fifo_ptr->count;
}

int fifo_status(fifo_t * fifo_ptr)
{
	int ret;
	int count;
	
	count = fifo_count(fifo_ptr);

	if(count == fifo_ptr->size - 1)
		ret = FIFO_FULL;
	else if(count == 0)
		ret = FIFO_EMPTY;
	else
		ret = FIFO_NORMAL;

	return ret;
}



