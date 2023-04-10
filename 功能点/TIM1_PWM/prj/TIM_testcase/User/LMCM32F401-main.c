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
	TIM_OCInitTypeDef TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);// 默认复用

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);   //GPIOA

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1);
	
  	TIM_TimeBaseInitStructure.TIM_Period = 100-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler=1-1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_CenterAligned1;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0x00;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);//初始化TIM1

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性：TIM输出极性高
	TIM_OCInitStructure.TIM_Pulse=50-1;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设

	TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);
 
  	TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPE使能

	TIM_ClearITPendingBit(TIM1,TIM_IT_CC1);
	TIM_ITConfig(TIM1,TIM_IT_CC1,ENABLE);

	TIM_SetCounter(TIM1,0);
	TIM_Cmd(TIM1,ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel=TIM1_CC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM1_CC_IRQHandler(void)
{
	printf("1\r\n");
	TIM_ClearITPendingBit(TIM1,TIM_IT_CC1);
}
