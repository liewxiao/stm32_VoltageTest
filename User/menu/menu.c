#include "menu.h"

/*Init LCD setup LCD scan mode word display size background
 color and clear screen*/  
void lcd_Strinit( void )
{
	ILI9341_Init();
	ILI9341_GramScan ( 6 );
	LCD_SetFont(&Font8x16);
	LCD_SetColors(RED,BLACK);
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
}

/*define menu struct array ,each struct have display function*/
key_table menu_table[35] = {
	//zero menu level
	{ 0, main_menu },
	
	//first menu level
	{ 1, password_menu },
	
	//second menu level
	{ 2, second_menu1 },
	{ 3, second_menu2 },
	{ 4, second_menu3 },
	{ 5, second_menu4 },
	{ 6, second_menu5 },
	
	//second menu submenu level
	{ 7, second_submenu1_1 },
	{ 8, second_submenu1_2 },
	{ 9, second_submenu1_3 },
	{ 10, second_submenu1_4 },
	{ 11, second_submenu2_1 },
	{ 12, second_submenu2_2 },
	{ 13, second_submenu3_1 },
	{ 14, second_submenu3_2 },
	{ 15, second_submenu3_3 },
	{ 16, second_submenu3_4 },
	{ 17, second_submenu4_1 },
	{ 18, second_submenu4_2 },
	{ 19, second_submenu5_1 },
	{ 20, second_submenu5_2 },
	
	//third menu level
	{ 21, lg_set },
	{ 22, btn_set },
	{ 23, pas_set },
	{ 24, time_set },
	{ 25, alram_up },
	{ 26, alram_low },
	{ 27, dev_adress },
	{ 28, batu_set },
	{ 29, parity_set },
	{ 30, stopbit_set },
	{ 31, ip_set },
	{ 32, devport_set },
	{ 33, data },
	{ 34, history_data }
};

/*LCD display scan each pirod have key press*/
static char scan_status = GET_DEFAULT;
static uint8_t menu_key = 0;

/*Only when the read queue is not N_KEY and the queue is received
successfully can the current menu button state be changed*/
static uint8_t wait_key_box( char *key_status )
{
	uint8_t key_value = 0;
	BaseType_t queuestatus;
	BaseType_t keyseam_status;
	
	queuestatus = xQueueReceive( key_queuehandle,(uint8_t*)&key_value,0 );
	
	if( (queuestatus == pdPASS) && (key_value != N_KEY) ) {
		*key_status = GET_KEY;
		return key_value;
	}
}

uint8_t main_menu( void )
{
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
	
	menu_key = 0;
	uint32_t rtc_value = 0;
	
	/*define voltage temperature buffer*/
	float voltage = 0;
	float temperature = 0;
	char Vbuf[50] = {0};
	char Tbuf[50] = {0};
	
	while(1){
		
		/*scan current menu whether key press*/
		menu_key = wait_key_box( &scan_status );
		
		if( scan_status == GET_KEY ) {
			scan_status = GET_DEFAULT;
			
			switch( menu_key )
			{
				case KEY_ENTER:
					return 1;
			}
		}
		
		/*Receive rtc temperature voltage value*/
		xQueueReceive( rtc_queuehandle, &rtc_value, 0 );
		xQueueReceive( adc_queuehandle, &temperature, 0 );
		xQueueReceive( adc_queuehandle, &voltage, 0 );
		
		/*analyze rtc value*/
		Time_Display( rtc_value, &systime );

		sprintf( Vbuf,"电压值：%.1f",voltage);
		sprintf( Tbuf,"环境温度：%.1f",temperature);

		ILI9341_DispStringLine_EN_CH( LINE(4),Vbuf );
		ILI9341_DispStringLine_EN_CH( LINE(5),Tbuf );
		ILI9341_DispStringLine_EN_CH( LINE(7),"KEY1单击上翻菜单" );
		ILI9341_DispStringLine_EN_CH( LINE(9),"KEY2单击下翻菜单" );
		ILI9341_DispStringLine_EN_CH( LINE(11),"KEY1双击向左" );
		ILI9341_DispStringLine_EN_CH( LINE(13),"KEY2单击向右" );
		ILI9341_DispStringLine_EN_CH( LINE(15),"KEY1长按确定" );
		ILI9341_DispStringLine_EN_CH( LINE(17),"KEY2长按退出" );
	}
}

uint8_t password_menu( void )
{
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
	
	menu_key = 0;
	uint8_t password[4] = {0, 0, 0, 0};
	uint8_t location = 0;
	uint8_t lastlocation = 0;
	char numbuf[10] = {0};
	
	while(1){
		menu_key = wait_key_box( &scan_status );
		
		if( scan_status == GET_KEY ) {
			scan_status = GET_DEFAULT;
			switch( menu_key )
			{
				case KEY_ENTER:
					return 2;
			}
		}
		
		lastlocation = location;
		password_op( menu_key, &location, password );
		
		if( lastlocation != location )
			ILI9341_Clear( 0, 132, 240, 16 );
		
		switch( location )
		{
			case 0:
				ILI9341_DispString_EN_CH( 98, 132, "^" );
				break;
			case 1:
				ILI9341_DispString_EN_CH( 82, 132, "^" );
				break;
			case 2:
				ILI9341_DispString_EN_CH( 66, 132, "^" );
				break;
			case 3:
				ILI9341_DispString_EN_CH( 50, 132, "^" );
				break;
		}
		
		sprintf(numbuf,"%d-",password[3]);
		ILI9341_DispString_EN_CH( 50, 116, numbuf );
		sprintf(numbuf,"%d-",password[2]);
		ILI9341_DispString_EN_CH( 66, 116, numbuf );
		sprintf(numbuf,"%d-",password[1]);
		ILI9341_DispString_EN_CH( 82, 116, numbuf );
		sprintf(numbuf,"%d",password[0]);
		ILI9341_DispString_EN_CH( 98, 116, numbuf );
		
		ILI9341_DispString_CH( 40, 100, "请输入四位数密码" );
	}
}

uint8_t second_menu1( void )
{
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
	menu_key = 0;
	
	while(1){
		menu_key = wait_key_box( &scan_status );
		
		if( scan_status == GET_KEY ) {
			scan_status = GET_DEFAULT;
			switch( menu_key )
			{
				case KEY_UP:
					return 6;
				case KEY_DOWN:
					return 3;
			}
		}

		ILI9341_DispString_EN_CH( 0, 0, "-->" );
		ILI9341_DispString_EN_CH( 24, 0, "系统设置" );
		ILI9341_DispStringLine_EN_CH( LINE(3),"报警设置" );
		ILI9341_DispStringLine_EN_CH( LINE(6),"通讯设置" );
		ILI9341_DispStringLine_EN_CH( LINE(9),"网络设置" );
		ILI9341_DispStringLine_EN_CH( LINE(12),"历史查询" );
	}
}

uint8_t second_menu2( void )
{
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
	menu_key = 0;
	
	while(1){
		menu_key = wait_key_box( &scan_status );
		
		if( scan_status == GET_KEY ) {
			scan_status = GET_DEFAULT;
			switch( menu_key )
			{
				case KEY_UP:
					return 2;
				case KEY_DOWN:
					return 4;
			}
		}
		
		ILI9341_DispStringLine_EN_CH( LINE(0), "系统设置" );
		ILI9341_DispString_EN_CH( 0, 48, "-->" );
		ILI9341_DispString_EN_CH( 24,48,"报警设置" );
		ILI9341_DispStringLine_EN_CH( LINE(6),"通讯设置" );
		ILI9341_DispStringLine_EN_CH( LINE(9),"网络设置" );
		ILI9341_DispStringLine_EN_CH( LINE(12),"历史查询" );
	}
}

uint8_t second_menu3( void )
{
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
	menu_key = 0;
	
	while(1){
		menu_key = wait_key_box( &scan_status );
		
		if( scan_status == GET_KEY ) {
			scan_status = GET_DEFAULT;
			switch( menu_key )
			{
				case KEY_UP:
					return 3;
				case KEY_DOWN:
					return 5;
			}
		}
		
		ILI9341_DispStringLine_EN_CH( LINE(0), "系统设置" );
		ILI9341_DispStringLine_EN_CH( LINE(3),"报警设置" );
		ILI9341_DispString_EN_CH( 0, 96, "-->" );
		ILI9341_DispString_EN_CH( 24,96,"通讯设置" );
		ILI9341_DispStringLine_EN_CH( LINE(9),"网络设置" );
		ILI9341_DispStringLine_EN_CH( LINE(12),"历史查询" );
	}
}

uint8_t second_menu4( void )
{
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
	menu_key = 0;
	
	while(1){
		menu_key = wait_key_box( &scan_status );
		
		if( scan_status == GET_KEY ) {
			scan_status = GET_DEFAULT;
			switch( menu_key )
			{
				case KEY_UP:
					return 4;
				case KEY_DOWN:
					return 6;
			}
		}
		
		ILI9341_DispStringLine_EN_CH( LINE(0), "系统设置" );
		ILI9341_DispStringLine_EN_CH( LINE(3),"报警设置" );
		ILI9341_DispStringLine_EN_CH( LINE(6),"通讯设置" );
		ILI9341_DispString_EN_CH( 0, 144, "-->" );
		ILI9341_DispString_EN_CH( 24,144,"网络设置" );
		ILI9341_DispStringLine_EN_CH( LINE(12),"历史查询" );
	}
}

uint8_t second_menu5( void )
{
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
	menu_key = 0;

	while(1){
		menu_key = wait_key_box( &scan_status );
		
		if( scan_status == GET_KEY ) {
			scan_status = GET_DEFAULT;
			switch( menu_key )
			{
				case KEY_UP:
					return 5;
				case KEY_DOWN:
					return 2;
			}
		}
		
		ILI9341_DispStringLine_EN_CH( LINE(0), "系统设置" );
		ILI9341_DispStringLine_EN_CH( LINE(3),"报警设置" );
		ILI9341_DispStringLine_EN_CH( LINE(6),"通讯设置" );
		ILI9341_DispStringLine_EN_CH( LINE(9),"网络设置" );
		ILI9341_DispString_EN_CH( 0, 192, "-->" );
		ILI9341_DispString_EN_CH( 24,192,"历史查询" );
	}
}

uint8_t second_submenu1_1( void )
{
	
}

uint8_t second_submenu1_2( void )
{
	
}

uint8_t second_submenu1_3( void )
{
	
}

uint8_t second_submenu1_4( void )
{
	
}

uint8_t second_submenu2_1( void )
{
	
}

uint8_t second_submenu2_2( void )
{
	
}

uint8_t second_submenu3_1( void )
{
	
}

uint8_t second_submenu3_2( void )
{
	
}

uint8_t second_submenu3_3( void )
{
	
}

uint8_t second_submenu3_4( void )
{
	
}

uint8_t second_submenu4_1( void )
{
	
}

uint8_t second_submenu4_2( void )
{
	
}

uint8_t second_submenu5_1( void )
{
	
}

uint8_t second_submenu5_2( void )
{
	
}

uint8_t lg_set( void )
{
	
}

uint8_t btn_set( void )
{
	
}

uint8_t pas_set( void )
{
	
}

uint8_t time_set( void )
{
	
}

uint8_t alram_up( void )
{
	
}

uint8_t alram_low( void )
{
	
}

uint8_t dev_adress( void )
{
	
}

uint8_t batu_set( void )
{
	
}

uint8_t parity_set( void )
{
	
}

uint8_t stopbit_set( void )
{
	
}

uint8_t ip_set( void )
{
	
}

uint8_t devport_set( void )
{
	
}

uint8_t data( void )
{
	
}

uint8_t history_data( void )
{
	
}
