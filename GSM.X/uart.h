#ifndef UART_H
#define	UART_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

#include"clock.h"
#include<uart.h>

#define BaudRateU1 9600
#define RXbuffLen1 100

#define CONNECTION_CHECK 0
#define SMS_FORMAT_DEFINE 1
#define SMS_LIST 2
#define SMS_READ 3
#define SMS_SEND 4
#define SMS_DELETE 5

//-----------------UART1-------------------------
extern char nizRXbuffer1[RXbuffLen1];
//-----------------TEMPSENOR---------------------
extern char temp_string[6];
extern char temp_string_avg[6];
//-----------------TIMER1------------------------
extern unsigned int TimerTemp;
extern unsigned char TimerTimeout;
//-----------------MODEM-------------------------
extern unsigned int state;
//-----------------------------------------------
void UART1_Init(void);
void Send_One_Char(unsigned char data);
void Send_String_UART1(char *str);

void ClearBuffer(void);
unsigned char ParseBuffer(char * bufferWhere,const char * bufferWhat);

void BACK_TO_CHECK_CONNECTION_STATE(void);

void MODEM_CHECK_CONNECTION(void);
void MODEM_SMS_FORMAT_DEFINE(void);
void MODEM_SMS_LIST(void);
void MODEM_SMS_READ(void);
void MODEM_SMS_SEND(void);
void MODEM_SMS_DELETE(void);


