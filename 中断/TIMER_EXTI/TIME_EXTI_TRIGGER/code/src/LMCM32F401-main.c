#include "uart_REG.h"


#include <stm32f4xx.h>
#include <stm32f4xx_flash.h>
#include "stdio.h"

void Tim5_init_TRGI(void);

int main()
{
	UartStdOutInit();  //uart初始化

	printf("Start testing\r\n");

	Tim5_init_TRGI();
	
	*((int*)0x40000C14) |= 0x0040;

	while (1)
	{
		/* code */
	}
	

}

void Tim5_init_TRGI(void)
{
	/*
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStre;
	NVIC_InitTypeDef NVIC_InitStre;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

	//计数频率100Hz=84/(TIM_Prescaler+1)/(TIM_Period+1)
	TIM_TimeBaseInitStre.TIM_Prescaler=8400-1;//定时器频率84M/(840)=1us
	TIM_TimeBaseInitStre.TIM_Period=50000-1;//周期计数值500 T=200Hz=5ms 
	TIM_TimeBaseInitStre.TIM_ClockDivision=0x00;//不分频
	TIM_TimeBaseInitStre.TIM_CounterMode=TIM_CounterMode_Up ;
	TIM_TimeBaseInitStre.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStre);

	NVIC_InitStre.NVIC_IRQChannel=TIM5_IRQn;
	NVIC_InitStre.NVIC_IRQChannelPreemptionPriority=0x02;
	NVIC_InitStre.NVIC_IRQChannelSubPriority=0x01;
	NVIC_InitStre.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStre);//初始化
	TIM_ITConfig(TIM5,TIM_IT_Trigger,ENABLE);   
	TIM_ClearITPendingBit(TIM5,TIM_IT_Trigger);

	TIM_Cmd(TIM5, ENABLE);//使能
	*/

	*((int*)0x40023840) |= 0x00000008;  //使能TIM5时钟

	*((int*)0x40000C28) = 1;
	*((int*)0x40000C2C) = 1;

	*((int*)0xE000E104) = 0x00040000;

	*((int*)0x40000C0C) |= 0x00000040;
	*((int*)0x40000C10) |= 0x00000040;

	*((int*)0x40000C00) |= 0x00000001;
}


void TIM5_IRQHandler(void)
{
    /*
	if(TIM_GetITStatus(TIM5,TIM_IT_Trigger)==SET)
    {
		printf("Trigger event TEST PASS\r\n");
		TIM_ClearITPendingBit(TIM5,TIM_IT_Trigger);//清除中断
    }*/

	if((*((int*)0x40000C10) & 0x0040) && ((*((int*)0x40000C0C) & 0x0040)))  //TIM1->SR  TIM1->DIER
	{
		printf("Trigger event TEST PASS\r\n");
	}
	*((int*)0x40000C10) = ~0x0040;  
}
