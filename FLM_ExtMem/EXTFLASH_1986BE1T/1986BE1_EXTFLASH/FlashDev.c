/***********************************************************************/
/*  This file is part of the ARM Toolchain package                     */
/*  Copyright KEIL ELEKTRONIK GmbH 2003 - 2008                         */
/***********************************************************************/
/*                                                                     */
/*  FlashDev.C:  Device Description for 1986VE1T                       */
/*               External Flash 1636PP1                                */
/*               using Flash Boot Loader with IAP                      */
/*                                                                     */
/***********************************************************************/

#include "FlashOS.H"        // FlashOS Structures


struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "1986BE1 IAP 1MB EXTERNAL 1636PP1 Flash",  // Device Name displayed in Keil (IAP - In-Application Programming)
   EXT32BIT,                   // Device Type
   0x00000000,                 // Device Start Address
	 0x00100000,                 // Device Size without Boot Loader: 1MB = 256KB*4 (0x40000<<2) (because A18 = 0)
	 0x400,                      // Programming Page Size (1KB)
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   1000,                       // Program Page Timeout 1000 mSec
   3000,                       // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
	 0x00040000, 0x000000,       // Sector Size: 256KB = 64KB*4 (0x10000<<2) 4 Sectors (because A18 = 0)
   SECTOR_END
};
