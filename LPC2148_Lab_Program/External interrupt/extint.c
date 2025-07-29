#include<nxp\iolpc2148.h>
#include "intrinsics.h"
#include "stdio.h"
void EXINT3(void);
int putchar(int ch)
{
  if(ch=='\n')
  {
    while((U0LSR&0x20)!=0x20);
    U0THR='\r';
  }
  while((U0LSR&0x20)!=0x20);
  return(U0THR=ch);
}
void delay(double x)
{
  unsigned int i;
  for(i=0;i<x;i++);
}
void main()
{       
    
   PINSEL0=0X00000005;  // Selecting/configuring the pins as RxD & TxD. 
  VPBDIV=0X01;         // Making Pclk = Xclk.
  int baudrate = (12000000/(19200*16)); // Baudrate setting (9600 bps)
  U0LCR=0X83;
  U0DLL = baudrate & 0X00FF;      
  U0DLM=( baudrate >>8) & 0X00FF;
  U0LCR=0X03;
        PINSEL1_bit.P0_30=2;
        PINSEL2=0x00000000;
        IO1DIR=0xff000000;
    	VICIntSelect &= ~(1<<VIC_EINT3);            //Select the interrupt             
  	VICVectAddr0 = (unsigned int)&EXINT3;    
  	VICVectCntl0 = 0x20 | VIC_EINT3;              
  	VICIntEnable |= (1<<VIC_EINT3);
        
	
  	__enable_interrupt();     //Enable Global Interrupts
        
       
         while(1)
         {
       IO1PIN=0xff000000;
       delay(5000);
       IO1PIN=0x00000000;
       delay(5000);
         }
      
}


void EXINT3(void)
{ 
       EXTINT_bit.EINT3=1;
         IO1PIN=0x00000000;
         printf("External interrupt occured\n\r");
}


#pragma vector=0x18
__irq __arm void irq_handler (void)
{
  	void (*interrupt_function)();
  	unsigned int vector;

  	vector = VICVectAddr;     
  	interrupt_function = (void(*)())vector;
  	(*interrupt_function)();  
  	VICVectAddr = 0;          
}


__fiq __arm void fiq_handler (void)  /*------ FIQ Handler Function ---------*/

{ while(1); }