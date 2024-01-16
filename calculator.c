#include "calculator.h"

// array for switching between integer and its character generation RAM code
int numbers_int[10]={0,1,2,3,4,5,6,7,8,9};
unsigned char numbers_cg[10]={0x03,0x13,0x23,0x33,0x43,0x53,0x63,0x73,0x83,0x93};


int expo(int number, int power){    //An exponential function
	int result;
	result=1;
	if(power==0){
		return 1;
	} else {
	for(int i=0;i<power;i++){
		result=result*number;
	}
}
	return result;
}

unsigned char int2_CG(int result_int){     //Function to change from integer value to the its character generation RAM value
	return numbers_cg[result_int];
}


int check_input_type(unsigned char Input_key){			//check_input_type whether it is number or not
	if((Input_key&0x0F)==0x03){
		return number;
	}	
	else {return operations;
	}
}


int CG_to_int(unsigned char input_cg){			//Function to change from character generation RAM value to the its integer value
	input_cg&=0xF0;
	input_cg=input_cg>>4;
	return numbers_int[input_cg];
}
		

int Execute(unsigned char Ram[40], int Ram_type[40]){					//Functions to execute the calculations
	int num_size; int num1,num2;int value;		int int_input;
	unsigned char operation_type; int result;
	int first_num_flag; 
	unsigned char num_array1[40], num_array2[40];
	num1=0; num2=0;
	value=0;
	num_size=0;
	for(int i=0; i<40; i++){						//Use a for loop to check each value in the memory
		if (Ram_type[i]==operations){				
			operation_type=Ram[i];				//Set the type of operation depending on the input CG code
			first_num_flag=i;						//Set the position of the operator
			if((i==0)&(Ram[i]==0xD2)){				//Check if the negative sign in the input memory is the negative sign for the first number
				//If yes, skip the break statement and countinue to store numbers
		} else{
			break;					//If not break the loop and proceed
			}
		}
		else if (Ram_type[i]==number){					//If input is an number 
			num_array1[num_size]=Ram[i];			//Store the CG code into an array
			num_size++;						//num1 size reprsent the number of digits that user entered
	  }
	}
	num1=Store_number(num_array1,num_size);				//Change the stored CG code of number into an integer value
	if(Ram[0]==0xD2){				//Toggle the number to negative if there is a minus sign in front of the number
		num1=-num1;
	}
		num_size=0;			
		for (int i=0;i<40; i++){
			num_array1[i]=0x00;
		}
	for(int i=first_num_flag+1; i<40; i++){				//check the ibput key starting from the position stopped at the last for loop
			if (Ram_type[i]==number){						//Store the CG code of numbers entered into an array if the input is a number
			num_array2[num_size]=Ram[i];
			num_size++;
			} else{break;}
		}
	if(Ram[first_num_flag]==0xD2){					//If there is a minus sign in front of the second number, it is a subtraction
		operation_type=0xD2;
	}
		num2=Store_number(num_array2,num_size);   //Change the stored CG code of the second number into an integer value
		result=calculate_function(num1,num2, operation_type);
			return result;
	}
		
int Store_number(unsigned char Ram[40],int num_size){					//A function to turn an array of individual digits to an integer value
	 int value;	
	value=0;
		for(int i=0; i<num_size; i++){
				int temp;
				temp=CG_to_int(Ram[i])*expo(10,num_size-i-1);				
				value=value+temp;
			}
			return value;
}


int calculate_function(int num1, int num2, unsigned char operations_type){   //A function do the calcalation depends on the operations
		switch (operations_type){
			case 0xB2:
				return (num1+num2);
			break;
			case 0xD2:
				return (num1-num2);
			break;
			case 0x7B:
				return (num1*num2);
			break;
			case 0x8B:
				return (num1/num2);
			break;
			default:
				return num1;
			break;
		}
	}

