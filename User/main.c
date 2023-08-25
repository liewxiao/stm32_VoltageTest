#include "stm32f10x.h"
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

//peripherals handle
#include "module_led.h"
#include "module_usart.h"
#include "module_key.h"
#include "module_adc.h"
#include "bsp_ili9341_lcd.h"
#include "menu.h"

static void prvSetupHardware( void );

//create LED and KEY taskhandle
TaskHandle_t led_taskhandle;
TaskHandle_t key_taskhandle;
TaskHandle_t environmental_taskhandle;
TaskHandle_t display_taskhandle;


void led_task( void *para )
{
	BaseType_t task_status;
	uint8_t button_vaule;
	
	for( ; ; ){
		task_status = xTaskNotifyWait(0,0,(uint32_t*)(&button_vaule),portMAX_DELAY);
		
		if( task_status==pdPASS ) {
			switch(button_vaule)
			{
				case Key1_S:
					LED_RED;
					break;
				case Key1_D:
					LED_GREEN;
					break;
				case Key1_L:
					LED_BLUE;
					break;
				case Key2_S:
					LED_YELLOW;
					break;
				case Key2_D:
					LED_PURPLE;
					break;
				case Key2_L:
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
	
	for( ; ; ) {
		key_value = Key_Read();
		xTaskNotify(led_taskhandle,(uint32_t)(key_value),eSetValueWithoutOverwrite);
		
		xTaskDelayUntil( &lastwake_time, scan_period );
	}
}

void environmental_task( void *para )
{
	ADC_Iint();
	
	volatile uint32_t ADC_value = 0;
	
	
	for( ; ; ) {
		ADC_value = ADC_Scan();
		xTaskNotify(display_taskhandle,ADC_value,eSetValueWithoutOverwrite);
		
	}
}

void (*current_operation_index)();
static uint8_t last_index = 35;
static uint8_t fun_index = 0;

void display_task( void *para )
{
	ILI9341_Init();
	ILI9341_GramScan ( 6 );
	LCD_SetFont(&Font8x16);
	LCD_SetColors(RED,BLACK);
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
	

	BaseType_t task_status;
	uint16_t temperature_value = 0;
	uint16_t voltage_value = 0;
	
	for( ; ; ) {
		second_menu1();		
		main_menu();
		password_menu();
	}
}

int main(void)
{
	BaseType_t led_taskstaus, key_taskstaus, environmental_taskstaus;
	//init peripheral
	GPIO_LED_Config();
	GPIO_Key_Iint();
	
	prvSetupHardware();
	
	led_taskstaus = xTaskCreate( led_task, "LED_Task", 100, NULL, 0, &led_taskhandle );
	key_taskstaus = xTaskCreate( key_task, "KEY_Task", 100, NULL, 1, &key_taskhandle );
	environmental_taskstaus = xTaskCreate( environmental_task, "environmental_Task", 100, NULL, 0, &environmental_taskhandle );
	xTaskCreate( display_task, "display_Task", 100, NULL, 0, &display_taskhandle );
	
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

	USART_Config();
}