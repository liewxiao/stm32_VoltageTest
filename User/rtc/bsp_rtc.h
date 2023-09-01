

#ifndef __RTC_H
#define	__RTC_H


#include "stm32f10x.h"
#include <stdio.h>
#include "bsp_ili9341_lcd.h"
#include "./rtc/bsp_calendar.h"
#include "./rtc/bsp_date.h"
#include "module_led.h"
#include "global.h"

#define RTC_BKP_DRX          BKP_DR1
// 写入到备份寄存器的数据宏定义
#define RTC_BKP_DATA         0xAAAA

//北京时间的时区秒数差
#define TIME_ZOOM						(8*60*60)

void RTC_init( rtc_time *tm );
void Time_Adjust(rtc_time *tm);
void Time_Show(rtc_time *tm);
void Time_Display( uint32_t rtc_value, rtc_time *tm );

#endif /* __XXX_H */
