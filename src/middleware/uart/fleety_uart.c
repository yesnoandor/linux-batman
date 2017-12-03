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
		
#include	"module/gb905/gb905_common.h"
//#include	"module/gb905/heart_beat/gb905_heart_beat.h"

#include	"module/gb905_peri/gb905_peri_common.h"
#include	"module/gb905_peri/meter/gb905_meter.h"
#include	"module/gb905_peri/toplight/gb905_toplight.h"
#include	"module/gb905_peri/tsm/gb905_tsm.h"

#include	"module/gb905_ex/mcu/mcu_common.h"

#include	"module/gps/gps_nmea.h"


		
#define		DEBUG_Y
#include	"libs/debug.h"

#define		MAX_UART_NUM			5

typedef	void(*PTR_UART_MSG_CB)(struct bufferevent*,void*);
typedef	void(*PTR_UART_EVENT_CB)(struct bufferevent *, short, void *);

typedef struct {
	struct bufferevent* bev;
	int state;		// =0,closed; =1,opened
}FleetyUartMngr;

typedef struct{
	int bps;
	int databits;
	int stopbits;
	int parity;

	int timeout;
}fleety_uart_params_t;


static char * uart_device[] = 
{
	meter_uart_device,
	toplight_uart_device,
	tsm_uart_device,
	gps_uart_device,
	mcu_uart_device
};


static fleety_uart_params_t termios_params[] =
{
	{
		B9600,8,1,'N',0,			// meter params
	},
	{
		B9600,8,1,'N',0,			// toplight params
	},
	{
		B9600,8,1,'N',0,			// tsm params
	},
	{
		B4800,8,1,'N',0,			// gps params
	},
	{
		B115200,8,1,'N',0,			// stm32 params
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


static PTR_UART_MSG_CB uart_read_cb[] = 
{
	meter_uart_read_cb,
	toplight_uart_read_cb,
	tsm_uart_read_cb,
	gps_uart_read_cb,
	stm32_uart_read_cb,
};

static PTR_UART_EVENT_CB uart_event_cb[] = 
{
	meter_uart_event_cb,
	toplight_uart_event_cb,
	tsm_uart_event_cb,
	gps_uart_event_cb,
	stm32_uart_event_cb
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

	// 设置数据位数
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
	
	// 设置停止位
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
	
	// 设置奇偶校验位
	switch (params->parity)
	{
		// 无校验
		case 'n':
		case 'N':
			options.c_cflag &= ~PARENB;		/* Clear parity enable */
			options.c_iflag &= ~INPCK;		/* Enable parity checking */   
			break;
		
		// 奇校验
		case 'o':
		case 'O':
			options.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/    
			options.c_iflag |= INPCK;             /* Disnable parity checking */   
			break;

		// 偶校验
		case 'e':
		case 'E':
			options.c_cflag |= PARENB;		/* Enable parity */      
			options.c_cflag &= ~PARODD;		/* 转换为偶效验*/       
			options.c_iflag |= INPCK;       /* Disnable parity checking */  
			break; 

		default:     
			DbgError("don't supported parity\r\n");      
			return false;
	}
	
	// 设置波特率
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

	// 设置新属性
	tcflush(fd, TCIFLUSH);				// 溢出数据可以接收，但不读  
	tcsetattr(fd, TCSANOW, &options);	// 设置终端参数生效，TCANOW = 修改立即发生

    return true;  
}

// 接收到服务器网络输入的处理回调函数
static void meter_uart_read_cb(struct bufferevent* bev, void* arg)  
{
	unsigned char msg[1024];
	int len =0,offset=0;
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
	
	offset = gb905_meter_protocol_ayalyze(msg,len);
	evbuffer_drain(input,offset);
	DbgPrintf("offset = %d\r\n",offset);
	
	DbgFuncExit(); 
}


// 接收到服务器网络出错的处理回调函数
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

	// 自动close 套接字和free  读写缓冲区  
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
	
	{
		int i;
		DbgPrintf("len = %d\r\n",len);
		for(i =0;i<len;i++)
		{
			DbgPrintf("msg[%d] = 0x%x\r\n",i,msg[i]);
		}
	}
	
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

	// 自动close 套接字和free  读写缓冲区  
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

	// 自动close 套接字和free  读写缓冲区  
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
	
	{
		int i;
		DbgPrintf("len = %d\r\n",len);
		for(i =0;i<len;i++)
		{
			DbgPrintf("msg[%d] = 0x%x\r\n",i,msg[i]);
		}
	}
	
	offset = gps_nmea_protocol_ayalyze(msg,len);
	
	evbuffer_drain(input,offset);

	DbgPrintf("size = %d\r\n",offset);
	 
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

	// 自动close 套接字和free  读写缓冲区  
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

	// 自动close 套接字和free  读写缓冲区  
	bufferevent_free(bev);

	uart_mngr_list[index].bev = NULL;
	uart_mngr_list[index].state = 0;
	
	DbgFuncExit();
}


//  串口定时器事件回调函数
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

			// 创建一个bufferevent
			// BEV_OPT_CLOSE_ON_FREE : 当 bufferevent 被释放同时关闭底层
			bev = bufferevent_socket_new(base,-1,BEV_OPT_CLOSE_ON_FREE);
			// 设置读取方法和error 时候的方法  
			bufferevent_setcb(bev, uart_read_cb[i], NULL, uart_event_cb[i],(void *)i); 
			// 设置类型  
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

	// 创建并初始化一个event_base  (  支持多线程)
	evthread_use_pthreads();
	base = event_base_new();	
	evthread_make_base_notifiable(base);
	
	// 创建并初始化一个event_base 
	//struct event_base* base = event_base_new();
	DbgPrintf("base = 0x%x\r\n",base);
	
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

		// 创建一个bufferevent
		// BEV_OPT_CLOSE_ON_FREE : 当 bufferevent 被释放同时关闭底层
		bev = bufferevent_socket_new(base,fd,BEV_OPT_CLOSE_ON_FREE);
		// 设置读取方法和error 时候的方法  
		bufferevent_setcb(bev, uart_read_cb[i], NULL, uart_event_cb[i],(void *)i); 
		// 设置类型  
		bufferevent_enable(bev, EV_READ | EV_PERSIST);
	
		uart_mngr_list[i].bev = bev;
		uart_mngr_list[i].state = 1;
	}
	
	struct event *timeout = NULL;
	struct timeval tv = {10, 0};
	
	// 增加定时器事件
	timeout = event_new(base, -1, EV_PERSIST, uart_timeout_cb, base);
	// 激活定时器
	evtimer_add(timeout, &tv);
		
	// 事件循环
	event_base_dispatch(base);
	
	// 取消定时器
	evtimer_del(timeout);
		
	//这将自动close  套接字和free  读写缓冲区  
	bufferevent_free(bev);
	
	//	销毁event_base
	event_base_free(base);
	
	DbgFuncExit();

	return ((void *)0);
}

// ----------
/** 
   * @brief 	初始化串口管理器
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
   * @brief 	通过串口管理器发送数据
   * @param index	串口序号
   * @param buff	发送数据的缓冲地址
   * @param len  	发送数据的长度
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

	bufferevent_write(uart_mngr_list[index].bev,buff,len);

	DbgFuncExit();
}

