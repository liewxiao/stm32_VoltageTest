#include "password.h"

/*Process the password array and update the cursor position in real time*/
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

/*Check whether the entered password is correct with the set password*/
mybool check_password( uint8_t* passwordpart )
{
	mybool password_state = MYTURE;
	uint8_t i = 0;
	uint8_t readbuff[4] = {0};
	
	//read AT24C02 data and check password array part
	if(ee_ReadBytes(readbuff, 0, 4)) {
		for( i=0; i<4; i++ ) {
			if(readbuff[i] == passwordpart[i])
				continue;
			else
				password_state = MYFALSE;
				break;
		}
	}
	
	return password_state;
}