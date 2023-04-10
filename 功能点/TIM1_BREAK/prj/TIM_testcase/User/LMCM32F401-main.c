/*#include "uart.h"
#include "stdio.h"

#include <stm32f4xx.h>
#include <stm32f4xx_flash.h>

void TIM_Int_Init(void);

int main()
{
	UartStdOutInit();  //uart初始化
	printf("TEXT START\r\n"); 

	TIM_Int_Init();
	GPIO_SetBits(GPIOA,GPIO_Pin_0);

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

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_12; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource12,GPIO_AF_TIM1);
	
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
	TIM_BDTRInitStruct.TIM_Break = TIM_Break_Enable;//断路控制使能
	TIM_BDTRInitStruct.TIM_BreakPolarity = TIM_BreakPolarity_High;//断路输入极性-高电平
	TIM_BDTRInitStruct.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;//自动输出使能
	TIM_BDTRConfig(TIM1,&TIM_BDTRInitStruct);

	TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);
 
  	TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPE使能

	TIM_SetCounter(TIM1,0);
	TIM_Cmd(TIM1,ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}*/

/**
 * @file       LMCM32F401-main.c
 * @brief      该文件主要功能是测试GPIO
 * @details    这个是测试Doxygen
 * @author     guowk
 * @warning    无
 * @version    V1.0
 * @date       2022-11-23
 * @copyright  卢米微电子(南京)有限责任公司
 */

#include <stm32f4xx.h>
#include <stm32f4xx_flash.h>

void PWM_OC_IN(void);

int main()
{
	//PWM_OC_IN();
	
	UartStdOutInit();

	PWM_OC_IN();
	
	// printf("TIM1->ARR = 0x%08x\r\n",TIM1->ARR); 
	
	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 
	
	// printf("TIM1->ARR = 0x%08x\r\n",TIM1->ARR); 
	
	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, DISABLE); 
	
	// printf("TIM1->ARR = 0x%08x\r\n",TIM1->ARR); 

	while(1)
	{

	}
}

void PWM_OC_IN(void)
{
	GPIO_InitTypeDef          GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
	TIM_OCInitTypeDef         TIM_OCInitStructure;
	TIM_BDTRInitTypeDef       TIM_BDTRInitStructure;
	NVIC_InitTypeDef          NVIC_InitStructure;
	
	//时钟配置
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB, ENABLE);// 默认复用
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 

	//复用管脚配置--输出比较通道
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);   //GPIOA
	
	//输出比较通道刹车通道
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉 
	GPIO_Init(GPIOB, &GPIO_InitStructure);   //GPIOB
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);//默认输出低电平

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource12,GPIO_AF_TIM1);
	
	//时基配置
	TIM_TimeBaseStructure.TIM_Prescaler = 8400-1;   //预分频 --72MHZ来驱动
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period = 1000 - 1;     //TimerPeriod;    //ARR (占空比CCR/AAR)10kHz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;   //定时器滤波，通常为0，这里死区时间用到
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;//配置寄存器TIM_RCR（重复次数寄存器） 
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	//输出比较功能配置
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//比较输出使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//比较输出互补输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//!!!!!!高电平有效!!!!!
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//!!!!!!高电平有效!!!!!
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;//经死区后输出高电平
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;//经死区后输出低电平
	TIM_OCInitStructure.TIM_Pulse = 250 - 1;;   //Channel1Pulse(CCR);
	TIM_OC1Init(TIM1, &TIM_OCInitStructure); //通道1
	TIM_OC2Init(TIM1, &TIM_OCInitStructure); //通道1
	TIM_OC3Init(TIM1, &TIM_OCInitStructure); //通道1
	TIM_OC4Init(TIM1, &TIM_OCInitStructure); //通道1
	
	//使能TIM的ARR和CRR，以及使能TIM定时器，开启pwm输出
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Disable);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Disable);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Disable);
	
	//刹车和死区结构体初始化
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;//运行状态下关闭状态选择
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;//空闲状态下关闭状态选择
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;//锁定配置级别
	TIM_BDTRInitStructure.TIM_DeadTime = 0x80;  //输出比较信号死区时间配置，具体如何计算可参考BDTR:UTG[7:0]的描述
												//这里配置的死区时间为152ns
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;//断路控制使能
	//当BKIN引脚--PB12检测到高电平时，输出比较信号被禁止，就好像是刹车一样
	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_Low;//断路输入极性-高电平
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;//自动输出使能
	TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);

	TIM_ITConfig(TIM1,TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4,ENABLE);
	TIM_ITConfig(TIM1,TIM_IT_Break,ENABLE);
		
	TIM_Cmd(TIM1, ENABLE);//计数使能
	TIM_CtrlPWMOutputs(TIM1, ENABLE);//主输出使能，当使用的是通用定时器时，这句话不要

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);	
}

void TIM1_BRK_TIM9_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_IT_Break)==SET) //溢出中断
	{
		//TIM_ClearITPendingBit(TIM1,TIM_IT_Break);  //清除中断标志位
	}

}

