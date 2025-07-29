#include <NXP/iolpc2148.h>  
#include <intrinsics.h>     // Header Files
#include <stdio.h>

/*--------------------- User-defined Function Declaration  -------------------------*/

            void timer0_ISR();       // Timer-0 Interrupt Service Routine Function Declaration 
            void timer0_init();      // Timer-0 Initialisation Function Declaration 
            void feed (void);        // PLL-Feed Function Declaration 
__irq __arm void irq_handler (void); // IRQ-Handler Function Declaration   
__fiq __arm void fiq_handler (void); // FIQ-Handler Function Declaration   
           

/*--------------------- Main Function Starts Here  -------------------------*/

void main(void)
{
    PINSEL2 = 0X00000000;    //  ( Optional )    
    IO1DIR  = 0xff000000;    // data bits configuration as I/O
    
           timer0_init();    // Interrupt Initialisation Function   
    __enable_interrupt();    // Global interrupt enable
    
    
    while(1)                 
   {
     IO1PIN  = 0x55000000;  // LED glow in this pattern (0x55)in default ( when no interrupt )
     
   } 
}          
     // main() function End .



/*--------------------- Function Definition -------------------------*/
/*-------------------------------------------------------------------*/

void timer0_ISR() // Interrupt Serice routine  for TIMER0 interrupt.
{
   IO1PIN  = 0xAA000000;           // LED glow in this pattern (0xAA)when there is an interrupt.
   for(int i = 0; i < 500000; ++i); // few ms Delay   
   T0IR = 1;                       // Clear timer interrupt
}


void timer0_init()  // TIMER0 and Interrupt initialisation for TIMER0 interrupt.
{
  PLLCON_bit.PLLC = PLLCON_bit.PLLE = 0; // Disable and disconnect PLL
  feed();                                // PLL feed sequence
  VPBDIV_bit.VPBDIV = 0;  /* Init Peripherial divider Pckl = Clk/4 */
  /*---- Timer setting ---------*/
  T0IR  =0xFF;       // reset match and capture event interrupts
  T0TC  = 12 ;        // Clear timer counter
  T0PR  = 0 ;        // No Prescalar
  T0MR0 = 1000000;   // Pckl 
  T0MCR = 3 ;        // Reset Timer Counter & Interrupt on match
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
}


__irq __arm void irq_handler (void)   /*------ IRQ Handler Function ---------*/
{ 
  void (*interrupt_function)(); // Get interrupt vector.  
  interrupt_function = (void(*)())VICVectAddr; // Call MM_TIMER0_ISR thru pointer
  (*interrupt_function)();     // Call vectored interrupt function
  VICVectAddr = 0;             // Clear interrupt in VIC to return the pointer to main function.
}


__fiq __arm void fiq_handler (void)  /*------ FIQ Handler Function ---------*/

  { while(1); }

void feed (void) // PLL FEED Function.
{
  PLLFEED=0xAA;
  PLLFEED=0x55;
}

/*-------------------------- Program Ends Hare --------------------------*/



/*
    Note :      For any register-setting clarification 
                
                please refer the PDF Document :
 
                " Volume 1: LPC2148 User Manual " .                */