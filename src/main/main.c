#include	"common.h"


#include	"module/itop/authorize/itop_authorize.h"
	
#include	"middleware/uart/fleety_uart.h"
#include	"middleware/event/fleety_event.h"
#include	"middleware/socket/fleety_socket.h"

#include	"main/fleety_report.h"

#define		DEBUG_Y
#include	"libs/debug.h"



#include 	"demo/demo_libevent_client.h"
#include 	"demo/demo_libevent_server.h"
#include 	"demo/demo_libevent_uart.h"

#include 	"demo/demo_socket_authorize.h"

#include 	"demo/demo_gb905_heartbeat.h"
#include 	"demo/demo_gb905_report.h"

#include 	"demo/demo_fleety_event.h"

#include 	"demo/demo_mxml.h"
#include 	"demo/demo_meter.h"
#include 	"demo/demo_toplight.h"

#include 	"demo/demo_gps_nmea.h"


#define		MAX_MAIN_LOOP_TIMEOUT		60

static		pthread_cond_t		main_cond;
static		pthread_mutex_t		main_lock;


void main_process(void)
{
	int ret;
	int exit = 0;
	
	struct timeval now;
	struct timespec outtime;

	DbgFuncEntry();

	platform_hardware_init();

	itop_authorize_loop();
		
	fleety_event_init();
	fleety_socket_init();
	fleety_uart_init();

	fleety_report_init();
	
	pthread_mutex_init(&main_lock, NULL);
	pthread_cond_init(&main_cond, NULL);

	sleep(2);

	while(!exit)
	{
		pthread_mutex_lock(&main_lock);

		gettimeofday(&now, NULL);
		outtime.tv_sec = now.tv_sec + MAX_MAIN_LOOP_TIMEOUT;
		outtime.tv_nsec = now.tv_usec * 1000;

		ret = pthread_cond_timedwait(&main_cond, &main_lock, &outtime);
        pthread_mutex_unlock(&main_lock);

        if(ret == 0)	// ---> condition ok
		{
			DbgPrintf("main cond ok!\r\n");
			sleep(5);
			exit = 1;
			continue;
		}
		else	// ---> timeout
		{
			DbgPrintf("main process timeout!\r\n");
		}

	}

	DbgFuncExit();
}

void exit_main_process(void)
{
	DbgFuncEntry();
	
	pthread_cond_signal(&main_cond);

	DbgFuncExit();
}


int main(int argc, char** argv)
{
	int ret = 0;

	//ret = demo_libevent_client(argc,argv);
	//ret = demo_libevent_server();
	//ret = demo_libevent_uart();
	//ret = demo_authorize_socket();
	//ret = demo_fleety_event();

	ret = demo_gb905_heartbeat();
	//ret = demo_gb905_report();
	//ret = demo_mxml();
	//ret = demo_toplight();
	//ret = demo_meter();

	//ret = demo_gps_nmea();
	//ret = demo_tsm();
	//ret = demo_mcu();

	//main_process();
	
	return ret;
}
