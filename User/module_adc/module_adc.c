#include "module_adc/module_adc.h"

__IO uint32_t ADC_convert;

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
	DMA_InitTypeDef DMA_InitStructure;
	//Iint DMA Clock
	DMA_APBxClock_FUN(DMA_CLK,ENABLE);
	DMA_DeInit(DMA_CHANNEL);
	//Iint DMA Structure
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(ADC1->DR));
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(&ADC_convert);
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA_CHANNEL, &DMA_InitStructure);
	DMA_Cmd(DMA_CHANNEL, ENABLE);
	
	ADC_APBxClock_FUN(ADC1_CLK,ENABLE);
	ADCIint_Struct.ADC_Mode = ADC_Mode_RegSimult;
	ADCIint_Struct.ADC_ScanConvMode = DISABLE;
	ADCIint_Struct.ADC_ContinuousConvMode = ENABLE;
	ADCIint_Struct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADCIint_Struct.ADC_DataAlign = ADC_DataAlign_Right;
	ADCIint_Struct.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADCIint_Struct);
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	ADC_RegularChannelConfig(ADC1, ADC1_CHANNEL, 1, ADC_SampleTime_239Cycles5);
	ADC_TempSensorVrefintCmd(ENABLE);
	ADC_DMACmd(ADC1, ENABLE);
	
	//初始化ADC外设时钟
	ADC_APBxClock_FUN(ADC2_CLK,ENABLE);
	//配置ADC模式单独，ADC扫描模式不可用，ADC连续转换可用，外部中断无，数据右对齐，ADC通道数1个
	ADCIint_Struct.ADC_Mode = ADC_Mode_RegSimult;
	ADCIint_Struct.ADC_ScanConvMode = DISABLE;
	ADCIint_Struct.ADC_ContinuousConvMode = ENABLE;
	ADCIint_Struct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADCIint_Struct.ADC_DataAlign = ADC_DataAlign_Right;
	ADCIint_Struct.ADC_NbrOfChannel = 1;
	ADC_Init(ADC2, &ADCIint_Struct);
	//配置ADC时钟为PCLK2时钟6分频
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	//配置ADC通道11的周期为252，采样周期 = sampletime + 12.5周期
	ADC_RegularChannelConfig(ADC2, ADC2_CHANNEL, 1, ADC_SampleTime_239Cycles5);
/**************************************************************************************/

	//使能ADC转换开关和使能ADCDMA
	ADC_Cmd(ADC1, ENABLE);
	
	//清除ADC校准位
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);
	//校准ADC
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1) == RESET);
	
	//使能ADC转换开关
	ADC_Cmd(ADC2, ENABLE);
	//清除ADC校准位
	ADC_ResetCalibration(ADC2);
	while(ADC_GetResetCalibrationStatus(ADC2) == SET);
	//校准ADC
	ADC_StartCalibration(ADC2);
	while(ADC_GetCalibrationStatus(ADC2) == RESET);
	
	//软件使能ADC转换开始
	ADC_ExternalTrigConvCmd(ADC2, ENABLE);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void ADC_Iint(void)
{
	ADCxGPIO_Config();
	ADCx_Config();
}

uint32_t ADC_Scan( void )
{
	uint8_t EOC_Flag = ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC);
	if(EOC_Flag) {
		return ADC_convert = ADC_GetDualModeConversionValue();
	} else {
		return ADC_convert = 0;
	}
	
}