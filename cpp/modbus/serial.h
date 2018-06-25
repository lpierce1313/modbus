

#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <windows.h>
#include <stdio.h>

// Serial communication class
class CSerial
{
 private:
	HANDLE m_hComHandle;

public:
	//functions
	CSerial();
	~CSerial();

	HANDLE OpenComPort(LPCTSTR lpPort, DWORD dwBaud, BYTE bParity, DWORD dwTimeout);
    bool CloseComPort();
   	bool Close();

    int ComWrite(unsigned char*,int);
	int ComRead(unsigned char*,int);
	void Flush(void);


};


#endif

 