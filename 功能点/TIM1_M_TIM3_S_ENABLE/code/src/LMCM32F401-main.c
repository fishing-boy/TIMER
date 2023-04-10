#include "uart_REG.h"
#include "stdio.h"

#include <stm32f4xx.h>
#include <stm32f4xx_flash.h>

int endFlag = 0;
int Flag = 0;
int num = 0;

void 	Tim1_init_M(void);
void	Tim3_init_S(void);

int main()
{
	UartStdOutInit();  //uart��ʼ��
	printf("TEXT START\r\n");  

	Tim1_init_M();
	Tim3_init_S();
	
	TIM_Cmd(TIM3, ENABLE);
	TIM_Cmd(TIM2, ENABLE);

	while (1)
	{
		if(Flag == 1)
		{
			if(endFlag)
			{

			}else
			{
				Flag = 0;
				printf("TIM2 UPDATA BUT TIM1/3 NO\r\n");
				TIM_Cmd(TIM1, ENABLE);
			}
		}
		if(endFlag)
		{
			printf("END\r\n");
			UartEndSimulation();
		}
	}
}

void Tim1_init_M(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStre;
	NVIC_InitTypeDef NVIC_InitStre;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitStre.TIM_Prescaler=1-1;
	TIM_TimeBaseInitStre.TIM_Period=50-1;
	TIM_TimeBaseInitStre.TIM_ClockDivision=0x00;//����Ƶ
	TIM_TimeBaseInitStre.TIM_CounterMode=TIM_CounterMode_Up ;
	TIM_TimeBaseInitStre.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStre);

	//TIM1_CR2�Ĵ�����д�� MMS=010,��ÿ�����ɸ����¼�ʱ,TRGO1�������һ�������ء�
	TIM1->CR2 &= (uint16_t)~TIM_CR2_MMS;
  	TIM1->CR2 |=  TIM_TRGOSource_Enable;


	//TIM_ARRPreloadConfig(TIM1, ENABLE);//ʹ���Զ�װ��
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);    
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);

	NVIC_InitStre.NVIC_IRQChannel=TIM1_UP_TIM10_IRQn;
	NVIC_InitStre.NVIC_IRQChannelPreemptionPriority=0x01;
	NVIC_InitStre.NVIC_IRQChannelSubPriority=0x01;
	NVIC_InitStre.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStre);//��ʼ��

	TIM_TimeBaseInitStre.TIM_Prescaler=1-1;
	TIM_TimeBaseInitStre.TIM_Period=50-1;
	TIM_TimeBaseInitStre.TIM_ClockDivision=0x00;//����Ƶ
	TIM_TimeBaseInitStre.TIM_CounterMode=TIM_CounterMode_Up ;
	TIM_TimeBaseInitStre.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStre);

	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);    
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);

	NVIC_InitStre.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_Init(&NVIC_InitStre);//��ʼ��
}

void Tim3_init_S(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStre;
	NVIC_InitTypeDef NVIC_InitStre;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_TimeBaseInitStre.TIM_Prescaler=1-1;
	TIM_TimeBaseInitStre.TIM_Period=50-1;
	TIM_TimeBaseInitStre.TIM_ClockDivision=0x00;//����Ƶ
	TIM_TimeBaseInitStre.TIM_CounterMode=TIM_CounterMode_Up ;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStre);

	//��ʱ��2����Ϊ��ģʽ,ʹ��ITR0��Ϊ�ڲ�����
	TIM3->SMCR &= (uint16_t)~TIM_SMCR_TS;
	TIM3->SMCR |= TIM_TS_ITR0;

	//��ʱ��2��ʱ�ӽ��ɶ�ʱ��1�����Դ����źŵ��������ṩ
	TIM3->SMCR &= (uint16_t)~TIM_SMCR_SMS;
	TIM3->SMCR |= TIM_SlaveMode_Gated;

	TIM_ARRPreloadConfig(TIM3, ENABLE);//ʹ���Զ�װ��
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);    
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);

	NVIC_InitStre.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStre.NVIC_IRQChannelPreemptionPriority=0x02;
	NVIC_InitStre.NVIC_IRQChannelSubPriority=0x01;
	NVIC_InitStre.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStre);//��ʼ��
}

void TIM1_UP_TIM10_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET)
    {
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);//����ж�
		Flag = 2;	
		TIM_Cmd(TIM1, DISABLE);
    }
}

void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)
    {
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//����ж�
		Flag = 1;
		TIM_Cmd(TIM2, DISABLE);
    }
}

void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)
    {
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);//����ж�
		endFlag = 1;
		TIM_Cmd(TIM3, DISABLE);
    }
}
