#include<nxp\iolpc2148.h>

void delay(double x)
{
double i;
 for(i=0;i<x;i++);
}
void main(void)
{
 PINSEL0_bit.P0_4=0;
 PINSEL0_bit.P0_5=0;
 PINSEL0_bit.P0_6=0;
 PINSEL0_bit.P0_7=0;
 //PCONP |= (0 << 8);
 //PCONP |= (0 << 12);
 IO0DIR=0xf0;
 IO1DIR=0xffffffff;
  while(1)
  {
    IO0PIN=0x90 ;
     delay(100);
    IO0PIN=0x50;
    delay(100);
    IO0PIN=0x60;
    delay(100);
    IO0PIN=0xA0;
     delay(100);
  }
}
 