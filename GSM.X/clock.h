#ifndef CLOCK_H
#define	CLOCK_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* CLOCK_H */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <xc.h>
#include <p24fxxxx.h>
#include <p24FJ256GA106.h>
#include <libpic30.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

#define FCY 16000000   
//FCY = (FOSC/2)*PLL4 = (8Mhz / 2)*PLL4 = 4Mhz*PLL4=16MHz
#define __delay_ms(d) __delay32( (unsigned long) (d)*(FCY/1600))
#define __delay_us(d) __delay32( (unsigned long) (d)*(FCY/1600000))

void MCUclockInit(void);