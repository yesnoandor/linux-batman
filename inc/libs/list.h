/****************************************Copyright (c)**************************************************                         
** File name:			list.h
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

#ifndef __LIST_H
#define __LIST_H

#ifdef __cplusplus
extern "C" {
#endif


typedef  void * elem_t;  
  
  
typedef struct node
{
	elem_t data;
	struct node * next;
}node_t;

typedef struct  
{
	node_t *head;
	node_t *tail;

	int node_size;
	int node_count;
}list_t;  


list_t *list_create(int size);
void list_destory(list_t * list);
bool list_lamda(list_t * list,bool (*f)(elem_t));

bool node_append(list_t * list,void * elem);
bool node_insert(list_t * list, int n ,void * elem);
bool node_delete(list_t * list,int n);
void node_clear(list_t * list);

int elem_get(list_t * list,int n,elem_t *data);
int elem_set(list_t * list,int n,elem_t *data);

#ifdef __cplusplus
}
#endif



#endif

