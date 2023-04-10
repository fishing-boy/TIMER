#include "delay.h"
#include "uart_REG.h"
#include "stdio.h"

#include <stm32f4xx.h>
#include <stm32f4xx_flash.h>

int line = 0;

void GPIO_INIT(void);

int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	UartStdOutInit();  //uart初始化
	printf("Chip2:TEXT START\r\n");  

	GPIO_INIT();
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);

	printf("Chip2:Wait for chip1 to send a signal\r\n");  
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) != Bit_SET);
	printf("Chip2:Receive a signal from chip1\r\nChip2:Pull the B12 level high\r\n");  
	
	GPIO_SetBits(GPIOB,GPIO_Pin_12);

	while (1)
	{

	}
	
}

void GPIO_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;      //验证要求不能浮空
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_0;      //B0作为通信脚，chip1检测到B0为高电平以后开始拉高电平
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
