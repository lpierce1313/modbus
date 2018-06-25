#pragma once



#include <stdio.h>
#include <time.h>
#include <winsock2.h>
#include <vector>
#include <iostream>

#pragma hdrstop


float get_F(unsigned char *p, int offset);
unsigned char Float2Byte(float fnum, int wb);
unsigned char LongInt2Byte(long int linum, int wb);
unsigned char Int2Byte(int inum, int wb);
int get_i32(unsigned char *p, int offset);
unsigned int get_I32(unsigned char *p, int offset);
short get_i16(unsigned char *p, int short offset);
unsigned short get_I16(unsigned char *p, int short offset);
int get_i16L(unsigned char *p, int offset);
int ModbusExceptionCode(int fc, char ec);
void expand(char *frm, char *frmt);
int generate(int argc, char* argv[]);
void send_arr(std::string arr[], int val, std::vector<std::string> &r);
char* string_char(std::string str);
