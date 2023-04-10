#ifndef _TIMER_H
#define _TIMER_H

#include <stm32f4xx.h>
#include <stm32f4xx_tim.h>

//#define TIMER_LIB

#ifndef TIMER_LIB
#define TIMER_REG
#endif

#ifdef TIMER_REG
void NVIC_PriorityGroupConfig_REG(uint32_t NVIC_PriorityGroup);
#endif

//#define  TIMER_COUNTER   //测试计数器
#ifdef   TIMER_COUNTER

void TIM1_Int_Init(u16 arr,u16 psc);
void TIM2_Int_Init(u16 arr,u16 psc);
void TIM3_Int_Init(u16 arr,u16 psc);
void TIM4_Int_Init(u16 arr,u16 psc);
void TIM5_Int_Init(u16 arr,u16 psc);

#endif

#define TIMER_PSC     //测试预分频器
#ifdef  TIMER_PSC

void TIM1_PSC_Init(u16 arr,u16 psc);
void TIM2_PSC_Init(u16 arr,u16 psc);
void TIM3_PSC_Init(u16 arr,u16 psc);

#endif

//#define TIMER_ARR     //测试自动重载器
#ifdef  TIMER_ARR

void TIM1_ARR_Init(u16 arr,u16 psc);
void RESET_ARR_ARPE(u16 arr,FunctionalState NewState);

#endif

//#define TIMER_RCR     //测试重复计数
#ifdef  TIMER_RCR

void TIM1_RCR_Init(u16 arr,u16 psc);

#endif

//#define   TIMER_PWM
#ifdef   TIMER_PWM

void TIM1_PWM_Init(u32 arr,u32 psc);
void TIM2_PWM_Init(u32 arr,u32 psc);
void TIM5_CH1_Cap_Init(u32 arr,u16 psc);

#endif

//#define  TIMER_TRGI
#ifdef TIMER_TRGI

void EN_TIM4(void);
void Tim5_init_TRGI(void);
void Tim4_init_TRGI(void);

#endif

//#define  TIMER_BREAK
#ifdef  TIMER_BREAK

void PWM_OC_IN(void);

#endif

//#define   TIMER_OPM      //单脉冲
#ifdef  TIMER_OPM

void TIM2_OPM(u32 arr,u16 psc);

#endif

//#define TIMER_OUTPUT        //强制输出
#ifdef  TIMER_OUTPUT

void TIM1_Config1(void);

#endif

//#define TIMER_ETR
#ifdef TIMER_ETR

void TIM2_init(void);
void MCO1_Init(void);
void TIM3_PWM_Init(u32 arr,u32 psc);

#endif

//#define TIMER_DMA
#ifdef TIMER_DMA

void TIM_Config(void);
void TIM1_Init(void);

#endif

//#define TIMER_ENCODE
#ifdef TIMER_ENCODE

void TIM4_ENCODER_Init(void);
int read_encoder(void);
void calc_motor_rotate_speed(void);
void PWM_OC_IN(void);

#endif

#endif
