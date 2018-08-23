#include "flmClock.h"

void HSI_Enable(void)
{
  MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_BKP;
  MDR_BKP->REG_0F |= (uint32_t)(BKP_REG_0F_HSI_ON | BKP_REG_0F_LSI_ON);
  while (!(MDR_BKP->REG_0F & BKP_REG_0F_HSI_RDY)) {}
    
//#endif
}

void ClocksDeinit_HSI(void)
{
  //  HSI and LSI generator On
  HSI_Enable();
  
  //  Set HSI as Core Clock
  MDR_RST_CLK->CPU_CLOCK = (uint32_t)0;
  
  //  Reset Other clock
  MDR_RST_CLK->PLL_CONTROL   &= (uint32_t)0x00000000;
  MDR_RST_CLK->HS_CONTROL    &= (uint32_t)0x00000000;
  MDR_RST_CLK->USB_CLOCK     &= (uint32_t)0x00000000;
  MDR_RST_CLK->ADC_MCO_CLOCK &= (uint32_t)0x00000000;
  MDR_RST_CLK->RTC_CLOCK     &= (uint32_t)0x00000000;
  
  MDR_RST_CLK->PER_CLOCK   = (uint32_t)PCLK_BIT(MDR_RST_CLK_BASE) | (uint32_t)PCLK_BIT(MDR_BKP_BASE);  
  
  //  Clear all other bits except for HSI_ON and LSI_ON
  MDR_BKP->REG_0F |= (uint32_t)(BKP_REG_0F_HSI_ON | BKP_REG_0F_LSI_ON);
}

