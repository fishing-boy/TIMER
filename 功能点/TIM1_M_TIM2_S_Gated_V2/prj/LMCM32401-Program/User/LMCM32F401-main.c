#include "uart_REG.h"
#include "stdio.h"

#include <stm32f4xx.h>
#include <stm32f4xx_flash.h>

void Tim1_init_M(void);
void Tim2_init_S(void);

int endFlag = 0;
int num = 0;

int main()
{
//	UartStdOutInit();  //uart初始化
//	printf("TEXT START\r\n");  

	Tim1_init_M();
	Tim2_init_S();
	
	TIM_Cmd(TIM1, ENABLE);
	TIM_Cmd(TIM2, ENABLE);

	//printf("OPEN TIM2\r\n");
	

	//printf("OPEN TIM\r\n");
	//printf("%d\r\n",TIM1->CCR1);

	//printf("%d\r\n",TIM2->CNT);

	endFlag = TIM2->CNT;
	while (1)
	{
		if(num ==100)
		{
			printf("test end\r\n");
			printf("%d\r\n",endFlag);
			//UartEndSimulation();
		}
	}
}

void Tim1_init_M(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStre;
	//NVIC_InitTypeDef NVIC_InitStre;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB, ENABLE);// 默认复用

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);   //GPIOA

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1);

	TIM_TimeBaseInitStre.TIM_Prescaler=84-1;
	TIM_TimeBaseInitStre.TIM_Period=500-1;
	TIM_TimeBaseInitStre.TIM_ClockDivision=0x00;//不分频
	TIM_TimeBaseInitStre.TIM_CounterMode=TIM_CounterMode_Up ;
	TIM_TimeBaseInitStre.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStre);

	TIM1->CR2 &= (uint16_t)~TIM_CR2_MMS;
  	TIM1->CR2 |=  TIM_TRGOSource_OC1Ref;

	//初始化TIM2 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性：TIM输出极性高
	TIM_OCInitStructure.TIM_Pulse=250;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM2在CCR2上的预装载寄存器
	TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPE使能
	TIM_CtrlPWMOutputs(TIM1, ENABLE);

	// TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);    
	// TIM_ClearITPendingBit(TIM1,TIM_IT_Update);

	// NVIC_InitStre.NVIC_IRQChannel=TIM1_UP_TIM10_IRQn;
	// NVIC_InitStre.NVIC_IRQChannelPreemptionPriority=0x01;
	// NVIC_InitStre.NVIC_IRQChannelSubPriority=0x01;
	// NVIC_InitStre.NVIC_IRQChannelCmd=ENABLE;
	// NVIC_Init(&NVIC_InitStre);//初始化
}

void Tim2_init_S(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStre;
	NVIC_InitTypeDef NVIC_InitStre;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitStre.TIM_Prescaler=10-1;
	TIM_TimeBaseInitStre.TIM_Period=50-1;
	TIM_TimeBaseInitStre.TIM_ClockDivision=0x00;//不分频
	TIM_TimeBaseInitStre.TIM_CounterMode=TIM_CounterMode_Up ;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStre);

	//定时器2配置为从模式,使用ITR0作为内部触发
	TIM2->SMCR &= (uint16_t)~TIM_SMCR_TS;
	TIM2->SMCR |= TIM_TS_ITR0;

	// //定时器2的时钟将由定时器1周期性触发信号的上升沿提供
	TIM2->SMCR &= (uint16_t)~TIM_SMCR_SMS;
	TIM2->SMCR |= TIM_SlaveMode_Gated;

	TIM_ARRPreloadConfig(TIM2, ENABLE);//使能自动装载
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);    
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);

	NVIC_InitStre.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStre.NVIC_IRQChannelPreemptionPriority=0x02;
	NVIC_InitStre.NVIC_IRQChannelSubPriority=0x01;
	NVIC_InitStre.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStre);//初始化
}

void TIM1_UP_TIM10_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET)
    {
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);//清除中断	
		num++;
    }
}

void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)
    {
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//清除中断
		num++;
		if(num ==100)
		{
			// TIM_Cmd(TIM2, DISABLE);
		}
		//printf("TIM2\r\n");
    }
}
