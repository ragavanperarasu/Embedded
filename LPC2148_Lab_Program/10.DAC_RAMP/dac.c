// To check digital to analog converter 
#include<nxp\iolpc2148.h>
void main(void)
{
   PINSEL1 =0X00080000; //To configure p0.25 as AOUT pin for the conversion output
   unsigned int val;
   while(1)
   {
     for(val=0;val<0x3ff;val++)
     {
         DACR=(val<<0X06);
     }
   }
}