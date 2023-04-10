#include "uart.h"
#include "stdio.h"

#include <stm32f4xx.h>
#include <stm32f4xx_flash.h>

void TIM_Int_Init(void);

int main()
{
	UartStdOutInit();  //uart初始化
	printf("chip1:TEXT START\r\n"); 

	TIM_Int_Init();
	

	while (1)
	{

	}
}

void TIM_Int_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1|RCC_APB2Periph_TIM9,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM4|RCC_APB1Periph_TIM5,ENABLE);
	
  	TIM_TimeBaseInitStructure.TIM_Period = 300-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler=1-1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0x00;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);//初始化TIM1

	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Down;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);

	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_CenterAligned1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);

	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_CenterAligned2;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);

	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_CenterAligned3;
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);

	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Prescaler=10-1;
	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseInitStructure);

	TIM_ClearITPendingBit(TIM9,TIM_IT_Update);
	TIM_ITConfig(TIM9,TIM_IT_Update,ENABLE);

	TIM_SetCounter(TIM1,0);
	TIM_SetCounter(TIM2,0);
	TIM_SetCounter(TIM3,0);
	TIM_SetCounter(TIM4,0);
	TIM_SetCounter(TIM5,0);
	TIM_SetCounter(TIM9,0);
	
	TIM_Cmd(TIM1,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
	TIM_Cmd(TIM4,ENABLE);
	TIM_Cmd(TIM5,ENABLE);
	TIM_Cmd(TIM9,ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel=TIM1_BRK_TIM9_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM1_BRK_TIM9_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM9,TIM_IT_Update)==SET) 
	{
		TIM_ITConfig(TIM9,TIM_IT_Update,DISABLE);
		TIM_ClearITPendingBit(TIM9,TIM_IT_Update);
		printf("Test end\n");
		UartEndSimulation();
	}
}

