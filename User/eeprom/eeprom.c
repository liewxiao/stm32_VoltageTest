#include "eeprom.h"

static void I2C_EE_Init( void )
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	EEPROM_I2C_APBxClock_FUN ( EEPROM_I2C_CLK, ENABLE );
	EEPROM_I2C_GPIO_APBxClock_FUN ( EEPROM_I2C_GPIO_CLK, ENABLE );
	
	GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SCL_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(EEPROM_I2C_SCL_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SDA_PIN;
	GPIO_Init(EEPROM_I2C_SDA_PORT, &GPIO_InitStructure);
}

static void I2C_modeconfig( void )
{
	I2C_InitTypeDef  I2C_InitStructure;
	
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 =I2Cx_OWN_ADDRESS; 
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
	
	I2C_Init(EEPROM_I2Cx, &I2C_InitStructure);
	I2C_Cmd(EEPROM_I2Cx, ENABLE);
}

