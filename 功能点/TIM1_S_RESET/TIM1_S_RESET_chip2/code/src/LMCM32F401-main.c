#include "uart_REG.h"
#include "stdio.h"

#include <stm32f4xx.h>
#include <stm32f4xx_flash.h>

void Tim1_init_M(void);
void Tim2_init_S(void);

int num1 = 0;
int num2 = 0;
int i = 0;

int main()
{
	UartStdOutInit();  //uart��ʼ��
	printf("chip2:TEXT START\r\n");  

	Tim1_init_M();
	//Tim2_init_S();
	
	//TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM1, ENABLE);

	i = 100;
	while (i--);

	printf("TIM1->CNT = %d\r\n",TIM1->CNT);

	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8) == Bit_RESET);

	i = 100;
	while (i--);

	printf("TIM1->CNT = %d\r\n",TIM1->CNT);

	UartEndSimulation();
	
	while (1)
	{
		if(num2)
		{
			if(num2 == num1)
			{
				printf("chip2:test pass\r\n");
				GPIO_ResetBits(GPIOB,GPIO_Pin_0);
				UartEndSimulation();
			}
		}
	}
}

void Tim1_init_M(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStre;
	NVIC_InitTypeDef NVIC_InitStre;
	//TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB, ENABLE); 	//GPIOʹ��	
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1); //GPIOA15����
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;        
	GPIO_Init(GPIOA,&GPIO_InitStructure);         
	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);     
	
	TIM_TimeBaseInitStre.TIM_Prescaler=200-1;
	TIM_TimeBaseInitStre.TIM_Period=500-1;
	TIM_TimeBaseInitStre.TIM_ClockDivision=0x00;//����Ƶ
	TIM_TimeBaseInitStre.TIM_CounterMode=TIM_CounterMode_Up ;
	TIM_TimeBaseInitStre.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStre);

	// TIM1->CR2 &= (uint16_t)~TIM_CR2_MMS;
  	// TIM1->CR2 |=  TIM_TRGOSource_Enable;

	// TIM1->SMCR &= (uint16_t)~TIM_SMCR_TS;
	// TIM1->SMCR |= TIM_TS_TI1F_ED;

	// TIM1->SMCR &= (uint16_t)~TIM_SMCR_SMS;
	// TIM1->SMCR |= TIM_SlaveMode_Reset;

		TIM_SelectSlaveMode(TIM1,TIM_SlaveMode_Reset);
	TIM_SelectInputTrigger(TIM1,TIM_TS_TI1F_ED);
	TIM_SelectMasterSlaveMode(TIM1,TIM_MasterSlaveMode_Disable);
	TIM_UpdateRequestConfig(TIM1, TIM_UpdateSource_Regular); /*enable URS*/

	// TIM1->SMCR &= (uint16_t)~TIM_SMCR_MSM;
	// TIM1->SMCR |= TIM_MasterSlaveMode_Enable;

	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);    
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);

	//��ʼ��TIM5���벶�����
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ�������IC1ӳ�䵽TI1��
  	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ������Ƶ 
  	TIM_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲������˲�
  	TIM_ICInit(TIM1, &TIM_ICInitStructure);

	NVIC_InitStre.NVIC_IRQChannel=TIM1_UP_TIM10_IRQn;
	NVIC_InitStre.NVIC_IRQChannelPreemptionPriority=0x01;
	NVIC_InitStre.NVIC_IRQChannelSubPriority=0x01;
	NVIC_InitStre.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStre);//��ʼ��
}

void Tim2_init_S(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStre;
	NVIC_InitTypeDef NVIC_InitStre;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitStre.TIM_Prescaler=10-1;
	TIM_TimeBaseInitStre.TIM_Period=80-1;
	TIM_TimeBaseInitStre.TIM_ClockDivision=0x00;//����Ƶ
	TIM_TimeBaseInitStre.TIM_CounterMode=TIM_CounterMode_Up ;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStre);

	//��ʱ��2����Ϊ��ģʽ,ʹ��ITR0��Ϊ�ڲ�����
	TIM2->SMCR &= (uint16_t)~TIM_SMCR_TS;
	TIM2->SMCR |= TIM_TS_ITR0;

	// //��ʱ��2��ʱ�ӽ��ɶ�ʱ��1�����Դ����źŵ��������ṩ
	TIM2->SMCR &= (uint16_t)~TIM_SMCR_SMS;
	TIM2->SMCR |= TIM_SlaveMode_Trigger;

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
		num1++;
		TIM_ITConfig(TIM1,TIM_IT_Update,DISABLE); 
		printf("chip2:test pass\r\n");
    }
}

void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)
    {
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//����ж�
		num2++;
		TIM_Cmd(TIM2, DISABLE);
		TIM_Cmd(TIM1, DISABLE);
    }
}
