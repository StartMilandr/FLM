#include <MDR32F9Qx_port.h>
#include <MDR32F9Qx_rst_clk.h>


void Delay(int waitTicks);

#define LED_PERIOD 300000

#define SDIO_D1 PORT_Pin_11

	
int main()
{
  PORT_InitTypeDef GPIOInitStruct;
	
  RST_CLK_PCLKcmd (RST_CLK_PCLK_PORTC, ENABLE);
  PORT_StructInit(&GPIOInitStruct);
  
  GPIOInitStruct.PORT_Pin        = SDIO_D1;
  GPIOInitStruct.PORT_OE         = PORT_OE_OUT;
  GPIOInitStruct.PORT_SPEED      = PORT_SPEED_SLOW;
  GPIOInitStruct.PORT_MODE       = PORT_MODE_DIGITAL;
  PORT_Init(MDR_PORTC, &GPIOInitStruct);
	
  while (1)
  {
		PORT_SetBits(MDR_PORTC, SDIO_D1);	// SDIO_D1 ON
		Delay(LED_PERIOD);
		
		PORT_ResetBits(MDR_PORTC, SDIO_D1); // SDIO_D1 OFF
    Delay(LED_PERIOD);
  }      
}


void Delay(int waitTicks)
{
  int i;
  for (i = 0; i < waitTicks; i++)
  {
    __NOP();
  }	
}

