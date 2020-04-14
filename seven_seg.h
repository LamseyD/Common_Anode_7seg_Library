#ifndef __seven_seg_H
#define __seven_seg_H
#include <stdint.h>
#include <ctype.h>
//typedef signed char             int8;
//typedef signed short int        int16;
//typedef signed long int         int32;
//
//typedef unsigned char           uint8;
//typedef unsigned short int      uint16;
//typedef unsigned long int       uint32;

#define HIGH 1
#define LOW 0

#define digit_0 0b00000001
#define digit_1 0b00000010
#define digit_2 0b00000100
#define digit_3 0b00001000

// DP G F E D C B A
#define num_0 0b11000000
#define num_1 0b11111001
#define num_2 0b10100100
#define num_3 0b10110000
#define num_4 0b10011001
#define num_5 0b10010010
#define num_6 0b10000010
#define num_7 0b11111000
#define num_8 0b10000000
#define num_9 0b10010000
#define let_C 0b11000110
#define let_F 0b10001110
#define let_P 0b10001100

void setup_pins(uint8_t rclk, uint8_t ser, uint8_t srclk);
void reset_pins(void);
void display_seg7(double);
void shift_out(uint8_t dataPin, uint8_t clockPin, uint8_t val);
void select_digit(int digit);
void write_data(int data, int dig);
void turn_off(void);
uint8_t seg7_converter(uint8_t);


#endif
