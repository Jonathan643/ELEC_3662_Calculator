// Keypad functions

#include "port_init.h"

unsigned char Keypad_Keys[4][4]={
	{0x01,0x02,0x03,0x11},
	{0x04,0x05,0x06,0x12},
	{0x07,0x08,0x09,0x13},
	{0x16,0x00,0x15,0x14}
};
	

void read_keypad(unsigned char* column,unsigned char*row);
void read_keypad(unsigned char* column,unsigned char*row){
	*column= GPIO_PORTD_DATA_R&0x0F;
	*row= GPIO_PORTE_DATA_R&0x0F;
}

unsigned char decode_keypad(unsigned char column,unsigned char row);
unsigned char decode_keypad(unsigned char column,unsigned char row){
	return Keypad_Keys[row][column];
	}