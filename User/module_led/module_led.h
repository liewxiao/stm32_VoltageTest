#ifndef __MODULE_LED_H
#define __MODULE_LED_H

#include "stm32f10x.h"
		//宏定义库函数中的GPIO口
#define LED_G_GPIO_CLK	RCC_APB2Periph_GPIOB
#define GPIO_G_PIN		GPIO_Pin_0
#define GPIO_G_PORT		GPIOB

#define LED_R_GPIO_CLK	RCC_APB2Periph_GPIOB
#define GPIO_R_PIN		GPIO_Pin_5
#define GPIO_R_PORT		GPIOB

#define LED_B_GPIO_CLK	RCC_APB2Periph_GPIOB
#define GPIO_B_PIN		GPIO_Pin_1
#define GPIO_B_PORT		GPIOB
		//使能端口高电平BSRR置1，使能端口低电平BRR置1，ODR输出寄存器异或1翻转ODR寄存器值
#define DigitalH( p, i )		{ p->BSRR = i; }
#define DigitalL( p, i )		{ p->BRR = i; }
#define DigitalToggle( p, i )	{ p->ODR ^= i; }

#define LED_G_OFF		DigitalH( GPIO_G_PORT, GPIO_G_PIN )
#define LED_G_ON		DigitalL( GPIO_G_PORT, GPIO_G_PIN )
#define LED_G_Toggle	DigitalToggle( GPIO_G_PORT, GPIO_G_PIN )

#define LED_R_OFF		DigitalH( GPIO_R_PORT, GPIO_R_PIN )
#define LED_R_ON		DigitalL( GPIO_R_PORT, GPIO_R_PIN )
#define LED_R_Toggle	DigitalToggle( GPIO_R_PORT, GPIO_R_PIN )

#define LED_B_OFF		DigitalH( GPIO_B_PORT, GPIO_B_PIN )
#define LED_B_ON		DigitalL( GPIO_B_PORT, GPIO_B_PIN )
#define LED_B_Toggle	DigitalToggle( GPIO_B_PORT, GPIO_B_PIN )

//红色
#define LED_RED  \
					LED_R_ON;\
					LED_G_OFF\
					LED_B_OFF

//绿色
#define LED_GREEN		\
					LED_R_OFF;\
					LED_G_ON\
					LED_B_OFF

//蓝色
#define LED_BLUE	\
					LED_R_OFF;\
					LED_G_OFF\
					LED_B_ON

					
//黄色					
#define LED_YELLOW	\
					LED_R_ON;\
					LED_G_ON\
					LED_B_OFF
//紫色
#define LED_PURPLE	\
					LED_R_ON;\
					LED_G_OFF\
					LED_B_ON

//橙色
#define LED_CYAN \
					LED_R_OFF;\
					LED_G_ON\
					LED_B_ON
					
//白色
#define LED_WHITE	\
					LED_R_ON;\
					LED_G_ON\
					LED_B_ON
					
//关闭所有灯
#define LED_RGBOFF	\
					LED_R_OFF;\
					LED_G_OFF\
					LED_B_OFF

void GPIO_LED_Config( void );

#endif