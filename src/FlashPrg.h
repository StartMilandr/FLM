#ifndef FLASH_PROG_H
#define FLASH_PROG_H

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


int Init (unsigned long adr, unsigned long clk, unsigned long fnc);
int UnInit (unsigned long fnc);
int EraseChip (void);
int EraseSector (unsigned long adr);
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf);

int BlankCheck (unsigned long adr, unsigned long sz, unsigned char pat);
unsigned long Verify (unsigned long adr, unsigned long sz, unsigned char *buf);


#endif //FLASH_PROG_H
