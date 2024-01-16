//main.c
#include "main.h"
#include "TExaS.h"
#include "PLL.c"
#include "LCD_Init.c"
#include "port_init.h"
#include "Keypad.c"
#include "calculator.h"

//main
int main(void){
	PLL_init();
	SysTick_Init();
	Port_F_Init();
	Port_A_init();
	Port_B_init();
	Port_D_init();
	Port_E_init();
	LCD_init();
	unsigned char Keys_CG=0x00;
	unsigned char Shift=0x00;
	unsigned char Input_memory[40];
	int Input_memory_type[40];
	int result;
	int j;			//j act as a pointer for the memory
	 j=0;				//initailze as 0 at first, so that first key pressed will be stored into first position of memory
	int result_mem=0;
	int result_flag=0;
	Clear_memory(Input_memory,Input_memory_type);    // Clear memory to prevent any floating values
		while(1){
			Keys_CG=read_keypad(Shift);				//read the input from the keypad
			if(Keys_CG>0x00){
					if(Keys_CG==0x10){   //check shift
						Shift^=0x01;
	GPIO_PORTF_DATA_R ^=0x04;	
						}
					else {
							if(Keys_CG==0x20){    //delet last
								Backspace();
								Input_memory[j]=0x00;
								j--;
							}
							else if(Keys_CG==0x30){   //Clear LCD and memory
								Clear_LCD();
								Home();
								Clear_memory(Input_memory,Input_memory_type);
								j=0;				//reset pointer
							}
							else if(Keys_CG==0xD3){   //Execute
								
								GPIO_PORTF_DATA_R ^=0x08;			//Turn on to show the result is being calculated
								Input_memory[j]=0x40;
								result=Execute(Input_memory,Input_memory_type);
								print_results(result);
								GPIO_PORTF_DATA_R ^=0x08;			//Switch off when the calculation is finished
								
								Home();
								j=0;					//Reset the pointer
								result_mem=result;
								
							}
							else {   //number and operation
								print_char(Keys_CG);						//Print the entered characters
								Input_memory[j]=Keys_CG;						//Store the keys entered and their types
								Input_memory_type[j]=check_input_type(Input_memory[j]);
								j++;   //Increment the pointer so that next input will be stored in the next position in the memory
		
							}
					}
					Keys_CG&=0x00;
					
			}
			SysTick_Wait_ms(100);
		}
}

void Port_F_Init(void){ 
	volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R |= 0x1F;           // allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R &= 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R &= 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R &= ~0x11;          // 5.1) PF4,PF0 input, 
  GPIO_PORTF_DIR_R |= 0x0E;          // 5.2) PF3 output  
  GPIO_PORTF_AFSEL_R &= 0x00;        // 6) no alternate function
  GPIO_PORTF_PUR_R |= 0x11;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R |= 0x1F;          // 7) enable digital pins PF4-PF0     
}
	
	void Port_A_init(void){	//Control LCD
		volatile unsigned long delay;
		SYSCTL_RCGC2_R |= 0x00000001;
		delay = SYSCTL_RCGC2_R;
		GPIO_PORTA_LOCK_R=0x4C4F434B;
		GPIO_PORTA_CR_R = 0x0C;
		GPIO_PORTA_AMSEL_R = 0x00;
		GPIO_PORTA_PCTL_R = 0x00000000;
		GPIO_PORTA_DIR_R = 0x0C;
		GPIO_PORTA_AFSEL_R = 0x00;
		GPIO_PORTA_PUR_R = 0x00;
		GPIO_PORTA_DEN_R = 0x0C;
	}
	void Port_B_init(void){	//LCD data output from MCU
		volatile unsigned long delay;
		SYSCTL_RCGC2_R |= 0x00000002;
		delay = SYSCTL_RCGC2_R;
		GPIO_PORTB_LOCK_R=0x4C4F434B;
		GPIO_PORTB_CR_R |= 0x0F;
		GPIO_PORTB_AMSEL_R &= 0x00;
		GPIO_PORTB_PCTL_R &= 0x00000000;
		GPIO_PORTB_DIR_R |= 0x0F;
		GPIO_PORTB_AFSEL_R &= 0x00;
		GPIO_PORTB_PUR_R &= 0x00;
		GPIO_PORTB_DEN_R |= 0x0F;
		}
	void Port_D_init(void){	//Column input from keypad
		volatile unsigned long delay;
		SYSCTL_RCGC2_R |= 0x00000008;
		delay = SYSCTL_RCGC2_R;
		GPIO_PORTD_LOCK_R=0x4C4F434B;
		GPIO_PORTD_CR_R |= 0x0F;
		GPIO_PORTD_AMSEL_R &= 0x00;
		GPIO_PORTD_PCTL_R &= 0x00000000;
		GPIO_PORTD_DIR_R &= 0x00;
		GPIO_PORTD_AFSEL_R &= 0x00;
		GPIO_PORTD_PDR_R|=0x0F;
		GPIO_PORTD_DEN_R |= 0x0F;
	}
	void Port_E_init(void){	//Row output to keypad	
		volatile unsigned long delay;
		SYSCTL_RCGC2_R |= 0x00000010;
		delay = SYSCTL_RCGC2_R;
		GPIO_PORTE_LOCK_R=0x4C4F434B;
		GPIO_PORTE_CR_R |= 0x0F;
		GPIO_PORTE_AMSEL_R &= 0x00;
		GPIO_PORTE_PCTL_R &= 0x00000000;
		GPIO_PORTE_DIR_R |= 0x0F;
		GPIO_PORTE_AFSEL_R &= 0x00;
		GPIO_PORTE_DEN_R |= 0x0F;
	}

void Clear_memory(unsigned char Input_memory[40],int Input_memory_type[40]){
		for(int i=0; i<40; i++){  //clear memory
			Input_memory[i]&=0x00;
			Input_memory_type[i]=undefined;
		}
	}
	
void Shift_toggle(unsigned char Shift){
	Shift^=0x01;
	GPIO_PORTF_DATA_R ^=0x04;	
}
void Execute_light(void){
	GPIO_PORTF_DATA_R ^=0x08;
}

void print_results(int result){				//A function to print result on the LCD from an integer
	int digit; unsigned char CG_digit;int result_array[20]; int size_counter; int zero=0;
	size_counter=0; 
		Clear_LCD();
	Set_second_line_display();			//Set the display address to the last line
	print_char(0x14);					// Print 'Ans='
	print_char(0xE6);
	print_char(0x37);
	print_char(0xD3);
	
	if(result==0){
		print_char(0x03);				//print 0 if result is 0
	}else{
		if(result<0){					//Add negative sign if the answer is negative
		result=-result;
		print_char(0xD2);
	}
	while (size_counter<20){				//Separate the interger into individual digits
		digit=result%10;
		result=result/10;
		result_array[size_counter]=digit;
		size_counter++;
	}
	for (int k= size_counter-1; k>=0; k--) {		//Print the individual digits out on the screen
		//Since the function above will reverse the order of digits, the loop has to start from the back
		
		if(zero==0){		//Check if first non zero integer  appeared yet
			if( result_array[k]!=0){
				CG_digit=int2_CG(result_array[k]);
				print_char(CG_digit);
				zero=1;				//toggle "zero" to let the code know that the zero afterwards is meangingful and print them
			}
		}
			else {
			CG_digit=int2_CG(result_array[k]);
			print_char(CG_digit);
			}	
		}
	}

}