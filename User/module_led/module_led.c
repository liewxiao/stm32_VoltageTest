#include "module_led/module_led.h"

void GPIO_LED_Config( void )
{	//初始化APB2系统时钟将3个GPIO或操作初始化
	RCC_APB2PeriphClockCmd(	LED_G_GPIO_CLK|\
							LED_R_GPIO_CLK|\
							LED_B_GPIO_CLK, ENABLE);
	//创建GPIO初始化结构
	GPIO_InitTypeDef GPIO_IintStruct;
	//设置GPIO端口为输出模式选择PUSH—PULL
	GPIO_IintStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	//设置GPIO端口输出速率
	GPIO_IintStruct.GPIO_Speed = GPIO_Speed_50MHz;
	//设置GPIO端口引脚
	GPIO_IintStruct.GPIO_Pin = GPIO_G_PIN;
	//设置GPIO初始化端口引脚
	GPIO_Init(GPIO_G_PORT, &GPIO_IintStruct);
	
	GPIO_IintStruct.GPIO_Pin = GPIO_R_PIN;
	
	GPIO_Init(GPIO_R_PORT, &GPIO_IintStruct);
	
	GPIO_IintStruct.GPIO_Pin = GPIO_B_PIN;
	
	GPIO_Init(GPIO_B_PORT, &GPIO_IintStruct);
	//设置该端口BSRR寄存器置1
	GPIO_SetBits( GPIO_G_PORT, GPIO_G_PIN );
	
	GPIO_SetBits( GPIO_R_PORT, GPIO_R_PIN );
	
	GPIO_SetBits( GPIO_B_PORT, GPIO_B_PIN );
}