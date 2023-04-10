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
	
	//TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM2, ENABLE);

	printf("%d\r\n",TIM2->CNT);
	printf("%d\r\n",TIM2->CNT);
	printf("%d\r\n",TIM2->CNT);
	printf("%d\r\n",TIM2->CNT);
	printf("%d\r\n",TIM2->CNT);

	printf("OPEN TIM1\r\n");
	TIM_Cmd(TIM1, ENABLE);

	printf("%d\r\n",TIM2->CNT);
	printf("%d\r\n",TIM2->CNT);
	printf("%d\r\n",TIM2->CNT);
	printf("%d\r\n",TIM2->CNT);
	printf("%d\r\n",TIM2->CNT);

	TIM_Cmd(TIM1, DISABLE);
	TIM_Cmd(TIM1, ENABLE);

	printf("%d\r\n",TIM2->CNT);
	printf("%d\r\n",TIM2->CNT);
	printf("%d\r\n",TIM2->CNT);
	printf("%d\r\n",TIM2->CNT);
	printf("%d\r\n",TIM2->CNT);


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
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_TimeBaseInitStre.TIM_Prescaler=100-1;
	TIM_TimeBaseInitStre.TIM_Period=100-1;
	TIM_TimeBaseInitStre.TIM_ClockDivision=0x00;//不分频
	TIM_TimeBaseInitStre.TIM_CounterMode=TIM_CounterMode_Up ;
	TIM_TimeBaseInitStre.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStre);

	//TIM1_CR2寄存器中写入 MMS=010,则每次生成更新事件时,TRGO1都会输出一个上升沿。
	TIM1->CR2 &= (uint16_t)~TIM_CR2_MMS;
  	TIM1->CR2 |=  TIM_TRGOSource_Enable;


	//TIM_ARRPreloadConfig(TIM1, ENABLE);//使能自动装载
	//TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);    
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);

	NVIC_InitStre.NVIC_IRQChannel=TIM1_UP_TIM10_IRQn;
	NVIC_InitStre.NVIC_IRQChannelPreemptionPriority=0x01;
	NVIC_InitStre.NVIC_IRQChannelSubPriority=0x01;
	NVIC_InitStre.NVIC_IRQChannelCmd=ENABLE;
	//NVIC_Init(&NVIC_InitStre);//初始化
}

void Tim2_init_S(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStre;
	NVIC_InitTypeDef NVIC_InitStre;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitStre.TIM_Prescaler=1000-1;
	TIM_TimeBaseInitStre.TIM_Period=5000-1;
	TIM_TimeBaseInitStre.TIM_ClockDivision=0x00;//不分频
	TIM_TimeBaseInitStre.TIM_CounterMode=TIM_CounterMode_Up ;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStre);

	//定时器2配置为从模式,使用ITR0作为内部触发
	TIM2->SMCR &= (uint16_t)~TIM_SMCR_TS;
	TIM2->SMCR |= TIM_TS_ITR0;

	TIM2->SMCR &= (uint16_t)~TIM_SMCR_SMS;
	TIM2->SMCR |= TIM_TRGOSource_Enable;

	TIM2->CR1 &= (uint16_t)~TIM_CR1_URS;

	TIM_ARRPreloadConfig(TIM2, ENABLE);//使能自动装载
	//TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);    
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);

	NVIC_InitStre.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStre.NVIC_IRQChannelPreemptionPriority=0x02;
	NVIC_InitStre.NVIC_IRQChannelSubPriority=0x01;
	NVIC_InitStre.NVIC_IRQChannelCmd=ENABLE;
	//NVIC_Init(&NVIC_InitStre);//初始化
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
