//Code by: Stefan Svenderman & Jovan Slavujevic
//date: 19.3.2020.

#include"temp.h"
#include"clock.h"
#include"uart.h"
#include"timer1.h"

_CONFIG2(FNOSC_FRCPLL); //fixed value of PLL4 with internal takt signal 8MHz*PLL4/2 = 16MHz
_CONFIG1(FWDTEN_OFF);
//-----------------------------------------------
int tempRX;
int i=0,get_values=0;
int counter=0;
float temp_avg=0;
//-----------------------------------------------

void __attribute__((interrupt,no_auto_psv)) _U1RXInterrupt(void)
{
    IFS0bits.U1RXIF = 0;         
    tempRX = U1RXREG;
    
    for (i=0;i<RXbuffLen1-1;i++)
    {
        nizRXbuffer1[i]=nizRXbuffer1[i+1];
    }
    nizRXbuffer1[RXbuffLen1-1]=tempRX;
}

void __attribute__((__interrupt__,no_auto_psv)) _T1Interrupt(void)
{
    TMR1 =0;
    IFS0bits.T1IF = 0;
    
    counter++;
     
    if(TimerTemp)
    {
        TimerTemp--;
    }
    
    if (TimerTemp==0)
    {
      TimerTimeout=1;
    }
    
    if(counter==5000)
    {
        get_values=1;
        counter=0;
    }
      
}

int main(void)
{
    PIN_Init();
    MCUclockInit();
    UART1_Init();
    TMR1_init();
    ClearBuffer();
   
    state=CONNECTION_CHECK;
    
    while(1)
    {
        
        if(get_values)
        {
            get_values=0;
            
            temp_avg=(temp_avg+temp_float)/2;
            ftoa(temp_avg,temp_string_avg,1);
        }
        
        if(!temp_avg)
        {
            temp_avg=calculate_temp();
        }
        
        temp_float=calculate_temp();    
        ftoa(temp_float,temp_string,1);        
      
        switch(state)
        {
        
            case CONNECTION_CHECK:
                
                MODEM_CHECK_CONNECTION();
            
            break;
            
            case SMS_FORMAT_DEFINE:
                
                MODEM_SMS_FORMAT_DEFINE();
                
            break;
            
            case SMS_LIST:
            
                MODEM_SMS_LIST();
             
            break;
            
            case SMS_READ:
                
                MODEM_SMS_READ();
                
            break;
            
            case SMS_SEND:
                
                MODEM_SMS_SEND();
            
            break;
            
            case SMS_DELETE:
                
                MODEM_SMS_DELETE();
                
            break;
            
            default:
                    
                state=CONNECTION_CHECK;
        }          
    }
    return 0;
}