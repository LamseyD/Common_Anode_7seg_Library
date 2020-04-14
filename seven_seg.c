#define _SVID_SOURCE

#include <stdio.h>
#include "LPC802.h"
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <stdint.h>
#include <seven_seg.h>

static int dp_pos = 0;
static int digit[4] = {-1,-1,-1,-1};

static uint8_t latch_pin_rclk = 0;
static uint8_t data_pin = 0;
static uint8_t clock_pin_srclk = 0;



static void pin_write(uint8_t dataPin, uint8_t val);
static void delay(uint8_t);
static void find_dp(double);
static void find_digits(double);

void setup_pins(uint8_t rclk, uint8_t ser, uint8_t srclk){
	latch_pin_rclk = rclk;
	data_pin = ser;
	clock_pin_srclk = srclk;
}

void reset_pins(void){
	latch_pin_rclk = 0;
	data_pin = 0;
	clock_pin_srclk = 0;
}

void write_data(int data,int dig){
	pin_write(latch_pin_rclk, LOW);
	shift_out(data_pin, clock_pin_srclk, dig);
	shift_out(data_pin, clock_pin_srclk, seg7_converter(data));
	delay(10);
	pin_write(latch_pin_rclk, HIGH);
	delay(10);
}

void reset_display(void){
	pin_write(latch_pin_rclk, LOW);
	shift_out(data_pin, clock_pin_srclk, 0b11110000);
	shift_out(data_pin, clock_pin_srclk, 0b11111111);
	delay(10);
	pin_write(latch_pin_rclk, HIGH);
	delay(10);
}

void shift_out(uint8_t ser_pin, uint8_t clock_pin, uint8_t val){
    for (int i = 0; i < 8; i++)  {
    	pin_write(ser_pin, !!(val & (1 << (7 - i))));
   	 	pin_write(clock_pin, LOW);
        delay(10);
        pin_write(clock_pin, HIGH);
        delay(10);
    }
}

void display_seg7(double input){
	find_dp(input);
	//write digit_4 --need to change to c and ph and F - finite state machine here
	pin_write(latch_pin_rclk, LOW);
	shift_out(data_pin, clock_pin_srclk, digit_3);
	//DP G F E D C B A
	//C -> F -> P
	if (dp_pos == 3){
		shift_out(data_pin, clock_pin_srclk, seg7_converter(digit[3] + 1) ^ (1 << 7));
	} else {
		shift_out(data_pin, clock_pin_srclk, seg7_converter(digit[3] + 1));
	}

	delay(10);
	pin_write(latch_pin_rclk, HIGH);
	delay(10);

	//write digit_3
	pin_write(latch_pin_rclk, LOW);
	shift_out(data_pin, clock_pin_srclk, digit_2);
	//    shift_out(data_pin, clock_pin_srclk, num_3);
	if (dp_pos == 2){
		shift_out(data_pin, clock_pin_srclk, seg7_converter(digit[2]) ^ (1 << 7));
	} else {
		shift_out(data_pin, clock_pin_srclk, seg7_converter(digit[2]));
	}
	delay(10);
	pin_write(latch_pin_rclk, HIGH);
	delay(10);

	//write digit_2
	pin_write(latch_pin_rclk, LOW);
	shift_out(data_pin, clock_pin_srclk, digit_1);
	//DP G F E D C B A
	//    shift_out(data_pin, clock_pin_srclk, num_2);
	if (dp_pos == 1){
	  	shift_out(data_pin, clock_pin_srclk, seg7_converter(digit[1]) ^ (1 << 7));
	} else {
	   	shift_out(data_pin, clock_pin_srclk, seg7_converter(digit[1]));
	}
	delay(10);
	pin_write(latch_pin_rclk, HIGH);
	delay(10);

	//write digit_1
	pin_write(latch_pin_rclk, LOW);
	shift_out(data_pin, clock_pin_srclk, digit_0);
	//DP G F E D C B A
	//   shift_out(data_pin, clock_pin_srclk, num_1);
	if (dp_pos == 0){
	   	shift_out(data_pin, clock_pin_srclk, seg7_converter(digit[0]) ^ (1 << 7));
	} else {
		shift_out(data_pin, clock_pin_srclk, seg7_converter(digit[0]));
	}
	delay(10);
	pin_write(latch_pin_rclk, HIGH);
	delay(10);
}

static void pin_write(uint8_t pin, uint8_t val){
	if(val == 0){
		GPIO -> CLR[0] = (1UL << pin);
	} else {
		GPIO -> SET[0] = (1UL << pin);
	}
}

uint8_t seg7_converter(uint8_t data){
	switch (data){
		case 1:
			return 0b11111001;
		case 2:
			return 0b10100100;
		case 3:
			return 0b10110000;
		case 4:
			return 0b10011001;
		case 5:
			return 0b10010010;
		case 6:
			return 0b10000010;
		case 7:
			return 0b11111000;
		case 8:
			return 0b10000000;
		case 9:
			return 0b10010000;
		case 0:
			return 0b11000000;
		default:
			break;
	}
	return 0b00000110;
}

static void delay(uint8_t count){
	for (int i = 0; i < count; i++){
		//empty delay -- blocking delay
	}
}


static void find_dp(double input){
	while (input > 10000){
		input = input / 10;
	}

	if (input / 1000 >= 1){
		dp_pos = 3;
		find_digits(input / 100);
	} else if (input / 100 >= 1){
		dp_pos = 2;
		find_digits(input / 10);
	} else if (input / 10 >= 1){
		dp_pos = 1;
		find_digits(input);
	} else {
		dp_pos = 0;
		find_digits(input);
	}
}

//static int get_empty_digit(){
//    for(int i=0;i<5;i++){
//        if(digit[i] == -1){
//            return i;
//        }
//    }
//    return -1;
//}

static void find_digits(double temp){
	temp = temp / 10;
	digit[0] = temp;

	temp = (temp - digit[0]) * 10;
	digit[1] = temp;

	temp = (temp - digit[1]) * 10;
	digit[2] = temp;

	temp = (temp - digit[2]) * 10;
	digit[3] = temp;

}

