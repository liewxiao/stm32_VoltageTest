#ifndef      __MENU_H
#define	     __MENU_H

#include "bsp_ili9341_lcd.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

typedef struct{
	uint8_t menu_id;//menu id
	void (*current_operation)();
}key_table;

extern key_table menu_table[35];

void main_menu( void );
void password_menu( void );
void second_menu1( void );
void second_menu2( void );
void second_menu3( void );
void second_menu4( void );
void second_menu5( void );
void second_submenu1_1( void );
void second_submenu1_2( void );
void second_submenu1_3( void );
void second_submenu1_4( void );
void second_submenu2_1( void );
void second_submenu2_2( void );
void second_submenu3_1( void );
void second_submenu3_2( void );
void second_submenu3_3( void );
void second_submenu3_4( void );
void second_submenu4_1( void );
void second_submenu4_2( void );
void second_submenu5_1( void );
void second_submenu5_2( void );
void lg_set( void );
void btn_set( void );
void pas_set( void );
void time_set( void );
void alram_up( void );
void alram_low( void );
void dev_adress( void );
void batu_set( void );
void parity_set( void );
void stopbit_set( void );
void ip_set( void );
void devport_set( void );
void data( void );
void history_data( void );

#endif