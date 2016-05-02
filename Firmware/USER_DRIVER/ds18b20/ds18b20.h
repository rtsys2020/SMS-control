/*********************************Copyright (c)*********************************
**                                      
**                          http://tri-iuh.blogspot.com
**
**--------------File Info-------------------------------------------------------
** File name:               ds18b20.h
** Descriptions:            Descriptions
**
**------------------------------------------------------------------------------
** Created by:              Cong Tri
** Created date:            23-05-2015
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
#ifndef _DS18B20_H
#define _DS18B20_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_gpio.h"
#include "delay/delay.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  DIR_OUT = 0,
  DIR_IN
}tONE_WIRE_DIR;

/* Private define ------------------------------------------------------------*/
#define READ_ROM            0x33
#define MATCH_ROM           0x55
#define SEARCH_ROM          0xf0
#define ARLAM_SEARCH        0xec
#define SKIP_ROM            0xcc
#define CONVERT_TEMP        0x44
#define READ_SCRATCHPAD     0xbe
#define WRITE_SCRATCHPAD    0x4e
#define COPY_SCRATCHPAD     0x48

/* DS18B20 pin map */
#define ONE_WIRE_PIN    GPIO_Pin_1
#define ONE_WIRE_PORT   GPIOA
#define ONE_WIRE_RCC    RCC_APB2Periph_GPIOA

/* Private macro -------------------------------------------------------------*/
#define __ONE_WIRE_OUT_HIGH()    GPIO_WriteBit(ONE_WIRE_PORT,ONE_WIRE_PIN,(BitAction)1)
#define __ONE_WIRE_OUT_LOW()      GPIO_WriteBit(ONE_WIRE_PORT,ONE_WIRE_PIN,(BitAction)0)
#define __ONE_WIRE_READ_INPUT()   GPIO_ReadInputDataBit(ONE_WIRE_PORT, ONE_WIRE_PIN)

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
int DS18B20_ReadTemperature(void);
void OneWireRCCInit(void);
/* Private functions ---------------------------------------------------------*/



/************************* End of File ****************************************/

#endif /* _DS18B20_H */
