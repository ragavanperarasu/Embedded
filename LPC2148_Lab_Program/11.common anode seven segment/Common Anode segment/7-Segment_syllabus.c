#include <nxp\iolpc2148.h>
#define counter_delay 0x4ffff // Counter Delay ( 1 second approx. )


void delay(unsigned int k)
{ 
  for(int i=0;i<k;i++); 
}

void i2c_config(void)
{
  PINSEL0|=0X00000050;      // To select the Port pins p0.2 and p0.3 as i2c configurable
  I2C0CONCLR=0X6C;          // To clear all bits in the i2c0 register
  I2C0CONSET=0X40;          // To enable the i2c                  
  I2C0SCLH=70; 
  I2C0SCLL=50;               // To set clock rate as 100 KHZ 
}

void i2c_write(char address,char data)
{
  I2C0CONSET=0X20;
  delay(50);
  while(I2C0STAT!=0x08);
  I2C0CONCLR=0X28;
  
  // To send a slave address
  I2C0DAT=address;
  delay(50);
  while(I2C0STAT!=0x18);
  I2C0CONCLR=0x08; 
  
  //Data has been written to appropriate address
  I2C0DAT=data;
  delay(50);
  while(I2C0STAT!=0x28);
  
  //stop condition
  I2C0CONSET=0X10;
  I2C0CONCLR=0x08; 
  delay(50);
}


void counter()
{
  
  unsigned char data[10]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
  unsigned char s=0;
  int cnt;
  
  for(s=0;s<10;s++)   // loop to run the counter
  {
    
    i2c_write(0x70,data[s]);   // writing data to 1st digit (LSB)
    i2c_write(0x72,data[s]);   // writing data to 2nd digit 
    i2c_write(0x74,data[s]);   // writing data to 3rd digit 
    i2c_write(0x76,data[s]);   // writing data to 4th digit (MSB)
    i2c_write(0x78,data[s]); 
    i2c_write(0x7A,data[s]); 
    delay(counter_delay); // Counter Delay ( 1 second approx. )
    

  }   
  

}
void main(void)
{
  unsigned char i,address[6]={0x70,0x72, 0x74, 0x76, 0x78, 0x7A};
  
  i2c_config(); // confugring I2C for 7 segment driver .
  
  for(i=0;i<6;i++) {i2c_write(address[i],0x00);} // making all 7 segment blank. 
  while(1)
  {
  counter(); 
  } // counting upto cunnting limit.
  // Here counting limit is 110.
}   




