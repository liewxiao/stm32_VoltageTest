#ifndef      __MENU_H
#define	     __MENU_H

#include "bsp_ili9341_lcd.h"
#include "module_key.h"
#include "module_adc.h"
#include "bsp_rtc.h"
#include <stdio.h>
#include "global.h"

#define GET_KEY		1
#define GET_DEFAULT -1

typedef struct{
	uint8_t menu_id;				//menu id
	uint8_t (*current_operation)(); //menu implement function
}key_table;

extern key_table menu_table[35];

void lcd_Strinit( void );

uint8_t main_menu( void );
uint8_t password_menu( void );
uint8_t second_menu1( void );
uint8_t second_menu2( void );
uint8_t second_menu3( void );
uint8_t second_menu4( void );
uint8_t second_menu5( void );
uint8_t second_submenu1_1( void );
uint8_t second_submenu1_2( void );
uint8_t second_submenu1_3( void );
uint8_t second_submenu1_4( void );
uint8_t second_submenu2_1( void );
uint8_t second_submenu2_2( void );
uint8_t second_submenu3_1( void );
uint8_t second_submenu3_2( void );
uint8_t second_submenu3_3( void );
uint8_t second_submenu3_4( void );
uint8_t second_submenu4_1( void );
uint8_t second_submenu4_2( void );
uint8_t second_submenu5_1( void );
uint8_t second_submenu5_2( void );
uint8_t lg_set( void );
uint8_t btn_set( void );
uint8_t pas_set( void );
uint8_t time_set( void );
uint8_t alram_up( void );
uint8_t alram_low( void );
uint8_t dev_adress( void );
uint8_t batu_set( void );
uint8_t parity_set( void );
uint8_t stopbit_set( void );
uint8_t ip_set( void );
uint8_t devport_set( void );
uint8_t data( void );
uint8_t history_data( void );

#endif