#include<iolpc2292.h>
#include<stdio.h>
#define PCLKFREQ 12000000
//To convert the data value into corresponding ASCII value 
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
    int i,j;
    for(i=0;i<=0xfff;i++)  
     for(j=0;j<=0xf0;j++); 
}
void serial_tx(char a)
{
   if(a=='\n')
   {
     while((U0LSR&0X20)!=0X20);
      U0THR=0X0D;   
   }  
   while((U0LSR&0X20)!=0X20);
   U0THR=a;                    
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
          while((U0LSR&0X20)!=0X20);
          U0THR=0x0d;               
      }
      while((U0LSR&0X20)!=0X20);// FOR CHEKING THE TRANSMITTER BUFFER REGISTER IS EMPTY OR NOT
      return(U0THR=ch);
}
void main()
{
     long int a=0,i=0,s,b=0;
     PINSEL0 |=0X00000005; 
     PINSEL2 |=0X00000000;
     IO1DIR  =0XF0000000;
     serial_init(19200);   // To initialze the uart0 for the serial communication
     printf("\nWelcome to keypad program press any key\t");
     while(1)
     {
       IO1PIN=0x00000000;
       while((IO1PIN & 0x0F000000) == 0x0F000000);
       IO1PIN=0x00000000;
        a=0;s=0;b=0;
       for(i=0;i<4;i++)
       {

            s=0xef000000;       
            s=(s<<i)&0xf0000000;
            IO1PIN=s; 
            a=IO1PIN&0x0F000000;
            if(a!=0x0F000000)
            {
                if(s==0xE0000000)   b=0x00;
                if(s==0xD0000000)   b=0x04;    
                if(s==0xB0000000)   b=0x08;    
                if(s==0x70000000)   b=0x0C;
                switch(a)
                {  
                    case 0x07000000 : serial_tx(convert_out(b=b+0x03));break;
                    case 0x0B000000 : serial_tx(convert_out(b=b+0x02));break;  
                    case 0x0D000000 : serial_tx(convert_out(b=b+0x01));break;  
                    case 0x0E000000 : serial_tx(convert_out(b=b+0x00));break;  
                }
            } 
        }
        delay();
    }     
}