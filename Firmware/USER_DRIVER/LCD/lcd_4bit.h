/*********************************Copyright (c)*********************************
**                                      
**                          http://www.webdemo.com
**
**--------------File Info-------------------------------------------------------
** File name:               lcd_4bit.c
** Descriptions:            
* CHUONG TRINH DIEU KHIEN LCD SU DUNG CHIP DRIVER HD4470 HITACHI
* 
* Che do hoat dong: 4-bit Mode
******************************************************************************
**
**------------------------------------------------------------------------------
** Created by:              Truong Cong Tri
** Created date:            2014-12-13
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
#ifndef _LCD_4BIT_H_
#define _LCD_4BIT_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include <stdlib.h>
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
/* set pin is input or output */
typedef enum {
  DIR_INPUT = 0,
  DIR_OUTPUT = 1
}tDIR;

typedef union {
  uint8_t Byte;
  struct 
  {
    uint8_t Bit0 : 1;
    uint8_t Bit1 : 1;
    uint8_t Bit2 : 1;
    uint8_t Bit3 : 1;
    uint8_t Bit4 : 1;
    uint8_t Bit5 : 1;
    uint8_t Bit6 : 1;
    uint8_t Bit7 : 1;
  } Bits;
} tBYTE;

/* Private define ------------------------------------------------------------*/
/* LCD pins mapping */
#define LCD_RS_PIN			GPIO_Pin_7
#define LCD_RS_PORT     GPIOB
#define LCD_RS_RCC      RCC_APB2Periph_GPIOB

#define LCD_RW_PIN			GPIO_Pin_8
#define LCD_RW_PORT     GPIOB
#define LCD_RW_RCC      RCC_APB2Periph_GPIOB

#define LCD_EN_PIN			GPIO_Pin_9
#define LCD_EN_PORT     GPIOB
#define LCD_EN_RCC      RCC_APB2Periph_GPIOB

#define LCD_DATA4_PIN		GPIO_Pin_3
#define LCD_DATA4_PORT  GPIOB
#define LCD_DATA4_RCC   RCC_APB2Periph_GPIOB

#define LCD_DATA5_PIN		GPIO_Pin_4
#define LCD_DATA5_PORT  GPIOB
#define LCD_DATA5_RCC   RCC_APB2Periph_GPIOB

#define LCD_DATA6_PIN		GPIO_Pin_5
#define LCD_DATA6_PORT  GPIOB
#define LCD_DATA6_RCC   RCC_APB2Periph_GPIOC

#define LCD_DATA7_PIN		GPIO_Pin_6
#define LCD_DATA7_PORT  GPIOB
#define LCD_DATA7_RCC   RCC_APB2Periph_GPIOC

//#define LCD_RS_PIN			GPIO_Pin_3
//#define LCD_RS_PORT     GPIOC
//#define LCD_RS_RCC      RCC_APB2Periph_GPIOC

//#define LCD_RW_PIN			GPIO_Pin_2
//#define LCD_RW_PORT     GPIOC
//#define LCD_RW_RCC      RCC_APB2Periph_GPIOC

//#define LCD_EN_PIN			GPIO_Pin_1
//#define LCD_EN_PORT     GPIOC
//#define LCD_EN_RCC      RCC_APB2Periph_GPIOC

//#define LCD_DATA4_PIN		GPIO_Pin_10
//#define LCD_DATA4_PORT  GPIOC
//#define LCD_DATA4_RCC   RCC_APB2Periph_GPIOC

//#define LCD_DATA5_PIN		GPIO_Pin_11
//#define LCD_DATA5_PORT  GPIOC
//#define LCD_DATA5_RCC   RCC_APB2Periph_GPIOC

//#define LCD_DATA6_PIN		GPIO_Pin_12
//#define LCD_DATA6_PORT  GPIOC
//#define LCD_DATA6_RCC   RCC_APB2Periph_GPIOC

//#define LCD_DATA7_PIN		GPIO_Pin_2
//#define LCD_DATA7_PORT  GPIOD
//#define LCD_DATA7_RCC   RCC_APB2Periph_GPIOD

/* LCD display commands (use operation AND BIT "&" to combine effects) */
#define _LCD_ON_ALL             0x0f		// display on, on cursor and blink it
#define _LCD_OFF                0x0b		// display off
#define _LCD_CUR_OFF			      0x0d		// cursor off
#define _LCD_BLINK_OFF          0x0e		// cursor not blink
#define _LCD_CUR_AND_BLINK_OFF  0x0c		// cursor off and not blink

#define	_LCD_CLEAR              0x01		// clear screen and move cursor to home
#define _LCD_CUR_TO_HOME        0x02		// move cursor to home

/* LCD Entry Mode Set commands */
#define _LCD_SHIFT_CUR_LEFT    	0x04		// Cursor shifts to the left
#define _LCD_SHIFT_CUR_RIGHT   	0x05		// Cursor shifts to the right
#define _LCD_SHIFT_DISP_LEFT   	0x06		// Display shifts to the left
#define _LCD_SHIFT_DISP_RIGHT  	0x07		// Display shifts to the right


/* LCD Function set commands (use operation OR BIT "|" to combine funcitons) */
#define _LCD_4BIT_MODE          0x20		// select 4-bit protocol mode
#define _LCD_8BIT_MODE          0x30		// select 8-bit protocol mode

#define _LCD_1LINE_MODE         0x20		// select 1 line display mode
#define _LCD_2LINE_MODE         0x28		// select 2 lines display mode

#define _LCD_FONT_5X7_MODE      0x20		// select font 5x7 display mode
#define _LCD_FONT_5X10_MODE     0x24		// select font 5x10 display mode

#define _LCD_4BIT_DEFAULT_MODE  0x28		// select 4-bit mode, 2 lines display, font 5x7
#define _LCD_8BIT_DEFAULT_MODE  0x38		// select 8-bit mode, 2 lines display, font 5x7

/* LCD set CGRAM */
#define _LCD_SET_CGRAM          0x40

/* LCD Commands */
#define _LCD_CLEAR_DISPLAY      0x01 		// Clear entire display and set Display Data Address to 0
#define _LCD_RETRN_HOME         0x02 		// sets DDRAM address 0 and returns display from being shifted to original position.
#define _LCD_DISP_INIT          0x28 		// function set is 4 bit data length and 2 lines
#define _LCD_INC_MODE           0x06 		// Entry mode is display Data RAM pointer incremented after write
#define _LCD_DISP_ON            0x0C		// Sets entire display on, cursor on and blinking of cursor position character
#define _LCD_DISP_OFF           0x08   	// Sets entire display off, cursor off
#define _LCD_CURSOR_ON          0x04		// turn on cursor
#define _LCD_CURSOR_OFF         0x00    // turn off cursor
#define _LCD_CUR_MOV_LEFT       0x10		// Cursor move and shift to left
#define _LCD_CUR_MOV_RIGHT			0x14		// Cursor move and shift to right
#define _LCD_BUSY               0x80    // LCD is busy


/* Private macro -------------------------------------------------------------*/
#define LCD_RS_PIN_HIGH()  GPIO_WriteBit(LCD_RS_PORT, LCD_RS_PIN, (BitAction)1)
#define LCD_RS_PIN_LOW()   GPIO_WriteBit(LCD_RS_PORT, LCD_RS_PIN, (BitAction)0)

#define LCD_RW_PIN_HIGH()  GPIO_WriteBit(LCD_RW_PORT, LCD_RW_PIN, (BitAction)1)
#define LCD_RW_PIN_LOW()   GPIO_WriteBit(LCD_RW_PORT, LCD_RW_PIN, (BitAction)0)

#define LCD_EN_PIN_HIGH()  GPIO_WriteBit(LCD_EN_PORT, LCD_EN_PIN, (BitAction)1)
#define LCD_EN_PIN_LOW()   GPIO_WriteBit(LCD_EN_PORT, LCD_EN_PIN, (BitAction)0)

#define LCD_DATA7_IN()  LCD_Pin_Dir(LCD_DATA7_PORT, LCD_DATA7_PIN, DIR_INPUT)
#define LCD_DATA7_OUT() LCD_Pin_Dir(LCD_DATA7_PORT, LCD_DATA7_PIN, DIR_OUTPUT)

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Functions using for control LCD */
void write_data(unsigned char _data_in);
void LCD_delay(uint32_t time);
void LCD_wait_busy(void);
void LCD_Pin_Dir(GPIO_TypeDef* LCD_Port, uint16_t LCD_Pin, tDIR Dir);

/* Functions using for init and config LCD */
void LCD_init(void);
void LCD_command(unsigned char _command);

/* Function using for put data to LCD */
void LCD_clear(void);
void LCD_putc(unsigned char charactor);
void LCD_puts(const char *string_char);
void LCD_gotoxy(unsigned char col, unsigned char row);

/* Function using for write data to CGRAM */
void LCD_write_cgram(char add_cgram, const char custom_char[]);
void LCD_custom_char(char row, char col, char add_cgram, const char custom_char[]);

/* Private functions ---------------------------------------------------------*/



/************************* End of File ****************************************/

#endif /* <MSN>_H */

