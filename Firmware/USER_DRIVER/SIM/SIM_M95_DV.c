/*********************************Copyright (c)*********************************
 **                                      
 **                          http://www.tri-iuh.blogspot.com
 **
 **--------------File Info-------------------------------------------------------
 ** File name:               SIM_M95_DV.c
 ** Descriptions:            This file content driver functions support to control
 **                          module SIM BG2
 **
 **------------------------------------------------------------------------------
 ** Created by:              Hoan Nguyen
 ** Created date:            2010-10-30
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
//#include "header.h"
#include "SIM_M95.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define  SIM_BUFF_SIZE  200

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* variable using for SIM Rx */
char SIM_Data_Buff[SIM_BUFF_SIZE];
uint16_t SIM_buff_count = 0;
uint8_t SIM_Rx_Complete = 0;
uint8_t SIM_Rx_data = 0;


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : DV_SIM_Buff_Clear(void)
* Description    : Clear buffer
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void DV_SIM_Buff_Clear(void)
{
  int i = SIM_BUFF_SIZE;
  
  SIM_Rx_Complete = 0;
  SIM_buff_count = 0;
  
  for(i=0;i<SIM_BUFF_SIZE;i++)
  {
    SIM_Data_Buff[i]=0;
  }
}



/*******************************************************************************
* Function Name  : str_find_first_location(char *source, char *str)
* Description    : find the first location of *str in *source string
* Input          : *source
*                  *str: string you need find
* Output         : None
* Return         : -1   : not found string
*                  other: location found of string
*******************************************************************************/
signed int str_find_first_location(char *source, char *str)
{
  unsigned int leng_source = 0;
  unsigned int leng_str;
  unsigned int i, j;

  leng_source = strlen(source);
  leng_str = strlen(str);

  if (leng_source >= leng_str)
  {
    for (i = 0; i <= leng_source - leng_str; i++)
    {
      for (j = 0; j < leng_str; j++)
      {
        if (source[i + j] != str[j]) break;
      }
      if (j == leng_str) return i;
    }
    return -1;
  }
  return -1;
}


/*******************************************************************************
* Function Name  : str_find_last_location(char *source, char *str)
* Description    : find the last location of *str in *source string
* Input          : *source
*                  *str: string you need find
* Output         : None
* Return         : -1   : not found string
*                  other: location found of string
*******************************************************************************/
signed int str_find_last_location(char *source, char *str)
{
  unsigned int leng_source = 0;
  unsigned int leng_str;
  unsigned int i, j;

  leng_source = strlen(source);
  leng_str = strlen(str);

  if (leng_source >= leng_str)
  {
    for (i = 0; i <= leng_source - leng_str; i++)
    {
      for (j = 0; j < leng_str; j++)
      {
        if (source[i + j] != str[j]) break;
      }
      if (j == leng_str) return (i+leng_str);
    }
    return -1;
  }
  return -1;
}



/*******************************************************************************
* Function Name  : DV_SIM_GPIO_Init
* Description    : Init GPIO for module sim
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DV_SIM_GPIO_Init(void)
{
  GPIO_InitTypeDef SIM_GPIO_Config;
  
  /* GPIO Periph clock enable */
  RCC_APB2PeriphClockCmd(SIM_PW_RCC, ENABLE);

  /* SIM ON pin config */
  SIM_GPIO_Config.GPIO_Pin= SIM_PW_PIN;
  SIM_GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;	//	Max Freq. is 50 Mhz.
  SIM_GPIO_Config.GPIO_Mode = GPIO_Mode_Out_PP;	  //	I/O output mode = Push Pull.
  GPIO_Init(SIM_PW_PORT,&SIM_GPIO_Config);
}


/*******************************************************************************
* Function Name  : DV_SIM_USART_Init
* Description    : Init Usart interface of module sim
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DV_SIM_USART_Init(void)
{
  GPIO_InitTypeDef SIM_GPIO_Config;
  USART_InitTypeDef SIM_USART_Config;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* GPIO Periph clock enable */
  RCC_APB2PeriphClockCmd(SIM_USART_IO_RCC, ENABLE);
  RCC_APB1PeriphClockCmd(SIM_USART_RCC, ENABLE);
  
  
  /* USART3 pins config */
  // USART TX Pin:
  SIM_GPIO_Config.GPIO_Pin   = SIM_TX_PIN;
  SIM_GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;	//	Max Freq. is 50 Mhz.
  SIM_GPIO_Config.GPIO_Mode  = GPIO_Mode_AF_PP;
  GPIO_Init(SIM_TX_PORT,&SIM_GPIO_Config);
  // USART RX Pin:
  SIM_GPIO_Config.GPIO_Pin   = SIM_RX_PIN;
  SIM_GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;	//	Max Freq. is 50 Mhz.
  SIM_GPIO_Config.GPIO_Mode  = GPIO_Mode_IN_FLOATING;	
  GPIO_Init(SIM_RX_PORT,&SIM_GPIO_Config);
  
  /* USART mode config */
  SIM_USART_Config.USART_BaudRate   = 9600;
  SIM_USART_Config.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;
  SIM_USART_Config.USART_Parity     = USART_Parity_No;
  SIM_USART_Config.USART_StopBits   = USART_StopBits_1;
  SIM_USART_Config.USART_WordLength = USART_WordLength_8b;
  SIM_USART_Config.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  
  USART_Init(SIM_USART_X, &SIM_USART_Config);
  
  /* Enable interrup when receive data from USART */
  USART_ITConfig(SIM_USART_X, USART_IT_RXNE, ENABLE);
  USART_ClearFlag(SIM_USART_X,USART_FLAG_TC);

  /* Enable USART */
  USART_Cmd(SIM_USART_X, ENABLE);
  
  /* Config SIM USART Interrupt */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel = SIM_USART_X_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);  
}


/*******************************************************************************
* Function Name  : USART3_IRQHandler
* Description    : demo Programmer
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART2_IRQHandler(void)                             
{  
	if(USART_GetITStatus(SIM_USART_X, USART_IT_RXNE)== SET)
	{
    USART_ClearFlag(USART2,USART_IT_RXNE);
    SIM_Rx_data = USART_ReceiveData(SIM_USART_X);
    SIM_Rx_Complete = 1;
    
    if(SIM_buff_count < SIM_BUFF_SIZE)
    {
      SIM_Data_Buff[SIM_buff_count] = SIM_Rx_data;
      SIM_buff_count++;	
    }
    else
    {
      SIM_buff_count = 0;
      SIM_Data_Buff[SIM_buff_count] = SIM_Rx_data;
    }
 
    /* print to usart debug consol */
    USART_SendData(USART1, SIM_Rx_data);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == 0);
	}	
}


/*******************************************************************************
* Function Name  : DV_SIM_printf(uint8_t *s)
* Description    : send a string to SIM. This funcion same as USART3_print_str
* Input          : - *s: string sends to SIM
* Output         : None
* Return         : None
*******************************************************************************/
void DV_SIM_printf(uint8_t *s)
{
  while (*s != 0x00)
  {
    USART_SendData(SIM_USART_X, *s);
    while (USART_GetFlagStatus(SIM_USART_X, USART_FLAG_TC) == 0);

    s++;
  }
}


/*******************************************************************************
* Function Name  : DV_SIM_AT_Cmd(char* data)
* Description    : Send a command to SIM
* Input          : - *data: this is string command 
* Output         : None
* Return         : - 1: not error - 0: error
*******************************************************************************/
uint8_t DV_SIM_AT_Cmd(char *data)
{
  uint8_t error = 1;
  uint16_t time_out = 100;
  
  /* send data string to sim */
  DV_SIM_printf((uint8_t *) data);
    
  /* send <CR> */
  USART_SendData(SIM_USART_X,0x0d);
  while (USART_GetFlagStatus(SIM_USART_X, USART_FLAG_TC) == 0);
  
  while (1)
  {
    if (str_find_first_location(&SIM_Data_Buff[0], "OK") >= 0)
    {
      error = 0;
      break;
    }
    if (str_find_last_location(&SIM_Data_Buff[0], "ERROR") >= 0)
    {
      error = 1;
      break;
    }
    delay_ms(10);
    time_out--;
    if (!time_out) return 0;
  }
  
  delay_ms(150);
  if (error == 0) return 1;
  else return 0; //Error
}


/*******************************************************************************
* Function Name  : DV_SIM_AT_Sync(void)
* Description    : synchronous baudrate of module sim with MCU
* Input          : None
* Output         : None
* Return         : - 1: not error - 0: error
*******************************************************************************/
uint8_t DV_SIM_AT_Sync(void)
{
  uint8_t count = 0;

  while (count < 5)
  {
    /* send "AT" command to synchronous module sim */
    if (DV_SIM_AT_Cmd("AT") == 0)
      count++;
    else 
      return 1;
  }

  return 0;
}


/*******************************************************************************
* Function Name  : DV_SIM_Check_Service(void)
* Description    : check sim service provider
* Input          : None
* Output         : None
* Return         : 0 - error
*                  1 - VinaPhone
*                  2 - Viettel
*                  3 - MobiPhone
*                  4 - VietnamMobile
*******************************************************************************/
uint8_t DV_SIM_Check_Service(void)
{
  uint16_t time_out = 60;
  signed char status = 0;
  
  
  /* clear receive buffer */
  DV_SIM_Buff_Clear();
  
  /* check sim service provider */  
  do
  {
    DV_SIM_Buff_Clear();
    /* check sim service provider */
    DV_SIM_AT_Cmd("AT+COPS?"); 
    delay_ms(1000);
    
    status= str_find_last_location(SIM_Data_Buff, ": 0,0,");
    
    time_out--;
    if (!time_out) return 0;
  }
  while (status < 0);
  
  if ((str_find_last_location(&SIM_Data_Buff[12], "VNM and VIETTEL") >= 0))
  {
    return Viettel;
  }

  else if ((str_find_last_location(&SIM_Data_Buff[12], "Mobifone") >= 0))
  {
    return MobiPhone;
  }

  else if ((str_find_last_location(&SIM_Data_Buff[12], "VN VINAPHONE") >= 0))
  {
    return VinaPhone;
  }

  else if ((str_find_last_location(&SIM_Data_Buff[12], "Beeline VN") >= 0))
  {
    return BeeLine;
  }
  
  else return 0;
}




/*******************************************************************************
 * Function Name  : DV_SIM_Startup(void)
 * Description    : startup module Sim BG2
 * Input          : None
 * Output         : None
 * Return         : - gsm_service: service provider
 *                  - 0: error
 *******************************************************************************/
uint8_t DV_SIM_Startup(void)
{
  uint8_t gsm_service = 0x00;
  
  /* clear receive buffer */
  DV_SIM_Buff_Clear();
  
  /* synchronous module sim */
  if (DV_SIM_AT_Sync() == 0)
  {
    return 0;
  }
  
  DV_SIM_AT_Cmd("ATE0");
  /* check sim service provider */
  gsm_service = DV_SIM_Check_Service();
  if (!gsm_service)
  {
    return 0;
  }

  /* Signal quality */
  DV_SIM_AT_Cmd("AT+CSQ");

//  /* turn on GPRS */
//  if (DV_SIM_Gprs_On(gsm_service) == 0)
//  {
//    return 0;
//  }
  
  delay_ms(1000);
  return gsm_service;
}


/*******************************************************************************
* Function Name  : DV_SIM_Reset(void)
* Description    : Reset module sim
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint8_t DV_SIM_Reset(void)
{
  uint8_t sim = 0;
  uint16_t i = 0;
  
  /* Clear buff */
  for (i = 0; i < 400; i++)
  {
    SIM_Data_Buff[i] = ' ';
  }
  __SIM_OFF();
  delay_ms(100);
  __SIM_ON();
  delay_ms(100);
  __SIM_OFF();
  delay_ms(2000);
  
  sim = DV_SIM_Startup();
  
  return sim;
}


/************************* End of File ****************************************/

