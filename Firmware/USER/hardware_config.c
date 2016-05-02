/*********************************Copyright (c)*********************************
**                                      
**                          http://www.tri-iuh.blogspot.com
**
**--------------File Info-------------------------------------------------------
** File name:               hardware_config.c
** Descriptions:            config hardware for GSM board
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


/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/*******************************************************************************
* Function Name  : GPIO_Board_Config(void)
* Description    : name_programmer
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void GPIO_Board_Config(void)
{
  
  GPIO_InitTypeDef GPIO_Config;
  
  /* Enable APB2 peripheral clock */ 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
  
  /* Config GPIO for relay-------------------------------------------
  - PB13 --> RELAY 1
  - PB12 --> RELAY 2
  */
  GPIO_Config.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_Config.GPIO_Pin   = GPIO_Pin_12 | GPIO_Pin_13;
  GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_Config);
  GPIO_WriteBit(GPIOB, GPIO_Pin_12, (BitAction)0);
  GPIO_WriteBit(GPIOB, GPIO_Pin_13, (BitAction)0);
  
  /* Config GPIO for input ------------------------------------------
  - IN1 --> PB0
  - IN2 --> PB1
  */
  GPIO_Config.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Config.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_Config.GPIO_Speed= GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_Config);
  
  /* Config GPIO for input data -------------------------------------
  - DATA1 --> PA0
  - DATA2 --> PA1
  */
  GPIO_Config.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Config.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_Config.GPIO_Speed= GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_Config);  
}


/*******************************************************************************
* Function Name  : USART1_Debug_Config(void)
* Description    : name_programmer
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void USART1_Debug_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
  NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
	
	/** USART1 GPIO Configuration -------------------------------------
  PA9   --> USART1_TX
  PA10  --> USART1_RX
  */
  /*Configure GPIO pin : PA9 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA10 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	/* USART1 Operation mode Configuration --------------------------*/
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  
	USART_Init(USART1, &USART_InitStruct);
  USART_ClearFlag(USART1,USART_FLAG_TC);
  
  /* Enable interrup when receive data from USART */
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  USART_ClearFlag(USART1, USART_FLAG_TC);

  /* Enable USART */
  USART_Cmd(USART1, ENABLE);
  
  /* Interrupt config for USART -----------------------------------*/
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


/*******************************************************************************
* Function Name  : USART2_GSM_Config(void)
* Description    : name_programmer
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void USART2_GSM_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
  NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

  /*Enable or disable APB2 peripheral clock -----------------------*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/** USART1 GPIO Configuration -------------------------------------
  PA2 --> USART1_TX
  PA3 --> USART1_RX
  */
  /*Configure GPIO pin : PA2 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA3 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	/* USART2 Operation mode Configuration --------------------------*/
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  
	USART_Init(USART2, &USART_InitStruct);
  USART_ClearFlag(USART2,USART_FLAG_TC);
  
  /* Enable interrup when receive data from USART */
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
  USART_ClearFlag(USART2, USART_FLAG_TC);

  /* Enable USART */
  USART_Cmd(USART2, ENABLE);
  
  /* Interrupt config for USART -----------------------------------*/
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


/*******************************************************************************
* Function Name  : Relay_SetState(uint8_t RLx, BitAction END_DIS)
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void Relay_SetState(uint8_t RLx, FunctionalState END_DIS)
{
  switch(RLx)
  {
    case 1: GPIO_WriteBit(GPIOB, GPIO_Pin_12, (BitAction)END_DIS); break;
    case 2: GPIO_WriteBit(GPIOB, GPIO_Pin_13, (BitAction)END_DIS); break;
  }
}

/*******************************************************************************
* Function Name  : uint8_t Relay_GetState(uint8_t RLx)
* Description    : read relay state
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
uint8_t Relay_GetState(uint8_t RLx)
{
  uint8_t state = 0;
  switch(RLx)
  {
    case 1: state = GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_12); break;
    case 2: state = GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_13); break;
  }
  
  return state;
}

/************************* End of File ****************************************/
