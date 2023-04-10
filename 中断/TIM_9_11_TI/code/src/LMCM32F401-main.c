#include "uart.h"
#include "stdio.h"

#include <stm32f4xx.h>
#include <stm32f4xx_flash.h>

void TIM_INIT(void);

int EVENT_FLAG = 0;

int main()
{
	UartStdOutInit();  //uart初始化
	printf("TEXT START\r\n"); 

	TIM_INIT();

	TIM9->CR1 |= 0x0001;
	TIM9->EGR = 0x0001;
	TIM9->EGR = 0x0002;
	TIM9->EGR = 0x0004;
	TIM9->EGR = 0x0040;
	TIM9->CR1 = 0;

	TIM10->CR1 |= 0x0001;
	TIM10->EGR = 0x0001;
	TIM10->EGR = 0x0002;
	TIM10->CR1 = 0;


	TIM11->CR1 |= 0x0001;
	TIM11->EGR = 0x0001;
	TIM11->EGR = 0x0002;
	TIM11->CR1 = 0;


	while (1)
	{
		if(EVENT_FLAG)
		{
			printf("Exit interrupt and test ends\r\n");
			UartEndSimulation();
		}
	}
	
}

void TIM_INIT(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC->APB2ENR |= RCC_APB2Periph_TIM9|RCC_APB2Periph_TIM10|RCC_APB2Periph_TIM11;
	
  	TIM_TimeBaseInitStructure.TIM_Period = 1000; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=8400;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0x00;   //这里一定要写0，不然tim1的计数会比别的计数器慢
	
	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInit(TIM10,&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInit(TIM11,&TIM_TimeBaseInitStructure);

	TIM9->DIER  |= 0x0047;
	TIM10->DIER |= 0x0003;
	TIM11->DIER |= 0x0003;

	TIM9 ->SR = 0;
	TIM10->SR = 0;
	TIM11->SR = 0;

	NVIC_InitStructure.NVIC_IRQChannel=TIM1_BRK_TIM9_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_TIM10_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_TRG_COM_TIM11_IRQn;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM1_BRK_TIM9_IRQHandler(void)
{
	TIM9->EGR = 0;
	
	if(TIM9->SR&&0x0001)
	{
		TIM9->SR &= ~0x0001;
		TIM9->DIER &= ~0x0001;
		printf("TIM9 updata open\r\n");
	}

	if(TIM9->SR&&0x0002)
	{
		TIM9->SR &= ~0x0002;
		TIM9->DIER &= ~0x0002;
		printf("TIM9 CC1 open\r\n");
	}

	if(TIM9->SR&&0x0004)
	{
		TIM9->SR &= ~0x0004;
		TIM9->DIER &= ~0x0004;
		printf("TIM9 CC2 open\r\n");
	}

	if(TIM9->SR&&0x0040)
	{
		TIM9->SR &= ~0x0040;
		TIM9->DIER &= ~0x0040;
		printf("TIM9 trig open\r\n");
	}
}

void TIM1_UP_TIM10_IRQHandler(void)
{
	TIM10->EGR = 0;
	
	if(TIM10->SR&&0x0001)
	{
		TIM10->SR &= ~0x0001;
		TIM10->DIER &= ~0x0001;
		printf("TIM10 updata open\r\n");
	}

	if(TIM10->SR&&0x0002)
	{
		TIM10->SR &= ~0x0002;
		TIM10->DIER &= ~0x0002;
		printf("TIM10 CC1 open\r\n");
	}
}

void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
	TIM11->EGR = 0;
	
	if(TIM11->SR&&0x0001)
	{
		TIM11->SR &= ~0x0001;
		TIM11->DIER &= ~0x0001;
		printf("TIM11 updata open\r\n");
	}

	if(TIM11->SR&&0x0002)
	{
		TIM11->SR &= ~0x0002;
		TIM11->DIER &= ~0x0002;
		printf("TIM11 CC1 open\r\n");
	}
}