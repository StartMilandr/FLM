/***********************************************************************/
/*  This file is part of the ARM Toolchain package                     */
/*  Copyright KEIL ELEKTRONIK GmbH 2003 - 2008                         */
/***********************************************************************/
/*                                                                     */
/*  FlashDev.C:  Flash Programming Functions adapted                   */
/*               for 1986VE91T External Flash 1636PP1                  */
/*               using Flash Boot Loader with IAP                      */
/*                                                                     */
/***********************************************************************/

#include "FlashOS.h"        // FlashOS Structures

// Memory Mapping Control
	  
#define FLSH_CMD      (*((volatile unsigned int *) 0x40018000))
#define FLSH_ADR      (*((volatile unsigned int *) 0x40018004))
#define FLSH_DI       (*((volatile unsigned int *) 0x40018008))
#define FLSH_DO       (*((volatile unsigned int *) 0x4001800C))
#define FLSH_KEY      (*((volatile unsigned int *) 0x40018010))

#define PORTA_RXTX 		(*((volatile unsigned int *) 0x400A8000))
#define PORTA_OE	 		(*((volatile unsigned int *) 0x400A8004))
#define PORTA_FUNC	 	(*((volatile unsigned int *) 0x400A8008))
#define PORTA_ANALOG	(*((volatile unsigned int *) 0x400A800C))
#define PORTA_PULL	 	(*((volatile unsigned int *) 0x400A8010))
#define PORTA_PD	 		(*((volatile unsigned int *) 0x400A8014))
#define PORTA_PWR	 		(*((volatile unsigned int *) 0x400A8018))

#define PORTB_RXTX 		(*((volatile unsigned int *) 0x400B0000))
#define PORTB_OE	 		(*((volatile unsigned int *) 0x400b0004))
#define PORTB_FUNC	 	(*((volatile unsigned int *) 0x400b0008))
#define PORTB_ANALOG	(*((volatile unsigned int *) 0x400b000C))
#define PORTB_PULL	 	(*((volatile unsigned int *) 0x400b0010))
#define PORTB_PD	 		(*((volatile unsigned int *) 0x400b0014))
#define PORTB_PWR	 		(*((volatile unsigned int *) 0x400b0018))

#define PORTC_RXTX 		(*((volatile unsigned int *) 0x400B8000))
#define PORTC_OE	 		(*((volatile unsigned int *) 0x400b8004))
#define PORTC_FUNC	 	(*((volatile unsigned int *) 0x400b8008))
#define PORTC_ANALOG	(*((volatile unsigned int *) 0x400b800C))
#define PORTC_PULL	 	(*((volatile unsigned int *) 0x400b8010))
#define PORTC_PD	 		(*((volatile unsigned int *) 0x400b8014))
#define PORTC_PWR	 		(*((volatile unsigned int *) 0x400b8018))

#define PORTE_RXTX 		(*((volatile unsigned int *) 0x400c8000))
#define PORTE_OE	 		(*((volatile unsigned int *) 0x400c8004))
#define PORTE_FUNC	 	(*((volatile unsigned int *) 0x400c8008))
#define PORTE_ANALOG	(*((volatile unsigned int *) 0x400c800C))
#define PORTE_PULL	 	(*((volatile unsigned int *) 0x400c8010))
#define PORTE_PD	 		(*((volatile unsigned int *) 0x400c8014))
#define PORTE_PWR	 		(*((volatile unsigned int *) 0x400c8018))

#define PORTF_RXTX 		(*((volatile unsigned int *) 0x400e8000))
#define PORTF_OE	 		(*((volatile unsigned int *) 0x400e8004))
#define PORTF_FUNC	 	(*((volatile unsigned int *) 0x400e8008))
#define PORTF_ANALOG	(*((volatile unsigned int *) 0x400e800C))
#define PORTF_PULL	 	(*((volatile unsigned int *) 0x400e8010))
#define PORTF_PD	 		(*((volatile unsigned int *) 0x400e8014))
#define PORTF_PWR	 		(*((volatile unsigned int *) 0x400e8018))

#define EX_BUS_CONTROL		  (*((volatile unsigned int *) 0x400F0054))
#define EX_BUS_NAND_CYCLES	(*((volatile unsigned int *) 0x400F0050))

#define PER_CLOCK 	  (*((volatile unsigned int *)0x4002001C))

#define STACK_SIZE     128      // Stack Size


/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int Init (unsigned long adr, unsigned long clk, unsigned long fnc) 
{
	
//Init clock: RST_CLK| PORTA | PORTB | PORTC | PORTE | PORTF | EXT_BUS_CNTRL 
	PER_CLOCK = (1<<4) |(1<<21)|(1<<22)|(1<<23)|(1<<25)|(1<<29)|(1<<30);

	PORTA_FUNC = 0x55555555; // FUNC Main
	PORTB_FUNC = 0x55555555;
	PORTC_FUNC = 0x14;
	PORTE_FUNC = 0x10000055;
	PORTF_FUNC = 0x55555550;

	PORTA_ANALOG = 0xFFFF;   // Digital
	PORTB_ANALOG = 0xFFFF;
	PORTC_ANALOG = 0x6;
	PORTE_ANALOG = 0x400F;
	PORTF_ANALOG = 0xFFFC;

	PORTA_PULL = 0xFFFF;     // PULL DOWN
	PORTB_PULL = 0xFFFF;
	PORTC_PULL = 0xFFFF;
  PORTE_PULL = 0xFFFF;
  PORTF_PULL = 0xFFFF;
	
	PORTA_PD = 0x0000;       // Driver
	PORTB_PD = 0x0000;
	PORTC_PD = 0x0000;
	PORTE_PD = 0x0000;
	PORTF_PD = 0x0000;
	
	PORTA_PWR = 0xFFFFFFFF;  // MAX SPEED
	PORTB_PWR = 0xFFFFFFFF;
	PORTC_PWR = 0x3C;
	PORTE_PWR = 0x300000FF;
	PORTF_PWR = 0xFFFFFFF0;
	
	EX_BUS_CONTROL = 0xF002; // RAM, 17*HCLK

  return (0);
}

/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int UnInit (unsigned long fnc) 
{
	//Init clock: RST_CLK
	PER_CLOCK = (1<<4);

	PORTA_FUNC = 0x0;
	PORTB_FUNC = 0x0;
	PORTC_FUNC = 0x0;
	PORTE_FUNC = 0x0;
	PORTF_FUNC = 0x0;

	PORTA_ANALOG = 0x0; 
	PORTB_ANALOG = 0x0;
	PORTC_ANALOG = 0x0;
	PORTE_ANALOG = 0x0;
	PORTF_ANALOG = 0x0;

	PORTA_PULL = 0x0;
	PORTB_PULL = 0x0;
	PORTC_PULL = 0x0;
  PORTE_PULL = 0x0;
  PORTF_PULL = 0x0;
	
	PORTA_PD = 0x0000;
	PORTB_PD = 0x0000;
	PORTC_PD = 0x0000;
	PORTE_PD = 0x0000;
	PORTF_PD = 0x0000;
	
	PORTA_PWR = 0x0;
	PORTB_PWR = 0x0;
	PORTC_PWR = 0x0;
	PORTE_PWR = 0x0;
	PORTF_PWR = 0x0;
	
	EX_BUS_CONTROL = 0x0080;
  	
  return (0);
}


/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseChip (void) 
{
	unsigned int i;	
	*(volatile unsigned int *)((0x10000000)) = 0xF0F0F0F0; // Reset	
	i = (*(volatile unsigned int *)((0x10000000)));

	*(volatile unsigned int *)((0x10000000+0x555*4)) = 0xAAAAAAAA;
	*(volatile unsigned int *)((0x10000000+0x2AA*4)) = 0x55555555;	
	*(volatile unsigned int *)((0x10000000+0x555*4)) = 0x80808080;	
	*(volatile unsigned int *)((0x10000000+0x555*4)) = 0xAAAAAAAA;	
	*(volatile unsigned int *)((0x10000000+0x2AA*4)) = 0x55555555;
	*(volatile unsigned int *)((0x10000000+0x555*4)) = 0x10101010;
	
	do i = (*(volatile unsigned int *)((0x10000000))); // Verify D7 
	while ((i & 0x80808080) != 0x80808080);

  return (0);                                  // Finished without Errors
}


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address - Value = 0x00040000*Nsector from FlashDev.c
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseSector (unsigned long adr) 
{
	unsigned int i, ADR;
	ADR = adr & 0x1FFFFF;
	*(volatile unsigned int *)((0x10000000)) = 0xF0F0F0F0; // Reset
	i = (*(volatile unsigned int *)((0x10000000)));

	*(volatile unsigned int *)((0x10000000+0x555*4)) = 0xAAAAAAAA;
	*(volatile unsigned int *)((0x10000000+0x2AA*4)) = 0x55555555;	
	*(volatile unsigned int *)((0x10000000+0x555*4)) = 0x80808080;	
	*(volatile unsigned int *)((0x10000000+0x555*4)) = 0xAAAAAAAA;	
	*(volatile unsigned int *)((0x10000000+0x2AA*4)) = 0x55555555;
	*(volatile unsigned int *)((0x10000000 + ADR)) = 0x30303030;
	
	do i = (*(volatile unsigned int *)((0x10000000))); // Verify D7 
	while ((i & 0x80808080) != 0x80808080);

  return (0);                                  // Finished without Errors
}


/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address  - Initial value = Device Start Address from FlashDev.c
 *                    sz:   Page Size           - Max Value = Programming Page Size from FlashDev.c
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) 
{


  int i,y;
  unsigned int DATA;
  unsigned int ADR;
	
	*(volatile unsigned int *)((0x10000000)) = 0xF0F0F0F0; // Reset
  i = (*(volatile unsigned int *)((0x10000000)));
	
	for (i=0;i<sz;i=i+4)
	{	
		ADR = (adr+i) & 0x1FFFFF;
		DATA  = buf[i+3]<<24|buf[i+2]<<16|buf[i+1]<<8|buf[i];
		
		*(volatile unsigned int *)((0x10000000+0x555*4)) = 0xAAAAAAAA;
		*(volatile unsigned int *)((0x10000000+0x2AA*4)) = 0x55555555;
		*(volatile unsigned int *)((0x10000000+0x555*4)) = 0xA0A0A0A0;
		*(volatile unsigned int *)((0x10000000+ADR)) = DATA;

		do y = (*(volatile unsigned int *)((0x10000000+ADR))); // Verify D7 
		while ((y & 0x80808080) == ((~DATA) & 0x80808080));
	}
	
  return (0);                                  // Finished without Errors
}

