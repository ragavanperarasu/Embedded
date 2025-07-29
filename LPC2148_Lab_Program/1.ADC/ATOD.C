#include<nxp\iolpc2148.h>
#include<stdio.h>
#define PCLKFREQ 12000000
void serial_init(unsigned long baud_rate)
{
      unsigned int  BAUDRATEDIVISOR;
      PINSEL0 |= 0x00000005;// TO SELCET THE UART0 TX AND RX PIN
      VPBDIV=0X01;// TO SET THE PERIPHERAL CLOCK AS SAME AS THE PROCESSOR CLOCK
      BAUDRATEDIVISOR=(PCLKFREQ/(baud_rate*16));      
      U0LCR=0X80;// TO SET THE DLAB BIT FOR ACCESSING THE DIVISOR LATCH REGISTER
      U0DLL = BAUDRATEDIVISOR & 0x00ff;
      U0DLM = (BAUDRATEDIVISOR >> 8) & 0x00ff;
      U0LCR=0X03;// TO SET THE WORD LENGTH AS 8 BIT
}
int putchar(int a)
{
   if(a=='\n')
   {
     while((U0LSR&0X20)!=0X20);// FOR CHEKING THE TRANSMITTER BUFFER
                                 // REGISTER IS EMPTY OR NOT
      
     U0THR=0X0D;   
   }  
   if(a=='\t')
   {
     while((U0LSR&0X20)!=0X20);// FOR CHEKING THE TRANSMITTER BUFFER
                                 // REGISTER IS EMPTY OR NOT
      
     U0THR=0X20;   
   }    
  
   while((U0LSR&0X20)!=0X20);// FOR CHEKING THE TRANSMITTER BUFFER
                                 // REGISTER IS EMPTY OR NOT
   return(U0THR=a);                    //TO TRANSMIT THE CHARACTER 
}
void main(void)
{
    unsigned int val=0,volt;
   PINSEL1_bit.P0_28=1;
    serial_init(19200);
     PINSEL1=0X01000000;      //ADC0.1
     AD0CR_bit.CLKDIV =0x02;  // VPB clock (PCLK) is divided by 2
     AD0CR_bit.BURST=1;       //To prevent the repeated conversion
     AD0CR_bit.CLKS=0;        //clk indicate 11 clocks / 10 bits
     AD0CR_bit.PDN=1;         //A/D converter is operational
     AD0CR_bit.SEL = 0x02;         
    while(1)
    {
     while(AD0DR_bit.DONE==0);             // Start A/D Conversion 
      val=AD0DR1_bit.RESULT;;
    volt=((val/3)*10);
      printf("ADC value = %d \t Volt= %d mv \n",val,volt);
      
       
    }
}