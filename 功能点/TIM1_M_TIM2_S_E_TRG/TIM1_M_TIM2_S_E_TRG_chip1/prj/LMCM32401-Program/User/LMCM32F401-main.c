#include "delay.h"
#include "uart_REG.h"
#include "stdio.h"

#include <stm32f4xx.h>
#include <stm32f4xx_flash.h>

int main()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	UartStdOutInit();  //uart初始化
	printf("Chip1:TEST START\r\n"); 

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;      //验证要求不能浮空
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	printf("Chip1:Wait Chip2\r\n");
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0) == Bit_RESET);
	printf("Chip1:Wait End,Start Set PA8\r\n");

	GPIO_SetBits(GPIOA,GPIO_Pin_8);

	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0) != Bit_RESET);

	printf("Chip1:Test End\r\n"); 
	UartEndSimulation();

	while (1)
	{
		/* code */
	}
}
