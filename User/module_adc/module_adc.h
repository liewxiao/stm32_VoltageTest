#ifndef __MODULE_ADC_H
#define __MODULE_ADC_H

#include "stm32f10x.h"

// ADC ���ѡ��
// ������ ADC1/2�����ʹ��ADC3���ж���ص�Ҫ�ĳ�ADC3��
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define	   ADC1_CLK						 RCC_APB2Periph_ADC1

#define    DMA_APBxClock_FUN             RCC_AHBPeriphClockCmd
#define    DMA_CLK                       RCC_AHBPeriph_DMA1
#define    DMA_CHANNEL                   DMA1_Channel1

// ADC GPIO�궨��
// ע�⣺����ADC�ɼ���IO����û�и��ã�����ɼ���ѹ����Ӱ��
#define    ADC_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADC_GPIO_CLK                  RCC_APB2Periph_GPIOC  
#define    ADC_PORT                      GPIOC
#define    ADC_PIN                       GPIO_Pin_1

#define	   ADC_DR_Address				 ((uint32_t)0x40012400+0x4C)
#define	   ADC_BUF_SIZE					 2
#define    TOTAL_CONVERT_CH				 2
// ADC ͨ���궨��
#define    ADC1_VCHANNEL				   ADC_Channel_16
#define    ADC1_TCHANNEL                   ADC_Channel_11

// ADC �ж���غ궨��
#define    ADC_IRQ                       ADC1_2_IRQn
#define    ADC_IRQHandler                ADC1_2_IRQHandler

void adc_Init( void );
float Get_Voltage( void );
float Get_Chip_Temperature( void );

#endif