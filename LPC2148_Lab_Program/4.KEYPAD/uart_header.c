#include<iolpc2292.h>
#define PCLKFREQ  12000000

//To convert the data value into corresponding ASCII value 
//to display on the serial port
char convert_out(char y)
{
     if(y<=0x09)
     {
       y=y+0x30;
       return(y);
     }
     else
     {
       y=y+0x37;
       return(y);
     }
}
/*delay routine*/

void delay()
{ 
  unsigned int i,j;
  for(i=0;i<0x2A;i++)
    for(j=0;j<0xFF;j++); 
}

void serial_init(unsigned long baud_rate)
{
 
      unsigned int  BAUDRATEDIVISOR;
      PINSEL0= 0x00000005;// TO SELCET THE UART0 TX AND RX PIN
      VPBDIV=0X01;// TO SET THE PERIPHERAL CLOCK AS SAME AS THE PROCESSOR CLOCK
      BAUDRATEDIVISOR=(PCLKFREQ/(baud_rate*16));      
      U0LCR=0X80;// TO SET THE DLAB BIT FOR ACCESSING THE DIVISOR LATCH REGISTER
      U0DLL = BAUDRATEDIVISOR & 0x00ff;
      U0DLM = (BAUDRATEDIVISOR >> 8) & 0x00ff;
      U0LCR=0X03;// TO SET THE WORD LENGTH AS 8 BIT
}

void serial_msg(char *a)
{
      while(*a!='\0')
      {
        while((U0LSR&0X20)!=0X20);// FOR CHEKING THE TRANSMITTER BUFFER
                                     // REGISTER IS EMPTY OR NOT
      
        U0THR=*a;                    //TO TRANSMIT THE CHARACTER 
        a++;
      }
      U0THR=0X0D;
}


void serial_tx(char a)
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

