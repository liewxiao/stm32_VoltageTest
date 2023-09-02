/**
  ******************************************************************************
  * @file    bsp_rtc.c
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   stm32 RTC 驱动
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-霸道 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
#include "./rtc/bsp_rtc.h"


/*星期，生肖用文字ASCII码*/
char const *WEEK_STR[] = {"日", "一", "二", "三", "四", "五", "六"};
char const *zodiac_sign[] = {"猪", "鼠", "牛", "虎", "兔", "龙", "蛇", "马", "羊", "猴", "鸡", "狗"};

/*英文，星期，生肖用文字ASCII码*/
char const *en_WEEK_STR[] = { "Sunday","Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char const *en_zodiac_sign[] = {"Pig", "Rat", "Ox", "Tiger", "Rabbit", "Dragon", "Snake", "Horse", "Goat", "Monkey", "Rooster", "Dog"};



/*
 * 函数名：NVIC_Configuration
 * 描述  ：配置RTC秒中断的主中断优先级为1，次优先级为0
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
static void RTC_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


/*
 * 函数名：RTC_CheckAndConfig
 * 描述  ：检查并配置RTC
 * 输入  ：用于读取RTC时间的结构体指针
 * 输出  ：无
 * 调用  ：外部调用
 */
static void RTC_CheckAndConfig(rtc_time *tm)
{
   	/*在启动时检查备份寄存器BKP_DR1，如果内容不是0xA5A5,
	  则需重新配置时间并询问用户调整时间*/
	if (BKP_ReadBackupRegister(RTC_BKP_DRX) != RTC_BKP_DATA)
	{
		/* 使用tm的时间配置RTC寄存器 */
		Time_Adjust(tm);
		
		/*向BKP_DR1寄存器写入标志，说明RTC已在运行*/
		BKP_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
	}
	else
	{
		/* 使能 PWR 和 Backup 时钟 */
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
		/* 允许访问 Backup 区域 */
	  PWR_BackupAccessCmd(ENABLE);
		
	  /*LSE启动无需设置新时钟*/
		
		/*等待寄存器同步*/
		RTC_WaitForSynchro();
		
		/*允许RTC秒中断*/
		RTC_ITConfig(RTC_IT_SEC, ENABLE);
		
		/*等待上次RTC寄存器写操作完成*/
		RTC_WaitForLastTask();
	 }
	
	  /* 清除复位标志 flags */
	  RCC_ClearFlag();

}



/*
 * 函数名：RTC_Configuration
 * 描述  ：配置RTC
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
static void RTC_Configuration(void)
{
	/* 使能 PWR 和 Backup 时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	/* 允许访问 Backup 区域 */
	PWR_BackupAccessCmd(ENABLE);
	
	/* 复位 Backup 区域 */
	BKP_DeInit();
	
	/* 使能 LSE */
	RCC_LSEConfig(RCC_LSE_ON);
	
	/* 等待 LSE 准备好 */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{}
	
	/* 选择 LSE 作为 RTC 时钟源 */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
	/* 使能 RTC 时钟 */
	RCC_RTCCLKCmd(ENABLE);
	
	/* 等待 RTC 寄存器 同步
	 * 因为RTC时钟是低速的，内环时钟是高速的，所以要同步
	 */
	RTC_WaitForSynchro();
	
	/* 确保上一次 RTC 的操作完成 */
	RTC_WaitForLastTask();
	
	/* 使能 RTC 秒中断 */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	
	/* 确保上一次 RTC 的操作完成 */
	RTC_WaitForLastTask();
	
	/* 设置 RTC 分频: 使 RTC 周期为1s  */
	/* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) = 1HZ */
	RTC_SetPrescaler(32767); 
	
	/* 确保上一次 RTC 的操作完成 */
	RTC_WaitForLastTask();
	
}



/*
 * 函数名：Time_Adjust
 * 描述  ：时间调节
 * 输入  ：用于读取RTC时间的结构体指针（北京时间）
 * 输出  ：无
 * 调用  ：外部调用
 */
void Time_Adjust(rtc_time *tm)
{
	
	  /* RTC 配置 */
	  RTC_Configuration();

	  /* 等待确保上一次操作完成 */
	  RTC_WaitForLastTask();
		  
	  /* 计算星期 */
	  GregorianDay(tm);

	  /* 由日期计算时间戳并写入到RTC计数寄存器 */
	  RTC_SetCounter(mktimev(tm)-TIME_ZOOM);

	  /* 等待确保上一次操作完成 */
	  RTC_WaitForLastTask();
}

void Time_Display( uint32_t rtc_value, rtc_time *tm )
{
	uint32_t BJ_time = 0;
	uint8_t str[200];
	
	BJ_time = rtc_value + TIME_ZOOM;
	to_tm(BJ_time, tm);
	
	sprintf((char *)str,"%d-%d-%d",tm->tm_year,tm->tm_mon,tm->tm_mday);
	ILI9341_DispStringLine_EN_CH( LINE(0),(char*)str );
	
	sprintf((char *)str,"中国%s年",zodiac_sign[(tm->tm_year-3)%12]);
	ILI9341_DispStringLine_EN_CH( LINE(1),(char*)str );
	
	sprintf((char *)str,"星期%s",WEEK_STR[tm->tm_wday]);
	ILI9341_DispStringLine_EN_CH( LINE(2),(char*)str );
	
	sprintf((char *)str,"%0.2d时:%0.2d分:%0.2d秒",tm->tm_hour,tm->tm_min,tm->tm_sec);
	ILI9341_DispStringLine_EN_CH( LINE(3),(char*)str );
}

void RTC_init( rtc_time *tm )
{
	RTC_NVIC_Config();
	RTC_CheckAndConfig(tm);
}




/************************END OF FILE***************************************/
