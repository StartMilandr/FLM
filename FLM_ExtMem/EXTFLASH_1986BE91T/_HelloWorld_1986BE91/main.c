#include <MDR32F9Qx_port.h>
#include <MDR32F9Qx_rst_clk.h>


void Delay(int waitTicks);

#define LED_PERIOD 100000
#define LED_5 PORT_Pin_10
#define LED_6 PORT_Pin_11
#define LED_7 PORT_Pin_12
#define LED_8 PORT_Pin_13
#define LED_9 PORT_Pin_14

#define LED_ALL LED_5|LED_6|LED_7|LED_8|LED_9

	
int main()
{
  PORT_InitTypeDef GPIOInitStruct;
	
  RST_CLK_PCLKcmd (RST_CLK_PCLK_PORTD, ENABLE);
  PORT_StructInit(&GPIOInitStruct);
  
  GPIOInitStruct.PORT_Pin        = LED_ALL;
  GPIOInitStruct.PORT_OE         = PORT_OE_OUT;
  GPIOInitStruct.PORT_SPEED      = PORT_SPEED_SLOW;
  GPIOInitStruct.PORT_MODE       = PORT_MODE_DIGITAL;
  PORT_Init(MDR_PORTD, &GPIOInitStruct);
	
  while (1)
  {
		PORT_SetBits(MDR_PORTD, LED_ALL);	// LED ON
		Delay(LED_PERIOD);
		
		PORT_ResetBits(MDR_PORTD, LED_ALL); // LED OFF
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

