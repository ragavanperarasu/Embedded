#include<nxp\iolpc2148.h>
#include<stdio.h>
#define PCLKFREQ 12000000
#define MAM_MODE 2
#define MAM_FETCH 3
char RTC_DATA1[]={25,45,1},RTC_DATA2[3];
void serial_init(unsigned long baud_rate)
{
      unsigned int  BAUDRATEDIVISOR;
      PINSEL0|= 0x00000005;// TO SELCET THE UART0 TX AND RX PIN
      VPBDIV=0X01;// TO SET THE PERIPHERAL CLOCK AS SAME AS THE PROCESSOR CLOCK
      BAUDRATEDIVISOR=(PCLKFREQ/(baud_rate*16));      
      U0LCR=0X80;// TO SET THE DLAB BIT FOR ACCESSING THE DIVISOR LATCH REGISTER
      U0DLL = BAUDRATEDIVISOR & 0x00ff;
      U0DLM = (BAUDRATEDIVISOR >> 8) & 0x00ff;
      U0LCR=0X03;// TO SET THE WORD LENGTH AS 8 BIT
}
int putchar(int a)
{
   if(a == '\n')
   {
        while((U0LSR&0X20)!=0X20);
        U0THR=0x0D;
   }  
   while((U0LSR&0X20)!=0X20);
   return(U0THR=a);
}
void delay(void)
{
  for(char i=0;i<5;i++)
    for(char j=0;j<50;j++);
}
void delay1(void)
{
  for(char i=0;i<0xff;i++)
    for(char j=0;j<0xff;j++);
}
void i2c_config(void)
{
   MAMCR = 0;	
   MAMTIM =MAM_FETCH;
   MAMCR = MAM_MODE;
   PINSEL0|=0X00000050;// To select the Port pins p0.2 and p0.3 as i2c configurable
   I2C0CONCLR=0X6C;   // To clear all bits in the i2c0 register
   I2C0CONSET=0X40;   // To enable the i2c
   // To set clock rate as 100 KHZ                      
   I2C0SCLH=70;
   I2C0SCLL=90;
   
}
void i2c_write(char data,int address)
{
       //Start condition
       I2C0CONSET=0X20;
       delay();
       while(I2C0STAT!=0x08);
       I2C0CONCLR=0X28;
       
       // To send a slave address with write mode
       I2C0DAT=0xA0;
       delay();
       while(I2C0STAT!=0x18);
       I2C0CONCLR=0x08; 
             
       // To send Register location address to slave to initiate that register
       I2C0DAT=address;
       delay();
       while(I2C0STAT!=0x28);
       delay1(); 
       I2C0CONCLR=0x08; 
       
       //Data has been written to appropriate address or register
       I2C0DAT=data;
       delay();
       while(I2C0STAT!=0x28);
 
       //stop condition
       I2C0CONSET=0X10;
       I2C0CONCLR=0x08; 
       delay1();
}
void i2c_read(void)
{
       I2C0CONSET=0X20;
       delay();
       while(I2C0STAT!=0x08);
       I2C0CONCLR=0X28;    
   
       // To send a slave address
        I2C0DAT=0XA0;
        delay();
        while(I2C0STAT!=0x18);
        I2C0CONCLR=0X08;  

        // To send Register location address to slave to READ that register
        I2C0DAT=0x02;
        delay();
        while(I2C0STAT!=0x28);
        I2C0CONCLR=0x08; 

        //stop condition		
        I2C0CONSET=0x10;
        I2C0CONCLR=0x8;
        delay(); 
 
        // To make a start condition  
        I2C0CONSET=0X20;
        delay();
        while(I2C0STAT!=0x08);
        I2C0CONCLR=0X2C;
         
        // To send a slave address
        I2C0DAT=0XA1;
        delay();
        while(I2C0STAT!=0x40);
        I2C0CONSET=0x04;
        I2C0CONCLR=0X08;  
      
        for(char k=0;k<0x03;k++)
        {
           while(I2C0STAT!=0x50);
           RTC_DATA1[k]=I2C0DAT;
           I2C0CONCLR = 0X0C;
           I2C0CONSET = 0X04;
	   I2C0CONCLR = 0X08;	
        }
      	//stop condition		
       	I2C0CONCLR=0x8;
	I2C0CONSET=0x10;
}
void main(void)
{
    char switch_status;
    serial_init(19200);
    i2c_config();
    printf("Welcome");
    PINSEL2=0X00000000;
    IO1DIR=0X00FF0000;
    switch_status=((IO1PIN&0XFF000000)>>31);
    if(switch_status==0x00)
    {
      for(char i=0;i<0x03;i++)
      {
          i2c_write(RTC_DATA1[i],(i+2));
      }
    }
    while(1)
    {
         i2c_read();
         printf("%d:%d:%d\n",RTC_DATA1[0X02],RTC_DATA1[0X01],RTC_DATA1[0X00]);
         I2C0CONCLR=0X6C;   // To clear all bits in the i2c0 register
         I2C0CONSET=0X40;   // To enable the i2c
    }
}
