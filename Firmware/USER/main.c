/*********************************Copyright (c)*********************************
**                                      
**                          http://www.tri-iuh.blogspot.com
**
**--------------File Info-------------------------------------------------------
** File name:               main.c
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private define ------------------------------------------------------------*/
#define SYS_BUFF_SIZE 100

/* Private define ------------------------------------------------------------*/
#define  PAGE63_START_ADDR (uint32_t)0x0800FC00
#define  PAGE63_END_ADDR   (uint32_t)0x0800FFFF

/* phone number address on flash */
#define  PHONE_ADDR_1     (uint32_t)PAGE63_START_ADDR
#define  PHONE_ADDR_2     (uint32_t)(PAGE63_START_ADDR+4)
#define  PHONE_ADDR_3     (uint32_t)(PAGE63_START_ADDR+8)

/* system password address */
#define  PASS_ADDR_1      (uint32_t)(PAGE63_START_ADDR+12)
#define  PASS_ADDR_2      (uint32_t)(PAGE63_START_ADDR+16)

/* system temperature and humidity */
#define  TEMP_HUMI_ADDR   (uint32_t)(PAGE63_START_ADDR+20)


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint8_t SIM_Rx_Complete;
extern char SIM_Data_Buff[200];

/* variables using for system */
char            SYS_Buff[SYS_BUFF_SIZE] = {0};
const char      SYS_Password_default[6] = "ilove";
char            SYS_Password[6]         = "12345";
char            SYS_Phonenum[13]        = "0938788432";
uint8_t         SYS_Temperature         = 35;
uint8_t         SYS_Humidity            = 70;
FunctionalState SYS_Report              = ENABLE;
tSMS_STATUS     SYS_SMS_Status          = {INIT, INIT};

GSM_Control_Typedef GSM_Control;

DHT11_Data_TypeDef DHT11_Data;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Backup_SYS_Phone(void)
* Description    : backup phone number
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void Backup_SYS_Phone(void)
{  
  /* unlock flash */
  FLASH_UnlockBank1();
  
  /* write data */  
  FLASH_ProgramWord(PHONE_ADDR_1, *(__IO uint32_t*)&SYS_Phonenum[0]);
  FLASH_ProgramWord(PHONE_ADDR_2, *(__IO uint32_t*)&SYS_Phonenum[4]);
  FLASH_ProgramWord(PHONE_ADDR_3, *(__IO uint32_t*)&SYS_Phonenum[8]);
  
  /* lock flash */
  //FLASH_LockBank1();
  
  SYS_Buff_Clear();
  
  /* copy data to buff and put to terminal */
  *(uint32_t *)&SYS_Buff[0] = *(__IO uint32_t*)PHONE_ADDR_1;
  *(uint32_t *)&SYS_Buff[4] = *(__IO uint32_t*)PHONE_ADDR_2;
  *(uint32_t *)&SYS_Buff[8] = *(__IO uint32_t*)PHONE_ADDR_3;
  
  printf("\n\r$>:_Phone backup: %s", SYS_Buff);
}

/*******************************************************************************
* Function Name  : Backup_SYS_Temp_Humi(void)
* Description    : backup temperature and humidity
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void Backup_SYS_Temp_Humi(void)
{
  /* unlock flash */
  FLASH_UnlockBank1();
  
  /* write data */
  SYS_Buff_Clear();
  SYS_Buff[0]=SYS_Temperature;
  SYS_Buff[1]=SYS_Humidity;
  
  FLASH_ProgramWord(TEMP_HUMI_ADDR, *(__IO uint32_t *)&SYS_Buff[0]);
  
  /* lock flash */
  //FLASH_LockBank1();
  
  /* read back data */
  SYS_Buff_Clear();
  
  *(uint32_t *)&SYS_Buff[0] = *(__IO uint32_t*)TEMP_HUMI_ADDR;
  
  printf("\n\r$>:_Temperature value backup: %u", SYS_Buff[0]); 
  printf("\n\r$>:_Humidity value backup    : %u", SYS_Buff[1]); 
}

/*******************************************************************************
* Function Name  : Backup_SYS_Password(void)
* Description    : backup system password
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void Backup_SYS_Password(void)
{
  /* unlock flash */
  FLASH_UnlockBank1();
  
  /* write data */
  FLASH_ProgramWord(PASS_ADDR_1, *(__IO uint32_t*)&SYS_Password[0]);
  FLASH_ProgramWord(PASS_ADDR_2, *(__IO uint32_t*)&SYS_Password[4]);
  
  /* lock flash */
  //FLASH_LockBank1();
  
  /* read back data */
  SYS_Buff_Clear();
  *(uint32_t *)&SYS_Buff[0] = *(__IO uint32_t*)PASS_ADDR_1;
  *(uint32_t *)&SYS_Buff[4] = *(__IO uint32_t*)PASS_ADDR_2;
  
  printf("\n\r$>:_System Password backup: %s", SYS_Buff);  
}

/*******************************************************************************
* Function Name  : Restore_SYS_Phone(void)
* Description    : restore system phone number
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
uint8_t Restore_SYS_Phone(void)
{
  char i;
  
  /* read data from flash */
  SYS_Buff_Clear();
  *(uint32_t *)&SYS_Buff[0] = *(__IO uint32_t*)PHONE_ADDR_1;
  *(uint32_t *)&SYS_Buff[4] = *(__IO uint32_t*)PHONE_ADDR_2;
  *(uint32_t *)&SYS_Buff[8] = *(__IO uint32_t*)PHONE_ADDR_3;  
  
  if(((*(uint32_t *)&SYS_Buff[0]) == 0xffffffff) && \
     ((*(uint32_t *)&SYS_Buff[4]) == 0xffffffff) && \
     ((*(uint32_t *)&SYS_Buff[8]) == 0xffffffff))
  {
    printf("\n\r$>:_Can not restore phone number");
    return 1; 
  }
  else
  {
    for(i=0;i<13;i++)
    {
      SYS_Phonenum[i] = SYS_Buff[i];
    }
    
    printf("\n\r$>:_Restore phone number: %s", SYS_Phonenum);
    
    return 0;    
  }
}

/*******************************************************************************
* Function Name  : Restore_SYS_Password(void)
* Description    : restore system password
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
uint8_t Restore_SYS_Password(void)
{
  char i;
  
  /* read data from flash */
  SYS_Buff_Clear();
  *(uint32_t *)&SYS_Buff[0] = *(__IO uint32_t*)PASS_ADDR_1;
  *(uint32_t *)&SYS_Buff[4] = *(__IO uint32_t*)PASS_ADDR_2;
  
  if(((*(uint32_t *)&SYS_Buff[0]) == 0xffffffff) && \
     ((*(uint32_t *)&SYS_Buff[4]) == 0xffffffff))
  {
    printf("\n\r$>:_Can not restore password");
    return 1;
  }
  else
  {
    for(i=0;i<6;i++)
    {
      SYS_Password[i] = SYS_Buff[i];
    }
    
    printf("\n\r$>:_Restore system password: %s", SYS_Password);
    
    return 0;    
  } 
}

/*******************************************************************************
* Function Name  : Restore_SYS_Temp_Humi(void)
* Description    : restore temperature and humidity
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
uint8_t Restore_SYS_Temp_Humi(void)
{
  /* read data from flash */
  SYS_Buff_Clear();
  *(uint32_t *)&SYS_Buff[0] = *(__IO uint32_t*)TEMP_HUMI_ADDR;
  
  if((*(uint32_t *)&SYS_Buff[0]) == 0xffffffff)
  {
    printf("\n\r$>:_Can not restore temperature and humidity");
    return 1;
  }
  else
  {
    SYS_Temperature = SYS_Buff[0];
    SYS_Humidity = SYS_Buff[1];
    
    printf("\n\r$>:_Restore temperature: %u", SYS_Temperature);
    printf("\n\r$>:_Restore humidity   : %u", SYS_Humidity);
    
    return 0;    
  } 
}

/*******************************************************************************
* Function Name  : Backup_System(void)
* Description    : backup all status off system
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void Backup_System(void)
{
  FLASH_UnlockBank1();
  FLASH_ErasePage(PAGE63_START_ADDR);
  
  Backup_SYS_Password();
  Backup_SYS_Phone();
  Backup_SYS_Temp_Humi();
  
  FLASH_LockBank1();
}

/*******************************************************************************
* Function Name  : str_to_int(char *num_str)
* Description    : convert string to number
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
long str_to_int(char *num_str)
{
  long num_int = 0;
  char num_len = 0;
  char i = 0;

  num_len = strlen(num_str);
  if (num_str[0] == '-')
  {
    if (num_len > 11) return 0;
  }
  else
  {
    if (num_len > 10) return 0;
  }

  if (num_str[0] == '-')
  {
    for (i = 1; i < num_len; i++)
    {
      if ((num_str[i] >= '0') && (num_str[i] <= '9'))
      {
        num_str[i] -= '0'; /* convert ascii to number */
        num_int = num_int * 10 + num_str[i];
      }
      else
        return 0;
    }
    num_int = num_int * (-1);
    return num_int;
  }
  else
  {
    for (i = 0; i < num_len; i++)
    {
      if ((num_str[i] >= '0') && (num_str[i] <= '9'))
      {
        num_str[i] -= '0'; /* convert ascii to number */
        num_int = num_int * 10 + num_str[i];
      }
      else
        return 0;
    }
    return num_int;
  }
}


/*******************************************************************************
* Function Name  : SYS_Buff_Clear(void)
* Description    : clear system buffer
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void SYS_Buff_Clear(void)
{
  char i;
  
  for(i=0;i<SYS_BUFF_SIZE;i++)
  {
    SYS_Buff[i]=0;
  }
}

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
int main()
{  
  int temp;
  
	SystemInit();
	//SysTick_Config(SystemCoreClock / 1000);
  
  /* disable JTAG interface and enable peripheral clock as peripheral IO for SD Card */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
  
  /* Init System */
  LCD_init();
  Sys_Delay_Init(72);
  GPIO_Board_Config();
  USART1_Debug_Config();
  DHT11_GPIO_Config(); 
  OneWireRCCInit();
  
  printf("\n\r******************************************************************");
  printf("\n\r*      -- TEMPERATURE AND HUMIDITY MONITORING GSM SYSTEM --      *");
  printf("\n\r-----------------------------------------------------------------*");
  
//  while(1)
//  {
//    temp = DS18B20_ReadTemperature();
//    printf("\n\rTemp: %d", temp);
//    delay_ms(1000);
//  }
  
//  while(1)
//  {
//    if (Read_DHT11(&DHT11_Data) == SUCCESS)
//    {
//      printf("\n\rThe success of read DHT11!\r\n\r\n The humidity is:%d.%d RH ,The temperature is: %d.%d 0C \r\n",\
//          DHT11_Data.humi_int, DHT11_Data.humi_deci, DHT11_Data.temp_int, DHT11_Data.temp_deci);
//      GSM_Control.Temperature = DHT11_Data.temp_int;
//      GSM_Control.Humidity    = DHT11_Data.humi_int;
//      Update_LCD();
//    }
//    else
//    {
//      printf("\n\rError");
//    }
//    delay_ms(1000);
//  }
  
  LCD_puts("Init GSM...");
  printf("\n\r$>:_Init GSM");
  if(SIM_M95_Init())
  {
    LCD_clear();
    LCD_puts("Init GSM OK");
    printf("\n\r$>:_Init GSM Success");
  }
  else
  {
    printf("\n\r$>:_Init GSM Error");
  }
  
  /* Restor system */
  Restore_SYS_Password();
  Restore_SYS_Phone();
  Restore_SYS_Temp_Humi();
  
  DV_SIM_Buff_Clear();
  SYS_Buff_Clear();
  
	while(1)
	{    
    /* xu ly khi co du lieu tu module sim */
    if(SIM_Rx_Complete == 1)
    {
      delay_ms(1000);
      if(str_find_first_location(SIM_Data_Buff, "CMT") >= 0)
      {
        SMS_Process();
        DV_SIM_Buff_Clear();
        DV_SIM_AT_Cmd("AT+QMGDA=\"DEL ALL\"");
      }
      else if(str_find_first_location(SIM_Data_Buff, "RING") >= 0) 
      {
        Call_Process();
      } 
      DV_SIM_Buff_Clear();      
    }
    
    /* kiem tra cam bien */
    Process_Sensor_State(); 

    /* update LCD */
    Update_LCD();   

    delay_ms(100);    
	}
}



/*******************************************************************************
* Function Name  : Check_SMS_Phone_Number(char *phone)
* Description    : split phone number from SMS
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
uint8_t Check_SMS_Phone_Number(char *phone)
{
  signed int i = 0;
  signed int point = 0;
  
  /* clear phone buff */
  for(i = 0; i < 13; i++)
  {
    GSM_Control.Phone[i]=0;
  }
  
  point = str_find_last_location(SIM_Data_Buff, "CMT: \"+84");
  
  if(point >=0)
  {
    GSM_Control.Phone[0] = '0';
    i=1;
    
    while(SIM_Data_Buff[point] != '\"')
    {
      GSM_Control.Phone[i] = SIM_Data_Buff[point];
      i++;
      point++;
      if(i>13) return 1; /* error */
    }
    return 0; /* not error */
  }
  else 
    return 1; /* error */
}


/*******************************************************************************
* Function Name  : void SMS_Process(void)
* Description    : process sms
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void SMS_Process(void)
{
  signed int point = 0;
  uint16_t temp;
  
  char i = 0;
  
  Check_SMS_Phone_Number(GSM_Control.Phone);
  
  /* check password */
  if((str_find_first_location(SIM_Data_Buff, SYS_Password) >=0) ||           /* kiem tra password */
    (str_find_first_location(SIM_Data_Buff, (char*)SYS_Password_default) >=0))/* hoac password mac dinh */
  {    
    /*-- bat TB 1 -------------------------------------------------------------*/
    if(str_find_first_location(SIM_Data_Buff, "TB1@ON") >= 0)
    {
      Relay_SetState(1, ENABLE);
      /* gui tin nhan */
      SIM_Send_SMS(GSM_Control.Phone,"Thiet bi 1 da bat");
      return;
    }
    
    /*-- tat TB1 --------------------------------------------------------------*/
    else if(str_find_first_location(SIM_Data_Buff, "TB1@OFF") >= 0)
    {
      Relay_SetState(1, DISABLE);
      /* gui tin nhan */
      SIM_Send_SMS(GSM_Control.Phone,"Thiet bi 1 da tat");
      return;
    }
    
    /*-- bat TB2 --------------------------------------------------------------*/
    else if(str_find_first_location(SIM_Data_Buff, "TB2@ON") >= 0)
    {
      Relay_SetState(2, ENABLE);
      /* gui tin nhan */
      SIM_Send_SMS(GSM_Control.Phone,"Thiet bi 2 da bat");
      return;
    }
    
    /*-- tat TB2 -----------------------------------------------------------------*/
    else if(str_find_first_location(SIM_Data_Buff, "TB2@OFF") >= 0)
    {
      Relay_SetState(2, DISABLE);
      /* gui tin nhan */
      SIM_Send_SMS(GSM_Control.Phone,"Thiet bi 2 da tat");
      return;
    }
    
    /*-- doi password - CP: SETPASS@NEW_PASS OLD_PASS ----------------------------*/
    else if((point = str_find_last_location(SIM_Data_Buff, "SETPASS@")) >= 0)
    {
      i = 0;
      while((SIM_Data_Buff[point] != ' ')&& (i < SYS_BUFF_SIZE))
      {
        SYS_Buff[i] = SIM_Data_Buff[point];
        i++;
        point++;
      }
      
      if(i!=5)
      {
        SIM_Send_SMS(GSM_Control.Phone, "Password phai co 5 chu so");
      }
      else
      {
        for(i=0;i<6;i++)
        {
          SYS_Password[i] = SYS_Buff[i];
        }
        SYS_Password[5] = '\0';
        SYS_Buff_Clear();
        sprintf(SYS_Buff,"Password moi la: %s", SYS_Password);
        SIM_Send_SMS(GSM_Control.Phone, SYS_Buff);
        //Backup_SYS_Password();
        Backup_System();
      }
    }
    
    /*-- doi nhiet do - CP: SETTEMP@NEW_TEMP PASSWORD ----------------------------*/
    else if((point = str_find_last_location(SIM_Data_Buff, "SETTEMP@")) >= 0)
    {
      i = 0;
      SYS_Buff_Clear();
      while(((SIM_Data_Buff[point] >= '0')&&(SIM_Data_Buff[point] <= '9'))&& (i < SYS_BUFF_SIZE))
      {
        SYS_Buff[i] = SIM_Data_Buff[point];
        i++;
        point++;
      }
      
      temp = str_to_int(SYS_Buff);
      
      if((temp >0) && (temp <= 255))
      {
        SYS_Temperature = temp;
        SYS_Buff_Clear();
        sprintf(SYS_Buff,"Nhiet do moi: %d do C", SYS_Temperature);
        SIM_Send_SMS(GSM_Control.Phone, SYS_Buff);
        //Backup_SYS_Temp_Humi();
        Backup_System();
      }
      else
      {
        SIM_Send_SMS(GSM_Control.Phone, "Gia tri nhiet do khong phu hop. Nhiet do phai tu 0 den 255"); 
      }
    }
    
    /*-- doi do am - CP: SETHUMI@NEW_HUMIDITY PASSWORD ----------------------------*/
    else if((point = str_find_last_location(SIM_Data_Buff, "SETHUMI@")) >= 0)
    {
      i = 0;
      SYS_Buff_Clear();
      while(((SIM_Data_Buff[point] >= '0')&&(SIM_Data_Buff[point] <= '9'))&& (i < SYS_BUFF_SIZE))
      {
        SYS_Buff[i] = SIM_Data_Buff[point];
        i++;
        point++;
      }
      
      temp = str_to_int(SYS_Buff);
      
      if((temp >0) && (temp <= 100))
      {
        SYS_Humidity = temp;
        SYS_Buff_Clear();
        sprintf(SYS_Buff,"Do am moi: %d%%", SYS_Humidity);
        SIM_Send_SMS(GSM_Control.Phone, SYS_Buff);
        //Backup_SYS_Temp_Humi();
        Backup_System();
      }
      else
      {
        SIM_Send_SMS(GSM_Control.Phone, "Gia tri do am khong phu hop. Do am phai tu 0 den 100"); 
      }
    }
    
    /*-- dat lai sdt admin - CP: SETPHONE@PHONE_NUM PASSSWORD ---------------------*/
    else if((point = str_find_last_location(SIM_Data_Buff, "SETPHONE@")) >= 0)
    {
      i = 0;
      while((SIM_Data_Buff[point] != ' ')&& (i < SYS_BUFF_SIZE))
      {
        SYS_Buff[i] = SIM_Data_Buff[point];
        i++;
        point++;
      }
      
      if(i > 11)
      {
        SIM_Send_SMS(GSM_Control.Phone, "So dien thoai phai it hon 12 so");
      }
      else
      {
        SYS_Phonenum[12] = '\0';
        SYS_Phonenum[11] = '\0';
        SYS_Phonenum[10] = '\0';
        
        for(point=0;point<i;point++)
        {
          SYS_Phonenum[point] = SYS_Buff[point];
        }
        
        SYS_Buff_Clear();
        sprintf(SYS_Buff,"So dien thoai Admin moi la: %s", SYS_Phonenum);
        SIM_Send_SMS(GSM_Control.Phone, SYS_Buff);
        //Backup_SYS_Phone();
        Backup_System();
      }
    }
    /*-- doc nhiet do do am, relay - CP: READ@ALL PASSSWORD -----------------------*/
    else if((point = str_find_last_location(SIM_Data_Buff, "READ@ALL")) >= 0)
    {
      SYS_Buff_Clear();
      sprintf(SYS_Buff, "Nhiet do: %u, Do am: %u, Nhiet do bao dong: %u, Relay 1: %u, Relay 2: %u", \
              GSM_Control.Temperature, GSM_Control.Humidity, SYS_Temperature,\
              Relay_GetState(1), Relay_GetState(2));
      SIM_Send_SMS(GSM_Control.Phone, SYS_Buff);
    }
    /*-- bat tat thong bao tin nhan - CP: REPORT@ON/OFF PASSWORD ------------------*/
    else if((point = str_find_last_location(SIM_Data_Buff, "REPORT@")) >= 0)
    {
      if(str_find_last_location(SIM_Data_Buff, "ON") >= 0)
      {
        SYS_Report = ENABLE;
        SIM_Send_SMS(GSM_Control.Phone, "Ban da bat chuc nang thong bao khi nhiet do thay doi");
      }
      else if(str_find_last_location(SIM_Data_Buff, "OFF") >= 0)
      {
        SYS_Report = DISABLE;
        SIM_Send_SMS(GSM_Control.Phone, "Ban da tat chuc nang thong bao khi nhiet do thay doi");
      }
    }
  }
}

/*******************************************************************************
* Function Name  : void Call_Process(void)
* Description    : 

* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void Call_Process(void)
{
  DV_SIM_AT_Cmd("ATH");
  
  /* kiem tra so dien thoai goi den */
  if(str_find_first_location(SIM_Data_Buff, SYS_Phonenum) >= 0)
  {
    if(Relay_GetState(1))
    {
      Relay_SetState(1, DISABLE);
      /* gui tin nhan */
      SIM_Send_SMS(SYS_Phonenum,"Thiet bi 1 da tat");      
    }
    else
    {
      Relay_SetState(1, ENABLE);
      /* gui tin nhan */
      SIM_Send_SMS(SYS_Phonenum,"Thiet bi 1 da bat");  
    }
  }
  
}


/*******************************************************************************
* Function Name  : Process_Sensor_State(void)
* Description    : read sensor data and process it
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void Process_Sensor_State(void)
{
  static uint32_t Temp_average = 0;
  static uint32_t Humi_average = 0;
  static uint8_t count = 0;
  
  /* get temperature and humidity sensor 1 */
  if (Read_DHT11(&DHT11_Data) == SUCCESS)
  {
    printf("\r$>:_The humidity is:%d.%d RH ,The temperature is: %d.%d 0C",\
        DHT11_Data.humi_int, DHT11_Data.humi_deci, DHT11_Data.temp_int, DHT11_Data.temp_deci);
    Temp_average += DHT11_Data.temp_int;
    Humi_average += DHT11_Data.humi_int;
    count++;
  }
  
  if(count >= 100)
  {
    /* tinh trung binh */
    GSM_Control.Temperature = Temp_average / 100;
    GSM_Control.Humidity    = Humi_average / 100;
    count = 0;    
    Temp_average = 0;
    Humi_average = 0;
    
    /* Temp_sensor >= SYS_Temp + 2 --- HIGH lever */
    if((GSM_Control.Temperature >= (SYS_Temperature + 2)) && (SYS_SMS_Status.Temp_SMS != HIGH_SEND))
    {
      Relay_SetState(1, ENABLE);
      Relay_SetState(2, ENABLE); 
      SYS_Buff_Clear();
      sprintf(SYS_Buff,"CANH BAO: Nhiet do hien tai la %u, do am %u, cao hon muc %u. He thong se bat 2 dong co lam mat",\
              GSM_Control.Temperature, GSM_Control.Humidity, SYS_Temperature+2);
      if(SYS_Report == ENABLE)
      {
        SIM_Send_SMS(SYS_Phonenum, SYS_Buff);
      }
      printf("\n\r$>:_%s\n\r",SYS_Buff);
      SYS_SMS_Status.Temp_SMS = HIGH_SEND;    
    }  
    
    /* SYS_temp <= Temp_sensor < SYS_Temp + 2 --- Middle lever */
    if(((GSM_Control.Temperature >= SYS_Temperature) && (GSM_Control.Temperature <(SYS_Temperature + 2))) && \
      (SYS_SMS_Status.Temp_SMS != MIDDLE_SEND))
    {
      Relay_SetState(1, ENABLE);
      Relay_SetState(2, DISABLE);
      SYS_Buff_Clear();
      sprintf(SYS_Buff,"CANH BAO: Nhiet do hien tai la %u, do am %u, cao hon muc %u. He thong se bat dong co lam mat ben trong",\
              GSM_Control.Temperature,GSM_Control.Humidity, SYS_Temperature);
      if(SYS_Report == ENABLE)
      {
        SIM_Send_SMS(SYS_Phonenum, SYS_Buff);
      }
      printf("\n\r$>:_%s\n\r",SYS_Buff);
      SYS_SMS_Status.Temp_SMS = MIDDLE_SEND;    
    }
    
    /* Temp_sensor < SYS_Temp --- Low lever */
    if((GSM_Control.Temperature < SYS_Temperature) && (SYS_SMS_Status.Temp_SMS != LOW_SEND))
    {
      Relay_SetState(1, DISABLE);
      Relay_SetState(2, DISABLE);
      SYS_Buff_Clear();
      sprintf(SYS_Buff,"CANH BAO: Nhiet do hien tai la %u, do am %u, thap hon muc %u. He thong se tat 2 dong co lam mat",\
              GSM_Control.Temperature, GSM_Control.Humidity, SYS_Temperature);
      if(SYS_Report == ENABLE)
      {
        SIM_Send_SMS(SYS_Phonenum, SYS_Buff);
      }
      printf("\n\r$>:_%s\n\r",SYS_Buff);
      SYS_SMS_Status.Temp_SMS = LOW_SEND;    
    }
  }
}

/*******************************************************************************
* Function Name  : Update_LCD(void)
* Description    : update status of system and display them on LCD
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void Update_LCD(void)
{
  /* relay 1 */
  if(Relay_GetState(1))
  {
    LCD_gotoxy(0,0);
    LCD_puts("RL1:ON  ");
  }
  else
  {
    LCD_gotoxy(0,0);
    LCD_puts("RL1:OFF ");
  }
  
  /* relay 2 */
  if(Relay_GetState(2))
  {
    LCD_gotoxy(9,0);
    LCD_puts("RL2:ON ");
  }
  else
  {
    LCD_gotoxy(9,0);
    LCD_puts("RL2:OFF");
  }
  
  /* temperature */
  SYS_Buff_Clear();
  sprintf(SYS_Buff, "Temp:%u  ", GSM_Control.Temperature);
  LCD_gotoxy(0, 1);
  LCD_puts(SYS_Buff);  
  
  /* hunidity */
  SYS_Buff_Clear();
  sprintf(SYS_Buff, "Humi:%u  ", GSM_Control.Humidity);
  LCD_gotoxy(9, 1);
  LCD_puts(SYS_Buff);  
}








