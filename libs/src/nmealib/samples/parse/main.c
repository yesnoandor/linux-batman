#include <nmea/nmea.h>
#include <string.h>
#include <stdio.h>

int main()
{
    const char *buff[] = {
		"$GPRMC,101227.000,V,,,,,,,281016,,,N*58\r\n",    //GNRMC
		"$GPGGA,101328.000,,,,,0,00,25.5,,,,,,*73\r\n",    //GNGGA
		
		"$GPGSV,3,3,12,24,69,047,32,253,3,12,24,69,046,32,25,08,173,,31,00,219,27,32,17,278,*7D\r\n",
		"$GPGSA,A,1,,,,,,,,,,,,,25.5,25.5,25.5*02\r\n",
		
		"$GPVTG,,,,,,,,,N*2E\r\n",   //GNVTG

        "$GPRMC,173843,A,3349.896,N,11808.521,W,000.0,360.0,230108,013.4,E*69\r\n",
        "$GPGGA,111609.14,5001.27,N,3613.06,E,3,08,0.0,10.2,M,0.0,M,0.0,0000*70\r\n",
        "$GPGSV,2,1,08,01,05,005,80,02,05,050,80,03,05,095,80,04,05,140,80*7f\r\n",
        "$GPGSV,2,2,08,05,05,185,80,06,05,230,80,07,05,275,80,08,05,320,80*71\r\n",
        "$GPGSA,A,3,01,02,03,04,05,06,07,08,00,00,00,00,0.0,0.0,0.0*3a\r\n",
        "$GPRMC,111609.14,A,5001.27,N,3613.06,E,11.2,0.0,261206,0.0,E*50\r\n",
        "$GPVTG,217.5,T,208.8,M,000.00,N,000.01,K*4C\r\n"
    };

    int it;
    nmeaINFO info;
    nmeaPARSER parser;

    nmea_zero_INFO(&info);
    nmea_parser_init(&parser);

    for(it = 0; it < 5; ++it)
        nmea_parse(&parser, buff[it], (int)strlen(buff[it]), &info);

	printf("---- info->utc.year = %d \r\n",info.utc.year);
	printf("---- info->utc.mon = %d \r\n",info.utc.mon);
	printf("---- info->utc.day = %d \r\n",info.utc.day);
	printf("---- info->utc.hour = %d \r\n",info.utc.hour);
	printf("---- info->utc.min = %d \r\n",info.utc.min);
	printf("---- info->utc.sec = %d \r\n",info.utc.sec);
	printf("---- info->utc.hsec = %d \r\n",info.utc.hsec);
	printf("---- info->smask = %d \r\n",info.smask);
	printf("---- info->sig = %d \r\n",info.sig);
	printf("---- info->fix = %d \r\n",info.fix);
	printf("---- info->PDOP = %lf \r\n",info.PDOP);
	printf("---- info->HDOP = %lf \r\n",info.HDOP);
	printf("---- info->VDOP = %lf \r\n",info.VDOP);
	printf("---- info->lat = %lf\r\n",info.lat); 
	printf("---- info->lon = %lf \r\n",info.lon); 
	printf("---- info->elv = %lf \r\n",info.elv);
	printf("---- info->speed = %lf \r\n",info.speed);
	printf("---- info->direction = %lf\r\n",info.direction);
	printf("---- info->declination = %lf \r\n",info.declination);
	printf("---- info->satinfo.inuse = %d \r\n",info.satinfo.inuse);
	printf("---- info->satinfo.inview = %d \r\n",info.satinfo.inview);
	
    nmea_parser_destroy(&parser);

    return 0;
}
