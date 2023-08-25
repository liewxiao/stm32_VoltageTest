#ifndef __MODULE_KEY_H
#define __MODULE_KEY_H

#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"

#define KEY_STATE_NOPRESS       0xA0
#define KEY_STATE_DEBOUNCE      0xB0
#define KEY_STATE_CLILK         0xC0
#define KEY_STATE_DOUBLE        0xD0
#define KEY_STATE_LONGPRESS     0xE0
#define KEY_STATE_RELEASE       0xF0

#define STATE         0xF0
#define VALUE         0x0F

#define KEY_NO_DOWN    0x00
#define KEY_K1         0x01
#define KEY_K2         0x02

#define N_KEY     0                       // no click
#define Key1_S    (KEY_STATE_CLILK | KEY_K1 ) // single click
#define Key1_D    (KEY_STATE_DOUBLE | KEY_K1 ) // double press
#define Key1_L    (KEY_STATE_LONGPRESS | KEY_K1 ) // long press
 
#define Key2_S    (KEY_STATE_CLILK | KEY_K2 ) // single click
#define Key2_D    (KEY_STATE_DOUBLE | KEY_K2 ) // double press
#define Key2_L    (KEY_STATE_LONGPRESS | KEY_K2 ) // long press



#define		DEBOUNCE_TIME		2
#define		DOUBLECILIK_TIME	25
#define		LONGPRESS_TIME	    100

#define		GPIO_Key1_CLK	RCC_APB2Periph_GPIOA
#define		GPIO_Key1_PIN	GPIO_Pin_0
#define		GPIO_Key1_PORT	GPIOA

#define		GPIO_Key2_CLK	RCC_APB2Periph_GPIOC
#define		GPIO_Key2_PIN	GPIO_Pin_13
#define		GPIO_Key2_PORT	GPIOC

#define		KEY_ON			1
#define		KEY_OFF			0

#define		Key1_Stae		GPIO_ReadInputDataBit(GPIO_Key1_PORT,GPIO_Key1_PIN)
#define		Key2_Stae		GPIO_ReadInputDataBit(GPIO_Key2_PORT,GPIO_Key2_PIN)


void GPIO_Key_Iint( void );
uint8_t Key_Read( void );

#endif