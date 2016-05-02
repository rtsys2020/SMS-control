/**********************************************************
*	Software I2C for PIC16F887 in master mode
*
*	Compier:	HT PIC
*	Author:		Truong Cong Tri
***********************************************************
*	LIST FUNCTIONS
*
*	soft_i2c_start()	- issue start condition
*	soft_i2c_stop()		- issue stop condition
*	soft_i2c_write()	- send byte data
*	soft_i2c_read(ack)	- read byte data:	ack=0 -> NACK,	ack=1 -> ACK
*
*---------------------------------------------------------*/
#include "soft_i2c.h"


//=========================================================
//	Soft I2C Start
//=========================================================
void soft_i2c_start()
{
	SDA_TRIS=0;
	SDA_TRIS=0;
	
	SDA_PIN=1;
	SCL_PIN=1;
	SDA_PIN=0;
	SCL_PIN=0;
}
//---------------------------------------------------------


//=========================================================
// Soft I2C Stop
//=========================================================
void soft_i2c_stop()
{	
	SDA_TRIS=0;
	SCL_TRIS=0;
	
	SDA_PIN=0;
	SCL_PIN=1;
	SDA_PIN=1;
}
//---------------------------------------------------------	


//=========================================================
//	I2C write
//=========================================================
void soft_i2c_write(unsigned char data_send)
{
	unsigned char i;
	
	SDA_TRIS=0;
	SCL_TRIS=0;
	
	for(i=0;i<=8;i++)
	{
		if(data_send & 0x80)	// MSB fist -> LSB last
			SDA_PIN=1;
		else
			SDA_PIN=0;
		
		SCL_PIN=1;
		data_send <<=1;
		SCL_PIN=0;
	}
}				
//---------------------------------------------------------


//=========================================================
//	Soft I2C read
//=========================================================
unsigned char soft_i2c_read(unsigned char ack)
{
	unsigned char get_data,i;
	
	SCL_PIN=0;		
	SDA_TRIS=1;		//set SDA_PIN as input
	
	for(i=0;i<8;i++)
	{
		get_data<<=1;
		SCL_PIN=1;
		
		if(SDA_PIN)
			get_data |=1;
		
		SCL_PIN=0;
	}
	
	SDA_TRIS=0;		//set SDA_PIN as output
	if(ack)
		SDA_PIN=1;	//ack=1 -> send not ack signal (NACK)
	else
		SDA_PIN=0;	//ack=0 -> send ack signal (ACK)
		
	SCL_PIN=1;
	SCL_PIN=0;

	return get_data;
}
//---------------------------------------------------------