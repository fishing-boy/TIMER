#include "uart_REG.h"
#include "stdio.h"

#include <stm32f4xx.h>
#include <stm32f4xx_flash.h>

int endFlag = 0;

void 	Tim1_init_M(void);
void	Tim4_init_S(void);

int a,b,c;

int main()
{
	UartStdOutInit();  //uart初始化
	printf("TEXT START\r\n");  

	Tim1_init_M();
	Tim4_init_S();
	
	TIM_Cmd(TIM4, ENABLE);
	
	int i = 50;
	while (i--);

	// int cnt_tim4[10] = {0};

	// cnt_tim4[0] = TIM4->CNT;
	// cnt_tim4[1] = TIM4->CNT;
	// cnt_tim4[2] = TIM4->CNT;
	// cnt_tim4[3] = TIM4->CNT;
	// cnt_tim4[4] = TIM4->CNT;

	printf("%d\r\n",TIM4->CNT);
	printf("%d\r\n",TIM4->CNT);
	printf("%d\r\n",TIM4->CNT);
	printf("%d\r\n",TIM4->CNT);
	printf("%d\r\n",TIM4->CNT);

	printf("OPEN TIM2\r\n");  
	TIM_Cmd(TIM2, ENABLE);

	i = 50;
	while (i--);

	// cnt_tim4[5] = TIM4->CNT;
	// cnt_tim4[6] = TIM4->CNT;
	// cnt_tim4[7] = TIM4->CNT;
	// cnt_tim4[8] = TIM4->CNT;
	// cnt_tim4[9] = TIM4->CNT;
	printf("%d\r\n",TIM4->CNT);
	printf("%d\r\n",TIM4->CNT);
	printf("%d\r\n",TIM4->CNT);
	printf("%d\r\n",TIM4->CNT);
	printf("%d\r\n",TIM4->CNT);

	//printf("%d %d %d %d %d\r\n",cnt_tim4[0],cnt_tim4[1],cnt_tim4[2],cnt_tim4[3],cnt_tim4[4]);
	//printf("%d %d %d %d %d\r\n",cnt_tim4[5],cnt_tim4[6],cnt_tim4[7],cnt_tim4[8],cnt_tim4[9]);
	printf("TEXT END\r\n");  
	UartEndSimulation();

	while (1)
	{
	}
}

void Tim1_init_M(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStre;
	NVIC_InitTypeDef NVIC_InitStre;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitStre.TIM_Prescaler=10-1;
	TIM_TimeBaseInitStre.TIM_Period=50-1;
	TIM_TimeBaseInitStre.TIM_ClockDivision=0x00;//不分频
	TIM_TimeBaseInitStre.TIM_CounterMode=TIM_CounterMode_Up ;
	TIM_TimeBaseInitStre.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStre);

	//TIM1_CR2寄存器中写入 MMS=010,则每次生成更新事件时,TRGO1都会输出一个上升沿。
	//TIM1->CR2 &= (uint16_t)~TIM_CR2_MMS;
  	//TIM1->CR2 |=  TIM_TRGOSource_Enable;


	//TIM_ARRPreloadConfig(TIM1, ENABLE);//使能自动装载
	//TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);    
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);

	NVIC_InitStre.NVIC_IRQChannel=TIM1_UP_TIM10_IRQn;
	NVIC_InitStre.NVIC_IRQChannelPreemptionPriority=0x01;
	NVIC_InitStre.NVIC_IRQChannelSubPriority=0x01;
	NVIC_InitStre.NVIC_IRQChannelCmd=ENABLE;
	//NVIC_Init(&NVIC_InitStre);//初始化

	TIM_TimeBaseInitStre.TIM_Prescaler=10-1;
	TIM_TimeBaseInitStre.TIM_Period=50-1;
	TIM_TimeBaseInitStre.TIM_ClockDivision=0x00;//不分频
	TIM_TimeBaseInitStre.TIM_CounterMode=TIM_CounterMode_Up ;
	TIM_TimeBaseInitStre.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStre);

	TIM2->CR2 &= (uint16_t)~TIM_CR2_MMS;
  	TIM2->CR2 |=  TIM_TRGOSource_Enable;

	//TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);    
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);

	NVIC_InitStre.NVIC_IRQChannel=TIM2_IRQn;
	//NVIC_Init(&NVIC_InitStre);//初始化
}

void Tim4_init_S(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStre;
	NVIC_InitTypeDef NVIC_InitStre;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	TIM_TimeBaseInitStre.TIM_Prescaler=8400-1;
	TIM_TimeBaseInitStre.TIM_Period=50000-1;
	TIM_TimeBaseInitStre.TIM_ClockDivision=0x00;//不分频
	TIM_TimeBaseInitStre.TIM_CounterMode=TIM_CounterMode_Up ;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStre);

	//定时器2配置为从模式,使用ITR0作为内部触发
	TIM4->SMCR &= (uint16_t)~TIM_SMCR_TS;
	TIM4->SMCR |= TIM_TS_ITR1;

	//定时器2的时钟将由定时器1周期性触发信号的上升沿提供
	TIM4->SMCR &= (uint16_t)~TIM_SMCR_SMS;
	TIM4->SMCR |= TIM_TRGOSource_Enable;

	printf("TIM4->SMCR = %08x\r\n",TIM4->SMCR);

	TIM4->CR1 &= (uint16_t)~TIM_CR1_URS;

	//TIM_ARRPreloadConfig(TIM4, ENABLE);//使能自动装载
	//TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);    
	//TIM_ClearITPendingBit(TIM4,TIM_IT_Update);

	NVIC_InitStre.NVIC_IRQChannel=TIM4_IRQn;
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
		//a++;
		//printf("%d\r\n",TIM4->CNT);
		//TIM_Cmd(TIM1, DISABLE);
    }
}

void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)
    {
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//清除中断
		printf("2\r\n");
		if(b == 1)
		{
			TIM_Cmd(TIM1, ENABLE);
		}
			
		b++;
		
		if(b==9)
			TIM_Cmd(TIM2, DISABLE);
    }
}

void TIM4_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)
    {
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);//清除中断
		c++;
		//printf("4\r\n");
		//TIM_Cmd(TIM3, DISABLE);
    }
}
