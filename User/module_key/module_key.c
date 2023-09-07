#include "module_key/module_key.h"

//Iint key GPIO clock mode
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

//get which key press
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
	
	//Each scan reads which key was pressed
	key_LatePress = key_press = GetPort_Value();
	
	//save last pressed key value
	if( key_LatePress ) {
		Key_LategValue = key_press;
	}
	
	//key state machine
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
				/*Because the key scan period is 20 milliseconds, this variable increases by
				20 milliseconds every time it is increased.*/
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
				/*Store the status and value of the button into one byte data*/
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
	
	/*The returned byte contains the key value and key state*/ 
	return key_return;
}

uint8_t Key_Read( void )
{
	static uint8_t key_status = KEY_STATE_NOPRESS;
	static uint8_t LategValue = N_KEY;
	uint8_t key_current_value, key_return;
	
	key_return = N_KEY;
	
	//Read the status of a single key each time
	key_current_value = Key_Drive();
	
	if( key_current_value != N_KEY ) {
		LategValue = key_current_value & VALUE;
	}
	
	//key state machine
	switch( key_status )
	{
		/*single clilk state*/
		case KEY_STATE_NOPRESS:
			/*Determine the single-click state of the same button that is continuously read,
		     and switch the state to the double-click state*/
			if( (key_current_value & STATE) == KEY_STATE_CLILK ) {
				key_time = 0;
				key_status = KEY_STATE_DOUBLE;
			}
			else{
				/*If the state of the scanning button is not clicked,
				then the returned value is long pressed*/
				key_return = key_current_value;
			}
			break;
		case KEY_STATE_DOUBLE:
			/*If the button is pressed again within 300 milliseconds after the first press,
		     then it is a double click; otherwise it is a single click*/
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
	/*The return value is the corresponding state of the key,
	such as single click, double click and long press.*/
	return key_return;
}