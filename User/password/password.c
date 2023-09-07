#include "password.h"

void password_op( uint8_t key, uint8_t*location, uint8_t* passwordpart )
{
	switch( key )
	{
		case KEY_UP:
			if( passwordpart[*location] < 9 ) {
				passwordpart[*location]++;
			}else{
				passwordpart[*location] = 0;
			}
			break;
		case KEY_DOWN:
			if( passwordpart[*location] > 0 ) {
				passwordpart[*location]--;
			}else{
				passwordpart[*location] = 9;
			}
			break;
		case KEY_RIGHT:
			++*location;
			if( *location >= 4 )
				*location = 0;
			break;
	}
}

mybool check_password( uint8_t* passwordpart )
{
	mybool password_state = MYFALSE;
	uint8_t i = 0;
	uint8_t offset = 10;
	uint16_t temp_password = 0;
	uint8_t readbuff[2] = {0};
	
	for( i=3; i<=0; i-- ) {
		temp_password *= offset;
		temp_password += passwordpart[i];
	}
	
	
	
	return password_state;
}