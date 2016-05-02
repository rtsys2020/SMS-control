#ifndef __DHT11_H
#define	__DHT11_H

#include "stm32f10x.h"
//#include "SysTick.h"

#define HIGH  1
#define LOW   0


typedef struct
{
	uint8_t  humi_int;		//Phan nguyen do am
	uint8_t  humi_deci;	 	//phan thap phan do am
	uint8_t  temp_int;	 	//phan nguyen nhiet do
	uint8_t  temp_deci;	 	//phan thap phan nhiet do
	uint8_t  check_sum;	 	//checksum
		                 
}DHT11_Data_TypeDef;


#define DHT11_DATA_OUT(a)	if (a)	\
					                  GPIO_SetBits(GPIOA,GPIO_Pin_0);\
					                else		\
					                  GPIO_ResetBits(GPIOA,GPIO_Pin_0)

#define  DHT11_DATA_IN()	   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)



void DHT11_GPIO_Config(void);
static void DHT11_Mode_IPU(void);
static void DHT11_Mode_Out_PP(void);
uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data);
static uint8_t Read_Byte(void);

#endif /* __DHT11_H */
