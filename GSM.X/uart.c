#include"uart.h"

unsigned int temp;
char message_index;
unsigned int temp_phone;
unsigned char number;
unsigned char phone_number[16];
//-----------------UART1-------------------------
char nizRXbuffer1[RXbuffLen1];
//-----------------TEMPSENOR---------------------
char temp_string[6];
char temp_string_avg[6];
//-----------------TIMER1------------------------
unsigned int TimerTemp;
unsigned char TimerTimeout;
//-----------------MODEM-------------------------
unsigned int state;
//-----------------------------------------------
void UART1_Init(void)
{  
    U1MODEbits.UARTEN=1;    //bit15 enable uart na kraju funkcije
    U1MODEbits.USIDL=0;     //bit13 continious idle mode
    U1MODEbits.IREN=0;      //bit12 IR encoder and decoder disabled
    U1MODEbits.RTSMD=1;     //bit11 UxRTS in simplex mode
    U1MODEbits.UEN1=0;      //bit9 and bit8 RX and TX enable, UxCTS and UxRTS disable
    U1MODEbits.UEN0=0;      //bit8
    U1MODEbits.WAKE=0;      //bit7 no wake up in sleep mode
    U1MODEbits.LPBACK=0;    //bit6 loopback bit disabled
    U1MODEbits.ABAUD=0;     //bit5 autobaud disable
    U1MODEbits.RXINV=0;     //bit4 =0,idle state '1'
    U1MODEbits.BRGH=0;      //bit3 high baud rate,standard mode=0
    U1MODEbits.PDSEL1=0;    //bit2 parity and data selection
    U1MODEbits.PDSEL0=0;    //bit1 =00 8bit data no parity
    U1MODEbits.STSEL=0;     //bit0 =0 one stop bit
    
    U1BRG=(FCY/((double)16 * BaudRateU1))-1;   //9600 na 16MHz
    
    U1STAbits.UTXISEL1=0;   //bit15 interrupt when char is transfered
    U1STAbits.UTXISEL0=0;   //bit13
    U1STAbits.UTXINV=0;     //bit14 no ir encoder transmit polarity inversion
    U1STAbits.UTXBRK=0;     //bit11 no transmit break bit
    U1STAbits.UTXEN=1;      //bit10 TX pin controled by peripheral
    U1STAbits.UTXBF=0;      //bit9 transmit buffer is not full,at least one more characte can be written
    U1STAbits.TRMT=0;       //bit8 transmit register is not empty,transmission is in progress
    U1STAbits.URXISEL1=0;   //bit7 receive interrupt mode selection bits
    U1STAbits.URXISEL0=0;   //bit6
    U1STAbits.ADDEN=0;      //bit5 address detect mode disabled
    U1STAbits.RIDLE=0;      //bit4 receiver is active idle bit read only
    U1STAbits.PERR=0;       //bit3 parity error status bit has not been detected
    U1STAbits.FERR=0;       //bit2 framing error has not been detected
    U1STAbits.OERR=0;       //bit1 receive buffer has not overfloved
    U1STAbits.URXDA=0;      //bit0 receive buffer is empty
    
    IPC3bits.U1TXIP=0;
	IPC2bits.U1RXIP=1;
	IFS0bits.U1TXIF = 0;    // clear TX interrupt flag
    IFS0bits.U1RXIF=0;
    IEC0bits.U1TXIE = 0;    // Enable UART Tx interrupt
	IEC0bits.U1RXIE=1;      //interapt rx enable
    
    __builtin_write_OSCCONL(OSCCON & 0xBF);
           
      RPINR18bits.U1RXR=4;//PIN 43=RD9=RP4 RX za zelenu UART1
      RPOR1bits.RP3R=3;//PIN 44=RD10=RP3 TX za zelenu UART1
  
    __builtin_write_OSCCONL(OSCCON | 0x40); 

//-----------UART1------------------------        
      TRISDbits.TRISD9=1; //RX UART
      TRISDbits.TRISD10=0; //TX UART     
}

void Send_One_Char(unsigned char data)
{
    while (!U1STAbits.TRMT);
    U1TXREG=data;
}

void Send_String_UART1(char *str)
{
    while(*str)
    {
        Send_One_Char(*str);
		str++;
    }
}

void ClearBuffer(void)
{
    int j;
    
    for (j=0; j < RXbuffLen1; j++)
	{
        nizRXbuffer1[j]='0';
	}
}

unsigned char ParseBuffer(char * bufferWhere,const char * bufferWhat)
{
       unsigned int res1;
       char *ptr1;
       ptr1 = strstr(bufferWhere, bufferWhat);
    
       if (ptr1 != NULL) 
       {
            res1 = ptr1 - bufferWhere + 1;
       }      
       else 
       {
            res1 = 0;
       }      
       return res1;
}

void BACK_TO_CHECK_CONNECTION_STATE(void)
{
        if (TimerTimeout)
        {
            TimerTimeout=0;
            state=CONNECTION_CHECK;
        }
          
        ClearBuffer();
        __delay_ms(500);
}

void MODEM_CHECK_CONNECTION(void)
{
        Send_String_UART1("AT\r");   
        
        TimerTemp=2000;
        TimerTimeout=0;
        temp=0;
   
        while((ParseBuffer(nizRXbuffer1,"OK")==0)&&(TimerTimeout==0)) ;///CEKA SE ODGOVOR OK
        
        temp=ParseBuffer(nizRXbuffer1,"OK");//VRACA INDEKS(REDNO MESTO) U KOM DELU NIZA SE NALAZI "OK"

        if(temp!=0)
        {  
            state = SMS_FORMAT_DEFINE;              
        }
                        
        BACK_TO_CHECK_CONNECTION_STATE();
        
}
void MODEM_SMS_FORMAT_DEFINE(void)
{
        Send_String_UART1("AT+CMGF=1\r"); //foramt as text
         
        TimerTemp=3000;
        TimerTimeout=0;
        temp=0;
   
        while((ParseBuffer(nizRXbuffer1,"OK")==0)&&(TimerTimeout==0)) ;
        
        temp=ParseBuffer(nizRXbuffer1,"OK");

        if(temp!=0)
        {  
            state = SMS_LIST;     
        }
       
        BACK_TO_CHECK_CONNECTION_STATE();
}
void MODEM_SMS_LIST(void)
{
        Send_String_UART1("AT+CMGL=ALL\r"); 
   
        TimerTemp=5000;
        TimerTimeout=0;
        temp=0;
   
        while((ParseBuffer(nizRXbuffer1,"REC UNREAD")==0)&&(TimerTimeout==0));
        
        temp=ParseBuffer(nizRXbuffer1,"REC UNREAD");

        message_index=nizRXbuffer1[temp-4];
                
        state = SMS_READ;
        
        BACK_TO_CHECK_CONNECTION_STATE();
                 
        __delay_ms(500);
}
void MODEM_SMS_READ(void)
{
        Send_String_UART1("AT+CMGR=");
        Send_One_Char(message_index);
        Send_One_Char('\r');
  
        TimerTemp=5000;
        TimerTimeout=0;
        temp=0;
   
        while((ParseBuffer(nizRXbuffer1,"Info")==0)&&(TimerTimeout==0));
        
        temp=ParseBuffer(nizRXbuffer1,"Info");
                
        if(temp!=0)
        {    
            temp_phone=ParseBuffer(nizRXbuffer1,",\"+");
            state = SMS_SEND;               
        }
                
        for(number=0; number < 16; number++)
        {
            phone_number[number]=0;
        }
                
        for(number=0; nizRXbuffer1[temp_phone+number] != ','; number++)
        {
            phone_number[number] = nizRXbuffer1[temp_phone+number];
        }
               
        BACK_TO_CHECK_CONNECTION_STATE();
}
void MODEM_SMS_SEND(void)
{
        Send_String_UART1("AT+CNMI=1,0,0,0,1\r");
        __delay_ms(100);
            
        ClearBuffer();
        
        Send_String_UART1("AT+CMGS=");
    
        const char* phoneNum = (const char*) phone_number;
            
        for(number=0; number<strlen(phoneNum); number++)
        {
            Send_One_Char(phone_number[number]);
        }
       
        Send_One_Char('\r');
                
        TimerTemp=1000;
        TimerTimeout=0;
        temp=0;
            
        while((ParseBuffer(nizRXbuffer1,">")==0)&&(TimerTimeout==0));
            
        temp=ParseBuffer(nizRXbuffer1,">");
            
        if(temp!=0)
        {
            Send_String_UART1("Trenutna temperatura: +");
            Send_String_UART1(temp_string);
            Send_One_Char('C');
            Send_One_Char('\r');
            Send_String_UART1("Prosecna temperatura: +");
            Send_String_UART1(temp_string_avg);
            Send_One_Char('C');
            Send_One_Char('\r');
            Send_One_Char(26);
        }
            
        TimerTemp=10000;
        TimerTimeout=0;
        temp=0;
           
        while((ParseBuffer(nizRXbuffer1,"OK")==0)&&(TimerTimeout==0));
            
        temp=(ParseBuffer(nizRXbuffer1,"OK"));
            
        if(temp!=0)
        {
            state=SMS_DELETE;
        }
            
        BACK_TO_CHECK_CONNECTION_STATE();
}
void MODEM_SMS_DELETE(void)
{
    Send_String_UART1("AT+CMGD=");
    Send_One_Char(message_index);
    Send_One_Char('\r');
                  
    TimerTemp=2000;
    TimerTimeout=0;
    temp=0;
   
    while((ParseBuffer(nizRXbuffer1,"OK")==0)&&(TimerTimeout==0));
                
    temp=ParseBuffer(nizRXbuffer1,"OK");
                
    if(temp!=0)
    {
        state=CONNECTION_CHECK;
    }
                
    BACK_TO_CHECK_CONNECTION_STATE();
}


