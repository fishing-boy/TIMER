#include "uart.h"
#include "stdio.h"

#include <stm32f4xx.h>
#include <stm32f4xx_flash.h>

void TIM1_init(void);
void MCO1_Init(void);

int EVENT_FLAG = 0;

int main()
{
	UartStdOutInit();  //uart初始化
	printf("Chip1:TEXT START\r\n"); 
	
	//TIM1_init();
	
	MCO1_Init();
	while (1)
	{
		if(EVENT_FLAG)
		{
			printf("Chip1:test ends\r\n");
			UartEndSimulation();
		}
	}
}

void TIM1_init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  	//TIM2时钟使能 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//GPIO使能	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//普通输入模式
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//
	GPIO_Init(GPIOA,&GPIO_InitStructure); 

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1);  

	TIM_DeInit(TIM1);
	TIM_TimeBaseStructure.TIM_Period = 50-1;     
	TIM_TimeBaseStructure.TIM_Prescaler =10-1; 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 

	TIM_TIxExternalClockConfig(TIM1,TIM_TIxExternalCLK1Source_TI1ED,TIM_ICPolarity_Rising,0);//设置输入滤波和边沿检测

	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);//清除更新中断请求位
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE); //允许定时器1更新中断

	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); 

	printf("Chip1:Enable TIM1 And TIM2,TIM1 Need Extern Clock\r\n");
	TIM_Cmd(TIM2, ENABLE);  
	TIM_Cmd(TIM1, ENABLE);  

	NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_TIM10_IRQn; //定时器1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; 
	NVIC_Init(&NVIC_InitStructure);
}

void TIM1_UP_TIM10_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET) //溢出中断
	{
		printf("Chip1:TIM1 Updata,Test Pass\r\n");
		EVENT_FLAG = 1;
		TIM_Cmd(TIM2, DISABLE);  
		TIM_Cmd(TIM1, DISABLE);  
	}
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);  //清除中断标志位
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //溢出中断
	{
		if(EVENT_FLAG)
		{
			printf("Chip1:Test Fail\r\n");
		}
		else
		{
			//MCO1_Init();
			printf("Chip1:TIM2 Updata,TIM1->CNT = 0x%08x\r\n",TIM1->CNT);
		}
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断标志位
}

void MCO1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_ClockSecuritySystemCmd(ENABLE);
 
    /* Enable GPIOs clocks */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_MCO);//打开引脚复用功能
         
    /* Configure MCO (PA8) */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  //UP
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	//输出时钟
    RCC_MCO1Config(RCC_MCO1Source_HSE, RCC_MCO1Div_3);
}
