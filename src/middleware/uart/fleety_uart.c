/****************************************Copyright (c)**************************************************                         
** File name:			fleety_uart.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-08
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
		  
#include	"event2/event.h"
#include	"event2/bufferevent.h" 
#include	"event2/buffer.h" 
#include	"event2/util.h"
#include	"event2/thread.h"

#include	"misc/misc.h"
		
#include	"module/gb905/gb905_common.h"
//#include	"module/gb905/heart_beat/gb905_heart_beat.h"
#include	"module/gb905/inspection/gb905_inspection.h"


#include	"module/gb905_peri/gb905_peri_common.h"
#include	"module/gb905_peri/meter/gb905_meter.h"
#include	"module/gb905_peri/toplight/gb905_toplight.h"
#include	"module/gb905_peri/tsm/gb905_tsm.h"

#include	"module/gb905_ex/mcu/mcu_common.h"

#include	"module/gps/gps_nmea.h"
#include	"module/gprs/gprs_at_cmd.h"


#include	"middleware/uart/fleety_uart.h"
		
#define	DEBUG_Y
#include	"libs/debug.h"


typedef	void(*PTR_UART_MSG_CB)(struct bufferevent*,void*);
typedef	void(*PTR_UART_EVENT_CB)(struct bufferevent *, short, void *);

typedef struct {
	struct bufferevent* bev;
	int state;		// =0,closed; =1,opened

	int fd;
	int mode;		// =0,async; =1,sync
}FleetyUartMngr;

typedef struct{
	int bps;
	int databits;
	int stopbits;
	int parity;

	int timeout;
}fleety_uart_params_t;

static char meter_uart_dev_name[32];
static char toplight_uart_dev_name[32];
static char tsm_uart_dev_name[32];
static char gps_uart_dev_name[32];
static char mcu_uart_dev_name[32];
static char gprs_uart_dev_name[32];
static char inspect_uart_dev_name[32];

static char * uart_device[] = 
{
	meter_uart_dev_name,
	toplight_uart_dev_name,
	tsm_uart_dev_name,
	gps_uart_dev_name,
	mcu_uart_dev_name,
	gprs_uart_dev_name,
	inspect_uart_dev_name
};



static fleety_uart_params_t termios_params[] =
{
	{
		METER_UART_DEV_BPS,8,1,'N',0,		// meter params
	},
	{
		TOPLIGHT_UART_DEV_BPS,8,1,'N',0,	// toplight params
	},
	{
		TSM_UART_DEV_BPS,8,1,'N',0,			// tsm params
	},
	{
		GPS_UART_DEV_BPS,8,1,'N',0,			// gps params
	},
	{
		MCU_UART_DEV_BPS,8,1,'N',0,			// stm32 params
	},
	{
		GPRS_UART_DEV_BPS,8,1,'N',0,		// gprs params
	},
	{
		INSPECT_UART_DEV_BPS,8,1,'N',0,		// inspect params
	},
};


static	pthread_t		fleety_uart_thread_id;
static	pthread_cond_t	fleety_uart_cond;
static	pthread_mutex_t fleety_uart_lock;

static FleetyUartMngr	uart_mngr_list[MAX_UART_NUM];

static void meter_uart_read_cb(struct bufferevent* bev, void* arg);
static void meter_uart_event_cb(struct bufferevent *bev, short event, void *arg);
static void toplight_uart_read_cb(struct bufferevent* bev, void* arg);
static void toplight_uart_event_cb(struct bufferevent *bev, short event, void *arg);
static void tsm_uart_read_cb(struct bufferevent* bev, void* arg);
static void tsm_uart_event_cb(struct bufferevent *bev, short event, void *arg);
static void gps_uart_read_cb(struct bufferevent* bev, void* arg);
static void gps_uart_event_cb(struct bufferevent *bev, short event, void *arg);
static void stm32_uart_read_cb(struct bufferevent* bev, void* arg);
static void stm32_uart_event_cb(struct bufferevent *bev, short event, void *arg);
static void gprs_uart_read_cb(struct bufferevent* bev, void* arg);
static void gprs_uart_event_cb(struct bufferevent *bev, short event, void *arg);
static void inspect_uart_read_cb(struct bufferevent* bev, void* arg);
static void inspect_uart_event_cb(struct bufferevent *bev, short event, void *arg);


static PTR_UART_MSG_CB uart_read_cb[] = 
{
	meter_uart_read_cb,
	toplight_uart_read_cb,
	tsm_uart_read_cb,
	gps_uart_read_cb,
	stm32_uart_read_cb,
	gprs_uart_read_cb,
	inspect_uart_read_cb,
};

static PTR_UART_EVENT_CB uart_event_cb[] = 
{
	meter_uart_event_cb,
	toplight_uart_event_cb,
	tsm_uart_event_cb,
	gps_uart_event_cb,
	stm32_uart_event_cb,
	gprs_uart_event_cb,
	inspect_uart_event_cb,
};


// ----------
static bool uart_set_params(int fd,fleety_uart_params_t * params)
{
	struct termios options;
	
	if(tcgetattr(fd,&options) != 0)
	{
		DbgError("get termios failed!\r\n");
		return false;
	}
	
	// defalut : 
	options.c_cflag = 0;
	options.c_lflag = 0;
	options.c_iflag = 0;
	options.c_oflag = 0;

	// ��������λ��
	options.c_cflag &= ~CSIZE;
	switch (params->databits)
	{
		case 7:
			options.c_cflag |= CS7;
			break;

		case 8:
			options.c_cflag |= CS8;
			break;

		default:      
			DbgError("don't supported data bits\r\n");
			return false;
	}
	
	// ����ֹͣλ
	switch (params->stopbits)
	{
		case 1:
			options.c_cflag &= ~CSTOPB;    
			break;
			
		case 2:      
			options.c_cflag |= CSTOPB;    
			break;
			
		default:
			DbgError("don't supported stop bits\r\n");
			return false;
	}
	
	// ������żУ��λ
	switch (params->parity)
	{
		// ��У��
		case 'n':
		case 'N':
			options.c_cflag &= ~PARENB;		/* Clear parity enable */
			options.c_iflag &= ~INPCK;		/* Enable parity checking */   
			break;
		
		// ��У��
		case 'o':
		case 'O':
			options.c_cflag |= (PARODD | PARENB); /* ����Ϊ��Ч��*/    
			options.c_iflag |= INPCK;             /* Disnable parity checking */   
			break;

		// żУ��
		case 'e':
		case 'E':
			options.c_cflag |= PARENB;		/* Enable parity */      
			options.c_cflag &= ~PARODD;		/* ת��ΪżЧ��*/       
			options.c_iflag |= INPCK;       /* Disnable parity checking */  
			break; 

		default:     
			DbgError("don't supported parity\r\n");      
			return false;
	}
	
	// ���ò�����
	cfsetospeed(&options,params->bps);	
	cfsetispeed(&options,params->bps);

	// Set the timeout options
    options.c_cc[VMIN]  = 0;        		// read blocked except 1bytes data ready
    options.c_cc[VTIME] = params->timeout;	// interval between two chars (unit : 100ms)
    options.c_cc[VINTR] = 0;
    options.c_cc[VQUIT] = 0;
    options.c_cc[VSTART] = 0;
    options.c_cc[VSTOP] = 0;
    options.c_cc[VSUSP] = 0;

	// ����������
	tcflush(fd, TCIFLUSH);				// ������ݿ��Խ��գ�������  
	tcsetattr(fd, TCSANOW, &options);	// �����ն˲�����Ч��TCANOW = �޸���������

    return true;  
}

// ���յ���������������Ĵ���ص�����
static void meter_uart_read_cb(struct bufferevent* bev, void* arg)  
{
	long index;
	unsigned char msg[1024];
	int len =0,offset=0;
	struct evbuffer *input;

	DbgFuncEntry();

	index = (long)arg;
	DbgPrintf("index = %d\r\n",index);

	// async mode
	if(uart_mngr_list[index].mode == 0)
	{

		//len = bufferevent_read(bev, msg, sizeof(msg));

		input = bufferevent_get_input(bev);
		len = evbuffer_copyout(input, msg, sizeof(msg));

		#if 0
		{
			int i;
			DbgPrintf("len = %d\r\n",len);
			for(i =0;i<len;i++)
			{
				DbgPrintf("msg[%d] = 0x%x\r\n",i,msg[i]);
			}
		}
		#endif
		
		offset = gb905_meter_protocol_ayalyze(msg,len);
		evbuffer_drain(input,offset);
		DbgPrintf("offset = %d\r\n",offset);
	}
	
	DbgFuncExit(); 
}


// ���յ��������������Ĵ���ص�����
static void meter_uart_event_cb(struct bufferevent *bev, short event, void *arg)  
{
	long index;
	
	DbgFuncEntry();

	index = (long)arg;

	DbgPrintf("index = %d\r\n",index);
	
	if (event & BEV_EVENT_EOF)
		DbgPrintf("meter uart closed!\r\n");
	else if (event & BEV_EVENT_ERROR)
		DbgPrintf("meter uart some other error!\n");

	// �Զ�close �׽��ֺ�free  ��д������  
	bufferevent_free(bev);

	uart_mngr_list[index].bev = NULL;
	uart_mngr_list[index].state = 0;
	
	DbgFuncExit();
}  

static void toplight_uart_read_cb(struct bufferevent* bev, void* arg)  
{
	unsigned char msg[1024];
	int len = 0;
	int offset = 0;
	struct evbuffer *input;

	DbgFuncEntry();

	//len = bufferevent_read(bev, msg, sizeof(msg));
	input = bufferevent_get_input(bev);
	len = evbuffer_copyout(input, msg, sizeof(msg));

	#if 0
	{
		int i;
		DbgPrintf("len = %d\r\n",len);
		for(i =0;i<len;i++)
		{
			DbgPrintf("msg[%d] = 0x%x\r\n",i,msg[i]);
		}
	}
	#endif
	
	offset = gb905_toplight_protocol_ayalyze(msg,len);
	
	evbuffer_drain(input,offset);

	DbgPrintf("size = %d\r\n",offset);
	 
	DbgFuncExit();
}

static void toplight_uart_event_cb(struct bufferevent *bev, short event, void *arg)  
{
	long index;
	
	DbgFuncEntry();

	index = (long)arg;

	DbgPrintf("index = %d\r\n",index);
	
	if (event & BEV_EVENT_EOF)
		DbgPrintf("toplight uart closed!\r\n");
	else if (event & BEV_EVENT_ERROR)
		DbgPrintf("toplight uart some other error\n");

	// �Զ�close �׽��ֺ�free  ��д������  
	bufferevent_free(bev);

	uart_mngr_list[index].bev = NULL;
	uart_mngr_list[index].state = 0;
	
	DbgFuncExit();
}


static void gps_uart_event_cb(struct bufferevent *bev, short event, void *arg)  
{
	long index;
	
	DbgFuncEntry();

	index = (long)arg;

	DbgPrintf("index = %d\r\n",index);
	
	if (event & BEV_EVENT_EOF)
		DbgPrintf("toplight uart closed!\r\n");
	else if (event & BEV_EVENT_ERROR)
		DbgPrintf("toplight uart some other error\n");

	// �Զ�close �׽��ֺ�free  ��д������  
	bufferevent_free(bev);

	uart_mngr_list[index].bev = NULL;
	uart_mngr_list[index].state = 0;
	
	DbgFuncExit();
}

static void gps_uart_read_cb(struct bufferevent* bev, void* arg)  
{
	unsigned char msg[1024];
	int len = 0;
	int offset = 0;
	struct evbuffer *input;

	DbgFuncEntry();

	//len = bufferevent_read(bev, msg, sizeof(msg));
	input = bufferevent_get_input(bev);
	len = evbuffer_copyout(input, msg, sizeof(msg));

	#if 0
	{
		int i;
		DbgPrintf("len = %d\r\n",len);
		for(i =0;i<len;i++)
		{
			DbgPrintf("msg[%d] = 0x%x\r\n",i,msg[i]);
		}
	}
	#endif
	
	offset = gps_nmea_protocol_ayalyze(msg,len);
	
	evbuffer_drain(input,offset);

	DbgPrintf("offset = %d\r\n",offset);
	 
	DbgFuncExit();
}

static void tsm_uart_read_cb(struct bufferevent* bev, void* arg)  
{
	unsigned char msg[1024];
	int len = 0;
	int offset = 0;
	struct evbuffer *input;

	DbgFuncEntry();

	//len = bufferevent_read(bev, msg, sizeof(msg));
	input = bufferevent_get_input(bev);
	len = evbuffer_copyout(input, msg, sizeof(msg));

	#if 0
	{
		int i;
		DbgPrintf("len = %d\r\n",len);
		for(i =0;i<len;i++)
		{
			DbgPrintf("msg[%d] = 0x%x\r\n",i,msg[i]);
		}
	}
	#endif
	
	offset = gb905_tsm_protocol_ayalyze(msg,len);
	
	evbuffer_drain(input,offset);

	DbgPrintf("size = %d\r\n",offset);
	 
	DbgFuncExit();
}

static void tsm_uart_event_cb(struct bufferevent *bev, short event, void *arg)  
{
	long index;
	
	DbgFuncEntry();

	index = (long)arg;

	DbgPrintf("index = %d\r\n",index);
	
	if (event & BEV_EVENT_EOF)
		DbgPrintf("tsm uart closed!\r\n");
	else if (event & BEV_EVENT_ERROR)
		DbgPrintf("tsm uart some other error\n");

	// �Զ�close �׽��ֺ�free  ��д������  
	bufferevent_free(bev);

	uart_mngr_list[index].bev = NULL;
	uart_mngr_list[index].state = 0;
	
	DbgFuncExit();
}

static void stm32_uart_read_cb(struct bufferevent* bev, void* arg)  
{
	unsigned char msg[1024];
	int len = 0;
	int offset = 0;
	struct evbuffer *input;

	DbgFuncEntry();

	//len = bufferevent_read(bev, msg, sizeof(msg));
	input = bufferevent_get_input(bev);
	len = evbuffer_copyout(input, msg, sizeof(msg));
	
	{
		int i;
		DbgPrintf("len = %d\r\n",len);
		for(i =0;i<len;i++)
		{
			DbgPrintf("msg[%d] = 0x%x\r\n",i,msg[i]);
		}
	}
	
	offset = mcu_protocol_ayalyze(msg,len);
	
	evbuffer_drain(input,offset);

	DbgPrintf("size = %d\r\n",offset);
	 
	DbgFuncExit();
}

static void stm32_uart_event_cb(struct bufferevent *bev, short event, void *arg)  
{
	long index;
	
	DbgFuncEntry();

	index = (long)arg;

	DbgPrintf("index = %d\r\n",index);
	
	if (event & BEV_EVENT_EOF)
		DbgPrintf("mcu uart closed!\r\n");
	else if (event & BEV_EVENT_ERROR)
		DbgPrintf("mcu uart some other error\n");

	// �Զ�close �׽��ֺ�free  ��д������  
	bufferevent_free(bev);

	uart_mngr_list[index].bev = NULL;
	uart_mngr_list[index].state = 0;
	
	DbgFuncExit();
}

static void gprs_uart_read_cb(struct bufferevent* bev, void* arg)  
{
	unsigned char msg[1024];
	int len = 0;
	int offset = 0;
	struct evbuffer *input;

	DbgFuncEntry();

	//len = bufferevent_read(bev, msg, sizeof(msg));
	input = bufferevent_get_input(bev);
	len = evbuffer_copyout(input, msg, sizeof(msg));

	#if 1
	{
		int i;
		DbgPrintf("len = %d\r\n",len);
		for(i =0;i<len;i++)
		{
			DbgPrintf("msg[%d] = 0x%x\r\n",i,msg[i]);
		}
	}
	#endif
	
	offset = gprs_at_protocol_ayalyze(msg,len);
	
	evbuffer_drain(input,offset);

	DbgPrintf("size = %d\r\n",offset);
	 
	DbgFuncExit();
}

static void gprs_uart_event_cb(struct bufferevent *bev, short event, void *arg)  
{
	long index;
	
	DbgFuncEntry();

	index = (long)arg;

	DbgPrintf("index = %d\r\n",index);
	
	if (event & BEV_EVENT_EOF)
		DbgPrintf("gprs uart closed!\r\n");
	else if (event & BEV_EVENT_ERROR)
		DbgPrintf("gprs uart some other error\n");

	// �Զ�close �׽��ֺ�free  ��д������  
	bufferevent_free(bev);

	uart_mngr_list[index].bev = NULL;
	uart_mngr_list[index].state = 0;
	
	DbgFuncExit();
}

static void inspect_uart_read_cb(struct bufferevent* bev, void* arg)  
{
	unsigned char msg[1024];
	int len = 0;
	//int offset = 0;
	struct evbuffer *input;

	DbgFuncEntry();

	//len = bufferevent_read(bev, msg, sizeof(msg));
	input = bufferevent_get_input(bev);
	len = evbuffer_copyout(input, msg, sizeof(msg));

	#if 1
	{
		int i;
		DbgPrintf("len = %d\r\n",len);
		for(i =0;i<len;i++)
		{
			DbgPrintf("msg[%d] = 0x%x\r\n",i,msg[i]);
		}
	}
	#endif
	
	gb905_inspection_device_treat(msg,len);
	
	evbuffer_drain(input,len);

	//DbgPrintf("size = %d\r\n",offset);
	 
	DbgFuncExit();
}

static void inspect_uart_event_cb(struct bufferevent *bev, short event, void *arg)  
{
	long index;
	
	DbgFuncEntry();

	index = (long)arg;

	DbgPrintf("index = %d\r\n",index);
	
	if (event & BEV_EVENT_EOF)
		DbgPrintf("inspect uart closed!\r\n");
	else if (event & BEV_EVENT_ERROR)
		DbgPrintf("inspect uart some other error\n");

	// �Զ�close �׽��ֺ�free  ��д������  
	bufferevent_free(bev);

	uart_mngr_list[index].bev = NULL;
	uart_mngr_list[index].state = 0;
	
	DbgFuncExit();
}


//  ���ڶ�ʱ���¼��ص�����
static void uart_timeout_cb(int fd, short event, void * pArg)
{
	DbgFuncEntry();

	long i;
	struct bufferevent* bev;
	struct event_base* base = (struct event_base*)pArg;

	DbgPrintf("base = 0x%x\r\n",base);

	for(i=0;i<MAX_UART_NUM;i++)
	{
		if(strcmp(uart_device[i],"") == 0)
		{
			continue;
		}
		
		if(0 == uart_mngr_list[i].state)
		{
			DbgPrintf("re-open uart %s\r\n",uart_device[i]);
			
			int fd = open(uart_device[i], O_RDWR);
			if(fd < 0)
			{
				DbgError("open device:%s error(err = %s)\r\n",uart_device[i],strerror(errno));
				continue;
			}
			DbgGood("open %s successful\r\n",uart_device[i]);
			uart_set_params(fd,&termios_params[i]);

			// ����һ��bufferevent
			// BEV_OPT_CLOSE_ON_FREE : �� bufferevent ���ͷ�ͬʱ�رյײ�
			bev = bufferevent_socket_new(base,-1,BEV_OPT_CLOSE_ON_FREE);
			// ���ö�ȡ������error ʱ��ķ���  
			bufferevent_setcb(bev, uart_read_cb[i], NULL, uart_event_cb[i],(void *)i); 
			// ��������  
			bufferevent_enable(bev, EV_READ | EV_PERSIST);
			
		}
		else
		{
			//gb905_heart_beat_send();
		}
			
	}
	
	DbgFuncExit();
}

static void * fleety_uart_loop_func(void *arg)
{
	long i;
	struct bufferevent* bev;
	struct event_base* base;
		
	DbgFuncEntry();

	// ��������ʼ��һ��event_base  (  ֧�ֶ��߳�)
	evthread_use_pthreads();
	base = event_base_new();	
	evthread_make_base_notifiable(base);
	
	// ��������ʼ��һ��event_base 
	//struct event_base* base = event_base_new();
	//DbgPrintf("base = 0x%x\r\n",base);
	
	for(i=0;i<MAX_UART_NUM;i++)
	{
		if(strcmp(uart_device[i],"") == 0)
		{
			continue;
		}

		int fd = open(uart_device[i], O_RDWR);
		if(fd < 0)
		{
			DbgError("open device:%s error(err = %s)\r\n",uart_device[i],strerror(errno));
		
			continue;
		}
		DbgGood("open %s successful\r\n",uart_device[i]);
		uart_set_params(fd,&termios_params[i]);

		// ����һ��bufferevent
		// BEV_OPT_CLOSE_ON_FREE : �� bufferevent ���ͷ�ͬʱ�رյײ�
		bev = bufferevent_socket_new(base,fd,BEV_OPT_CLOSE_ON_FREE);
		// ���ö�ȡ������error ʱ��ķ���  
		bufferevent_setcb(bev, uart_read_cb[i], NULL, uart_event_cb[i],(void *)i); 
		// ��������  
		bufferevent_enable(bev, EV_READ | EV_PERSIST);
	
		uart_mngr_list[i].bev = bev;
		uart_mngr_list[i].state = 1;
		uart_mngr_list[i].fd = fd;
		uart_mngr_list[i].mode = 0;
	}
	
	struct event *timeout = NULL;
	struct timeval tv = {10, 0};
	
	// ���Ӷ�ʱ���¼�
	timeout = event_new(base, -1, EV_PERSIST, uart_timeout_cb, base);
	// ���ʱ��
	evtimer_add(timeout, &tv);
		
	// �¼�ѭ��
	event_base_dispatch(base);
	
	// ȡ����ʱ��
	evtimer_del(timeout);
		
	//�⽫�Զ�close  �׽��ֺ�free  ��д������  
	bufferevent_free(bev);
	
	//	����event_base
	event_base_free(base);
	
	DbgFuncExit();

	return ((void *)0);
}

// ----------
/** 
   * @brief 	��ʼ�����ڹ�����
   *
   */
void fleety_uart_init(void)
{
	int ret;

	DbgFuncEntry();
	
	pthread_mutex_init(&fleety_uart_lock, NULL);
	pthread_cond_init(&fleety_uart_cond, NULL);

	ret = pthread_create(&fleety_uart_thread_id, NULL, fleety_uart_loop_func, NULL);
	if(ret != 0){
		DbgError("can't create fleety socket thread(error = %s)\r\n",strerror(ret));
	}

	DbgFuncExit();
}

/** 
   * @brief 	ͨ�����ڹ�������������
   * @param index	�������
   * @param buff	�������ݵĻ����ַ
   * @param len  	�������ݵĳ���
   *
   */
void fleety_uart_send(int index,unsigned char *buff,int len)
{
	DbgFuncEntry();

	DbgPrintf("index = %d\r\n",index);
	
	{
		int i;
		DbgPrintf("len = %d\r\n",len);
		for(i =0;i<len;i++)
		{
			DbgPrintf("buff[%d] = 0x%x\r\n",i,buff[i]);
		}
	}

	if(uart_mngr_list[index].mode == 0)
	{
		bufferevent_write(uart_mngr_list[index].bev,buff,len);
	}
	
	DbgFuncExit();
}

int fleety_uart_sync_init(int index)
{
	int fd = 0;

	if(strcmp(uart_device[index],"") == 0)
	{
		return fd;
	}

	fd = open(uart_device[index], O_RDWR);
	if(fd < 0)
	{
		DbgError("open device:%s error(err = %s)\r\n",uart_device[index],strerror(errno));
		return fd;
	}
		
	uart_set_params(fd,&termios_params[index]);

	return fd;
}

int fleety_uart_set_mode(int index,int mode)
{
	unsigned char msg[1024];

	// async --> sync
	if(uart_mngr_list[index].mode == 0 && mode == 1)
	{
		uart_mngr_list[index].mode = mode;
		bufferevent_read(uart_mngr_list[index].bev,msg,sizeof(msg));
	}
	else
	{
		uart_mngr_list[index].mode = mode;
	}
	
	return uart_mngr_list[index].fd;
}

void fleety_modify_uart_device(unsigned char index,char * uart_name,int uart_bps)
{
	DbgFuncEntry();

	if(index < ARRAY_SIZE(uart_device))
	{
		strcpy(uart_device[index],uart_name);
		termios_params[index].bps = uart_bps;
	}
	else
	{
		DbgError("error uart index(%d)\r\n",index);
	}

	debug_uart_params();
	
	DbgFuncExit();
}

