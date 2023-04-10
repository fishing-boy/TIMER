#include "uart_REG.h"


#include <stm32f4xx.h>
#include <stm32f4xx_flash.h>
#include "stdio.h"

void TIM1_EXTI(void);


int main()
{
	UartStdOutInit();  //uart��ʼ��

	TIM1_EXTI();

	*((int*)0x40010014) |= 0x0001;

	while (1)
	{
		/* code */
	}
	

}

void TIM1_EXTI(void)
{
//TIM����
	*((int*)0x40023844) |= 0x00000001;  //APB2ENR ʹ��TIM1ʱ��

	*((int*)0X40010000) |= 0x0020;      //CR1 
	*((int*)0X4001002C)  = 1;         //ARR �Զ�����
	*((int*)0x40010028)  = 1;         //PSC Ԥ��Ƶ
	*((int*)0x40010030) &= 0x00000000;  //RCR �ظ�����
	*((int*)0x40010014) |= 0x0001;      //EGR ���³�ʼ��������������һ���Ĵ��������¼�
	*((int*)0x40010010)  = ~0x0001;     //SR  ��������ж�����λ
	*((int*)0x4001000C) |= 0x0001;      //DIER����ʱ��1�����ж�
	*((int*)0X40010000) |= 0x0001;      //CR1 ʹ�ܶ�ʱ��1
	*((int*)0x40010010)  = ~0x0001;     //SR  ��������ж�����λ
	
	//�ж�����
	//*((int*)0xE000E418)  = 0X00006000;    //IPR6
	*((int*)0XE000E100)  = 0x02000000;    //NVIC->ISER0
}

//��ʱ��1�жϷ�����
void TIM1_UP_TIM10_IRQHandler(void)
{
	if((*((int*)0x40010010) & 0x0001) && ((*((int*)0x4001000C) & 0x0001)))  //TIM1->SR  TIM1->DIER
	{
		printf("TIM1 EXTI TEST:UPDATA PASS\r\n");
		*((int*)0X40010000) &= 0xFFFFFFFE;      //CR1
	}
	*((int*)0x40010010) = ~0x0001;  //TIM1->SR  ����жϱ�־λ
}
