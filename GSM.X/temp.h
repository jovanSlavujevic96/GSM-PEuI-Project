#ifndef TEMP_H
#define	TEMP_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* TEMP_H */
#include "clock.h"


extern float temp_float;

uint8_t sensor_init(void);
void write(uint8_t data);
uint8_t read(void);
float calculate_temp(void);

void reverse(char* str, int len);
int intToStr(int x, char str[], int d);
void ftoa(float n, char* res, int afterpoint);
void PIN_Init(void);