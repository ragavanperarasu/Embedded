#include<nxp\iolpc2148.h>
#include<stdio.h>
#define PCLKFREQ 12000000 
void delay_ms()
{ 
  unsigned int i,j; 
  for(i=0;i<0x0f;i++)
    for(j=0;j<0xF0;j++); 
}
void serial_tr(char a)
{
   if(a=='\t')
   {
     while((U0LSR&0X20)!=0X20);// FOR CHEKING THE TRANSMITTER BUFFER
                                 // REGISTER IS EMPTY OR NOT
     U0THR=0X20;   
   }  
   if(a=='\n')
   {
     while((U0LSR&0X20)!=0X20);// FOR CHEKING THE TRANSMITTER BUFFER
                                 // REGISTER IS EMPTY OR NOT
     U0THR=0X0D;   
   }  
   while((U0LSR&0X20)!=0X20);// FOR CHEKING THE TRANSMITTER BUFFER
                                 // REGISTER IS EMPTY OR NOT
   U0THR=a;                    //TO TRANSMIT THE CHARACTER 
}
void serial_init(unsigned long baud_rate)
{
      unsigned int  BAUDRATEDIVISOR;
      VPBDIV=0X01;// TO SET THE PERIPHERAL CLOCK AS SAME AS THE PROCESSOR CLOCK
      BAUDRATEDIVISOR=(PCLKFREQ/(baud_rate*16));      
      U0LCR=0X80;// TO SET THE DLAB BIT FOR ACCESSING THE DIVISOR LATCH REGISTER
      U0DLL = BAUDRATEDIVISOR & 0x00ff;
      U0DLM = (BAUDRATEDIVISOR >> 8) & 0x00ff;
      U0LCR=0X03;// TO SET THE WORD LENGTH AS 8 BIT
}
int putchar(int ch)
{
      if(ch=='\n')
      {  
          while((U0LSR&0X20)!=0X20);// FOR CHEKING THE TRANSMITTER BUFFER
                                 // REGISTER IS EMPTY OR NOT
          U0THR=0x0d;                    //TO TRANSMIT THE CHARACTER 
      }
      while((U0LSR&0X20)!=0X20);// FOR CHEKING THE TRANSMITTER BUFFER
                                 // REGISTER IS EMPTY OR NOT
      return(U0THR=ch);                    //TO TRANSMIT THE CHARACTER 
}
void arm_init(void)
{
  PINSEL0 = 0X00000005;
  PINSEL1 = 0X00000000;
  IO0DIR  = 0X000F0000;         
}
void main(void)
{
  arm_init();
  serial_init(19200);
  printf("welcome to keypad program");
    while(1)
    {
              IO0PIN=0x00000000;
              while((IO0PIN & 0x00F00000) == 0x00F00000);
              IO0PIN=0x00000000;
              IO0PIN=0x000E0000;
              if(( IO0PIN & 0x00F00000 )!= 0x000F0000)
              {
                       switch(IO0PIN & 0x00F00000)
                       {
                         case 0x00700000 : serial_tr('0');break;
                         case 0x00B00000 : serial_tr('1');break;  
                         case 0x00D00000 : serial_tr('2');break;  
                         case 0x00E00000 : serial_tr('3');break;  
                       }
                  
              }
              IO0PIN=0x000D0000;
              if(( IO0PIN & 0x00F00000 )!= 0x00F00000)
              {
                      switch(IO0PIN & 0x00F00000)
                      {
                        case 0x00700000 : serial_tr('4');break;
                        case 0x00B00000 : serial_tr('5');break;  
                        case 0x00D00000 : serial_tr('6');break;  
                        case 0x00E00000 : serial_tr('7');break;  
                      }
                    
                      
              }         
              IO0PIN=0x000B0000;  
              if(( IO0PIN & 0x00F00000 )!= 0x00F00000)
              {
                      switch(IO0PIN & 0x00F00000)
                      {
                        case 0x00700000 : serial_tr('8');break;
                        case 0x00B00000 : serial_tr('9');break;  
                        case 0x00D00000 : serial_tr('A');break;  
                        case 0x00E00000 : serial_tr('B');break;  
                      }
               
             }   
             IO0PIN=0x00070000;
             if(( IO0PIN & 0x00F00000 )!= 0x00F00000)
             {
                      switch(IO0PIN & 0x00F00000)
                      {
                        case 0x00700000 : serial_tr('C');break;
                        case 0x00B00000 : serial_tr('D');break;  
                        case 0x00D00000 : serial_tr('E');break;  
                        case 0x00E00000 : serial_tr('F');break;  
                      }
                        
                  
            }
               serial_tr(0x0d);
               serial_tr(0x0A);
              delay_ms();  
        }
}

  
  
 