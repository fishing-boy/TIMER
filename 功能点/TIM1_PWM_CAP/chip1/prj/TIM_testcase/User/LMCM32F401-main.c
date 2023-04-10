#include "uart.h"
#include "stdio.h"

#include <stm32f4xx.h>
#include <stm32f4xx_flash.h>

void TIM_Int_Init(void);

int main()
{
	GPIO_InitTypeDef GPIO_InitStructure;
//	UartStdOutInit();  //uart初始化
//	printf("chip1:TEXT START\r\n"); 

	TIM_Int_Init();
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB, ENABLE);// 默认复用
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
//	//GPIO_Init(GPIOA, &GPIO_InitStructure);   //GPIOA
//	GPIO_Init(GPIOB, &GPIO_InitStructure);   //GPIOA
//	//GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_TIM1);
//	GPIO_PinAFConfig(GPIOB,GPIO_PinSource12,GPIO_AF_TIM1);
//	
//	TIM1->BDTR = 0xfcff;
//	printf("TIM1->BDTR = 0x%08x\r\n",TIM1->BDTR);
	

	while (1)
	{

	}
}

void TIM_Int_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_BDTRInitTypeDef TIM_BDTRInitStruct;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);// 默认复用

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);   //GPIOA

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1);
	
  	TIM_TimeBaseInitStructure.TIM_Period =300-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler=0;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0x00;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);//初始化TIM1

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性：TIM输出极性高
	TIM_OCInitStructure.TIM_Pulse=150-1;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设

	TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);
 
  	TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPE使能

	TIM_SetCounter(TIM1,0);
	TIM_Cmd(TIM1,ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}
