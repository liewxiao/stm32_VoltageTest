#include "module_usart/module_usart.h"
#include <stdio.h>

/*static void NVIC_Configuration(void)
{ //定义中断结构体
  NVIC_InitTypeDef NVIC_InitStructure;
  //定义中断优先级分组
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  //定义中断通道
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
  //定义抢占优先级
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  //定义响应优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  //使用中断
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);
}*/

void USART_Config(void)
{	//定义使用串口GPIO结构和串口结构
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//初始化串口1PA口时钟和串口1总线时钟
	DEBUG_USART_GPIO_APBxClkCmd( DEBUG_USART_GPIO_CLK, ENABLE );
	DEBUG_USART_APBxClkCmd( DEBUG_USART_CLK, ENABLE );
	//配置GPIOSend口复用PUSH_PULL初始化结构
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);
	//配置GPIORecv口Floating初始化结构
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	//配置usart参数，波特率、字长、停止位、校验位、硬件流控制、串口模式使能
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(DEBUG_USARTx, &USART_InitStructure);
	//配置串口中断参数
	//NVIC_Configuration();
	//配置串口接收中断使能
	//USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);
	//使能串口
	USART_Cmd(DEBUG_USARTx, ENABLE);
}
//串口发送字节
void Usart_SendByte( USART_TypeDef *pUSARTx, uint8_t ch)
{
	USART_SendData(pUSARTx, ch);
	while( USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET );
}
//串口发送4字节通过内存入侵拆分32位数据
void Usart_Send_32Word( USART_TypeDef *pUSARTx, uint32_t ch )
{
	uint8_t *p = (uint8_t*)&ch;
	uint8_t i;
	for( i=0; i<4; i++ ) {
		USART_SendData(pUSARTx, *p++);
	}
	while( USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET );
}
//串口发送字节数组
void Usart_SendArray( USART_TypeDef *pUSARTx, uint8_t *Array, uint8_t Array_Size )
{
	uint8_t i;
	for( i=0; i<Array_Size; i++ ) {
		USART_SendData(pUSARTx, Array[i]);
		while( USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET );
	}
	while( USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET );
}
//串口发送字符串
void Usart_String( USART_TypeDef *pUSARTx, uint8_t *String )
{
	while( *String != '\0' )
	{
		USART_SendData(pUSARTx, *String++);
		while( USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET );
	}
  
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

int fputc(int ch, FILE *f)
{
		USART_SendData(DEBUG_USARTx, (uint8_t) ch);
		
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

int fgetc(FILE *f)
{
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USARTx);
}
