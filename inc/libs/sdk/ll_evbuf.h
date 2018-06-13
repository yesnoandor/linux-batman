
/************************************************************************
�߼��û�������, �ο�libevent
************************************************************************/

#ifndef __LL_EVBUF_H__
#define __LL_EVBUF_H__

//These functions deal with buffering input and output 
// �����û�������
struct evbuffer 
{
	unsigned char *buffer;		// ��Ч����ָ��
	unsigned char *orig_buffer;	//������ָ��, ����realloc/free
	
	size_t misalign;		//���������ݵ�ָ��, ָ orig_buffer��buffer�Ŀ���
	size_t totallen;		//���������ܳ���
	size_t off;			// ��Ч���ݵĳ��� 
};

#define EVBUFFER_LENGTH(x)	((x)->off)
#define EVBUFFER_DATA(x)	(unsigned char *)((x)->buffer)
#define EVBUFFER_INPUT(x)	(x)->input
#define EVBUFFER_OUTPUT(x)	(x)->output


struct evbuffer *evbuffer_new(void);
void evbuffer_free(struct evbuffer *buffer);

int evbuffer_add_buffer(struct evbuffer *outbuf, struct evbuffer *inbuf);
int evbuffer_add_printf(struct evbuffer *buf, const char *fmt, ...);
int evbuffer_remove(struct evbuffer *buf, void *data, size_t datlen);
char *evbuffer_readline(struct evbuffer *buffer);
int evbuffer_expand(struct evbuffer *buf, size_t datlen);
int evbuffer_add(struct evbuffer *buf, const void *data, size_t datlen);
void evbuffer_drain(struct evbuffer *buf, size_t len);

int evbuffer_read(struct evbuffer *buf, int fd, int howmuch);
int evbuffer_write(struct evbuffer *buffer, int fd);
int evbuffer_writefile(struct evbuffer *buf, int fd, int datlen);
int evbuffer_readfile(struct evbuffer *buf, int fd, int howmuch);
unsigned char *evbuffer_find(struct evbuffer *buffer, const unsigned char *what, size_t len);


#endif //__LL_EVBUF_H__

