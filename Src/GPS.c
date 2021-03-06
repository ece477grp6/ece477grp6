#include "usart.h"
#include "GPS.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define	GPS_USART huart1
uint8_t	GPS_Buffer[512];
extern uint8_t GPS_rcv;

extern uint8_t GPS_message[33];

void GPS_Init(void)
{
//	memset(GPS_message,'0',sizeof(GPS_message));
	strcpy(GPS_message, "4026.87226,N,08652.02631,E,0.134\0");
	HAL_UART_Receive_IT(&GPS_USART,GPS_Buffer,512);
}

void GPS_RxCpltCallback(void)
{
	GPS_rcv = 1;
}

void GPS_Check(void)
{
	if(GPS_rcv){
		char *str;
		str = strstr((char*)GPS_Buffer,"$GPRMC,");
		if(str!=NULL)
		{
			memcpy( GPS_message, &str[19], 33);
			GPS_message[32] = '\0';
			if(!strstr((char*)GPS_message,",N,") && !strstr((char*)GPS_message,",S,")){
				strcpy(GPS_message, "0000.00000,N,00000.00000,E,0.000\0");
			}
		}
		GPS_rcv = 0;
		memset(GPS_Buffer,0,sizeof(GPS_Buffer));
	}
	HAL_UART_Receive_IT(&GPS_USART,GPS_Buffer,512);
}
