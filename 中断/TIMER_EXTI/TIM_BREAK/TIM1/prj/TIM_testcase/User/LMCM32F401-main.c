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
	TIM1->EGR |= 0x0080;


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
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  ///使能TIM1时钟
	
  	TIM_TimeBaseInitStructure.TIM_Period = 1000; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=8400;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0x00;   //这里一定要写0，不然tim1的计数会比别的计数器慢
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);//初始化TIM1

	TIM_ClearITPendingBit(TIM1,TIM_IT_Break);//清除更新中断请求位
	TIM_ITConfig(TIM1,TIM_IT_Break,ENABLE); //允许定时器1更新中断
	TIM_Cmd(TIM1,ENABLE); //使能定时器1
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_BRK_TIM9_IRQn; //定时器1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//定时器1中断服务函数
void TIM1_BRK_TIM9_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_IT_Break)==SET) //溢出中断
	{
		TIM_ClearITPendingBit(TIM1,TIM_IT_Break);  //清除中断标志位
		printf("TIM1 Break EVENT PASS\r\n");
		TIM_Cmd(TIM1,DISABLE); //失能定时器1
		EVENT_FLAG = 1;
	}

}
