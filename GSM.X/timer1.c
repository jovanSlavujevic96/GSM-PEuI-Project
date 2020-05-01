#include"timer1.h"

void TMR1_init(void)
{
	T1CONbits.TON = 0;
	IFS0bits.T1IF = 0;

	T1CONbits.TSIDL = 1;
	T1CONbits.TGATE = 0;
	T1CONbits.TCKPS = 0; 
	T1CONbits.TSYNC = 0;
	T1CONbits.TCS = 0;

	IPC0bits.T1IP = 1;
	IEC0bits.T1IE = 1;

	PR1 = ((double)FCY/1600);
	TMR1 = 0;

	T1CONbits.TON = 1;
}

