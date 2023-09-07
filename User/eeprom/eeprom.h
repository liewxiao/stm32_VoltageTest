#ifndef __EEPROM_H
#define __EEPROM_H

#include "stm32f10x.h"

#define             EEPROM_I2Cx                                I2C1
#define             EEPROM_I2C_APBxClock_FUN                   RCC_APB1PeriphClockCmd
#define             EEPROM_I2C_CLK                             RCC_APB1Periph_I2C1
#define             EEPROM_I2C_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             EEPROM_I2C_GPIO_CLK                        RCC_APB2Periph_GPIOB     
#define             EEPROM_I2C_SCL_PORT                        GPIOB   
#define             EEPROM_I2C_SCL_PIN                         GPIO_Pin_8
#define             EEPROM_I2C_SDA_PORT                        GPIOB 
#define             EEPROM_I2C_SDA_PIN                         GPIO_Pin_9

/* STM32 I2C 快速模式 */
#define I2C_Speed             400000
#define I2Cx_OWN_ADDRESS      0X0A 
#define AT24C02_ADDRESS		  0xA0


#endif