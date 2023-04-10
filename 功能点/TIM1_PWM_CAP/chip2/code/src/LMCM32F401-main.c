#include "uart.h"
#include "stdio.h"

#include <stm32f4xx.h>
#include <stm32f4xx_flash.h>

void TIM_Int_Init(void);
void TIM1_INIT_PWM_CAP(void);
void TIM2_pwm_Init(void);

int cap1,cap2;

int flag = 0;

void delay(int i)
{
	while(i--)
	{
		int j = 84;
		while(j--)
		{
			__NOP();
		}
	}

}

int main()
{
	UartStdOutInit();  //uart初始化
	printf("chip2:TEXT START\r\n"); 

	TIM1_INIT_PWM_CAP();
	TIM2_pwm_Init();

	while (1)
	{
		if(flag)
		{
			flag = 0;
			printf("chip2:cap1 = %d,cap2 = %d\r\n",cap1,cap2);
			//delay(100);

			//UartEndSimulation();
		}
	}
}

void TIM1_CC_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_IT_CC1)==SET)
    {
		TIM_ClearITPendingBit(TIM1,TIM_IT_CC1);//清除中断
		cap1 = TIM_GetCapture1(TIM1);   //获取周期值CCR1
  		cap2 = TIM_GetCapture2(TIM1);   //获取高电平时间CCR2
		flag = 1;
	}
}

void TIM1_INIT_PWM_CAP(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStre;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);// 默认复用

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);   //GPIOA

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1);

	TIM_ITConfig(TIM1,TIM_IT_CC1,ENABLE);    
	TIM_ClearITPendingBit(TIM1,TIM_IT_CC1);
	//TIM_ITConfig(TIM1,TIM_IT_CC2,ENABLE);    
	//TIM_ClearITPendingBit(TIM1,TIM_IT_CC2);

	NVIC_InitStre.NVIC_IRQChannel=TIM1_CC_IRQn;
	NVIC_InitStre.NVIC_IRQChannelPreemptionPriority=0x01;
	NVIC_InitStre.NVIC_IRQChannelSubPriority=0x01;
	NVIC_InitStre.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStre);//初始化

  //配置CH1
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;     //上升沿触发
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //IC1直接连接TI1FP1
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;           //对输入的PWM信号不分频
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_PWMIConfig(TIM1, &TIM_ICInitStructure);
  
  //配置CH2
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;      //下降沿触发；
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI; //IC2间接连接TI1FP2
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;				//对输入的PWM信号不分频
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_PWMIConfig(TIM1, &TIM_ICInitStructure);
	
  //选择输入捕获的触发信号；
  TIM_SelectInputTrigger(TIM1, TIM_TS_TI1FP1);		

  // 选择从模式
  // PWM输入模式时，从模式必须工作在复位模式，当捕获开始时，计数器CNT被复位清零；
  TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Reset);
  TIM_SelectMasterSlaveMode(TIM1,TIM_MasterSlaveMode_Enable); 

  // 使能捕获中断，这个中断主要针对的是主捕获通道（TI1FP1）
  TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);	
  // 清除中断标志位
  TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
	
	TIM1->PSC = 84-1;
  // 计数器开始计数
  TIM_Cmd(TIM1, ENABLE);
/*
	//TIM1_CCMR1_CC1S = 01 选择TI1
	TIM1->CCMR1 &= (uint16_t)(~TIM_CCMR1_CC1S);
	TIM1->CCMR1 |= TIM_CCMR1_CC1S_0;

	//CC1P与CC1NP写0选择极性为上升沿有效
	TIM1->CCER &=  (uint16_t)(~TIM_CCER_CC1P);
	TIM1->CCER &=  (uint16_t)(~TIM_CCER_CC1NP);

	//TIM1_CCMR1_CC2S = 10 选择TI1
	TIM1->CCMR1 &= (uint16_t)(~TIM_CCMR1_CC2S);
	TIM1->CCMR1 |= TIM_CCMR1_CC2S_1;

	//CC2P与CC2NP写1选择极性为下降沿有效
	TIM1->CCER |=  (uint16_t)(TIM_CCER_CC2P);
	TIM1->CCER |=  (uint16_t)(TIM_CCER_CC2NP);

	//配置定时器的输入通道以及从模式
	TIM1->SMCR &= (uint16_t)(~TIM_SMCR_TS);
	TIM1->SMCR |= TIM_TS_TI1FP1;
	TIM1->SMCR &= (uint16_t)(~TIM_SMCR_SMS);
	TIM1->SMCR |= TIM_SlaveMode_Reset;
	TIM1->SMCR |= TIM_MasterSlaveMode_Enable;

	TIM1->CCER &= (uint16_t)(~TIM_CCER_CC1E);
	TIM1->CCER &= (uint16_t)(~TIM_CCER_CC2E);
	TIM1->CCER |= TIM_CCER_CC1E|TIM_CCER_CC2E;
*/
	//TIM1->PSC = 84-1;

	//TIM1->CR2 |= 0x00000001;

	//TIM1->CR1 |= 0x00000001;
}

void TIM2_pwm_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);// 默认复用

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);   //GPIOA

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_TIM2);
	
  	TIM_TimeBaseInitStructure.TIM_Period = 4000-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler=84-1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0x00;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//初始化TIM1

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性：TIM输出极性高
	TIM_OCInitStructure.TIM_Pulse=2000;
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设

	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);
 
  	TIM_ARRPreloadConfig(TIM2,ENABLE);//ARPE使能

	TIM_SetCounter(TIM2,0);
	TIM_Cmd(TIM2,ENABLE);
}
