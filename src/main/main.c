#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <sys/types.h>      
#include <sys/socket.h>      
#include <netinet/in.h>      
#include <arpa/inet.h>     
#include <string.h>  
#include <fcntl.h>   
  
#include "event2/event.h"  
#include "event2/bufferevent.h" 

#include "libs/debug.h"

#include "demo/demo_libevent_client.h"
#include "demo/demo_libevent_server.h"
#include "demo/demo_libevent_uart.h"


#include "demo/demo_socket_authorize.h"

#include "demo/demo_gb905_heartbeat.h"
#include "demo/demo_gb905_report.h"

#include "demo/demo_fleety_event.h"

int main(int argc, char** argv)
{
	int ret = 0;

	//ret = demo_libevent_client(argc,argv);
	//ret = demo_libevent_server();
	//ret = demo_libevent_uart();
	//ret = demo_authorize_socket();
	//ret = demo_fleety_event();

	//ret = demo_gb905_heartbeat();
	ret = demo_gb905_report();
	
	return ret;
}
