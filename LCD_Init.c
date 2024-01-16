//LCD Functions
#include "port_init.h"

void SendDisplayNibble(unsigned char In, unsigned char Rs);
void SendDisplayByte(unsigned char In, unsigned char Rs);
void LCD_init(void);
void print_char(unsigned char In);
void cursor_right(void);
void cursor_left(void);
void Clear_LCD(void);
void Backspace(void);
void Set_second_line_display(void);
void Reset_display_address(void);
void Home(void);
void Clear_result(void);



void SendDisplayNibble(unsigned char In, unsigned char Rs){

	GPIO_PORTB_DATA_R=GPIO_PORTB_DATA_R&0x00;	//clear Port B 
	GPIO_PORTB_DATA_R = In&0x0F;	//Only read the bit0-bit3 from input
	GPIO_PORTA_DATA_R&=~(0x08);	//clear PA3
	GPIO_PORTA_DATA_R=Rs<<3;	//Shift Rs to bit 3 and read it to PA3
	GPIO_PORTA_DATA_R|=0x04;		//set En=1
	SysTick_Wait(30);		//25x30ns=750ns
	GPIO_PORTA_DATA_R&=~(0x04);	//clear PA2 En=0
	SysTick_Wait_ms(10);
}
	
void SendDisplayByte(unsigned char In, unsigned char Rs){
	SendDisplayNibble(In,Rs);	//Send less significant nibble first
	In=In>>4;	//The shift the first 4 bit rightwards and send them
	SendDisplayNibble(In,Rs);
}

void LCD_init(void){
	SysTick_Wait_ms(100);
	
	SysTick_Wait_ms(50);//15ms
	SendDisplayNibble(0x03,0);			//function set 03
	
	SysTick_Wait_ms(10);//4.1ms
	SendDisplayNibble(0x03,0);			//function set
	
	SysTick_Wait_us(150);//100us
	SendDisplayNibble(0x03,0);		//function set
	
	SysTick_Wait_us(37);//37us
	SendDisplayNibble(0x02,0);		//Set 4 bit inerface 02
	
	SysTick_Wait_us(37);//37us
	SendDisplayByte(0x82,0);			//Number of diaply lines and character font 82
	SysTick_Wait_us(37);
	
	SendDisplayByte(0x80,0);   //display off  0x80
	SysTick_Wait_us(37);

	SendDisplayByte(0x10,0);	//clear display 10
	SysTick_Wait_us(37);
			
	SendDisplayByte(0x70,0); 		//Entry mode set 50
	SysTick_Wait_ms(100);
	SendDisplayByte(0xE0,0);		//set cursor
	SysTick_Wait_ms(100);
	SendDisplayByte(0xD0,0);		//blinking cursor
	SysTick_Wait_ms(100);
	
}
void cursor_right(void){
	SendDisplayByte(0xC1,0);    
	SysTick_Wait_us(40);
}
void cursor_left(void){
	SendDisplayByte(0x01,0);    
	SysTick_Wait_us(40);
}
	

void print_char(unsigned char In){
	SendDisplayByte(In,0x01);  //write character code
	cursor_right();		//shift cursor after print 
	SysTick_Wait_us(40);
}
void Clear_LCD(void){
	SendDisplayByte(0x10,0);
	SysTick_Wait_ms(2);
}

void Backspace(void){				//Delete last character
	cursor_left();
	print_char(0x02);
	cursor_left();
	SysTick_Wait_us(40);
}

void Set_second_line_display(void){				//Set the address to the last line in the LCD display to print the result
		SendDisplayByte(0x4D,0);
}

void Reset_display_address(void){				//Reset the display address to first line
			SendDisplayByte(0x08,0);
}

void Home(void){
	SendDisplayByte(0x20,0);        //Back to homw
}
	
void Clear_result(void){	
	Set_second_line_display();
	for (int i=0; i<20;i++){
		print_char(0x02);
	}
}