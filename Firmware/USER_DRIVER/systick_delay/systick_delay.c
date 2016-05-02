/*********************************Copyright (c)*********************************
 **                                      
 **                          http://www.tri-iuh.blogspot.com
 **
 **--------------File Info-------------------------------------------------------
 ** File name:               main.c
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

/* Includes ------------------------------------------------------------------*/
#include "systick_delay.h"

/* Private define ------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t fac_us = 0;
static uint16_t fac_ms = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
 * Function Name  : Sys_Delay_init(uint8_t SYSCLK)
 * Description    : Init systick
 * Input          : None
 * Output         : None
 * Return         : None
 * Attention		 : None
 *******************************************************************************/
void Sys_Delay_Init(uint8_t SYSCLK)
{
  SysTick -> CTRL &= ~(1 << 2);
  fac_us = SYSCLK / 8;
  fac_ms = (uint16_t) fac_us * 1000;
}

/*******************************************************************************
 * Function Name  : Sys_Delay_ms(uint16_t nms)
 * Description    : make ms delay time
 * Input          : None
 * Output         : None
 * Return         : None
 * Attention		 : None
 *******************************************************************************/
void Sys_Delay_ms(uint16_t nms)
{
  uint32_t temp;
  SysTick->LOAD = (uint32_t) nms*fac_ms;
  SysTick->VAL = 0x00;
  SysTick->CTRL = 0x01;
  do
  {
    temp = SysTick->CTRL;
  }
  while (temp & 0x01 && !(temp & (1 << 16)));
  SysTick->CTRL = 0x00;
  SysTick->VAL = 0X00;
}

/*******************************************************************************
 * Function Name  : Sys_Delay_us(uint32_t nus)
 * Description    : make ns delay time
 * Input          : None
 * Output         : None
 * Return         : None
 * Attention		 : None
 *******************************************************************************/
void Sys_Delay_us(uint32_t nus)
{
  uint32_t temp;
  SysTick->LOAD = nus*fac_us;
  SysTick->VAL = 0x00;
  SysTick->CTRL = 0x01;
  do
  {
    temp = SysTick->CTRL;
  }
  while (temp & 0x01 && !(temp & (1 << 16)));
  SysTick->CTRL = 0x00;
  SysTick->VAL = 0X00;
}

