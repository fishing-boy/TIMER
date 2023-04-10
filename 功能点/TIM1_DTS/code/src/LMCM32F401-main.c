#include "uart.h"
#include "stdio.h"

#include <stm32f4xx.h>
#include <stm32f4xx_flash.h>

void TIM_Int_Init(void);

int main()
{
	UartStdOutInit();  //uart初始化
	printf("TEXT START\r\n"); 

	TIM_Int_Init();

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
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB, ENABLE);// 默认复用

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);   //GPIOA

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_TIM1);
	
  	TIM_TimeBaseInitStructure.TIM_Period = 49999;
	TIM_TimeBaseInitStructure.TIM_Prescaler=0;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0x00;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);//初始化TIM1

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性：TIM输出极性高
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_Pulse=10000;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;//经死区后输出高电平
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;//经死区后输出低电平
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设

	TIM_BDTRInitStruct.TIM_OSSRState = TIM_OSSRState_Enable;//运行状态下关闭状态选择
	TIM_BDTRInitStruct.TIM_OSSIState = TIM_OSSIState_Enable;//空闲状态下关闭状态选择
	TIM_BDTRInitStruct.TIM_LOCKLevel = TIM_LOCKLevel_OFF;//锁定配置级别
	TIM_BDTRInitStruct.TIM_DeadTime = 192;  //输出比较信号死区时间配置，具体如何计算可参考BDTR:UTG[7:0]的描述					
	TIM_BDTRInitStruct.TIM_Break = TIM_Break_Disable;//断路控制使能
	//当BKIN引脚--PB12检测到高电平时，输出比较信号被禁止，就好像是刹车一样
	TIM_BDTRInitStruct.TIM_BreakPolarity = TIM_BreakPolarity_High;//断路输入极性-高电平
	TIM_BDTRInitStruct.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;//自动输出使能
	TIM_BDTRConfig(TIM1,&TIM_BDTRInitStruct);

	TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);
 
  	TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPE使能

	TIM_SetCounter(TIM1,0);
	TIM_Cmd(TIM1,ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}
