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

#ifndef _SOFTWARE_I2C_H_
#define	_SOFTWARE_I2C_H_

//define IO pin for i2c communicate
#define	SCL_PIN		RC0
#define	SDA_PIN		RC1

#define	SCL_TRIS	TRISC0
#define	SDA_TRIS	TRISC1

void soft_i2c_start();

void soft_i2c_stop();

unsigned char soft_i2c_read(unsigned char ack);
		//ack = 1 -> send not ack signal (NACK)
		//ack = 0 -> send ack signal (ACK)
		
void soft_i2c_write(unsigned char data_send);

#endif