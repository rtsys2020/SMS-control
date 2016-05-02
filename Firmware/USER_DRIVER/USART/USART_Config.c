/*********************************Copyright (c)*********************************
**                                      
**                          http://www.tri-iuh.blogspot.com
**
**--------------File Info-------------------------------------------------------
** File name:               USART_config.c
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


/* Includes ------------------------------------------------------------------*/
#include "USART_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

PUTCHAR_PROTOTYPE
{
  USART_SendData(USART1, (uint8_t) ch);
  /* Loop until data complete tranmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == 0);

  return ch;
}

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/*******************************************************************************
* Function Name  : USART1_putc(char ch)
* Description    : The function transmits a byte via the UART module
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART1_putc(char ch)
{
  USART_SendData(USART1, (uint8_t) ch);
  /* Loop until data complete tranmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == 0);
}


/*******************************************************************************
* Function Name  : USART_putc(USART_TypeDef* USARTx, char ch)
* Description    : The function transmits a byte via the UART module
* Input          : - USART_TypeDef* USARTx : USART1, USART2,...
*                  - char ch: data
* Output         : None
* Return         : None
*******************************************************************************/
void USART_putc(USART_TypeDef* USARTx, char ch)
{
  USART_SendData(USARTx, (uint8_t) ch);
  /* Loop until data complete tranmission */
  while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == 0);
}

/*******************************************************************************
* Function Name  : USART1_puts(char * str)
* Description    : The function transmits a string via the UART module
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART1_puts(char * str)
{
  while(*str)
  {
    USART1_putc(*str);
    str++;
  }
}


/*******************************************************************************
* Function Name  : USART_puts(USART_TypeDef* USARTx, char * str)
* Description    : The function transmits a string via the UART module
* Input          : - USART_TypeDef* USARTx : USART1, USART2,...
*                  - char * str: string data
* Output         : None
* Return         : None
*******************************************************************************/
void USART_puts(USART_TypeDef* USARTx, char* str)
{
  while(*str)
  {
    USART_putc(USARTx, *str);
    str++;
  }
}


/*******************************************************************************
* Function Name  : USART1_put_var(int32_t var)
* Description    : this function prints a numeric number to console monitor
* Input          : None
* Output         : None
* Return         : None
* Attention      : this function used to debug
*******************************************************************************/
void USART1_put_var(int32_t var)
{
  uint8_t str[10];
  int i;
  uint8_t start=9; /* gia tri cua start bang gia tri cua mang str - 1*/
  
  /* if number < 0 then put '-' charactor */
  if(var < 0)
  {
    USART1_putc('-');
    var *= -1;
  }
  
  /* split the number */
  for(i=0;i<10;i++)
  {
    str[i] = var % 10 + '0';
    var /= 10;
  }
  
  /* start point */
  for(i=9;i>=0;i--)
  {
    if(str[i]=='0')
      start--;
    else
      break;
  }

  /* put number to console debug */
  for(i=start;i>=0;i--)
  {
    USART1_putc(str[i]);
  }

}


/*******************************************************************************
* Function Name  : USART_put_var(USART_TypeDef* USARTx, int32_t var)
* Description    : this function prints a numeric number to console monitor
* Input          : - USART_TypeDef* USARTx : USART1, USART2,...
*                  - int32_t var: int number (max 10 number)
* Output         : None
* Return         : None
*******************************************************************************/
void USART_put_var(USART_TypeDef* USARTx, int32_t var)
{
  uint8_t str[10];
  int i;
  uint8_t start=9; /* gia tri cua start bang gia tri cua mang str - 1*/
  
  /* if number < 0 then put '-' charactor */
  if(var < 0)
  {
    USART_putc(USARTx, '-');
    var *= -1;
  }
  
  /* split the number */
  for(i=0;i<10;i++)
  {
    str[i] = var % 10 + '0';
    var /= 10;
  }
  
  /* start point */
  for(i=9;i>=0;i--)
  {
    if(str[i]=='0')
      start--;
    else
      break;
  }
  
  /* put number to console debug */
  for(i=start;i>=0;i--)
  {
    USART_putc(USARTx, str[i]);
  }
}






/************************* End of File ****************************************/
