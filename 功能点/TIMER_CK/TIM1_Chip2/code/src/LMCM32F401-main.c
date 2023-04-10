#include "uart.h"
#include "stdio.h"

#include <stm32f4xx.h>
#include <stm32f4xx_flash.h>

void TIM_INIT(void);

int EVENT_FLAG = 0;

void MCO1_Init(void);

int main()
{
	UartStdOutInit();  //uart初始化
	printf("Chip2:TEXT START\r\n"); 

	printf("Chip2:Open Clock\r\n"); 
	MCO1_Init();
	
	while (1)
	{
		
	}
}

void MCO1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_ClockSecuritySystemCmd(ENABLE);
 
    /* Enable GPIOs clocks */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_MCO);//打开引脚复用功能
         
    /* Configure MCO (PA8) */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  //UP
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	//输出时钟
    RCC_MCO1Config(RCC_MCO1Source_HSE, RCC_MCO1Div_1);
}
