#include "module_key/module_key.h"

void GPIO_Key_Iint( void )
{
	RCC_APB2PeriphClockCmd(	GPIO_Key1_CLK|\
							GPIO_Key2_CLK,ENABLE);
	GPIO_InitTypeDef GPIO_IintStruct;
	GPIO_IintStruct.GPIO_Pin = GPIO_Key1_PIN;
	GPIO_IintStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIO_Key1_PORT, &GPIO_IintStruct);
	GPIO_IintStruct.GPIO_Pin = GPIO_Key2_PIN;
	GPIO_Init(GPIO_Key2_PORT, &GPIO_IintStruct);
}

static uint8_t GetPort_Value( void )
{
	if( Key1_Stae == KEY_ON ) {
		return KEY_K1;
	}
	else if( Key2_Stae == KEY_ON ) {
		return KEY_K2;
	}
	
	return KEY_NO_DOWN;
}

uint8_t key_time = 0;

static uint8_t Key_Drive( void )
{
	static uint8_t key_status = KEY_STATE_NOPRESS;
	static uint8_t Key_LategValue = 0;
	uint8_t key_LatePress, key_press, key_return;
	
	key_return = N_KEY;
	
	key_LatePress = key_press = GetPort_Value();
	
	if( key_LatePress ) {
		Key_LategValue = key_press;
	}
	
	switch( key_status )
	{
		case KEY_STATE_NOPRESS:
			if( (key_press != N_KEY) && (key_press == key_LatePress) ) {
				key_time = 0;
				key_status = KEY_STATE_DEBOUNCE;
			}
			break;
		case KEY_STATE_DEBOUNCE:
			if( (key_press != N_KEY) && (key_press == key_LatePress) ) {
				key_time++;
				if( key_time >= DEBOUNCE_TIME ) {
					key_status = KEY_STATE_CLILK;
				}
			}
			else{
				key_status = KEY_STATE_NOPRESS;
			}
			break;
		case KEY_STATE_CLILK:
			if( key_press == N_KEY ) {
				key_return = Key_LategValue | KEY_STATE_CLILK;
				key_status = KEY_STATE_NOPRESS;
			}
			else{
				key_time++;
				if( key_time >= LONGPRESS_TIME ) {
					key_return = Key_LategValue | KEY_STATE_LONGPRESS;
					key_status = KEY_STATE_RELEASE;
				}
			}
			break;
		case KEY_STATE_RELEASE:
			if( key_press == N_KEY ) {
				key_status = KEY_STATE_NOPRESS;
			}
			break;
		default:
			key_status = KEY_STATE_NOPRESS;		
	}
	
	return key_return;
}

uint8_t Key_Read( void )
{
	static uint8_t key_status = KEY_STATE_NOPRESS;
	static uint8_t LategValue = N_KEY;
	uint8_t key_current_value, key_return;
	
	key_return = N_KEY;
	key_current_value = Key_Drive();
	
	if( key_current_value != N_KEY ) {
		LategValue = key_current_value & VALUE;
	}
	
	switch( key_status )
	{
		case KEY_STATE_NOPRESS:
			if( (key_current_value & STATE) == KEY_STATE_CLILK ) {
				key_time = 0;
				key_status = KEY_STATE_DOUBLE;
			}
			else{
				key_return = key_current_value;
			}
			break;
		case KEY_STATE_DOUBLE:
			if( (key_current_value & STATE) == KEY_STATE_CLILK ) {
				key_return = LategValue | KEY_STATE_DOUBLE;
				key_status = KEY_STATE_RELEASE;
			}
			else{
				if( ++key_time >= DOUBLECILIK_TIME ) {
					key_return = LategValue | KEY_STATE_CLILK;
					key_status = KEY_STATE_NOPRESS;
				}
			}
			break;
		case KEY_STATE_RELEASE:
			if( key_current_value == N_KEY ) {
				key_status = KEY_STATE_NOPRESS;
			}
			break;
		default:
			key_status = KEY_STATE_NOPRESS;
	}
	
	return key_return;
}