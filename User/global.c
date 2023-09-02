#include "global.h"

//create taskhandle
TaskHandle_t led_taskhandle;
TaskHandle_t key_taskhandle;
TaskHandle_t adc_taskhandle;
TaskHandle_t menu_taskhandle;
TaskHandle_t rtc_taskhandle;

//create handle
QueueHandle_t  key_queuehandle;
QueueHandle_t adc_queuehandle;
QueueHandle_t rtc_queuehandle;
SemaphoreHandle_t rtc_semaphorehandle;

rtc_time systime = { 59, 30, 17, 2, 9, 2023, 6 };

/*struct rtc_time {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
};*/