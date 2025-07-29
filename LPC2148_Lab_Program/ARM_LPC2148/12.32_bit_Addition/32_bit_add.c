//Header Files
#include<nxp/iolpc2148.h>
#include<stdio.h>
#define f 12000000 // PCLKFREQ ( controller frequency )

//Function to initialize Serial Port
void serial_Init(void) // Function to initialise the serial port  
{
  unsigned int baudrate  ;
  PINSEL0=0X00000005;
  VPBDIV=0X01;
  baudrate = (f/(9600*16)); // Baudrate setting (9600 bps)
  U0LCR=0X80;
  U0DLL = baudrate & 0X00FF;
  U0DLM=( baudrate >>8) & 0X00FF;
  U0LCR=0X03;
}

//Function to print the data in the serial port
int putchar(int ch)
{
  if (ch == '\n')
  {
    while (!(U0LSR & 0x20));
    U0THR = 0x0d;
  }
  while (!(U0LSR & 0x20));
  return (U0THR = ch);
}

//Main Routine
void main()
{
  unsigned int a,b,c;
  
  serial_Init();  // Initialise the serial port
  a = 0x10Ab295C;
  b = 0x5366EA9F;
  c = a + b;
  printf("\n\r Addition of two given 32-bit numbers is %x",c);
  while(1);  
    
}