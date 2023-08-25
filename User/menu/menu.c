#include "menu.h"

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

void menu_scan( uint8_t key )
{
	
}

void main_menu( void )
{
	uint8_t key_value = 0;
	
	while(1){
		
		ILI9341_DispStringLine_EN_CH( LINE(0),"北京时间：" );
		ILI9341_DispStringLine_EN_CH( LINE(5),"电压值：" );
		ILI9341_DispStringLine_EN_CH( LINE(7),"KEY1单击上翻菜单" );
		ILI9341_DispStringLine_EN_CH( LINE(8),"KEY2单击下翻菜单" );
		ILI9341_DispStringLine_EN_CH( LINE(9),"KEY1双击向左" );
		ILI9341_DispStringLine_EN_CH( LINE(10),"KEY2单击向右" );
		ILI9341_DispStringLine_EN_CH( LINE(11),"KEY1长按确定" );
		ILI9341_DispStringLine_EN_CH( LINE(12),"KEY2长按退出" );
	}
}

void password_menu( void )
{
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
	ILI9341_DispStringLine_EN_CH( LINE(0),"请输入设置密码：" );
}

void second_menu1( void )
{
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
	while(1){
	ILI9341_DispString_EN_CH( 0, 0, "->" );
	ILI9341_DispString_EN_CH( 16, 0, "系统设置" );
	ILI9341_DispStringLine_EN_CH( LINE(1),"报警设置" );
	ILI9341_DispStringLine_EN_CH( LINE(2),"通讯设置" );
	ILI9341_DispStringLine_EN_CH( LINE(3),"网络设置" );
	ILI9341_DispStringLine_EN_CH( LINE(4),"历史查询" );
	}
}

void second_menu2( void )
{
//	ILI9341_Clear(0,0,16,16);
//	
//	ILI9341_DispStringLine_EN_CH( LINE(0), "系统设置" );
//	ILI9341_DispString_EN_CH( 0, 16, "->" );
//	ILI9341_DispString_EN_CH( 16,16,"报警设置" );
//	ILI9341_DispStringLine_EN_CH( LINE(2),"通讯设置" );
//	ILI9341_DispStringLine_EN_CH( LINE(1),"网络设置" );
//	ILI9341_DispStringLine_EN_CH( LINE(1),"历史查询" );
}

void second_menu3( void )
{
//	ILI9341_Clear(0,16,16,16);
//	
//	ILI9341_DispStringLine_EN_CH( LINE(0), "系统设置" );
//	ILI9341_DispStringLine_EN_CH( LINE(1),"报警设置" );
//	ILI9341_DispString_EN_CH( 0, 32, "->" );
//	ILI9341_DispString_EN_CH( 16,32,"通讯设置" );
//	ILI9341_DispStringLine_EN_CH( LINE(1),"网络设置" );
//	ILI9341_DispStringLine_EN_CH( LINE(1),"历史查询" );
}

void second_menu4( void )
{
//	ILI9341_Clear(0,32,16,16);
//	
//	ILI9341_DispStringLine_EN_CH( LINE(0), "系统设置" );
//	ILI9341_DispStringLine_EN_CH( LINE(1),"报警设置" );
//	ILI9341_DispStringLine_EN_CH( LINE(2),"通讯设置" );
//	ILI9341_DispString_EN_CH( 0, 48, "->" );
//	ILI9341_DispString_EN_CH( 16,48,"网络设置" );
//	ILI9341_DispStringLine_EN_CH( LINE(1),"历史查询" );
}

void second_menu5( void )
{
//	ILI9341_Clear(0,48,16,16);
//	
//	ILI9341_DispStringLine_EN_CH( LINE(0), "系统设置" );
//	ILI9341_DispStringLine_EN_CH( LINE(1),"报警设置" );
//	ILI9341_DispStringLine_EN_CH( LINE(2),"通讯设置" );
//	ILI9341_DispStringLine_EN_CH( LINE(1),"网络设置" );
//	ILI9341_DispString_EN_CH( 0, 64, "->" );
//	ILI9341_DispString_EN_CH( 16,64,"历史查询" );
}

void second_submenu1_1( void )
{
	
}

void second_submenu1_2( void )
{
	
}

void second_submenu1_3( void )
{
	
}

void second_submenu1_4( void )
{
	
}

void second_submenu2_1( void )
{
	
}

void second_submenu2_2( void )
{
	
}

void second_submenu3_1( void )
{
	
}

void second_submenu3_2( void )
{
	
}

void second_submenu3_3( void )
{
	
}

void second_submenu3_4( void )
{
	
}

void second_submenu4_1( void )
{
	
}

void second_submenu4_2( void )
{
	
}

void second_submenu5_1( void )
{
	
}

void second_submenu5_2( void )
{
	
}

void lg_set( void )
{
	
}

void btn_set( void )
{
	
}

void pas_set( void )
{
	
}

void time_set( void )
{
	
}

void alram_up( void )
{
	
}

void alram_low( void )
{
	
}

void dev_adress( void )
{
	
}

void batu_set( void )
{
	
}

void parity_set( void )
{
	
}

void stopbit_set( void )
{
	
}

void ip_set( void )
{
	
}

void devport_set( void )
{
	
}

void data( void )
{
	
}

void history_data( void )
{
	
}


/*typedef struct{
	uint8_t current_id;//current menu id
	uint8_t up;//turn up id
	uint8_t down;//turn down id
	uint8_t enter;//enter id
	uint8_t exit;//exit id
	void (*current_operation)();
}key_table;*/