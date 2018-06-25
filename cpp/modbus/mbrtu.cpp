//---------------------------------------------------------------------------
#using <System.dll>

#include <stdio.h>
#include <time.h>
#include <winsock2.h>
#include <iostream>
#include <string>
#include <vector>
//#pragma hdrstop
using namespace System;
using namespace System::IO::Ports;
using namespace System::ComponentModel;
using namespace Microsoft::Win32;
//---------------------------------------------------------------------------

#include "ModbusRTU.h"
char buffer[50];

//#pragma argsused

#define TIMEOUT 3000

char* stringToChar(std::string str) {
	char *temp = new char[str.length() + 1];
	strcpy(temp, str.c_str());
	return temp;
}

int invert = 0; // if 1 use MSW, LSW order
std::vector<std::string> result;

//---------------------------------------------------------------------------
float get_F(unsigned char *p, int offset)
{
	union
	{
		float i;
		unsigned char  b[4];
	}u;
	if (!invert)
	{
		u.b[0] = *(p + 1 + offset);
		u.b[1] = *(p + 0 + offset);
		u.b[2] = *(p + 3 + offset);
		u.b[3] = *(p + 2 + offset);
	}
	else
	{
		u.b[0] = *(p + 3 + offset);
		u.b[1] = *(p + 2 + offset);
		u.b[2] = *(p + 1 + offset);
		u.b[3] = *(p + 0 + offset);
	}
	return u.i;
}
//---------------------------------------------------------------------------

unsigned char Float2Byte(float fnum, int wb)
{
	// fnum = the float number to convert
	// wb = which byte to return from the conversion
	union
	{
		float i;
		unsigned char b[4];
	}u;
	u.i = fnum;
	return u.b[wb];
}
//---------------------------------------------------------------------------

unsigned char LongInt2Byte(long int linum, int wb)
{
	// linum = the long int number to convert
	// wb = which byte to return from the conversion
	union
	{
		long int i;
		unsigned char b[4];
	}u;
	u.i = linum;
	return u.b[wb];
}
//---------------------------------------------------------------------------

unsigned char Int2Byte(int inum, int wb)
{
	// inum = the int value to convert
	// wb = which byte to return from the conversion
	union
	{
		int i;
		unsigned char b[1];
	}u;
	u.i = inum;
	return u.b[wb];
}
//---------------------------------------------------------------------------

int get_i32(unsigned char *p, int offset)
{
	union
	{
		int i;
		unsigned char  b[4];
	}u;
	if (!invert)
	{
		u.b[0] = *(p + 1 + offset);
		u.b[1] = *(p + 0 + offset);
		u.b[2] = *(p + 3 + offset);
		u.b[3] = *(p + 2 + offset);
	}
	else
	{
		u.b[0] = *(p + 3 + offset);
		u.b[1] = *(p + 2 + offset);
		u.b[2] = *(p + 1 + offset);
		u.b[3] = *(p + 0 + offset);
	}
	return u.i;
}
unsigned int get_I32(unsigned char *p, int offset)
{
	union
	{
		unsigned int i;
		unsigned char  b[4];
	}u;
	if (!invert)
	{
		u.b[0] = *(p + 1 + offset);
		u.b[1] = *(p + 0 + offset);
		u.b[2] = *(p + 3 + offset);
		u.b[3] = *(p + 2 + offset);
	}
	else
	{
		u.b[0] = *(p + 3 + offset);
		u.b[1] = *(p + 2 + offset);
		u.b[2] = *(p + 1 + offset);
		u.b[3] = *(p + 0 + offset);
	}
	return u.i;
}
//---------------------------------------------------------------------------

//Added short to int decl to make sure it is a 16bit int.
short get_i16(unsigned char *p, int short offset)
{
	union
	{
		int i;
		unsigned char  b[2];
	}u;
	u.b[0] = *(p + 1 + offset);
	u.b[1] = *(p + 0 + offset);
	return u.i;
}
unsigned short get_I16(unsigned char *p, int short offset)
{
	union
	{
		unsigned int i;
		unsigned char  b[2];
	}u;
	u.b[0] = *(p + 1 + offset);
	u.b[1] = *(p + 0 + offset);
	return u.i;
}
//---------------------------------------------------------------------------

int get_i16L(unsigned char *p, int offset)
{
	union
	{
		int i;
		unsigned char  b[2];
	}u;
	u.b[0] = *(p + 1 + offset);
	u.b[1] = *(p + 0 + offset);
	return u.i;
}
//---------------
int ModbusExceptionCode(int fc, char ec)
{
	switch (ec)
	{
	case 0x01:
		printf("Modbus Exception code(1)-function code not supported\n");
		sprintf(buffer, "Modbus Exception code(1)-function code not supported\n");
		result.push_back((buffer));
		break;
	case 0x02:
		printf("Modbus Exception code(2)-illegal start address\n");
		sprintf(buffer, "Modbus Exception code(2)-illegal start address\n");
		result.push_back((buffer));
		break;
	case 0x03:
		printf("Modbus Exception code(3)-illegal data value\n");
		sprintf(buffer, "Modbus Exception code(3)-illegal data value\n");
		result.push_back((buffer));
		break;
	case 0x04:
		printf("Modbus Exception code(4)-slave device failed\n");
		sprintf(buffer, "Modbus Exception code(4)-slave device failed\n");
		result.push_back((buffer));
		break;
	}
	return MODBUS_EXCEPTION;
}
//---------------
void expand(char *frm, char *frmt)
{  // dst, src
   //   memset(formstr,0,sizeof(formstr));
	int rep = 0;
	int pt = 0;
	char last;

	for (int j = 0;j<strlen(frmt);j++)
	{
		char ch = frmt[j];
		//if(isdigit(frmt[j])&& last!='a')
		if (isdigit(frmt[j]))
			while (isdigit(frmt[j]))
			{
				rep = rep * 10 + (frmt[j] - '0');
				j++;

			}
		else
		{
			frm[pt++] = frmt[j];
			if (!isdigit(frmt[j]))last = frmt[j];
			rep = 0;
		}
		if (rep)
		{
			for (int k = 1;k<rep;k++)
			{
				frm[pt++] = last;
			}
			rep = 0; j--;
		}

	}  // for
}
//---------------------------------------------------------------------------
//Main Starts here
//---------------------------------------------------------------------------

int generate(int argc1, char* argv2[])
{
	//for (int i = 0; i < argc1; i++) {
	//	std::cout << argv2[i] << std::endl;
	//}
	clock_t t1, t2, t3, Toffset = 0;
	// used by list com ports
	if (argc1 >= 2 && strnicmp(argv2[1], "-s", 2) == 0)
	{// list com ports

		array<String^>^ serialPorts = nullptr;
		try
		{
			// Get a list of serial port names.
			serialPorts = SerialPort::GetPortNames();
		}
		catch (Win32Exception^ ex)
		{
			Console::WriteLine(ex->Message);
		}

		Console::WriteLine("The following serial ports were found:");

		// Display each port name to the console.
		for each(String^ port in serialPorts)
		{
			Console::WriteLine(port);
		}
#ifdef JUNK
		printf("\n COM Ports:\n");
		for (int j = 0;j<str->Count;j++)
		{
			Str = str->Strings[j];
			//Str1 = Registry->ReadString(Str);
			printf("%s\t%s\n", Str.c_str() + 7, Str1.c_str());
		}
#endif

		//exit(0);
	}
	if (argc1<4)
	{
		/*
		printf(
		"Modbus RTU client 1.5\n"

		"Usage: \n"
		" mbrtu -h  Show this help\n"
		" mbrtu -s  Show available Com ports\n"
		" mbrtu [options] unit_addrs reg_no num_regs [format_string] [value(s)]\n"
		"Examples:\n"
		" mbrtu 1 10 2 Read 2 regs by fc03 from unit 1 start addrs 10\n"
		" mbrtu -f 17 1 (reg_no & num_regs not required for FC17)\n"
		" mbrtu 1 206 5 iili Read from unit 1 addrs 206 registers and format as short,short,long,short\n"
		" mbrtu -l 19 -y 0 -s 19200 -f 16 1 4000 f2 123.4 456.8\n"
		"       com19, no-parity, baud 19200, FC16, Unit 1, Adr 4000, format \"ff\"\n"
		"       write by fc16 two float values to addr 4000\n"
		" mbrtu -f 5 1 10 0xff  - write single coil to address 10\n"
		"Options:\n"
		" -l com port number [1]\n"
		" -y parity 0 - none,1 - odd, 2 - even [odd]\n"
		" -s speed [9600]\n"
		" -T time offset in ms for T format\n"
		" -I invert long values\n"

		" -f function code 1-FC01, 3-FC03, 4-FC04, 5-FC05, 17-FC17 6-FC06 16-FC16 default:FC3\n"
		"Format specifiers:\n"
		" a - ascii, b[B] - byte, i[I] - int16, l[L] - int32, f - float,\n"
		" h - hex (16), H - hex (32), t - time(ms),T - time since Windoz started(ms)\n,"
		" B,L,I - for unsigned\n"
		" format multipliers like i20a10 are OK\n"
		); */

		return 1;
	}

	//extracting data from the cmdline
	char Port[10] = ""; //default port name  -COM1
	int portnum = 1;
	int BaudRate = 9600;    //default speed      -9600
	int Parity = ODDPARITY;  //default parity     -odd

	int FunCode = 0x03; //default function code  -0x03
	int StartAddr = 0x00; //default address      -0x00
	int RegNum = 0x01;   //default  regnum       -0x01
	int DevAddr = 0x01;  //default device addrs  -0x01

	bool FormatFlag = false;

	char *CurrOpt = argv2[1];
	int ArgPtr = 1;

	while (true)
	{
		if (argc1 < (ArgPtr))
		{
			printf("wrong number of parameters");
			sprintf(buffer, "wrong number of parameters");
			result.push_back((buffer));
			return 1;
		}

		CurrOpt = argv2[ArgPtr];

		if (CurrOpt == NULL)
		{
			printf("wrong parameters"); // added to avoid error if to enter
			sprintf(buffer, "wrong parameters");
			result.push_back((buffer));
			return 2;
		}

		if (CurrOpt[0] != '-')
		{
			break;
		}
		else
		{
			switch (CurrOpt[1])
			{
			case 'l':
				portnum = atoi(argv2[++ArgPtr]);
				break;
			case 'T':
				Toffset = atoi(argv2[++ArgPtr]);
				break;
			case 'y':
			{
				switch (atoi(argv2[++ArgPtr]))
				{
				case 0:
					Parity = NOPARITY;
					break;
				case 1:
					Parity = ODDPARITY;
					break;
				case 2:
					Parity = EVENPARITY;
					break;
				default:
					printf("Invalid Option for -y");
					sprintf(buffer, "Invalid Option for -y");
					result.push_back((buffer));
					return 3;
				}
			}
			break;
			case 's':
				BaudRate = atoi(argv2[++ArgPtr]);
				break;
			case 'I':
				invert = 1;  // invert long values
				break;
			case 'm':  //to clarify 'm' option
				++ArgPtr;//BaudRate = atoi(argv2[++ArgPtr]);
						 //printf(" Adaptor(RS232\RS485) must be used for RS485\n");
				break;
			case 'f':
				FunCode = atoi(argv2[++ArgPtr]);
				break;
			default:
				printf("Unknown Option %s", CurrOpt);
				sprintf(buffer, "Unknown Option %s", CurrOpt);
				result.push_back((buffer));
				return 4;
			}
			++ArgPtr;
		}
	}

	sprintf(Port, "COM%d", portnum); //get the port number
	DevAddr = atoi(argv2[ArgPtr++]);  //get the dev addr

	if (FunCode != 17)
	{
		if ((ArgPtr + 2) <= argc1)
		{
			StartAddr = atoi(argv2[ArgPtr++]);
			if (FunCode != 6 && FunCode != 16 && FunCode != 5)
				RegNum = atoi(argv2[ArgPtr++]);
		}
		else
		{
			printf("Mandatory parameters missing");
			sprintf(buffer, "Mandatory parameters missing");
			result.push_back((buffer));
			return 5;
		}
	}

	if (ArgPtr < argc1)
		FormatFlag = true;
	//printf("reached here 1: %d %d",ArgPtr,argc1);  //testing
	//printf("reached here 1: %d %d %d",DevAddr,StartAddr,RegNum);//testing

	//sending data through the com port
	BYTE rlen = 0;
	BYTE rdata[300] = { 0 };
	int ErrCode;
	char * frmt = NULL;
	memset(rdata, 0, sizeof(rdata));
	ModbusRTU *m_CModbusRTU = new ModbusRTU(DevAddr);
	if (m_CModbusRTU->OpenComPort((LPCTSTR)Port, BaudRate, Parity, TIMEOUT) != INVALID_HANDLE_VALUE)
	{
		int k = 0, n = 0;
		char format[512];
		memset(format, 0, sizeof(format));
		if (FunCode == 6 || FunCode == 16)
		{
			bool wasFrmt = false;
			if (argv2[ArgPtr]) frmt = argv2[ArgPtr];
			if (frmt && isalpha(frmt[0]))
			{
				expand(format, frmt);
				wasFrmt = true;
				ArgPtr++;
			}

			int v = 0; // values
			float f;

			while (ArgPtr < argc1)
			{
				if (format[k])
				{// process formatted
					switch (format[k])
					{
					case 'i':
					case 'I':
					default:
						sscanf(argv2[ArgPtr++], "%i", &v);
						rdata[n * 2] = (v >> 8) & 0xff;
						rdata[n * 2 + 1] = (v) & 0xff;
						n++;

						break;
					case 'l':
					case 'L':
						sscanf(argv2[ArgPtr++], "%i", &v);
						if (invert)
						{ // msw first
							rdata[n * 2] = (v >> 24) & 0xff;
							rdata[n * 2 + 1] = (v >> 16) & 0xff;
							n++;
							rdata[n * 2] = (v >> 8) & 0xff;
							rdata[n * 2 + 1] = (v) & 0xff;
							n++;
						}
						else
						{ // lsw first
							rdata[n * 2] = (v >> 8) & 0xff;
							rdata[n * 2 + 1] = (v) & 0xff;
							n++;
							rdata[n * 2] = (v >> 24) & 0xff;
							rdata[n * 2 + 1] = (v >> 16) & 0xff;
							n++;
						}
						break;
					case 'f':
					case 'F':
						sscanf(argv2[ArgPtr++], "%f", &f);
						if (invert) // msw first
						{
							rdata[n * 2] = Float2Byte(f, 3);
							rdata[n * 2 + 1] = Float2Byte(f, 2);
							n++;
							rdata[n * 2] = Float2Byte(f, 1);
							rdata[n * 2 + 1] = Float2Byte(f, 0);
							n++;
						}
						else
						{
							rdata[n * 2] = Float2Byte(f, 1);
							rdata[n * 2 + 1] = Float2Byte(f, 0);
							n++;
							rdata[n * 2] = Float2Byte(f, 3);
							rdata[n * 2 + 1] = Float2Byte(f, 2);
							n++;
						}
						break;


					}
					k++;
				}
				else   // number of formatters can be less that params
				{ // default

					sscanf(argv2[ArgPtr++], "%i", &v);
					rdata[n * 2] = (v >> 8) & 0xff;
					rdata[n * 2 + 1] = (v) & 0xff;
					n++;

				}
			}

		}
		else if (FunCode == 5) sscanf(argv2[ArgPtr++], "%i", &RegNum); // it is not reg, it is value for fc5
		if (FunCode == 16) RegNum = n;
		//t1 = timeGetTime();
		ErrCode = m_CModbusRTU->RecieveSerialData(FunCode, StartAddr, RegNum, &rlen, rdata);
		//t2 = timeGetTime();
		m_CModbusRTU->CloseComPort();
		delete m_CModbusRTU;
	}
	else
	{
		BYTE comName[50];
		printf("com -%s \n", Port);
		DWORD result1 = QueryDosDevice((LPCSTR)Port, (LPTSTR)&comName[0], sizeof(comName));
		if (!result1) {
			printf("com error -%s port does not exist\n", Port);
			sprintf(buffer, "com error -%s port does not exist\n", Port);
			result.push_back(buffer);
		}
		else {
			printf("com error -%s port busy\n", Port);
			sprintf(buffer, "com error -%s port busy\n", Port);
			result.push_back(buffer);
		}

		delete m_CModbusRTU;
		return 6;
	}


	//printf("reached here main(): %d\n",rlen);  //testing

	//displaying the data received
	if (ErrCode == MODBUS_ERR_CRC)
	{
		printf("CRC Error Reading Data\n");
		sprintf(buffer, "CRC Error Reading Data\n");
		result.push_back(buffer);
	}
	else if (ErrCode == MODBUS_ERR_TIMEOUT)
	{
		printf("Timed Out -No response from the slave device\n");
		sprintf(buffer, "Timed Out -No response from the slave device\n");
		result.push_back(buffer);
	}
	else if (ErrCode == MODBUS_EXCEPTION)
	{
		printf("Modbus Exception Occured\n");
		ModbusExceptionCode(FunCode, rdata[2]);
	}
	else if (FunCode != 6 && FunCode != 16 && FunCode != 5) // was else before
	{
		char formstr[256] = "";
		char *frm = formstr;
		char * frmt=argv2[ArgPtr++];

		rlen = rlen - 3; //removing devid,funcode,bytecount

		if(!frmt)
		//if (1)
		{
			unsigned short w;
			for (int i = 0;i<rlen;)
			{
				if (FunCode == 0x03)   //fc3
				{
					w = (rdata[3 + i] << 8) + rdata[4 + i];
					printf(" %d,", w);
					//std::cout << "function code is 3" << std::endl;
					result.push_back(std::to_string(w));
					i = i + 2;
				}
				else if (FunCode == 0x01)  //fc1
				{
					w = (rdata[3 + i]);
					printf(" %d,", w);
					//std::cout << "function code is 1" << std::endl;
					result.push_back(std::to_string(w));

					i++;
				}
				else if (FunCode == 0x11) //fc17
				{
					w = (rdata[3 + i]);
					printf(" %d,", w);
					//std::cout << "function code is 11" << std::endl;
					result.push_back(std::to_string(w));
					i++;
				}
				else
					i++;  // GP to avoid loop
			}
		}
		else
		{
			//std::cout << "else executed instead" << std::endl;
		// have additional parameter in argv like: ilbfa - how to show data
		//char * fm=argv[argPtr++];   // atoi(argv[argPtr++]);
		memset(formstr,0,sizeof(formstr));
		int rep = 0;
		int pt=0;
		char last;
		for(int j=0;j<strlen(frmt);j++)
		{
		char ch = frmt[j];
		//if(isdigit(frmt[j])&& last!='a')
		if(isdigit(frmt[j]))
		while(isdigit(frmt[j]))
		{
		rep=rep*10+(frmt[j]-'0');
		j++;

		}
		else
		{
		frm[pt++] = frmt[j];
		if(!isdigit(frmt[j]))last =  frmt[j];
		rep = 0;
		}
		if(rep)
		{
		for(int k=1;k<rep;k++)
		{
		frm[pt++] = last;
		}
		rep = 0; j--;
		}

		}// for

		int i=0;
		while(*frm)
		{
		//printf("\n reg=%i  ",i/2+reg_no);
		switch(*frm)
		{
		case 'i':

		//printf(" %i",get_i16(rdata,3+i));
	  //std::cout << "i executed" << get_i16(rdata, 3 + i) << std::endl;
		result.push_back(std::to_string(get_i16(rdata, 3 + i)));
		i+=2;
		break;
		case 'I':

		//printf(" %u",get_I16(rdata,3+i));
		//std::cout << "I executed" << get_I16(rdata, 3 + i) << std::endl;
		result.push_back(std::to_string(get_I16(rdata, 3 + i)));
		i+=2;
		break;
		case 'f':

		//printf(" %f",get_F(rdata,3+i));
		//std::cout << "f executed" << get_F(rdata, 3 + i) << std::endl;
		result.push_back(std::to_string(get_F(rdata, 3 + i)));
		i+=4;
		break;
		case 'l':

		//printf(" %i",get_i32(rdata,3+i));
		//std::cout << "l executed" << get_i32(rdata, 3 + i) << std::endl;
		result.push_back(std::to_string(get_i32(rdata, 3 + i)));
		i+=4;
		break;

		case 'L':
		//printf(" %u",get_I32(rdata,3+i));
		//std::cout << "L executed" << get_I32(rdata, 3 + i) << std::endl;
		result.push_back(std::to_string(get_I32(rdata, 3 + i)));
		i+=4;
		break;
		case 'b':
		//printf(" %i",rdata[3+i]);
		//std::cout << "b executed" << rdata[3 + i] << std::endl;
		result.push_back(std::to_string(rdata[3 + i]));
		i+=1;
		break;

		case 'B':
		//printf(" %u",rdata[3+i]);
		//std::cout << "B executed" << rdata[3 + i] << std::endl;
		result.push_back(std::to_string(rdata[3 + i]));
		i+=1;
		break;

		case 'a':
		if (rdata[3 + i] < 32 || rdata[3 + i]>126) {
		//	printf(" 0x%02X", rdata[3 + i]);  // do not print non-printable
		//	std::cout << "a executed" << rdata[3 + i] << std::endl;
			result.push_back(std::to_string(rdata[3 + i]));
		}
		else
		{
		//	printf(" %c", rdata[3 + i]);
		//	result.push_back(std::to_string(rdata[3 + i]));
			i += 1;
		}
		break;

		case 'h':
	//	printf("0x%04X",get_I16(rdata,3+i));
	//	std::cout << "h executed" << get_I16(rdata, 3 + i) << std::endl;
		char buffer1[50];
		sprintf(buffer1, "0x%04X", get_I16(rdata, 3 + i));
		result.push_back((buffer1));
		i+=2;
		break;
		case 'H':
	//	printf(" 0x%08X",get_I32(rdata,3+i));
//		std::cout << "H executed" << get_I32(rdata, 3 + i) << std::endl;
		char buffer2[50];
		sprintf(buffer2, " 0x%08X", get_I32(rdata, 3 + i));
		result.push_back((buffer2));

		i+=4;
		break;
		case 't':
	//	printf(" %ld",(t2-t1));
//		std::cout << "t executed" << t2 - t1 << std::endl;
		result.push_back(std::to_string(t2-t1));
		break;
		case 'T':
		std::cout << "T executed" << std::endl;
		//                       printf(" %ld",timeGetTime()-Toffset);
		break;
		default:
		printf(" unknown format %c\n",*frm);
		sprintf(buffer, " unknown format %c\n", *frm);
		result.push_back(buffer);
		return 1;
		}

		if(*(++frm))printf(",");
		}

		}
		//printf("\n");
	}
	else printf(" OK"); // get OK to fc 5,6 and 16
	return 0;
}
//---------------------------------------------------------------------------

//Change datatype from string to char*
char* string_char(std::string str) {
	char *temp = new char[str.length() + 1];
	strcpy(temp, str.c_str());
	return temp;
}

//Setup variables like argv and argc duplicates and call generate
void send_arr(std::string arr[], int val, std::vector<std::string> &r) {

	arr[7] = arr[7] + arr[6];

	char** argv2;
	argv2 = new char*[val + 1];
	argv2[0] = string_char("");
	argv2[1] = string_char("-l");
	argv2[2] = string_char(arr[0]);
	argv2[3] = string_char("-y");
	argv2[4] = string_char(arr[1]);
	argv2[5] = string_char("-s");
	argv2[6] = string_char(arr[2]);
	argv2[7] = string_char("-f");
	argv2[8] = string_char(arr[3]);
	argv2[9] = string_char(arr[4]);
	argv2[10] = string_char(arr[5]);
	argv2[11] = string_char(arr[6]);
	argv2[12] = string_char(arr[7]);

	//Call generate function
	generate(val, argv2);

	r.assign(result.begin(), result.end());
	result.clear();
}
