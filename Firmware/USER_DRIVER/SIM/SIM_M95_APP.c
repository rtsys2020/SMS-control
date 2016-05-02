/*********************************Copyright (c)*********************************
 **                                      
 **                          http://www.tri-iuh.blogspot.com
 **
 **--------------File Info-------------------------------------------------------
 ** File name:               SIM_M95_DV.c
 ** Descriptions:            This file content functions for SIM application
 **
 **------------------------------------------------------------------------------
 ** Created by:              Cong Tri
 ** Created date:            15-09-2014
 ** Version:                 v1.0
 ** Descriptions:            The original version
 **
 **------------------------------------------------------------------------------
 ** Modified by:             Cong Tri             
 ** Modified date:           15-09-2014
 ** Version:                 v1.0
 ** Descriptions:            
 **
 *******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "SIM_M95.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Global variables ----------------------------------------------------------*/
extern char SIM_Data_Buff[400];


/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : SIM_M95_Init(void)
* Description    : Init module sim
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
uint8_t SIM_M95_Init(void)
{
  char time_out = 10;
  char status = 0;
  
  DV_SIM_GPIO_Init();
  DV_SIM_USART_Init(); 
  
  /* turn on module sim */
  __SIM_OFF();
  delay_ms(100);
  __SIM_ON();
  delay_ms(100);
  __SIM_OFF();
  delay_ms(5000);

  /* clear receive buffer */
  DV_SIM_Buff_Clear();
  
  /* synchronous module sim */
  time_out = 10;
  do
  {
    status = DV_SIM_AT_Sync();
    time_out--;
    if(time_out == 0) return 0;
  }
  while(status == 0);
 
  /* reset config module sim */
  time_out = 10;
  do
  {
    status = DV_SIM_AT_Cmd("ATZ");
    time_out--;
    if(time_out == 0) return 0;
  }
  while(status == 0);

  /* echo on */
  DV_SIM_AT_Cmd("ATE");
  delay_ms(100);
  
  /* check sim service provider */
  DV_SIM_Check_Service();
  
  /* fomat data string return when calling */
  DV_SIM_AT_Cmd("AT+CLIP=1");
  delay_ms(100);
  
//  /* save config */
//  DV_SIM_AT_Cmd("AT&W");
//  delay_ms(100);
  
  /* set sms text mode */
  DV_SIM_AT_Cmd("AT+CMGF=1");
  delay_ms(100);
  
  /* set mode when receive SMS */
  DV_SIM_AT_Cmd("AT+CNMI=2,2,0,0,0\r");
  delay_ms(100);
  
  /* save config SMS */
  DV_SIM_AT_Cmd("AT+CSAS");
  delay_ms(100);

  /* delete all SMS */
  DV_SIM_AT_Cmd("AT+CMGD=1");
  DV_SIM_AT_Cmd("AT+QMGDA=\"DEL ALL\"");
  delay_ms(300);
  
  /* echo off */
  DV_SIM_AT_Cmd("ATE0");
  delay_ms(100);
  
  return 1;
}

/*******************************************************************************
* Function Name  : SIM_Init
* Description    : demo Programmer
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
uint8_t SIM_Init(void)
{
  uint8_t result = 0;
  
  DV_SIM_GPIO_Init();
  DV_SIM_USART_Init();
  
  result = DV_SIM_Reset();
  if(result==0)
    printf("\n\rInit error\r\n");
  else
    printf("\n\rInit Success\r\n");
  
  return result;
}


/*******************************************************************************
* Function Name  : SIM_Send_SMS(char *num, char *sms)
* Description    : demo Programmer
* Input          : - *num: mobile number need sending
                 : - *sms: message content
* Output         : None
* Return         : 0 - error; 1 - ok
*******************************************************************************/
uint8_t SIM_Send_SMS(char *num, char *sms)
{
  uint8_t time_out = 100;
  
  /* set SMS text mode */
  DV_SIM_AT_Cmd("AT+CMGF=1");
  
  /* delete message stored at memory stack 1 */
  DV_SIM_AT_Cmd("AT+CMGD=1");
  
  /* send mobile number */
  DV_SIM_printf((uint8_t*) "AT+CMGS=\"");
  DV_SIM_printf((uint8_t*) num);
  DV_SIM_printf((uint8_t*) "\"\r");
  delay_ms(100);
  
  while (str_find_last_location(SIM_Data_Buff, ">") < 0)
  {
    delay_ms(500);
    time_out--;
    if (!time_out) return 0;
  }
  
  /* send SMS content */
  DV_SIM_printf((uint8_t*) sms);
  USART_SendData(SIM_USART_X,0x1a);
  
  /* set timeout */
  time_out = 100;
  
  do
  {
    delay_ms(500);
    time_out--;
    if (!time_out || str_find_first_location(SIM_Data_Buff, "ERROR") >= 0) return 0;
  }
  while (str_find_first_location(SIM_Data_Buff, "OK") < 0);
  
  return 1;
}


///*******************************************************************************
//* Function Name  : SIM_Sync_Time(uint8_t service_sim)
//* Description    : synchronous the time of device with sim service provider
//* Input          : service_sim: sim service provider
//* Output         : None
//* Return         : 0 - error; 1 - success
//*******************************************************************************/
//uint8_t SIM_Sync_Time(uint8_t service_sim)
//{
//  uint8_t time_out = 100, i = 0, tem = 0;
//  tDATETIME Synch_time;
//  
//  DV_SIM_AT_Cmd("AT+CNMI=1,1");
//  
//  switch (service_sim)
//  {
//    case VinaPhone:
//      if (!SIM_Send_SMS("333", "Test")) return 0;
//      break;
//    case Viettel:
//      if (!SIM_Send_SMS("9119", "Test")) return 0;
//      break;
//    case MobiPhone:
//      if (!SIM_Send_SMS("900", "Test")) return 0;
//      break;
//    case VietnaMobile:
//      if (!SIM_Send_SMS("123", "Test")) return 0;
//      break;
//    case BeeLine:
//      if (!SIM_Send_SMS("9119", "Test")) return 0;
//      break;
//  }
//  
//  while (str_find_last_location(&SIM_Data_Buff[0], "+CMTI:") == 1)
//  {
//    delay_ms(500);
//    time_out--;
//    if (!time_out) return 0;
//  }
//  
//  time_out = 100;
//  flag_gsm = 0;
//  delay_ms(10);
//  
//  /* read SMS stored at memory stack 1 */
//  DV_SIM_AT_Cmd("AT+CMGR=1");
//  
//  while (str_find_last_location(&SIM_Data_Buff[12], "+CMGR:") == 1)
//  {
//    delay_ms(100);
//    time_out--;
//    if (!time_out) return 0;
//  }
//  
//  /* export time synch to terminal console */
//  i = str_find_last_location(SIM_Data_Buff,"\",,\"");
//  tem = (SIM_Data_Buff[i++] - 48)*10;
//  Synch_time.Year = 2000 + SIM_Data_Buff[i++] - 48 + tem;
//  i++;
//  tem = (SIM_Data_Buff[i++] - 48)*10;
//  Synch_time.Month = SIM_Data_Buff[i++] - 48 + tem;
//  i++;
//  tem = (SIM_Data_Buff[i++] - 48)*10;
//  Synch_time.Day = SIM_Data_Buff[i++] - 48 + tem;
//  i++;
//  tem = (SIM_Data_Buff[i++] - 48)*10;
//  Synch_time.Hour = SIM_Data_Buff[i++] - 48 + tem;
//  i++;
//  tem = (SIM_Data_Buff[i++] - 48)*10;
//  Synch_time.Minutes = SIM_Data_Buff[i++] - 48 + tem;
//  i++;
//  tem = (SIM_Data_Buff[i++] - 48)*10;
//  Synch_time.Second = SIM_Data_Buff[i++] - 48 + tem;
//  
//  printf("\n\rTIME:%0.2d:%0.2d:%0.2d,%0.4d/%0.2d/%0.2d\n\r"\
//         ,Synch_time.Hour, Synch_time.Minutes, Synch_time.Second, Synch_time.Year, Synch_time.Month, Synch_time.Day);

//  CLD_Set_Calendar(Synch_time);
//  flag_gsm = 1;
//  time_out = 100;
//  
//  /* delete sms */
//  DV_SIM_AT_Cmd("AT+CMGD=1");
//  while (str_find_last_location(&SIM_Data_Buff[0], "OK") == 1)
//  {
//    delay_ms(500);
//    time_out--;
//    if (!time_out) return 0;
//  }
//  
//  DV_SIM_AT_Cmd("AT+CNMI=0");
//  return 1;
//}


/*******************************************************************************
* Function Name  : SIM_Signal_Quality(void)
* Description    : check signal quality
* Input          : None
* Output         : 0 -113 dBm or less
                   1 -111 dBm
                   2..30 -109... -53 dBm
                   31 -51 dBm or greater
                   99 not known or not detectable
* Return         : None
* Attention      : None
*******************************************************************************/
uint8_t SIM_Signal_Quality(void)
{
  DV_SIM_AT_Cmd("AT+CSQ");
  if (SIM_Data_Buff[7] != ',')
    return ((SIM_Data_Buff[6] - 0x30)*10 + (SIM_Data_Buff[7] - 0x30));
  else return (SIM_Data_Buff[6] - 0x30);
}


/************************* End of File ****************************************/

