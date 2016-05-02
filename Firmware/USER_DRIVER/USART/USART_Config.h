/*********************************Copyright (c)*********************************
**                                      
**                          http://www.titans.com.vn
**
**--------------File Info-------------------------------------------------------
** File name:               USART_config.h
** Descriptions:            
**
**------------------------------------------------------------------------------
** Created by:              Cong Tri
** Created date:            2014-10-01
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
#ifndef _USART_CONFIG_H
#define	_USART_CONFIG_H


/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_usart.h"
#include <stdio.h>
#include "stdlib.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void USART1_putc(char ch);
void USART1_puts(char * str);
void USART1_put_var(int32_t var);

void USART_putc(USART_TypeDef* USARTx, char ch);
void USART_puts(USART_TypeDef* USARTx, char * str);
void USART_put_var(USART_TypeDef* USARTx, int32_t var);

/* Private functions ---------------------------------------------------------*/



/************************* End of File ****************************************/

#endif /* <MSN>_H */
