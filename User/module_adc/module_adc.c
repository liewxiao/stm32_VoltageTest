#include "module_adc/module_adc.h"

vu16 ADC_Buf[ADC_BUF_SIZE] = {0};

//ADCGPIO口初始化为模拟量输入
static void ADCxGPIO_Config(void)
{
	GPIO_InitTypeDef GPIOIint_Struct;
	
	ADC_GPIO_APBxClock_FUN(ADC_GPIO_CLK,ENABLE);
	
	GPIOIint_Struct.GPIO_Pin = ADC_PIN;
	GPIOIint_Struct.GPIO_Mode = GPIO_Mode_AIN;
	
	GPIO_Init(ADC_PORT, &GPIOIint_Struct);
}

static void ADCx_Config(void)
{
	ADC_InitTypeDef ADCIint_Struct;

	ADC_APBxClock_FUN(ADC1_CLK,ENABLE);
	
	ADCIint_Struct.ADC_Mode = ADC_Mode_Independent;
	ADCIint_Struct.ADC_ScanConvMode = ENABLE;
	ADCIint_Struct.ADC_ContinuousConvMode = ENABLE;
	ADCIint_Struct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADCIint_Struct.ADC_DataAlign = ADC_DataAlign_Right;
	ADCIint_Struct.ADC_NbrOfChannel = TOTAL_CONVERT_CH;
	ADC_Init(ADC1, &ADCIint_Struct);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	ADC_RegularChannelConfig(ADC1, ADC1_VCHANNEL, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC1_TCHANNEL, 2, ADC_SampleTime_239Cycles5);
	ADC_TempSensorVrefintCmd(ENABLE);

/**************************************************************************************/

	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
	
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	//校准ADC
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	
	
	//软件使能ADC转换开始
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

static void dma_Init( void )
{
	DMA_InitTypeDef DMA_InitStructure;
	
	//Iint DMA Clock
	DMA_APBxClock_FUN(DMA_CLK,ENABLE);
	DMA_DeInit(DMA_CHANNEL);
	//Iint DMA Structure
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC_DR_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(ADC_Buf);
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = ADC_BUF_SIZE;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	DMA_Init(DMA_CHANNEL, &DMA_InitStructure);
	DMA_Cmd(DMA_CHANNEL, ENABLE);
}

void adc_Init( void )
{
	ADCxGPIO_Config();
	ADCx_Config();
	dma_Init();
}

float Get_Voltage( void )
{
	float voltage = (float)ADC_Buf[1] * (3.3 / 4096);
	
	return voltage;
}

float Get_Chip_Temperature( void )
{
	float temperate = 0;
	temperate = (float)ADC_Buf[0] * (3.3/4096);
	temperate = (1.43-temperate) / 0.0043 + 25;
	
	return temperate;
}