#include "uart.h"
#include "stdio.h"

#include <stm32f4xx.h>
#include <stm32f4xx_flash.h>

void TIM_Int_Init(void);
int EVENT_FLAG = 0;

int cnt1_2[4];
int cnt3_5[9];

int main()
{
	UartStdOutInit();  //uart初始化
	printf("TEXT START\r\n"); 

	TIM_Int_Init();

	while (1)
	{
		if(EVENT_FLAG)
		{
			if (cnt1_2[2] > cnt1_2[0])
			{
				printf("TIM1 UP_MODE PASS\r\n");
			}
			else
			{
				printf("TIM1 UP_MODE FAIL\r\n");
			}

			if(cnt1_2[3] < cnt1_2[1])
			{
				printf("TIM2 DOWM_MODE PASS\r\n");
			}
			else
			{
				printf("TIM2 DOWM_MODE FAIL\r\n");
			}

			printf("TIM1->CNT = %d,TIM2->CNT = %d\r\n",cnt1_2[0],cnt1_2[1]);
			printf("TIM1->CNT = %d,TIM2->CNT = %d\r\n",cnt1_2[2],cnt1_2[3]);

			printf("Exit interrupt and test ends\r\n");
			UartEndSimulation();
		}
	}
}

void TIM_Int_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1|RCC_APB2Periph_TIM9,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM4|RCC_APB1Periph_TIM5,ENABLE);
	
  	TIM_TimeBaseInitStructure.TIM_Period = 499;
	TIM_TimeBaseInitStructure.TIM_Prescaler=0;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0x00;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);//初始化TIM1

	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Down;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);

	//TIM_TimeBaseInitStructure.TIM_Period = 100;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_CenterAligned1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);

	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_CenterAligned2;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);

	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_CenterAligned3;
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);
	
	TIM_TimeBaseInitStructure.TIM_Period = 19;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseInitStructure);

	TIM_SetCounter(TIM1,0);
	TIM_SetCounter(TIM2,0);
	TIM_SetCounter(TIM9,0);


	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);//清除更新中断请求位
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE); //允许定时器1更新中断
	TIM_Cmd(TIM1,ENABLE); //使能定时器1

	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
	
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM3,ENABLE);

	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM4,ENABLE);

	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM5,ENABLE);

	TIM_ClearITPendingBit(TIM9,TIM_IT_Update);
	TIM_ITConfig(TIM9,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM9,ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_TIM10_IRQn; //定时器1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel=TIM1_BRK_TIM9_IRQn;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM1_UP_TIM10_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET) 
	{
		//printf("TIM1 Event\r\n");
	}
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
}

int num = 0;


void TIM1_BRK_TIM9_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM9,TIM_IT_Update)==SET) 
	{
		TIM_ClearITPendingBit(TIM9,TIM_IT_Update);

		if(num == 0)
		{
			cnt1_2[0] = TIM1->CNT;
			cnt1_2[1] = TIM2->CNT;
			//printf("TIM1->CNT = %d,TIM2->CNT = %d\r\n",cnt1_2[0],cnt1_2[1]);
/*
			cnt3_5[0] = TIM3->CNT;
			cnt3_5[1] = TIM4->CNT;
			cnt3_5[2] = TIM5->CNT;
			printf("TIM3->CNT = %d,TIM4->CNT = %d,TIM5->CNT = %d\r\n",cnt3_5[0],cnt3_5[1],cnt3_5[2]);*/
		}
/*
		if(num == 6)
		{
			cnt3_5[3] = TIM3->CNT;
			cnt3_5[4] = TIM4->CNT;
			cnt3_5[5] = TIM5->CNT;
			printf("TIM3->CNT = %d,TIM4->CNT = %d,TIM5->CNT = %d\r\n",cnt3_5[3],cnt3_5[4],cnt3_5[5]);
		}

		if(num == 10)
		{
			cnt3_5[6] = TIM3->CNT;
			cnt3_5[7] = TIM4->CNT;
			cnt3_5[8] = TIM5->CNT;
			printf("TIM3->CNT = %d,TIM4->CNT = %d,TIM5->CNT = %d\r\n",cnt3_5[6],cnt3_5[7],cnt3_5[8]);

			if((cnt3_5[0] < cnt3_5[3]) && (cnt3_5[6] < cnt3_5[3]))
			{
				printf("TIM1 CounterMode1 PASS\r\n");
			}

			if((cnt3_5[1] > cnt3_5[4]) && (cnt3_5[4] < cnt3_5[7]))
			{
				printf("TIM1 CounterMode2 PASS\r\n");
			}

			if((cnt3_5[2] < cnt3_5[5]) && (cnt3_5[5] < cnt3_5[8]))
			{
				printf("TIM1 CounterMode3 PASS\r\n");
			}
			
		}
		*/

		if (num == 3)
		{
			cnt1_2[2] = TIM1->CNT;
			cnt1_2[3] = TIM2->CNT;

			EVENT_FLAG = 1;
			TIM_ITConfig(TIM9,TIM_IT_Update,DISABLE);
/*
			if (cnt1_2[2] > cnt1_2[0])
			{
				printf("TIM1 UP_MODE PASS\r\n");
			}
			else
			{
				printf("TIM1 UP_MODE FAIL\r\n");
			}

			if(cnt1_2[3] < cnt1_2[1])
			{
				printf("TIM2 DOWM_MODE PASS\r\n");
			}
			else
			{
				printf("TIM2 DOWM_MODE FAIL\r\n");
			}
			
			printf("TIM1->CNT = %d,TIM2->CNT = %d\r\n",cnt1_2[2],cnt1_2[3]);
			UartEndSimulation();
		*/
		}
		num++;
	}
	
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)
	{
		//printf("TIM2 Event\r\n");
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)
	{
		//printf("TIM3 Event\r\n");
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}

void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)
	{
		//printf("TIM4 Event\r\n");
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
}

void TIM5_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM5,TIM_IT_Update)==SET)
	{
		//printf("TIM5 Event\r\n");
	}
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
}
