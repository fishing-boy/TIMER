#include "uart_REG.h"
#include "stdio.h"

#include <stm32f4xx.h>
#include <stm32f4xx_flash.h>

void Tim1_init(void);
void Tim2_3_4_5_init(void);

void Tim1_init_M(void);
void Tim2_init_S(void);

int endFlag = 0;
int num = 0;

int main()
{
	UartStdOutInit();  //uart初始化
	printf("TEXT START\r\n");  

	Tim1_init_M();
	Tim2_init_S();
	
	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM1, ENABLE);

	while (1)
	{
		if(endFlag)
		{
			
		}
	}
}

void Tim1_init_M(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStre;
	NVIC_InitTypeDef NVIC_InitStre;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	TIM_TimeBaseInitStre.TIM_Prescaler=100-1;
	TIM_TimeBaseInitStre.TIM_Period=100-1;
	TIM_TimeBaseInitStre.TIM_ClockDivision=0x00;//不分频
	TIM_TimeBaseInitStre.TIM_CounterMode=TIM_CounterMode_Up ;
	TIM_TimeBaseInitStre.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStre);

	//TIM1_CR2寄存器中写入 MMS=010,则每次生成更新事件时,TRGO1都会输出一个上升沿。
	TIM1->CR2 &= (uint16_t)~TIM_CR2_MMS;
  	TIM1->CR2 |=  TIM_TRGOSource_Update;


	//TIM_ARRPreloadConfig(TIM1, ENABLE);//使能自动装载
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);    
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);

	NVIC_InitStre.NVIC_IRQChannel=TIM1_UP_TIM10_IRQn;
	NVIC_InitStre.NVIC_IRQChannelPreemptionPriority=0x01;
	NVIC_InitStre.NVIC_IRQChannelSubPriority=0x01;
	NVIC_InitStre.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStre);//初始化
}

void Tim2_init_S(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStre;
	NVIC_InitTypeDef NVIC_InitStre;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitStre.TIM_Prescaler=1-1;
	TIM_TimeBaseInitStre.TIM_Period=50-1;
	TIM_TimeBaseInitStre.TIM_ClockDivision=0x00;//不分频
	TIM_TimeBaseInitStre.TIM_CounterMode=TIM_CounterMode_Up ;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStre);

	//定时器2配置为从模式,使用ITR0作为内部触发
	TIM2->SMCR &= (uint16_t)~TIM_SMCR_TS;
	TIM2->SMCR |= TIM_TS_ITR0;

	//定时器2的时钟将由定时器1周期性触发信号的上升沿提供
	TIM2->SMCR &= (uint16_t)~TIM_SMCR_SMS;
	TIM2->SMCR |= TIM_SlaveMode_External1;

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
		//TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);   
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//清除中断
		endFlag = 1;
		//TIM_Cmd(TIM2, DISABLE);
		//TIM_Cmd(TIM1, DISABLE);
		if(num == 50)
		{
			printf("test pass,num = %d,add = %d\r\n",num,TIM2->ARR);
		}
		else{
			printf("test fail,num = %d,add = %d\r\n",num,TIM2->ARR);
		}
		UartEndSimulation();
    }
}

/*
void Tim1_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStre;
	NVIC_InitTypeDef NVIC_InitStre;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	TIM_TimeBaseInitStre.TIM_Prescaler=10-1;
	TIM_TimeBaseInitStre.TIM_Period=50-1;
	TIM_TimeBaseInitStre.TIM_ClockDivision=0x00;//不分频
	TIM_TimeBaseInitStre.TIM_CounterMode=TIM_CounterMode_Up ;
	TIM_TimeBaseInitStre.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStre);

	TIM_SelectSlaveMode(TIM1,TIM_SlaveMode_Reset);
	TIM_SelectInputTrigger(TIM1,TIM_TS_ITR0);//设置输入触发源

	TIM_ARRPreloadConfig(TIM1, ENABLE);//使能自动装载
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);    
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);

	TIM_Cmd(TIM1, ENABLE);//使能时钟

	NVIC_InitStre.NVIC_IRQChannel=TIM1_UP_TIM10_IRQn;
	NVIC_InitStre.NVIC_IRQChannelPreemptionPriority=0x02;
	NVIC_InitStre.NVIC_IRQChannelSubPriority=0x01;
	NVIC_InitStre.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStre);//初始化
}

void Tim2_3_4_5_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStre;
	NVIC_InitTypeDef NVIC_InitStre;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM4|RCC_APB1Periph_TIM5, ENABLE);

	TIM_TimeBaseInitStre.TIM_Prescaler=10-1;
	TIM_TimeBaseInitStre.TIM_Period=50-1;
	TIM_TimeBaseInitStre.TIM_ClockDivision=0x00;//不分频
	TIM_TimeBaseInitStre.TIM_CounterMode=TIM_CounterMode_Up ;
	TIM_TimeBaseInitStre.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStre);
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStre);
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStre);
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStre);

	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Reset);//设置输入触发源
	TIM_SelectMasterSlaveMode(TIM2,TIM_MasterSlaveMode_Enable);//使能主从模式
	TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Reset);//设置输入触发源
	TIM_SelectMasterSlaveMode(TIM3,TIM_MasterSlaveMode_Enable);//使能主从模式
	TIM_SelectOutputTrigger(TIM4, TIM_TRGOSource_Reset);//设置输入触发源
	TIM_SelectMasterSlaveMode(TIM4,TIM_MasterSlaveMode_Enable);//使能主从模式
	TIM_SelectOutputTrigger(TIM5, TIM_TRGOSource_Reset);//设置输入触发源
	TIM_SelectMasterSlaveMode(TIM5,TIM_MasterSlaveMode_Enable);//使能主从模式

	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);    
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);    
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);    
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);    
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);

	NVIC_InitStre.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStre.NVIC_IRQChannelPreemptionPriority=0x02;
	NVIC_InitStre.NVIC_IRQChannelSubPriority=0x01;
	NVIC_InitStre.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStre);//初始化

	NVIC_InitStre.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_Init(&NVIC_InitStre);//初始化

	NVIC_InitStre.NVIC_IRQChannel=TIM4_IRQn;
	NVIC_Init(&NVIC_InitStre);//初始化

	NVIC_InitStre.NVIC_IRQChannel=TIM5_IRQn;
	NVIC_Init(&NVIC_InitStre);//初始化
}

void TIM1_UP_TIM10_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET)
    {
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);//清除中断
		TIM_Cmd(TIM1, DISABLE);
		printf("TIM1 UPDATA,CLOSE TIM1\r\n");
    }
}

void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)
    {
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//清除中断
		TIM_Cmd(TIM2, DISABLE);
		printf("TIM2 UPDATA,CLOSE TIM2\r\n");
    }
}

void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)
    {
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);//清除中断
		TIM_Cmd(TIM3, DISABLE);
		printf("TIM3 UPDATA,CLOSE TIM3\r\n");
    }
}

void TIM4_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)
    {
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);//清除中断
		TIM_Cmd(TIM4, DISABLE);
		printf("TIM4 UPDATA,CLOSE TIM4\r\n");
    }
}

void TIM5_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM5,TIM_IT_Update)==SET)
    {
		TIM_ClearITPendingBit(TIM5,TIM_IT_Update);//清除中断
		TIM_Cmd(TIM5, DISABLE);
		printf("TIM5 UPDATA,CLOSE TIM5\r\n");
    }
}
*/
