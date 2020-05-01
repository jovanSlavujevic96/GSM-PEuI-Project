#include"clock.h"

void MCUclockInit(void)
{
    RCONbits.SWDTEN=0;//wdt is disabled
    CLKDIVbits.DOZEN=0;
    CLKDIVbits.DOZE=0;
    CLKDIVbits.RCDIV=0;
    CLKDIVbits.ROI=0;
}
