#include "uart.h"
#include "stdio.h"

#include <stm32f4xx.h>
#include <stm32f4xx_flash.h>

void dma_Init(void);
void TIM4_Init(void);
void p_aSRC_Buffer(void);

int main()
{
	UartStdOutInit();
	
	printf("START\r\n");
	dma_Init();
	TIM4_Init();
	
	p_aSRC_Buffer();

	TIM_GenerateEvent(TIM1,TIM_EventSource_CC4);
	printf("CC4 event has happen\r\n");
	p_aSRC_Buffer();

	TIM_GenerateEvent(TIM1,TIM_EventSource_Trigger);
	printf("trig event has happen\r\n");
	p_aSRC_Buffer();

	TIM_GenerateEvent(TIM1,TIM_EventSource_COM);
	printf("com event has happen\r\n");
	p_aSRC_Buffer();

	printf("end\r\n");
	UartEndSimulation();
}

uint16_t aSRC_Buffer[3] = {0, 0, 0};

void p_aSRC_Buffer(void)
{
	printf("Buffer[0] = %d,Buffer[1] = %d,Buffer[2] = %d\r\n",aSRC_Buffer[0],aSRC_Buffer[1],aSRC_Buffer[2]);
}

void dma_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	DMA_DeInit(DMA2_Stream4);
	DMA_InitStructure.DMA_Channel = DMA_Channel_6;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(TIM1->ARR));
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)(aSRC_Buffer);
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 3;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	DMA_Init(DMA2_Stream4, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream4, ENABLE);
}

void TIM4_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	TIM_TimeBaseStructure.TIM_Prescaler = 0XFFFF;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 0XFFFF;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	TIM_Cmd(TIM1, ENABLE);

	TIM_DMACmd(TIM1,TIM_DMA_CC4,ENABLE);
	TIM_DMACmd(TIM1,TIM_DMA_Trigger,ENABLE);
	TIM_DMACmd(TIM1,TIM_DMA_COM,ENABLE);
}
