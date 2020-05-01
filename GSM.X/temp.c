#include"temp.h"

float temp_float=0;

uint8_t sensor_init(void)
{
    TRISEbits.TRISE6 = 0;   //kontroler povuce magistralu na 0 480us
    LATEbits.LATE6 = 0;
    __delay_us(480);
    
    TRISEbits.TRISE6 = 1;   //pin ulazni
    __delay_us(60);         //ceka 80us da sens odgovori
    
    if(PORTEbits.RE6==0)
    {
        __delay_us(480);
        return 0;    
    }    
    else
    {
        __delay_us(480);
        return 1;
    }
}

void write(uint8_t data)
{
    TRISEbits.TRISE6=0;
    int i=0;
    
    for(i=0; i<8; i++)
    {
        if((data & (1<<i)) !=0)         //write 1
        {
            LATEbits.LATE6=0;
            __delay_us(6);
            LATEbits.LATE6=1;
            __delay_us(64);
        }
        
        else                            //write 0
        {
            LATEbits.LATE6=0;
            __delay_us(60);
            LATEbits.LATE6=1;
            __delay_us(10);
        }            
    }
}

uint8_t read(void)
{
    char value=0;
    TRISEbits.TRISE6=1;
    int i=0;
    
    for(i=0; i<8; i++)
    {
        TRISEbits.TRISE6=0;
        LATEbits.LATE6=0;
        __delay_us(6);//2  
        TRISEbits.TRISE6=1;             
         __delay_us(9);
        
        if(PORTEbits.RE6!=0)
        {
            value |= 1<<i;              //read 1 here
        }
        __delay_us(55);
    }
    
    return value;
}

float calculate_temp(void)
{
    uint8_t temp_l,temp_h;
    uint16_t temp;
    float temperature;
    
      
    if(!sensor_init())
    {
        write(0xcc);      //skip ROM
        write(0x44);      //convert temp
        
        __delay_ms(800);
        
        sensor_init();
        write(0xcc);
        write(0xbe);      //read scratchpad
        
        temp_l = read();
        temp_h = read();       

       temp=(temp_h << 8)|temp_l;
       temperature=(float)temp/16;
    }
    return temperature;
}

void reverse(char* str, int len) 
{ 
    int i = 0, j = len - 1, temp; 
    while (i < j)
    { 
        temp = str[i]; 
        str[i] = str[j]; 
        str[j] = temp; 
        i++; 
        j--; 
    } 
} 

int intToStr(int x, char str[], int d) 
{ 
    int i = 0; 
    while (x)
    { 
        str[i++] = (x % 10) + '0'; 
        x = x / 10; 
    } 
    while (i < d) 
        str[i++] = '0'; 
  
    reverse(str, i); 
    str[i] = '\0'; 
    return i; 
} 

void ftoa(float n, char* res, int afterpoint) 
{ 

    int ipart = (int)n; 
  
    float fpart = n - (float)ipart; 
  
    int i = intToStr(ipart, res, 0); 
  
    if (afterpoint != 0)
    { 
        res[i] = '.';

        fpart = fpart * pow(10, afterpoint); 
  
        intToStr((int)fpart, res + i + 1, afterpoint); 
    } 
} 

void PIN_Init(void)
{
    TRISEbits.TRISE6 = 0;   //data temp sens
    LATEbits.LATE6 = 1;
    
    TRISDbits.TRISD6=0;     //led check
    LATDbits.LATD6=0;
}