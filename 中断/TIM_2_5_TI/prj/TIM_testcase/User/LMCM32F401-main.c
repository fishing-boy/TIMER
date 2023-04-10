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

	TIM2->CR1 |= 0x0001;
	TIM2->EGR = 0x0001;
	TIM2->EGR = 0x0002;
	TIM2->EGR = 0x0004;
	TIM2->EGR = 0x0008;
	TIM2->EGR = 0x0010;
	TIM2->EGR = 0x0040;
	TIM2->CR1 = 0;


	TIM3->CR1 |= 0x0001;
	TIM3->EGR = 0x0001;
	TIM3->EGR = 0x0002;
	TIM3->EGR = 0x0004;
	TIM3->EGR = 0x0008;
	TIM3->EGR = 0x0010;
	TIM3->EGR = 0x0040;
	TIM3->CR1 = 0;

	TIM4->CR1 |= 0x0001;
	TIM4->EGR = 0x0001;
	TIM4->EGR = 0x0002;
	TIM4->EGR = 0x0004;
	TIM4->EGR = 0x0008;
	TIM4->EGR = 0x0010;
	TIM4->EGR = 0x0040;
	TIM4->CR1 = 0;

	TIM5->CR1 |= 0x0001;
	TIM5->EGR = 0x0001;
	TIM5->EGR = 0x0002;
	TIM5->EGR = 0x0004;
	TIM5->EGR = 0x0008;
	TIM5->EGR = 0x0010;
	TIM5->EGR = 0x0040;
	TIM5->CR1 = 0;


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
	
	RCC->APB1ENR |= RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM4|RCC_APB1Periph_TIM5;
	
  	TIM_TimeBaseInitStructure.TIM_Period = 1000; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=8400;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0x00;   //这里一定要写0，不然tim1的计数会比别的计数器慢
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);

	TIM2->DIER |= 0x005F;
	TIM3->DIER |= 0x005F;
	TIM4->DIER |= 0x005F;
	TIM5->DIER |= 0x005F;

	TIM2->SR = 0;
	TIM3->SR = 0;
	TIM4->SR = 0;
	TIM5->SR = 0;
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM2_IRQHandler(void)
{
	TIM2->EGR = 0;
	
	if(TIM2->SR&&0x0001)
	{
		TIM2->SR &= ~0x0001;
		TIM2->DIER &= ~0x0001;
		printf("TIM2 updata open\r\n");
	}

	if(TIM2->SR&&0x0002)
	{
		TIM2->SR &= ~0x0002;
		TIM2->DIER &= ~0x0002;
		printf("TIM2 CC1 open\r\n");
	}

	if(TIM2->SR&&0x0004)
	{
		TIM2->SR &= ~0x0004;
		TIM2->DIER &= ~0x0004;
		printf("TIM2 CC2 open\r\n");
	}

	if(TIM2->SR&&0x0008)
	{
		TIM2->SR &= ~0x0008;
		TIM2->DIER &= ~0x0008;
		printf("TIM2 CC3 open\r\n");
	}

	if(TIM2->SR&&0x0010)
	{
		TIM2->SR &= ~0x0010;
		TIM2->DIER &= ~0x0010;
		printf("TIM2 CC4 open\r\n");
	}

	if(TIM2->SR&&0x0040)
	{
		TIM2->SR &= ~0x0040;
		TIM2->DIER &= ~0x0040;
		printf("TIM2 trig open\r\n");
	}
}

void TIM3_IRQHandler(void)
{
	TIM3->EGR = 0;
	
	if(TIM3->SR&&0x0001)
	{
		TIM3->SR &= ~0x0001;
		TIM3->DIER &= ~0x0001;
		printf("TIM3 updata open\r\n");
	}

	if(TIM3->SR&&0x0002)
	{
		TIM3->SR &= ~0x0002;
		TIM3->DIER &= ~0x0002;
		printf("TIM3 CC1 open\r\n");
	}

	if(TIM3->SR&&0x0004)
	{
		TIM3->SR &= ~0x0004;
		TIM3->DIER &= ~0x0004;
		printf("TIM3 CC2 open\r\n");
	}

	if(TIM3->SR&&0x0008)
	{
		TIM3->SR &= ~0x0008;
		TIM3->DIER &= ~0x0008;
		printf("TIM3 CC3 open\r\n");
	}

	if(TIM3->SR&&0x0010)
	{
		TIM3->SR &= ~0x0010;
		TIM3->DIER &= ~0x0010;
		printf("TIM3 CC4 open\r\n");
	}

	if(TIM3->SR&&0x0040)
	{
		TIM3->SR &= ~0x0040;
		TIM3->DIER &= ~0x0040;
		printf("TIM3 trig open\r\n");
	}
}

void TIM4_IRQHandler(void)
{
	TIM4->EGR = 0;
	
	if(TIM4->SR&&0x0001)
	{
		TIM4->SR &= ~0x0001;
		TIM4->DIER &= ~0x0001;
		printf("TIM4 updata open\r\n");
	}

	if(TIM4->SR&&0x0002)
	{
		TIM4->SR &= ~0x0002;
		TIM4->DIER &= ~0x0002;
		printf("TIM4 CC1 open\r\n");
	}

	if(TIM4->SR&&0x0004)
	{
		TIM4->SR &= ~0x0004;
		TIM4->DIER &= ~0x0004;
		printf("TIM4 CC2 open\r\n");
	}

	if(TIM4->SR&&0x0008)
	{
		TIM4->SR &= ~0x0008;
		TIM4->DIER &= ~0x0008;
		printf("TIM4 CC3 open\r\n");
	}

	if(TIM4->SR&&0x0010)
	{
		TIM4->SR &= ~0x0010;
		TIM4->DIER &= ~0x0010;
		printf("TIM4 CC4 open\r\n");
	}

	if(TIM4->SR&&0x0040)
	{
		TIM4->SR &= ~0x0040;
		TIM4->DIER &= ~0x0040;
		printf("TIM4 trig open\r\n");
	}
}

void TIM5_IRQHandler(void)
{
	TIM5->EGR = 0;
	
	if(TIM5->SR&&0x0001)
	{
		TIM5->SR &= ~0x0001;
		TIM5->DIER &= ~0x0001;
		printf("TIM5 updata open\r\n");
	}

	if(TIM5->SR&&0x0002)
	{
		TIM5->SR &= ~0x0002;
		TIM5->DIER &= ~0x0002;
		printf("TIM5 CC1 open\r\n");
	}

	if(TIM5->SR&&0x0004)
	{
		TIM5->SR &= ~0x0004;
		TIM5->DIER &= ~0x0004;
		printf("TIM5 CC2 open\r\n");
	}

	if(TIM5->SR&&0x0008)
	{
		TIM5->SR &= ~0x0008;
		TIM5->DIER &= ~0x0008;
		printf("TIM5 CC3 open\r\n");
	}

	if(TIM5->SR&&0x0010)
	{
		TIM5->SR &= ~0x0010;
		TIM5->DIER &= ~0x0010;
		printf("TIM5 CC4 open\r\n");
	}

	if(TIM5->SR&&0x0040)
	{
		TIM5->SR &= ~0x0040;
		TIM5->DIER &= ~0x0040;
		printf("TIM5 trig open\r\n");
	}
}
