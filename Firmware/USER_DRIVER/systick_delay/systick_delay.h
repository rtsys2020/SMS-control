/*********************************Copyright (c)*********************************
**                                      
**                          http://www.tri-iuh.blogspot.com
**
**--------------File Info-------------------------------------------------------
** File name:               systick_delay.h
** Descriptions:            main file
**
**------------------------------------------------------------------------------
** Created by:              Cong Tri
** Created date:            2014-09-23
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
#ifndef __SYSTICK_DELAY_H
#define __SYSTICK_DELAY_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Private define ------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void Sys_Delay_Init(uint8_t SYSCLK);
void Sys_Delay_ms(uint16_t nms);
void Sys_Delay_us(uint32_t nus);

#endif
