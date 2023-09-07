#ifndef __GLOBAL_H
#define __GLOBAL_H

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "semphr.h"
#include "bsp_rtc.h"

extern TaskHandle_t led_taskhandle;
extern TaskHandle_t key_taskhandle;
extern TaskHandle_t adc_taskhandle;
extern TaskHandle_t menu_taskhandle;
extern TaskHandle_t rtc_taskhandle;

extern QueueHandle_t  key_queuehandle;
extern QueueHandle_t adc_queuehandle;
extern QueueHandle_t rtc_queuehandle;

extern rtc_time systime;

typedef enum{ MYFALSE = 0, MYTURE = 1 }mybool;

#endif