#include<nxp/iolpc2148.h>

void uart0_conf()
{
  VPBDIV = 0X01;                //12Mhz clk 
  U0LCR  = 0X83;                //8bit data,
  U0DLL  = 0X4E;                //9600 baudrate LSB
  U0DLM  = 0X00;                //MSB
  U0LCR  = 0X03;                //8bit data
}

void main()
{

  PINSEL0 = 0X00000005;         //uart0 enable
  PINSEL2 = 0X00000000;         //port 1 configura
  IO1DIR  = 0XFF000000;         //pin conf the output p0.31 to p0.24
  uart0_conf();
  while(1)
  {
    char t =U0RBR;              //USART0 Rx Buffer
    if(t==0x30)
    {
     IO1CLR = 0xff000000;
     //IO1SET = 0x11000000;
    }
   if(t==0x31)
    {
     IO1CLR = 0xff000000;
     IO1SET = 0x11000000;
    }
   if(t==0x32)
    {
     IO1CLR = 0xff000000;
     IO1SET = 0x22000000;
    }
    if(t==0x33)
    {
     IO1CLR = 0xff000000;
     IO1SET = 0x33000000;
    }
    if(t==0x34)
    {
     IO1CLR = 0xff000000;
     IO1SET = 0x44000000;
    }
    if(t==0x35)
    {
     IO1CLR = 0xff000000;
     IO1SET = 0x55000000;
    }
    if(t==0x36)
    {
     IO1CLR = 0xff000000;
     IO1SET = 0x66000000;
    }
   if(t==0x37)
    {
     IO1CLR = 0xff000000;
     IO1SET = 0x77000000;
    }
   if(t==0x38)
    {
     IO1CLR = 0xff000000;
     IO1SET = 0x88000000;
    }
   if(t==0x39)
    {
     IO1CLR = 0xff000000;
     IO1SET = 0x99000000;
    }
   if(t==0x41)
    {
     IO1CLR = 0xff000000;
     IO1SET = 0xaa000000;
    }
   if(t==0x42)
    {
     IO1CLR = 0xff000000;
     IO1SET = 0xbb000000;
    }
   if(t==0x43)
    {
     IO1CLR = 0xff000000;
     IO1SET = 0xcc000000;
    }
   if((t==0x44))
    {
     IO1CLR = 0xff000000;
     IO1SET = 0xdd000000;
    }
   if(t==0x45)
    {
     IO1CLR = 0xff000000;
     IO1SET = 0xee000000;
    }
   if(t==0x46)          
    {
     IO1CLR = 0xff000000;
     IO1SET = 0xff000000;
    }
   
  }}