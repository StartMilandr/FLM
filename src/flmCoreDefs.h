#ifndef FLM_CORE_DEFS_H
#define FLM_CORE_DEFS_H

#include <stdint.h>

#define     __IO    volatile             /*!< Defines 'read / write' permissions              */

#if defined ( USE_MDR1986VE9x )
	#include "MDR32Fx.h"
  #define FLASH_ADDR_0      0x08000000

#elif defined (USE_MDR1986VE1T)
	#include "MDR1986VE1T.h"
  #define FLASH_ADDR_0      0

#elif defined ( USE_MDR1986VE3 )
	#include "MDR1986VE3.h"
  #define FLASH_ADDR_0      0

#elif defined ( USE_MDR1901VC1T )
	#include "MDR1901VC1T.h"
  #define FLASH_ADDR_0      0x08000000

#elif defined (USE_MDR1986BE4)
	#include "MDR1986BE4.h"
  #define FLASH_ADDR_0      0

#endif


#endif  //  FLM_CORE_DEFS_H
