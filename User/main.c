#include "stm32f10x.h"
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

//peripherals handle
#include "module_led.h"
#include "module_usart.h"
#include "module_key.h"
#include "module_adc.h"
#include "bsp_ili9341_lcd.h"
#include "menu.h"
#include "global.h"

static void prvSetupHardware( void );
void menu_task( void *para );

void led_task( void *para )
{
	BaseType_t task_status;
	uint8_t button_vaule;
	
	for( ; ; ){
		task_status = xTaskNotifyWait(0,0,(uint32_t*)(&button_vaule),portMAX_DELAY);
		
		if( task_status==pdPASS ) {
			switch(button_vaule)
			{
				case KEY_UP:
					LED_RED;
					break;
				case KEY_DOWN:
					LED_GREEN;
					break;
				case KEY_LEFT:
					LED_BLUE;
					break;
				case KEY_RIGHT:
					LED_YELLOW;
					break;
				case KEY_ENTER:
					LED_PURPLE;
					break;
				case KEY_EXIT:
					LED_CYAN;
					break;
				default:
					LED_RGBOFF;
			}
		}
	}
}

void key_task( void *para )
{
	TickType_t lastwake_time;
	const TickType_t scan_period = pdMS_TO_TICKS( 20 );
	lastwake_time = xTaskGetTickCount();
	
	uint8_t key_value = N_KEY;
	static uint16_t scan_cnt = 0;
	key_queuehandle = xQueueCreate( 1,sizeof(uint8_t) );
	
	for( ; ; ) {
		key_value = Key_Read();
		
		xTaskNotify(led_taskhandle,(uint32_t)(key_value),eSetValueWithoutOverwrite);
		xQueueSend( key_queuehandle,&key_value,NULL );
		
		if( key_value == N_KEY ) {
			scan_cnt++;
			if( scan_cnt == 3000 ) {
				vTaskDelete( menu_taskhandle );
				xTaskCreate( menu_task, "MENU_Task", 200, NULL, 1, &menu_taskhandle );
				scan_cnt = 0;
		}
	}
		
		xTaskDelayUntil( &lastwake_time, scan_period );
	}
}

void adc_task( void *para )
{
	float voltage = 0;
	float temperature = 0;
	TickType_t lastwake_time;
	const TickType_t scan_period = pdMS_TO_TICKS( 1000 );
	lastwake_time = xTaskGetTickCount();
	
	adc_queuehandle = xQueueCreate( 2, sizeof(float) );
	
	adc_Init();
	
	for( ; ; ) {
		voltage = Get_Voltage();
		temperature = Get_Chip_Temperature();
		if( adc_queuehandle != NULL ) {
			xQueueSend( adc_queuehandle, &temperature, 0 );
			xQueueSend( adc_queuehandle, &voltage, 0);
		}
		
		xTaskDelayUntil( &lastwake_time, scan_period );
	}
}

uint8_t current_menu_index = 0;

void menu_task( void *para )
{
	current_menu_index = 0;
	
	for( ; ; ) {
		current_menu_index = menu_table[current_menu_index].current_operation();
	}
}

void rtc_task( void *para )
{
	//RTC_init( &systime );
	rtc_semaphorehandle = xSemaphoreCreateBinary();
	rtc_queuehandle = xQueueCreate( 1, sizeof(uint32_t) );
	BaseType_t rtc_semaphorestatus;
	uint32_t rtc_value = 0;
	
	while(1)
	{
		rtc_semaphorestatus = xSemaphoreTake( rtc_semaphorehandle, portMAX_DELAY );
		if( rtc_semaphorestatus == pdPASS )
		{
			rtc_value = RTC_GetCounter();
			xQueueSend( rtc_queuehandle, &rtc_value, portMAX_DELAY );
		}
	}
}

int main(void)
{
	BaseType_t led_taskstaus, key_taskstaus, adc_taskstaus;
	
	prvSetupHardware();
	
	led_taskstaus = xTaskCreate( led_task, "LED_Task", 50, NULL, 1, &led_taskhandle );
	key_taskstaus = xTaskCreate( key_task, "KEY_Task", 50, NULL, 2, &key_taskhandle );
	adc_taskstaus = xTaskCreate( adc_task, "ADC_Task", 100, NULL, 1, &adc_taskhandle );
	xTaskCreate( menu_task, "MENU_Task", 200, NULL, 1, &menu_taskhandle );
	xTaskCreate( rtc_task, "RTC_Task", 100, NULL, 3, &rtc_taskhandle );
	
	/* Start the scheduler. */
	vTaskStartScheduler();
	return 0;//if heap overflow have erroe
}

static void prvSetupHardware( void )
{
	/* Start with the clocks in their expected state. */
	RCC_DeInit();

	/* Enable HSE (high speed external clock). */
	RCC_HSEConfig( RCC_HSE_ON );

	/* Wait till HSE is ready. */
	while( RCC_GetFlagStatus( RCC_FLAG_HSERDY ) == RESET )
	{
	}

	/* 2 wait states required on the flash. */
	*( ( unsigned long * ) 0x40022000 ) = 0x02;

	/* HCLK = SYSCLK */
	RCC_HCLKConfig( RCC_SYSCLK_Div1 );

	/* PCLK2 = HCLK */
	RCC_PCLK2Config( RCC_HCLK_Div1 );

	/* PCLK1 = HCLK/2 */
	RCC_PCLK1Config( RCC_HCLK_Div2 );

	/* PLLCLK = 8MHz * 9 = 72 MHz. */
	RCC_PLLConfig( RCC_PLLSource_HSE_Div1, RCC_PLLMul_9 );

	/* Enable PLL. */
	RCC_PLLCmd( ENABLE );

	/* Wait till PLL is ready. */
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
	{
	}

	/* Select PLL as system clock source. */
	RCC_SYSCLKConfig( RCC_SYSCLKSource_PLLCLK );

	/* Wait till PLL is used as system clock source. */
	while( RCC_GetSYSCLKSource() != 0x08 )
	{
	}

	/* Enable GPIOA, GPIOB, GPIOC, GPIOD, GPIOE and AFIO clocks */
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC
							| RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE );

	/* SPI2 Periph clock enable */
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2, ENABLE );


	/* Set the Vector Table base address at 0x08000000 */
	NVIC_SetVectorTable( NVIC_VectTab_FLASH, 0x0 );

	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

	/* Configure HCLK clock as SysTick clock source. */
	SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK );
	
	//init peripheral
	lcd_Strinit();
	USART_Config();
	GPIO_LED_Config();
	GPIO_Key_Iint();
}