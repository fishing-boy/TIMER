#include "uart_REG.h"
#include "stdio.h"

#include <stm32f4xx.h>
#include <stm32f4xx_flash.h>

int t1,t2;

void Tim1_init_S(void);
void Tim5_init_M(void);
void Tim2_init(void);

int endFlag = 0;
int num = 0;

int main()
{
	UartStdOutInit();  //uart��ʼ��
	printf("TEXT START\r\n");  
	t1 = 0;
	t2 = 0;

	Tim1_init_S();
	Tim5_init_M();
	Tim2_init();
	
	TIM_Cmd(TIM1,ENABLE);
	TIM_Cmd(TIM5,ENABLE);
	TIM_Cmd(TIM2,ENABLE);

	while (1)
	{
		if(t2 == 8)
		{
			if(t1*2 == t2)
			{
				printf("TEST PASS,t1 = %d ,t2 = %d\r\n",t1,t2);
			}
			else{
				printf("TEST FAIL,t1 = %d ,t2 = %d\r\n",t1,t2);
			}
		}
	}
}

void Tim1_init_S(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStre;
	NVIC_InitTypeDef NVIC_InitStre;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	// RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//��GPIOAʱ��

	// GPIO_InitTypeDef GPIO_InitStruct={0};
	// GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	// GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 ;
	// GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	// GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_TIM1);//����ʹ��

	TIM_TimeBaseInitStre.TIM_Prescaler=100-1;
	TIM_TimeBaseInitStre.TIM_Period=100-1;
	TIM_TimeBaseInitStre.TIM_ClockDivision=0x00;//����Ƶ
	TIM_TimeBaseInitStre.TIM_CounterMode=TIM_CounterMode_Up ;
	TIM_TimeBaseInitStre.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStre);

	TIM1->SMCR &= (uint16_t)~TIM_SMCR_TS;
	TIM1->SMCR |= TIM_TS_ITR0;

	TIM5->SMCR &= (uint16_t)~TIM_SMCR_SMS;
	TIM5->SMCR |= TIM_SlaveMode_Gated;

	//TIM_ARRPreloadConfig(TIM1, ENABLE);//ʹ���Զ�װ��
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);    
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);

	NVIC_InitStre.NVIC_IRQChannel=TIM1_UP_TIM10_IRQn;
	NVIC_InitStre.NVIC_IRQChannelPreemptionPriority=0x01;
	NVIC_InitStre.NVIC_IRQChannelSubPriority=0x01;
	NVIC_InitStre.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStre);//��ʼ��
}

void Tim5_init_M(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStre;
	//NVIC_InitTypeDef NVIC_InitStre;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

	TIM_TimeBaseInitStre.TIM_Prescaler=100-1;
	TIM_TimeBaseInitStre.TIM_Period=100-1;
	TIM_TimeBaseInitStre.TIM_ClockDivision=0x00;//����Ƶ
	TIM_TimeBaseInitStre.TIM_CounterMode=TIM_CounterMode_Up ;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStre);

	//TIM1_CR2�Ĵ�����д�� MMS=010,��ÿ�����ɸ����¼�ʱ,TRGO1�������һ�������ء�
	TIM5->CR2 &= (uint16_t)~TIM_CR2_MMS;
  	TIM5->CR2 |=  TIM_TRGOSource_OC1Ref;

	//��ʼ��TIM2 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //������ԣ�TIM������Ը�
	TIM_OCInitStructure.TIM_Pulse=50;
	TIM_OC1Init(TIM5, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������

	TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);  //ʹ��TIM2��CCR2�ϵ�Ԥװ�ؼĴ���

	TIM_ARRPreloadConfig(TIM5, ENABLE);//ʹ���Զ�װ��
	//TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);    
	//TIM_ClearITPendingBit(TIM5,TIM_IT_Update);

	// NVIC_InitStre.NVIC_IRQChannel=TIM5_IRQn;
	// NVIC_InitStre.NVIC_IRQChannelPreemptionPriority=0x02;
	// NVIC_InitStre.NVIC_IRQChannelSubPriority=0x01;
	// NVIC_InitStre.NVIC_IRQChannelCmd=ENABLE;
	// NVIC_Init(&NVIC_InitStre);//��ʼ��
}

void Tim2_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStre;
	NVIC_InitTypeDef NVIC_InitStre;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitStre.TIM_Prescaler=100-1;
	TIM_TimeBaseInitStre.TIM_Period=100-1;
	TIM_TimeBaseInitStre.TIM_ClockDivision=0x00;//����Ƶ
	TIM_TimeBaseInitStre.TIM_CounterMode=TIM_CounterMode_Up ;
	TIM_TimeBaseInitStre.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStre);

	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);    
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);

	NVIC_InitStre.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStre.NVIC_IRQChannelPreemptionPriority=0x01;
	NVIC_InitStre.NVIC_IRQChannelSubPriority=0x01;
	NVIC_InitStre.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStre);//��ʼ��
}

void TIM1_UP_TIM10_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET)
    {
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);//����ж�	
		t1++;
    }
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)
    {
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//����ж�	
		t2++;
    }
}

