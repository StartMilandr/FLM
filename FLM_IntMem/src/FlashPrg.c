/***********************************************************************/
/*  This file is part of the ARM Toolchain package                     */
/*  Copyright (c) 2010 Keil - An ARM Company. All rights reserved.     */
/***********************************************************************/
/*                                                                     */
/*  FlashDev.C:  Flash Programming Functions adapted                   */
/*               for New Device 256kB Flash                            */
/*                                                                     */
/***********************************************************************/

//#include "..\FlashOS.H"        // FlashOS Structures

/* 
   Mandatory Flash Programming Functions (Called by FlashOS):
                int Init        (unsigned long adr,   // Initialize Flash
                                 unsigned long clk,
                                 unsigned long fnc);
                int UnInit      (unsigned long fnc);  // De-initialize Flash
                int EraseSector (unsigned long adr);  // Erase Sector Function
                int ProgramPage (unsigned long adr,   // Program Page Function
                                 unsigned long sz,
                                 unsigned char *buf);

   Optional  Flash Programming Functions (Called by FlashOS):
                int BlankCheck  (unsigned long adr,   // Blank Check
                                 unsigned long sz,
                                 unsigned char pat);
                int EraseChip   (void);               // Erase complete Device
      unsigned long Verify      (unsigned long adr,   // Verify Function
                                 unsigned long sz,
                                 unsigned char *buf);

       - BlanckCheck  is necessary if Flash space is not mapped into CPU memory space
       - Verify       is necessary if Flash space is not mapped into CPU memory space
       - if EraseChip is not provided than EraseSector for all sectors is called
*/

#include  "flmCoreDefs.h"
#include  "MDR32F9Qx_eeprom.h"
#include  "flmClock.h"

#include <stdint.h>

// CASE_1: for Info memory
//#define SELECT_INFO_MEM

#ifdef SELECT_INFO_MEM
  #define BANK_SELECTOR  EEPROM_Info_Bank_Select
  #define READ_BY_EEPROM      // Forced read by EEPROM Controller
#else
  #define BANK_SELECTOR  EEPROM_Main_Bank_Select
  //#define READ_BY_EEPROM    //  User defines
#endif  

// CASE_2: Read mistmuch bug fix
//#define FIX_DBL_READ

//  Page Sizes
#define PAGE_SIZE           0x1000
#define PAGE1_ANY_ADDR    (FLASH_ADDR_0 * 0 + 100)
#define PAGE2_ANY_ADDR    (FLASH_ADDR_0 * 2 + 100)


/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int Init (unsigned long adr, unsigned long clk, unsigned long fnc) { 
  //  Запрет всех прерываний
  __disable_irq();
  
  //  Включение HSI
  ClocksDeinit_HSI();
  
  //  Clock from HSI
  MDR_RST_CLK->CPU_CLOCK = 0;
  
  //  EEPROM Clock On
  if ((fnc == 1) || (fnc == 2))
    MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_EEPROM;
  
  return (0);                
}


/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int UnInit (unsigned long fnc) {

  volatile uint32_t d;
  
  MDR_EEPROM->CMD = 0;  
  MDR_EEPROM->KEY = 0;
  MDR_RST_CLK->PER_CLOCK = 0x00000010;
  
  //  Update page cache
  d =(*((volatile uint32_t *) PAGE1_ANY_ADDR));  

  //  Update page cache
  d =(*((volatile uint32_t *) PAGE2_ANY_ADDR));  
  
  return (0);
}


/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseChip (void) {

  EEPROM_EraseAllPages(BANK_SELECTOR);
  return (0);                                  // Finished without Errors
}


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseSector (unsigned long adr) {

  EEPROM_ErasePage(adr, BANK_SELECTOR);
  return (0);
}


/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {

  uint32_t i;
  
  for (i = 0; i < sz; i += 4)
    EEPROM_ProgramWord(adr + i, BANK_SELECTOR, buf[i+3] <<24 | buf[i+2]<<16 | buf[i+1]<<8 | buf[i]);
  
  return (0);
}


/*
 *  Read Word from Flash Memory
 *    Parameter:      adr:  Page Start Address
 *    Return Value:   Data word
 */

uint32_t ReadWord(uint32_t Address)
{
#ifndef READ_BY_EEPROM
  return *((volatile uint32_t *)(Address));
#else
  return EEPROM_ReadWord(Address, EEPROM_Info_Bank_Select);
#endif
}

/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    pat	Pattern to compare
 *    Return Value:   0 - OK,  1 - Failed
 */

int BlankCheck (unsigned long adr, unsigned long sz, unsigned char pat)
{
  uint32_t i;
  volatile uint32_t value;
  
  for (i = 0; i < sz; i +=4)
  {
  #ifdef FIX_DBL_READ
    value = *((volatile uint32_t *)(adr + i));
  #endif
    
    value = ReadWord(adr + i);
  #ifdef FIX_DBL_READ
    if (0xFFFFFFFF != value)
      value = ReadWord(adr + i);
  #endif
    
    if (0xFFFFFFFF != value)
      return 1;
  }
  
  return 0;
}

/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

unsigned long Verify (unsigned long adr, unsigned long sz, unsigned char *buf)
{
  uint32_t i;
  //uint32_t addrI;
  uint32_t* pData32 = (uint32_t*)buf;
  volatile uint32_t value;
  
  //  Loop by Int
  sz = sz >> 2;
  for (i = 0; i < sz; ++i)
  {
    value = ReadWord(adr + (i << 2));
  #ifdef FIX_DBL_READ
    if (pData32[i] != value)
      value = ReadWord(adr + (i << 2));
  #endif

    if (pData32[i] != value)
      break;
  }
  
  return adr + (i << 2);
}
