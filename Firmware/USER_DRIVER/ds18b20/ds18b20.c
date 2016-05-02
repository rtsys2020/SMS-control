/*********************************Copyright (c)*********************************
**                                      
**                          http://www.webdemo.com
**
**--------------File Info-------------------------------------------------------
** File name:               ds18b20.c
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


/* Includes ------------------------------------------------------------------*/
#include "ds18b20.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void OneWireDir(tONE_WIRE_DIR DIR);
void OneWireReset(void);
void OneWireSendByte(char active_data);
char OneWireReadByte(void);


/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : name
* Description    : name_programmer
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void OneWireRCCInit(void)
{
  RCC_APB2PeriphClockCmd(ONE_WIRE_RCC, ENABLE);
  OneWireDir(DIR_OUT);
}

/*******************************************************************************
 * Function Name  : OneWireReset(void)
 * Description    : reset device
 * Input          : None
 * Output         : None
 * Return         : None
 * Attention      : None
*******************************************************************************/
void OneWireDir(tONE_WIRE_DIR DIR)
{
  GPIO_InitTypeDef GPIO_Config;
  
  GPIO_Config.GPIO_Pin   = ONE_WIRE_PIN;
  GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
  if(DIR == DIR_IN)
  {
    GPIO_Config.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  }
  else
  {
    GPIO_Config.GPIO_Mode = GPIO_Mode_Out_PP;
  }
  
  GPIO_Init(ONE_WIRE_PORT, &GPIO_Config);
}
/*******************************************************************************
 * Function Name  : OneWireReset(void)
 * Description    : reset device
 * Input          : None
 * Output         : None
 * Return         : None
 * Attention      : None
*******************************************************************************/
void OneWireReset(void)
{
  OneWireDir(DIR_OUT);
  __ONE_WIRE_OUT_LOW();
  delay_us(500);
  OneWireDir(DIR_IN);
  delay_us(500);
}

/*******************************************************************************
 * Function Name  : OneWireSendByte(char data)
 * Description    : send data
 * Input          : None
 * Output         : None
 * Return         : None
 * Attention      : None
*******************************************************************************/
void OneWireSendByte(char active_data)
{
  unsigned char i = 0;

  OneWireDir(DIR_OUT);
  for (i = 8; i > 0; i--)
  {
    __ONE_WIRE_OUT_LOW();
    delay_us(6);

    if(active_data & 0x01)
      __ONE_WIRE_OUT_HIGH();
    else
      __ONE_WIRE_OUT_LOW();
    delay_us(60);
    
    __ONE_WIRE_OUT_HIGH();
    active_data >>= 1;
  }
}

/*******************************************************************************
 * Function Name  : OneWireReadByte(void)
 * Description    : read a data byte from device
 * Input          : None
 * Output         : None
 * Return         : data
 * Attention      : None
*******************************************************************************/
char OneWireReadByte(void)
{
  unsigned char i = 0;
  unsigned char active_data = 0;
  for (i = 0; i < 8; i++)
  {
    OneWireDir(DIR_OUT);
    __ONE_WIRE_OUT_LOW();
    active_data >>= 1;
    __ONE_WIRE_OUT_HIGH();
    
    OneWireDir(DIR_IN);

    if (__ONE_WIRE_READ_INPUT())
      active_data |= 0x80;

    delay_us(120);

  }
  return (active_data);
}

/*******************************************************************************
 * Function Name  : ReadDS18B20(void)
 * Description    : read temperature data from DS18B20
 * Input          : None
 * Output         : None
 * Return         : None
 * Attention      : None
*******************************************************************************/
int DS18B20_ReadTemperature(void)
{
  char TempH = 0, TempL = 0;
  int Temp = 0;

  OneWireReset();
  OneWireSendByte(SKIP_ROM);
  OneWireSendByte(CONVERT_TEMP);
  delay_us(200);

  OneWireReset();
  OneWireSendByte(SKIP_ROM);
  OneWireSendByte(READ_SCRATCHPAD);

  TempL = OneWireReadByte();
  TempH = OneWireReadByte();
  Temp = TempH * 256 + TempL;

  Temp /= 16;
  delay_ms(200);

  return Temp;
}
/************************* End of File****************************************/




