#include"modem.h"
#include "uart.h"
/*
void Modem_Init()
{
    Send_String_UART1("AT\r");
   
    __delay_ms(500);
    
    TimerTemp=2000;
    TimerTimeout=0;
   
    while((ParseBuffer(nizRXbuffer1,"OK")==0)&&(TimerTimeout==0)) ;///CEKA SE ODGOVOR OK
        
    temp=ParseBuffer(nizRXbuffer1,"OK");//VRACA INDEKS(REDNO MESTO) U KOM DELU NIZA SE NALAZI "OK"

    if(temp!=0)
    {  
        state = SMS_FORMAT_DEFINE;
        LATDbits.LATD6=1;
        __delay_ms(2000);
    }
        LATDbits.LATD6=0;
       
    if (TimerTimeout)
    {
        TimerTimeout=0;	
    }
       
    ClearBuffer();
    __delay_ms(1000);

}
 */