//Header Files
#include<nxp\iolpc2148.h>
#include<intrinsics.h>
#include<stdio.h>

//Declarations
#define f 12000000 // PCLKFREQ

unsigned int value[1000];
unsigned long int i=0;

//Function to initialize Serial Port
void serial_Init(void) 
{
  unsigned int baudrate  ;
  PINSEL0|=0X00000005;
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

//IRQ Handler
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

//FIQ Handler
__fiq __arm void fiq_handler (void)
{
  while(1);
}

//Function to Initialize ADC1
void ADC1_Init()
{
  PINSEL0_bit.P0_12=0X03;              
  AD1CR_bit.CLKDIV=2;  
  AD1CR_bit.BURST=1;
  AD1CR_bit.PDN=1;
  AD1CR_bit.SEL=0X08;
  AD1CR_bit.START=0X001;
}

//Interrrupt Service Routine for Timer0
void timer0_ISR()
{ 
  T0IR = 1;  
  T0TC = 0xFFFFD11F;
  while(AD1DR_bit.DONE==0);
  value[i]=(AD1DR3_bit.RESULT & 0x3FF);  
  i = i+1;  
}

//Function to Initialize Timer0 for 1 second
void Timer0_Init()
{
  T0IR  =0xFF;       // reset match and capture event interrupts
  T0TC  = 0 ;        // Clear timer counter
  T0PR  = 0 ;        // No Prescalar
  T0MR0 = 0xFFFFFFFF;// Match Register, Interrupt occurs when T0TC=0xFFFFFFFF
  T0MCR = 3;        // Reset Timer Counter & Interrupt on match
  T0TCR = 1 ;        // Counting enable
  /*---- Vector interrupt setting ---------*/
  VICIntSelect  = 0;             // Set all VIC interrupts to IRQ for now
  VICIntEnClear = 0xFFFFFFFF;     // Diasable all interrupts
  VICProtection = 0;              // VIC registers can be accessed in User or privileged mode
  VICVectAddr   = 0;                // Clear interrupt
  VICProtection = 0;              // Accesss VIC in USR | PROTECT

  VICIntSelect &= ~(1<<VIC_TIMER0);            // Timer 0 intrpt is an IRQ (VIC_TIMER0 = 4)
  VICVectAddr0 = (unsigned int)&timer0_ISR; // Install ISR in VIC addr slot 0
  VICVectCntl0 = 0x20 | VIC_TIMER0;            // IRQ type, TIMER 0 int enabled
  VICIntEnable |= (1<<VIC_TIMER0);             // Turn on Timer0 Interrupt
  T0TC = 0xFFFFD11F;
}

//Main Routine
void main()
{
  serial_Init();
  printf("\nSampling Sine Wave");
  Timer0_Init();
  ADC1_Init();
  i=0;
  
  VICIntEnable |= (1<<VIC_TIMER0);             // Turn on Timer0 Interrupt
  __enable_interrupt();
    
  while(1)
  {      
    if(i==1000)
    {    
      T0IR = 1;
      VICIntEnClear = 0xFFFFFFFF;     // Diasable all interrupts
      printf("\nSamples Per Second");
      for(i=0;i<1000;i++)
        printf("\n%d",value[i]);
      T0TC=0;
      T0TCR = 0;
      __disable_interrupt();
      i=0;
    }
    
  }
}


