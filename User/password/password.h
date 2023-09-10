#ifndef      __PASSWORD_H
#define	     __PASSWORD_H

#include "menu.h"
#include "eeprom.h"


void password_op( uint8_t key, uint8_t*location, uint8_t* passwordpart );
mybool check_password( uint8_t* passwordpart );

#endif