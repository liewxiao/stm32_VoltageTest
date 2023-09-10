#include "eeprom.h"

static void I2C_EE_Init( void )
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	EEPROM_I2C_APBxClock_FUN( EEPROM_I2C_CLK, ENABLE );
//	//RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );
	
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

mybool read_eeprom( uint8_t* readbuff, uint8_t pos, uint8_t read_num )
{
	mybool read_state = MYFALSE;
	
	while(I2C_GetFlagStatus(EEPROM_I2Cx, I2C_FLAG_BUSY));
	I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);
	while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(EEPROM_I2Cx, AT24C02_ADDRESS, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_Cmd(EEPROM_I2Cx, ENABLE);
	
	I2C_SendData(EEPROM_I2Cx, pos); 
	while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);
	while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(EEPROM_I2Cx, AT24C02_ADDRESS, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	
	while( read_num )
	{
		if( read_num == 1 ) {
			I2C_AcknowledgeConfig(EEPROM_I2Cx, DISABLE);
			I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE);
			read_state = MYTURE;
		}
		
		while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));
		
		*readbuff = I2C_ReceiveData(EEPROM_I2Cx);
		readbuff++;
		read_num--;
	}
	
	I2C_AcknowledgeConfig(EEPROM_I2Cx, ENABLE);
	
	
	return read_state;
}

mybool write_eeprom( uint8_t *writebuff, uint8_t pos, uint8_t write_num )
{
	mybool write_state = MYFALSE;
	
	while(I2C_GetFlagStatus(EEPROM_I2Cx, I2C_FLAG_BUSY));
	I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);
	while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(EEPROM_I2Cx, AT24C02_ADDRESS, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	
	I2C_SendData(EEPROM_I2Cx, pos);
	while(! I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	while( write_num-- )
	{
		I2C_SendData(EEPROM_I2Cx, *writebuff);
		writebuff++;
		while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
		write_state = MYTURE;
	}
	
	I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE);
	
	return write_state;
}

void eeprom_init( void )
{
	I2C_EE_Init();
	I2C_modeconfig();
}