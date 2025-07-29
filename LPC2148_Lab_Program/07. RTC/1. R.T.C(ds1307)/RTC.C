
#include<NXP/iolpc2148.h>
#include<stdio.h>

#define DESIRED_BAUDRATE 19200   
#define CRYSTAL_FREQUENCY_IN_HZ 12000000
#define PCLK        CRYSTAL_FREQUENCY_IN_HZ	      // since VPBDIV=0x01
#define DIVISOR (PCLK/(16*DESIRED_BAUDRATE))
char rtc_data[12];

//FUNCTION DEFINITION

void delay(void )
{
	volatile  int i,j;
		for (i=0;i<5;i++)
		for (j=0;j<50;j++);
}

void delay1(void )
{
	volatile  int i,j;
	for (i=0;i<25;i++)
	 for (j=0;j<500;j++);
}

int putchar(int i)
{
  while (!(U0LSR&0x20));   //wait until Transmit Holding Register is empty		
    return(U0THR=i);		  //then store to Transmit Holding Register	


}
void Arm_Uart0_Init()                           //UART Config.
{
       
      U0LCR=0x83;			//	U0LCR: UART0 Line Control Register.
                 			//  0x83: enable Divisor Latch access, set 8-bit word length.
		        		//  1 stop bit, no parity, disable break transmission.		
      
      VPBDIV=0x01;     	        	//	VPBDIV: VPB bus clock divider 	0x01: PCLK = processor clock	.
      
      U0DLL=DIVISOR&0xFF;               //	U0DLL: UART0 Divisor Latch (LSB).	
      U0DLM=DIVISOR>>8;                 //	U0DLM: UART0 Divisor Latch (MSB).		
      U0LCR=0x03 ;		        //  U0LCR: UART0 Line Control Register
					//  0x03: same as above, but disable Divisor Latch access.
                                        //  And same time U0THR (Transmitting register writing)holding the data.
      U0FCR=0x05 ;      		// 	U0FCR: UART0 FIFO Control Register
				   	//  0x05: Clear Tx FIFO and enable Rx and Tx FIFOs	
  
}  

void i2c_write(unsigned int addr,unsigned int data)
{
 
//START CONDITION
  putchar('a');
    
  I2C0CONSET=0x60;          //start I2C data transmission when set STA flag.
  delay();			
  while(I2C0STAT!=0x08);
				
//SLAVE ADDRESS      	          
  putchar('b');  
  
  I2C0DAT=0xD0;            //RTC ADDRESS(PCF8583)
  I2C0CONCLR=0x28;	
  delay();
  while(I2C0STAT!=0x18)
   delay();
	
  putchar('c');  
//RTC WRITE ADDRESS	 
  
  I2C0DAT=addr;
  I2C0CONCLR=0x8;
  while(I2C0STAT!=0x28)
    delay();
  delay1();
				
//RTC DATA				

  putchar('d');  
  
  I2C0DAT=data;
  I2C0CONCLR=0x8;
  while(I2C0STAT!=0x28)
   delay();
	
//STOP CONDITION			
  
  I2C0CONSET=0x10;
  I2C0CONCLR=0x8;	
  delay1();
    putchar('e');
}

void i2c_read(unsigned int addr)
{
  
  I2C0CONSET=0x10;
  I2C0CONCLR=0x8;	
  delay1();

//START CONDITION

  I2C0CONSET=0x60;          //start I2C data transmission when set STA flag.
  delay();			
  while(I2C0STAT!=0x08);
  	
//SLAVE ADDRESS
 
  I2C0DAT=0xD0;
  I2C0CONCLR=0x28;	
  delay();			
  while(I2C0STAT!=0x18)
    delay();
				 
//RTC READ ADDRESS		
  
  I2C0DAT=addr;			
  I2C0CONCLR=0x8;
  while(I2C0STAT!=0x28)
    delay();            

//STOP CONDITION			
  
  I2C0CONSET=0x10;
  I2C0CONCLR=0x8;
  delay();
	
//RESTART CONDITION
  
  I2C0CONSET=0x60;          //start I2C data transmission when set STA flag.
  delay();			
  while(I2C0STAT!=0x08);
                		
//SLAVE ADDRESS WITH READ MODE		
  
  I2C0DAT=0xD1;
  I2C0CONCLR=0x2C;	
  while(I2C0STAT!=0x40);
  
  I2C0CONSET = 0X04;
  I2C0CONCLR = 0X08;	
  		
//READ TIME VALUE	
  
for(int i=0;i<0x03;i++)
{	  
  I2C0CONCLR=0x08;	
  while(I2C0STAT!=0x50);
  
  delay();
  rtc_data[i]=I2C0DAT;
  I2C0CONCLR = 0X0C;
  
  I2C0CONSET = 0X04;
  I2C0CONCLR = 0X08;			
}		
				
//STOP CONDITION				

  I2C0CONSET=0x10;
  I2C0CONCLR=0x8;	
  delay();
}



void main()
{
  PINSEL0=0x00000055;
  Arm_Uart0_Init();
  I2C0CONCLR=0x6C;      //clear I2C0CONCLR register
  I2C0CONSET=0x40;      //Enable I2C.
  I2C0SCLH=110;         //SET CLOCK VALUE  
  I2C0SCLL=90; 

  while(1)
  {
    delay1();
    
    i2c_write(0x00,0X00);  //WRITE FOR SECONDS 
    i2c_write(0x01,0X02);	//WRITE FOR MINUTES
    i2c_write(0x02,0X08); //WRITE FOR HOURS
    
    while(1)
    {
      i2c_read(0x00);    //READ RTC VALUES
      printf("\n\rRTC=%x:%x:%x",rtc_data[2],rtc_data[1],rtc_data[0]);
    }
  }
}
