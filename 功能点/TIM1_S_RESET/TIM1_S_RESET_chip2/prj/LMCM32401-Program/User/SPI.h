#ifndef _SPI_H
#define _SPI_H

#include <stm32f4xx.h>
#include <stm32f4xx_tim.h>

#define SPI_REG
//#define IIS_REG

#ifdef  IIS_REG

void IIS_W_FF(void);
void IIS_W_FF_CHECK(void);

void IIS_W_00(void);
void IIS_W_00_CHECK(void);

void IIS_W_A5(void);
void IIS_W_A5_CHECK(void);

void IIS_W_5A(void);
void IIS_W_5A_CHECK(void);

void output_IIS(void);
void Rtc_Init(void);

#endif

#ifdef SPI_REG

void SPI1_W_FF(void);
void SPI1_W_FF_CHECK(void);

void SPI1_W_00(void);
void SPI1_W_00_CHECK(void);

void SPI1_W_A5(void);
void SPI1_W_A5_CHECK(void);

void SPI1_W_5A(void);
void SPI1_W_5A_CHECK(void);

void output_SPI1(void);
void Rtc_Init(void);

void SPI2_3_W_FF(void);
void SPI2_3_W_FF_CHECK(void);

void SPI2_3_W_00(void);
void SPI2_3_W_00_CHECK(void);

void SPI2_3_W_A5(void);
void SPI2_3_W_A5_CHECK(void);

void SPI2_3_W_5A(void);
void SPI2_3_W_5A_CHECK(void);

void output_SPI2_3(void);

void SPI_W(unsigned int data);

#endif

#ifndef SPI_REG
#ifndef IIS_REG
#define  SPI_FULL_HALF  //ȫ/��˫��
#define  SPI_MASTER     //����
#ifndef  SPI_MASTER
#define  SPI_SLAVE      //�ӻ�
#endif  //SPI_MASTER
#endif  //IIS_REG
#endif //SPI_REG
#ifdef   SPI_FULL_HALF

void GPIO_SPI_Init(void);
void SPI_Init_User(void);
u8 SPI2_ReadWriteByte(u8 TxData);
u8 SPI2_ReadByte(u8 TxData);
void SPI2_WriteByte(u8 TxData);
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler);

#endif   //SPI_FULL_HALF

#endif
