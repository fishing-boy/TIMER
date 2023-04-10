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
	UartStdOutInit();  //uart��ʼ��
	printf("TEXT START\r\n");  

	Tim1_init_M();
	Tim2_init_S();
	
	TIM_Cmd(TIM2, ENABLE);
	//printf("OPEN TIM\r\n");
	TIM_Cmd(TIM1, ENABLE);

	//printf("OPEN TIM\r\n");
	//printf("OPEN TIM\r\n");
	//printf("OPEN TIM\r\n");
	//printf("OPEN TIM\r\n");
	
	
	
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
	//NVIC_InitTypeDef NVIC_InitStre;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	TIM_TimeBaseInitStre.TIM_Prescaler=84-1;
	TIM_TimeBaseInitStre.TIM_Period=50-1;
	TIM_TimeBaseInitStre.TIM_ClockDivision=0x00;//����Ƶ
	TIM_TimeBaseInitStre.TIM_CounterMode=TIM_CounterMode_Up ;
	TIM_TimeBaseInitStre.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStre);

	TIM1->CR2 &= (uint16_t)~TIM_CR2_MMS;
  	TIM1->CR2 |=  TIM_TRGOSource_OC1Ref;

	//��ʼ��TIM2 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //������ԣ�TIM������Ը�
	TIM_OCInitStructure.TIM_Pulse=50;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������

	// TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);    
	// TIM_ClearITPendingBit(TIM1,TIM_IT_Update);

	// NVIC_InitStre.NVIC_IRQChannel=TIM1_UP_TIM10_IRQn;
	// NVIC_InitStre.NVIC_IRQChannelPreemptionPriority=0x01;
	// NVIC_InitStre.NVIC_IRQChannelSubPriority=0x01;
	// NVIC_InitStre.NVIC_IRQChannelCmd=ENABLE;
	// NVIC_Init(&NVIC_InitStre);//��ʼ��
}

void Tim2_init_S(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStre;
	NVIC_InitTypeDef NVIC_InitStre;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitStre.TIM_Prescaler=10-1;
	TIM_TimeBaseInitStre.TIM_Period=50-1;
	TIM_TimeBaseInitStre.TIM_ClockDivision=0x00;//����Ƶ
	TIM_TimeBaseInitStre.TIM_CounterMode=TIM_CounterMode_Up ;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStre);

	//��ʱ��2����Ϊ��ģʽ,ʹ��ITR0��Ϊ�ڲ�����
	TIM2->SMCR &= (uint16_t)~TIM_SMCR_TS;
	TIM2->SMCR |= TIM_TS_ITR0;

	// //��ʱ��2��ʱ�ӽ��ɶ�ʱ��1�����Դ����źŵ��������ṩ
	TIM2->SMCR &= (uint16_t)~TIM_SMCR_SMS;
	TIM2->SMCR |= TIM_SlaveMode_Gated;

	TIM_ARRPreloadConfig(TIM2, ENABLE);//ʹ���Զ�װ��
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);    
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);

	NVIC_InitStre.NVIC_IRQChannel=TIM2_IRQn;
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
		num++;
    }
}

void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)
    {
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//����ж�
		
		printf("TIM2\r\n");
    }
}
