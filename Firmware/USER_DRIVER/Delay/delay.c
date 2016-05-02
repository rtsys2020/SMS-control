/*********************************Copyright (c)*********************************
**                                      
**                          http://www.tri-iuh.blogspot.com
**
**--------------File Info-------------------------------------------------------
** File name:               delay.c
** Descriptions:            include functions delay
**
**------------------------------------------------------------------------------
** Created by:              Cong Tri
** Created date:            2014-09-25
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
#include "delay.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/*******************************************************************************
* Function Name  : delay_ms(uint16_t time)
* Description    : CPU does nothing in us
* Input          : + uint16_t time: time to delay in us
* Output         : None
* Return         : None
*******************************************************************************/
void delay_us(uint32_t time) 
{
	uint32_t index=0;
	
	index = time*8;
	while(index--);	
}

/*******************************************************************************
* Function Name  : delay_ms(uint16_t time)
* Description    : CPU does nothing in ms
* Input          : + uint16_t time: time to delay in ms
* Output         : None
* Return         : None
*******************************************************************************/
void delay_ms(uint32_t time)
{
	uint32_t index=0;
	
	index = time*8000;
	while(index--);	
}

/************************* End of File ****************************************/





