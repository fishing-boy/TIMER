#include "uart_REG.h"


#include <stm32f4xx.h>
#include <stm32f4xx_flash.h>
#include "stdio.h"

void TIM1_EXTI(void);


int main()
{
	UartStdOutInit();  //uart初始化

	TIM1_EXTI();

	*((int*)0x40010014) |= 0x0001;

	while (1)
	{
		/* code */
	}
	

}

void TIM1_EXTI(void)
{
//TIM配置
	*((int*)0x40023844) |= 0x00000001;  //APB2ENR 使能TIM1时钟

	*((int*)0X40010000) |= 0x0020;      //CR1 
	*((int*)0X4001002C)  = 1;         //ARR 自动重载
	*((int*)0x40010028)  = 1;         //PSC 预分频
	*((int*)0x40010030) &= 0x00000000;  //RCR 重复计数
	*((int*)0x40010014) |= 0x0001;      //EGR 重新初始化计数器并生成一个寄存器更新事件
	*((int*)0x40010010)  = ~0x0001;     //SR  清除更新中断请求位
	*((int*)0x4001000C) |= 0x0001;      //DIER允许定时器1更新中断
	*((int*)0X40010000) |= 0x0001;      //CR1 使能定时器1
	*((int*)0x40010010)  = ~0x0001;     //SR  清除更新中断请求位
	
	//中断配置
	//*((int*)0xE000E418)  = 0X00006000;    //IPR6
	*((int*)0XE000E100)  = 0x02000000;    //NVIC->ISER0
}

//定时器1中断服务函数
void TIM1_UP_TIM10_IRQHandler(void)
{
	if((*((int*)0x40010010) & 0x0001) && ((*((int*)0x4001000C) & 0x0001)))  //TIM1->SR  TIM1->DIER
	{
		printf("TIM1 EXTI TEST:UPDATA PASS\r\n");
		*((int*)0X40010000) &= 0xFFFFFFFE;      //CR1
	}
	*((int*)0x40010010) = ~0x0001;  //TIM1->SR  清除中断标志位
}
