#include	"common.h"


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


#include	"main/fleety_system.h"

#define		DEBUG_Y
#include	"libs/debug.h"


int main(int argc, char** argv)
{
	int ret = 0;

	DbgFuncEntry();

	//ret = demo_libevent_client(argc,argv);
	//ret = demo_libevent_server();
	//ret = demo_libevent_uart();
	//ret = demo_authorize_socket();
	//ret = demo_fleety_event();

	//ret = demo_gb905_heartbeat();
	//ret = demo_gb905_report();
	//ret = demo_mxml();
	//ret = demo_toplight();
	//ret = demo_meter();

	//ret = demo_gps_nmea();
	//ret = demo_tsm();
	//ret = demo_mcu();
	//ret = demo_ui();
	//ret = demo_gprs();

	system_process();

	DbgFuncExit();
	
	return ret;
}
