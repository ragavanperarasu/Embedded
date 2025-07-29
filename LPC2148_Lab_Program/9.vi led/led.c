#include<nxp\iolpc2148.h>

int x;
void delay()
{
for( int i=0;i<=1500;i++);
}
void pwm(int x)
{
//  PINSEL0_bit.P0_7=1;
  PINSEL0  = 0X00088005; // To select Port pin P0.7 and also p0.9 as pwm4 and pwm6 
  PWMPR    = 0X00000000;  // To set Prescaler register value to one  
  PWMPCR   = 0X00004400;  // choose the pwm2,pwm6
  PWMMCR   = 0X00000002;   // To Reset pwm timer counter when matches occur
  PWMMR0   =  12000 ;   // To set total time period of pwm output  
  PWMMR2   =  x; 
  PWMMR6   =  x; 
  PWMTCR   = 0X00000002;   // To reset pwm timer
  PWMTCR   = 0X00000009;   //counter and PWM enable 
}
void main()
{
  while(1)
  {
    for(int i=0;i<=12000;i++)
    {
      x=x+10;
      pwm(x);
      delay();
      if(x>=12000)
      { 
        x=0;
      }
    }
  }
}


