//Header Files
#include<nxp/iolpc2148.h>

//Variable Declarations
unsigned int i;

//Main Routine
void main()
{
  PINSEL1 = 0x00080000; /* set P0.25 to DAC output */
  while ( 1 )
  {
    for(i=0;i<1024;i++)
    {
      DACR = (i << 6);
    }
    for(i=1024;i>0;i--)
    {
      DACR = (i << 6);
    }
  }    
}