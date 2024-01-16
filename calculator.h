#ifndef CALCULATOR_H
  #define CALCULATOR_H
	
enum Input_types{
	undefined=0,
		number=1,
		operations=2,
		end_bit=3,
	negative=4
};


int check_input_type(unsigned char Input_key);
int Execute(unsigned char Ram[40], int Ram_type[40]);
int calculate_function(int num1, int num2, unsigned char operations_type);
unsigned char int2_CG(int result_int);
int expo(int number, int power);
int Store_number(unsigned char Ram[40], int num_size);

	#endif