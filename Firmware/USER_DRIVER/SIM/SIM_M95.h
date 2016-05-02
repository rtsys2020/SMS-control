/*********************************Copyright (c)*********************************
**                                      
**                          http://www.tri-iuh.blogspot.com
**
**--------------File Info-------------------------------------------------------
** File name:               SIM_M95.h
** Descriptions:            this header file
**
**------------------------------------------------------------------------------
** Created by:              Cong Tri
** Created date:            14-05-2015
** Version:                 v1.0
** Descriptions:            The original version
**
**------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*******************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _SIM_M95_H
#define _SIM_M95_H

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "string.h"
#include "Delay\delay.h"
#include "USART\USART_Config.h"
//#include "..\CLD\HSTM32_CLD.h"
//#include "USART_User_Config.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"

/* Private typedef -----------------------------------------------------------*/
#define  SIM_BUFF_SIZE  200

typedef struct
{
  char Buff[SIM_BUFF_SIZE];
  uint16_t Counter;
  uint8_t RxComplete;
}tGSM_DATA;

/* Private define ------------------------------------------------------------*/

/* MODULE SIM BG2 PIN MAPPING */
/* Power On Pin */
#define SIM_PW_PIN          GPIO_Pin_15
#define SIM_PW_PORT         GPIOA
#define SIM_PW_RCC          RCC_APB2Periph_GPIOA

/* Tx and Rx (MCU) Pin */
#define SIM_TX_PIN          GPIO_Pin_2
#define SIM_TX_PORT         GPIOA

#define SIM_RX_PIN          GPIO_Pin_3
#define SIM_RX_PORT         GPIOA

#define SIM_USART_IO_RCC    RCC_APB2Periph_GPIOA
#define SIM_USART_RCC       RCC_APB1Periph_USART2
#define SIM_USART_X         USART2
#define SIM_USART_X_IRQn    USART2_IRQn

/* BASE INSTRUCTION */
#define __SIM_ON()				GPIO_WriteBit(SIM_PW_PORT,SIM_PW_PIN,(BitAction)1)	
#define __SIM_OFF()				GPIO_WriteBit(SIM_PW_PORT,SIM_PW_PIN,(BitAction)0)	

#define VinaPhone 		1
#define Viettel 			2
#define MobiPhone 		3
#define VietnaMobile	4
#define BeeLine				5
#define	FIFO_max			500




/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
signed int str_find_first_location(char *source, char *str);
signed int str_find_last_location(char *source, char *str);

/* driver functions */
void DV_SIM_GPIO_Init(void);
void DV_SIM_USART_Init(void);
void DV_SIM_printf(uint8_t *s);
uint8_t DV_SIM_Gprs_On(uint8_t SIM_Service);
uint8_t DV_SIM_Startup(void);
uint8_t DV_SIM_AT_Cmd(char *data);
uint8_t DV_SIM_AT_Sync(void);
uint8_t DV_SIM_Check_Service(void);
uint8_t DV_SIM_Reset(void);
void DV_SIM_Buff_Clear(void);
uint8_t SIM_M95_Init(void);

/* application functions */

uint8_t SIM_Init(void);
uint8_t SIM_Send_SMS(char num[], char sms[]);
uint8_t SIM_Sync_Time(uint8_t service_sim);
uint8_t SIM_Signal_Quality(void);

/* Private functions ---------------------------------------------------------*/

/************************* End of File ****************************************/

#endif /* <MSN>_H */
