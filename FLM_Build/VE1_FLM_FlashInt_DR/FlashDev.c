/***********************************************************************/
/*  This file is part of the ARM Toolchain package                     */
/*  Copyright (c) 2010 Keil - An ARM Company. All rights reserved.     */
/***********************************************************************/
/*                                                                     */
/*  FlashDev.C:  Device Description for New Device Flash               */
/*                                                                     */
/***********************************************************************/

#include "FlashOS.H"        // FlashOS Structures


struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "StartMilandr 1986VE1/VE3 FlashInt DR",   // Device Name 
   ONCHIP,                     // Device Type
   0x00000000,                 // Device Start Address
   0x00020000,                 // Device Size in Bytes
   0x1000,                     // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   5000,                       // Program Page Timeout 5 Sec
   5000,                       // Erase Sector Timeout 5 Sec

// Specify Size and Address of Sectors
   0x00020000, 0x000000,       // Sector Size  128kB (32 Pages)
   SECTOR_END
};
