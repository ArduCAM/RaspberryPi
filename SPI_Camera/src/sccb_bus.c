#include "sccb_bus.h"
#include "bcm283x_board_driver.h"
#define SCCB_SIC_H()      digitalWrite(scl_port,HIGH)	 	//SCL H
#define SCCB_SIC_L()      digitalWrite(scl_port,LOW)		 	//SCL H

#define SCCB_SID_H()      digitalWrite(sda_port,HIGH)   //SDA	H
#define SCCB_SID_L()      digitalWrite(sda_port,LOW)    //SDA	H 


#define SCCB_DATA_IN      pinMode(sda_port, INPUT);
#define SCCB_DATA_OUT     pinMode(sda_port, OUTPUT);

#define SCCB_SID_STATE	  digitalRead(sda_port) 


unsigned char I2C_TIM  = 30;
void sccb_bus_init(void)
{
	pinMode(sda_port, OUTPUT);
	pinMode(scl_port, OUTPUT);
	digitalWrite(sda_port, HIGH);
	digitalWrite(scl_port, HIGH);
}

void sccb_bus_start(void)
{
    SCCB_SID_H();             
    delay_us(I2C_TIM);
    SCCB_SIC_H();	           
    delay_us(I2C_TIM);
    SCCB_SID_L();
    delay_us(I2C_TIM);
    SCCB_SIC_L();	           
    delay_us(I2C_TIM);
}

void sccb_bus_stop(void)
{
    SCCB_SID_L();
    delay_us(I2C_TIM);
    SCCB_SIC_H();	
    delay_us(I2C_TIM);  
    SCCB_SID_H();	
    delay_us(I2C_TIM);  
}

void sccb_bus_send_noack(void)
{	
	SCCB_SID_H();	
	delay_us(I2C_TIM);	
	SCCB_SIC_H();	
	delay_us(I2C_TIM);	
	SCCB_SIC_L();	
	delay_us(I2C_TIM);	
	SCCB_SID_L();	
	delay_us(I2C_TIM);
}

void sccb_bus_send_ack(void)
{	
	SCCB_SID_L();	
	delay_us(I2C_TIM);	
	SCCB_SIC_L();	
	delay_us(I2C_TIM);	
	SCCB_SIC_H();	
	delay_us(I2C_TIM);	
	SCCB_SIC_L();	
	delay_us(I2C_TIM);	
	SCCB_SID_L();	
	delay_us(I2C_TIM);
}

unsigned char sccb_bus_write_byte(unsigned char data)
{
	unsigned char i;
	unsigned char tem;
	for(i = 0; i < 8; i++) 
	{
		if((data<<i) & 0x80)
		{
			SCCB_SID_H();
		}
		else 
		{
			SCCB_SID_L();
		}
		delay_us(I2C_TIM);
		SCCB_SIC_H();	
		delay_us(I2C_TIM);
		SCCB_SIC_L();	
	}
	SCCB_DATA_IN;
	delay_us(I2C_TIM);
	SCCB_SIC_H();	
	delay_us(I2C_TIM);
	if(SCCB_SID_STATE)
	{
		tem = 0;              
	}
	else 
	{
		tem = 1;               
	}

	SCCB_SIC_L();	
	delay_us(I2C_TIM);	
	SCCB_DATA_OUT;
	return tem;  
}

unsigned char sccb_bus_read_byte(void)
{	
	unsigned char i;
	unsigned char read = 0;
	SCCB_DATA_IN;
	for(i = 8; i > 0; i--)
	{		     
		delay_us(I2C_TIM);
		SCCB_SIC_H();
		delay_us(I2C_TIM);
		read = read << 1;
		if(SCCB_SID_STATE)
		{
			read += 1; 
		}
		SCCB_SIC_L();
		delay_us(I2C_TIM);
	}	
    SCCB_DATA_OUT;
	return read;
}

