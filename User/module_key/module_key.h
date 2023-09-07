#ifndef __MODULE_KEY_H
#define __MODULE_KEY_H

#include "stm32f10x.h"

/*Define the key state and put the state value in the high 4 bits*/
#define KEY_STATE_NOPRESS       0xA0
#define KEY_STATE_DEBOUNCE      0xB0
#define KEY_STATE_CLILK         0xC0
#define KEY_STATE_DOUBLE        0xD0
#define KEY_STATE_LONGPRESS     0xE0
#define KEY_STATE_RELEASE       0xF0

/*this macro divided key state and value*/
#define STATE         0xF0
#define VALUE         0x0F

/*number each key*/
#define KEY_NO_DOWN    0x00
#define KEY_K1         0x01
#define KEY_K2         0x02

/*Abstract different states of a single button into different functions*/
#define N_KEY     0                       // no click
#define KEY_UP    (KEY_STATE_CLILK | KEY_K1 ) // single click
#define KEY_LEFT    (KEY_STATE_DOUBLE | KEY_K1 ) // double press
#define KEY_ENTER    (KEY_STATE_LONGPRESS | KEY_K1 ) // long press
 
#define KEY_DOWN    (KEY_STATE_CLILK | KEY_K2 ) // single click
#define KEY_RIGHT    (KEY_STATE_DOUBLE | KEY_K2 ) // double press
#define KEY_EXIT    (KEY_STATE_LONGPRESS | KEY_K2 ) // long press


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