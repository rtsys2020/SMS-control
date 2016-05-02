/*********************************Copyright (c)*********************************
**                                      
**                          http://www.tri-iuh.blogspot.com
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


/* Includes ------------------------------------------------------------------*/
#include "lcd_4bit.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : LCD_Pin_Dir(GPIO_TypeDef* LCD_Port, uint16_t LCD_Pin, tDIR Dir)
* Description    : set dir for IO
* Input          : - LCD_Port : Port of PIN
*                  - LCD_Pin  : Pin need set dir
* Output         : - Dir : DIR_INPUT or DIR_OUTPUT
* Return         : None
* Attention      : None
*******************************************************************************/
void LCD_Pin_Dir(GPIO_TypeDef* LCD_Port, uint16_t LCD_Pin, tDIR Dir)
{
  GPIO_InitTypeDef GPIO_Configuration;
  
  if(Dir == DIR_INPUT)
  {
    GPIO_Configuration.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  }
  else
  {
    GPIO_Configuration.GPIO_Mode = GPIO_Mode_Out_PP;
  }
  GPIO_Configuration.GPIO_Pin = LCD_Pin;
  GPIO_Configuration.GPIO_Speed = GPIO_Speed_50MHz;
  
  GPIO_Init(LCD_Port, &GPIO_Configuration);  
}

/*******************************************************************************
* Function Name  : write_data(tBYTE _data)
* Description    : write data to LCD
* Input          : _data
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void write_data(unsigned char _data_in)
{
  tBYTE _data;
  _data.Byte = _data_in;
  /* disable EN pin */
  LCD_EN_PIN_LOW();

  /* write 4-bit high to LCD*/
  GPIO_WriteBit(LCD_DATA7_PORT, LCD_DATA7_PIN, (BitAction)_data.Bits.Bit7);
  GPIO_WriteBit(LCD_DATA6_PORT, LCD_DATA6_PIN, (BitAction)_data.Bits.Bit6);
  GPIO_WriteBit(LCD_DATA5_PORT, LCD_DATA5_PIN, (BitAction)_data.Bits.Bit5);
  GPIO_WriteBit(LCD_DATA4_PORT, LCD_DATA4_PIN, (BitAction)_data.Bits.Bit4);

  LCD_EN_PIN_HIGH();
  LCD_EN_PIN_LOW();
  LCD_delay(1);

  /* write 4-bit low to LCD */
  GPIO_WriteBit(LCD_DATA7_PORT, LCD_DATA7_PIN, (BitAction)_data.Bits.Bit3);
  GPIO_WriteBit(LCD_DATA6_PORT, LCD_DATA6_PIN, (BitAction)_data.Bits.Bit2);
  GPIO_WriteBit(LCD_DATA5_PORT, LCD_DATA5_PIN, (BitAction)_data.Bits.Bit1);
  GPIO_WriteBit(LCD_DATA4_PORT, LCD_DATA4_PIN, (BitAction)_data.Bits.Bit0);
  
  LCD_EN_PIN_HIGH();
  LCD_EN_PIN_LOW();
  LCD_delay(1);
}


/*******************************************************************************
* Function Name  : LCD_wait_busy(void)
* Description    : wait for LCD not busy
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void LCD_wait_busy(void)
{
  unsigned char busy;

  /* set pin LCD_DATA7 is input */
  LCD_Pin_Dir(LCD_DATA7_PORT, LCD_DATA7_PIN, DIR_INPUT);

  LCD_RW_PIN_HIGH();
  LCD_RS_PIN_LOW();

  do
  {
    LCD_EN_PIN_HIGH(); //get 4-bit hight data
    //LCD_delay(5);
    busy = GPIO_ReadInputDataBit(LCD_DATA7_PORT, LCD_DATA7_PIN); //and assign MSB-bit for busy
    LCD_EN_PIN_LOW();

    LCD_EN_PIN_HIGH(); //get 4-bit low data
    //LCD_delay(5);
    LCD_EN_PIN_LOW();  //and do nothing else
  }
  while (busy);

  /* set pin LCD_DATA7 is output */
  LCD_Pin_Dir(LCD_DATA7_PORT, LCD_DATA7_PIN, DIR_OUTPUT);
}


/*******************************************************************************
* Function Name  : LCD_command(tBYTE _command)
* Description    : write command control to LCD
* Input          : _command
* Output         : None
* Return         : None
* Attention      : this function write command to LCD. The commands can be 
*                  hex number or defines at LCD_4bit.h file
*******************************************************************************/
void LCD_command(unsigned char _command)
{
  LCD_wait_busy(); // wait for LCD not busy
  LCD_RS_PIN_LOW();
  LCD_RW_PIN_LOW();
  write_data(_command);
}


/*******************************************************************************
* Function Name  : LCD_init(void)
* Description    : init LCD
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void LCD_init(void)
{
  /* config RCC */
  RCC_APB2PeriphClockCmd(LCD_RS_RCC, ENABLE);
  RCC_APB2PeriphClockCmd(LCD_RW_RCC, ENABLE);
  RCC_APB2PeriphClockCmd(LCD_EN_RCC, ENABLE);
  RCC_APB2PeriphClockCmd(LCD_DATA4_RCC, ENABLE);
  RCC_APB2PeriphClockCmd(LCD_DATA5_RCC, ENABLE);
  RCC_APB2PeriphClockCmd(LCD_DATA6_RCC, ENABLE);
  RCC_APB2PeriphClockCmd(LCD_DATA7_RCC, ENABLE);
  
  /* set dir for IO of LCD */
  LCD_Pin_Dir(LCD_RS_PORT, LCD_RS_PIN, DIR_OUTPUT);
  LCD_Pin_Dir(LCD_RW_PORT, LCD_RW_PIN, DIR_OUTPUT);
  LCD_Pin_Dir(LCD_EN_PORT, LCD_EN_PIN, DIR_OUTPUT);
  
  LCD_Pin_Dir(LCD_DATA7_PORT, LCD_DATA7_PIN, DIR_OUTPUT);
  LCD_Pin_Dir(LCD_DATA6_PORT, LCD_DATA6_PIN, DIR_OUTPUT);
  LCD_Pin_Dir(LCD_DATA5_PORT, LCD_DATA5_PIN, DIR_OUTPUT);
  LCD_Pin_Dir(LCD_DATA4_PORT, LCD_DATA4_PIN, DIR_OUTPUT);

  LCD_EN_PIN_LOW();
  LCD_RS_PIN_LOW();
  LCD_RW_PIN_LOW();
  LCD_delay(100);

  // reset LCD
  LCD_command(0x30);
  LCD_command(0x30);
  LCD_command(0x30);
  LCD_command(0x32);
  LCD_delay(100); //wait for LCD reset

  // setup LCD
  LCD_command(0x20);
  LCD_command(_LCD_4BIT_DEFAULT_MODE); //command value: 0x28 - set function
  LCD_command(_LCD_CUR_OFF & _LCD_BLINK_OFF); //command value: 0x0c - set display control
  LCD_command(_LCD_SHIFT_DISP_LEFT); //command value: 0x06 - set entry mode
  LCD_command(_LCD_CLEAR);
  LCD_delay(1000);

}




/*******************************************************************************
* Function Name  : LCD_putc(char charactor)
* Description    : write a charactor to LCD
* Input          : charactor
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void LCD_putc(unsigned char charactor)
{
  LCD_wait_busy(); // wait for LCD not busy
  LCD_RS_PIN_HIGH();
  LCD_RW_PIN_LOW();
  LCD_EN_PIN_LOW();

  write_data(charactor);
}



/*******************************************************************************
* Function Name  : LCD_puts(const char *string_char)
* Description    : put string to LCD
* Input          : *string_char
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void LCD_puts(const char *string_char)
{
  do
  {
    LCD_putc(*string_char);
    string_char++;
  }
  while (*string_char);
}


/*******************************************************************************
* Function Name  : LCD_clear(void)
* Description    : clear LCD
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void LCD_clear(void)
{
  LCD_wait_busy(); // wait for LCD not busy
  LCD_RS_PIN_LOW();
  LCD_RW_PIN_LOW();
  write_data(_LCD_CLEAR);
}



/*******************************************************************************
* Function Name  : LCD_delay(uint32_t time)
* Description    : make time for delay
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void LCD_delay(uint32_t time)
{
	uint32_t index=0;
	
	index = time*8000;
	while(index--);	
}




/*******************************************************************************
* Function Name  : LCD_gotoxy(unsigned char col, unsigned char row)
* Description    : put point to columm and row
* Input          : - col: colume of LCD (0 -> 1 with LCD 16x2 and 0->4 with LCD 20x4)
*                  - row: row of LCD (0 -> 1 with LCD 16x2 and 0->4 with LCD 20x4)
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void LCD_gotoxy(unsigned char col, unsigned char row)
{
  unsigned char address;

  switch (row)
  {
  case 0: address = 0x00;
    break; //line 1: begin 0  (0x00) -> 39  (0x27)
  case 1: address = 0x40;
    break; //line 2: begin 64 (0x40) -> 103 (0x67)
  case 2: address = 0x14;
    break; //line 3: begin 64 (0x40) -> 103 (0x67)
  case 3: address = 0x54;
    break; //line 4: begin 64 (0x40) -> 103 (0x67)
  default:address = 0x00;
}

  address += col;
  LCD_command(0x80 | address); //goto memory cell at address of DDRAM (setup DDRAM address)
  LCD_wait_busy();
}



/*******************************************************************************
* Function Name  : LCD_custom_char(char row, char col, char add_cgram, const char custom_char[])
* Description    : write a custome charactor to LCD
* Input          : + row		: 0 -> 4 : vi tri hang muon di chuyen den
*                  + col		: 0 -> 39: vi tri cot muon di chuyen den
*                  + add_cgram	: 0 -> 7 : dia chi vung nho dung de luu ky tu muon tao
*                                          bo nho CGRAM co 64 o tuong ung voi 8 ky tu ,
*                  + custom_char:  mang chua ma cua ky tu muon tao ra
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void LCD_custom_char(char row, char col, char add_cgram, const char custom_char[])
{
  char i, add_cgram_cell = 1;

  add_cgram_cell = add_cgram * 8;

  LCD_command(_LCD_SET_CGRAM | add_cgram_cell); //set CGRAM address

  for (i = 0; i < 8; i++)LCD_putc(custom_char[i]);

  LCD_gotoxy(col, row);
  LCD_putc(add_cgram);
}


/*******************************************************************************
* Function Name  : LCD_write_cgram(char add_cgram, const char custom_char[])
* Description    : ghi mot ky tu tu tao vao CGRAM
* Input          : + add_cgram	: 0 -> 7 : dia chi vung nho dung de luu ky tu muon tao
*                                 bo nho CGRAM co 64 o tuong ung voi 8 ky tu ,
*                  + custom_char:  mang chua ma cua ky tu muon tao ra
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void LCD_write_cgram(char add_cgram, const char custom_char[])
{
  char i, add_cgram_cell = 1;

  add_cgram_cell = add_cgram * 8;
  LCD_command(_LCD_SET_CGRAM | add_cgram_cell); //set CGRAM address

  for (i = 0; i < 8; i++)
  {
    LCD_putc(custom_char[i]);
  }

  LCD_gotoxy(0, 0); //dua con tro ve dau dong hang thu nhat
}



/************************* End of File ****************************************/

