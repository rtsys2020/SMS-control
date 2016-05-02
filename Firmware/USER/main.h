/*********************************Copyright (c)*********************************
**                                      
**                          http://www.tri-iuh.blogspot.com
**
**--------------File Info-------------------------------------------------------
** File name:               main.h
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
#ifndef _MAIN_H
#define _MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "systick_delay/systick_delay.h"
#include "string.h"
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "LCD/lcd_4bit.h"
#include "USART/USART_Config.h"
#include "SIM/SIM_M95.h"
#include "DHT11/dht11.h"
#include "ds18b20/ds18b20.h"

//#include "SoftI2C/soft_i2c.h"

/* Private typedef -----------------------------------------------------------*/
/* SIM status enumeration */
typedef enum 
{
  BUSY = 0,
  IDLE
}tSTATUS;

typedef enum
{
  SEND = 0,
  NOTSEND
}tSMS_SEND;

typedef enum
{
  INIT,
  HIGH_SEND,
  MIDDLE_SEND,
  LOW_SEND
}tSMS_LEVER;
/* relay state enumeration*/

typedef struct 
{
  tSTATUS Status;
  char Phone[13];
  char Password[6];
  uint8_t Temperature;
  uint8_t Humidity;  
}GSM_Control_Typedef;

typedef struct
{
  tSMS_LEVER Humi_SMS;
  tSMS_LEVER Temp_SMS;
}tSMS_STATUS;
  
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void SYS_Buff_Clear(void);

void GPIO_Board_Config(void);
void USART1_Debug_Config(void);
void USART2_GSM_Config(void);
uint8_t Check_SMS_Phone_Number(char *phone);
void Relay_SetState(uint8_t RLx, FunctionalState END_DIS);
uint8_t Relay_GetState(uint8_t RLx);

void SMS_Process(void);
void Call_Process(void);
void Process_Sensor_State(void);
void Update_LCD(void);


/* Private functions ---------------------------------------------------------*/



/************************* End of File ****************************************/

#endif /* _MAIN_H */
