#include<nxp\iolpc2148.h>
#include<stdio.h>
 
void delay(unsigned int x)
{
  T0TC = 0;
  T0PR = 12;
  T0TCR= 1;
  loop:
  if(T0TC<=x)
  goto loop;
}
void main()
{
PINSEL0=0x00000000;
PINSEL1=0x00000000;
PINSEL2=0x00000000;
/*PLLCFG=0x00000024; 
  PLLCON=0x00000001;
  PLLFEED=0x000000AA;
  PLLFEED=0x00000055; 
  
  while(!(PLLSTAT & 0x00000400));
  
  PLLCON=0x00000003;
  PLLFEED=0x000000AA;
  PLLFEED=0x00000055;*/
IO0DIR=0xFFFFFFFF;
IO1DIR=0xFFFFFFFF;

while(1)
{
IO0PIN=0xFFFFFFFF;
IO1PIN=0xFFFFFFFF;
//delay(25000);
//IO0PIN=0x00000000;
//IO1PIN=0x00000000;
//delay(25000);

}
}