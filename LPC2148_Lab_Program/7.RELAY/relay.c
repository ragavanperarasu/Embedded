#include<nxp\iolpc2148.h>
void delay(double x)
{
    double i;
     for(i=0;i<x;i++);
  
}
void main(void)
{
PINSEL1_bit.P0_31=0;
PINSEL1_bit.P0_29=0;
    IO0DIR_bit.P0_29=1;
     IO0DIR_bit.P0_31=1;
    while(1)
    {
     IO0PIN_bit.P0_29=1;
     IO0PIN_bit.P0_31=1;
        delay(5000);
        IO0PIN_bit.P0_29=0;
     IO0PIN_bit.P0_31=0;
        delay(5000);
    }           
}