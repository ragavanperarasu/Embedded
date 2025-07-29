#include<nxp/iolpc2148.h>
#include<stdio.h>

void timer0_conf()
{
  VPBDIV = 0X01;        //12MHZ
  T0TCR  = 0X00;        //STOP TIMER
  T0PR   = 0X12000;     //load prescasle or timer value
  T0TCR  = 0X02;        //reset timer0
}

void delay(unsigned int ms)
{
  T0TC  = 0x00;         //reset the timer0 counter
  T0TCR = 0X02;         //reset timer0
  T0TCR = 0X01;         //start timer0
  while(T0TC<=ms);      //wait for matching timer counter
  T0TCR = 0x00;         //stop timer0
}

void uart0_conf()
{
 PINSEL0 = 0X05;        
  VPBDIV = 0X01;
  U0LCR  = 0X83;        // 8bit data,divisior latch
  U0DLL  = 0X4E;        //9600 baudrate
  U0DLM  = 0X00;        //msb
  U0LCR  = 0X03;        //8bit data
  
}

int putchar(int dat)
{
  U0THR = dat;                  //tx holding reg
  while(!(U0LSR & 0x20));       //wait for tx holding reg empty
  return dat;
}

void main()
{
  PINSEL1 = 0X00000000;
  IO0DIR  = 0X00F00005;        //P0.20 t0 P0.23 as o/p or P0.16 t0 P0.20 as i/P
  timer0_conf();
  uart0_conf();
  while(1)
  {
IO0PIN=0x00E00000; // First Scan Line
    if(( IO0PIN & 0x000F0000 )!= 0x000F0000) //Check if any key is pressed in //4th row
      {
          switch(IO0PIN & 0x000F0000) //Check which one of the key is pressed
          {
            case 0x00070000 : 
              printf("F");
              break;
            case 0x000B0000 : 
              printf("B");
              break;
            case 0x000D0000 : 
              printf("7");
              break;
            case 0x000E0000 : 
              printf("3");
              break;
          }
      }
IO0PIN=0x00D00000; //Move second data to scan line
        if(( IO0PIN & 0x000F0000 )!= 0x000F0000) //Check if any key is pressed in 3rd row.
          {
            switch(IO0PIN & 0x000F0000) //check which one of the key is pressed
            {
              case 0x00070000 : 
                printf("E");
                break;
              case 0x000B0000 : 
                printf("A");
                break;
              case 0x000D0000 :
                printf("6");
                break;
              case 0x000E0000 :
                printf("2");
                break;
            }
          }
IO0PIN=0x00B00000; //Move 3rd scan data to port line
        if(( IO0PIN & 0x000F0000 )!= 0x0F000000)//Scan any key is pressed in 2nd row
        {
            switch(IO0PIN & 0x000F0000) //Check which one of the key is pressed in 2nd row
            {
              case 0x00070000 :
                printf("D");
                break;
              case 0x000B0000 : 
                printf("9");
                break;
              case 0x000D0000 : 
                printf("5");
                break;
              case 0x000E0000 : 
                printf("1");
                break;
            }
        }
IO0PIN=0x00700000; //Move 4th scan data to port line
       if(( IO0PIN & 0x000F0000 )!= 0x000F0000) //Check any key is pressed in 1st row
        {
            switch(IO0PIN & 0x000F0000) //Check which one of the key is pressed in 1st row
            {
              case 0x00070000 : 
                printf("C");
                break;
              case 0x000B0000 : 
                printf("8");
                break;
              case 0x000D0000 : 
                printf("4");
                break;
              case 0x000E0000 : 
                printf("0");
                break;
            }
        }
  }
}