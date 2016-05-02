#include "DHT11.h"
#include "systick_delay/systick_delay.h"

void DHT11_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOA, GPIO_Pin_0);
}

static void DHT11_Mode_IPU(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static void DHT11_Mode_Out_PP(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static uint8_t Read_Byte(void)
{
  uint8_t i, temp = 0;
  uint32_t time_out;
  
  for (i = 0; i < 8; i++)
  {
    time_out = 10000;
    while (DHT11_DATA_IN() == Bit_RESET)
    {
      time_out--;
      if(time_out==0) return 0;
    };
    Sys_Delay_us(60);
    
    if (DHT11_DATA_IN() == Bit_SET)
    {
      time_out = 10000;
      while (DHT11_DATA_IN() == Bit_SET)
      {
        time_out--;
        if(time_out==0) return 0;
      }
      
      temp |= (uint8_t) (0x01 << (7 - i));
    }
    else
    {
      temp &= (uint8_t) ~(0x01 << (7 - i));
    }
  }
  return temp;
}

uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data)
{
  uint32_t time_out;
  
  DHT11_Mode_Out_PP();
  DHT11_DATA_OUT(LOW);
  Sys_Delay_ms(18);
  DHT11_DATA_OUT(HIGH);
  Sys_Delay_us(30);
  DHT11_Mode_IPU();
  if (DHT11_DATA_IN() == Bit_RESET)
  {
    time_out = 10000;
    while (DHT11_DATA_IN() == Bit_RESET) 
    {
      time_out--;
      if(time_out == 0) return 0;
    }
    
    time_out = 10000;
    while (DHT11_DATA_IN() == Bit_SET)
    {
      time_out--;
      if(time_out == 0) return 0;
    }
    DHT11_Data->humi_int = Read_Byte();
    DHT11_Data->humi_deci = Read_Byte();
    DHT11_Data->temp_int = Read_Byte();
    DHT11_Data->temp_deci = Read_Byte();
    DHT11_Data->check_sum = Read_Byte();
    DHT11_Mode_Out_PP();
    DHT11_DATA_OUT(HIGH);

    if (DHT11_Data->check_sum == DHT11_Data->humi_int + DHT11_Data->humi_deci + DHT11_Data->temp_int + DHT11_Data->temp_deci)
      return SUCCESS;
    else
      return ERROR;
  }
  else
  {
    return ERROR;
  }

}




/*************************************END OF FILE******************************/
