
#include "serial.h"



//initialize the object to default settings
CSerial::CSerial()
{
	m_hComHandle =  INVALID_HANDLE_VALUE;
}

//close the com port once the object is destoryed
CSerial::~CSerial()
{
	m_hComHandle = NULL;
}

//open the com port
HANDLE CSerial::OpenComPort(LPCTSTR lpPort, DWORD dwBaud, BYTE bParity, DWORD dwTimeout)
{
	COMMTIMEOUTS    timeouts;
	DCB				dcb;
    char comport[20] = "\\\\.\\";    //to access com ports no higher than com9

    strcat(comport,(const char*)lpPort);

	m_hComHandle = CreateFile((LPCSTR)comport,
		      GENERIC_READ | GENERIC_WRITE,
		      0,
		      0,
		      OPEN_EXISTING,
		      0,
		      0);

	if (m_hComHandle != INVALID_HANDLE_VALUE)
    {
		FillMemory(&dcb, sizeof(dcb), 0);
		dcb.DCBlength = sizeof(dcb);

	    // Allow BuildCommDCB() to set defaults.
		if (!BuildCommDCB((LPCSTR)"9600,o,8,1", &dcb))
        {
			CloseHandle(m_hComHandle);
            m_hComHandle = INVALID_HANDLE_VALUE;
	    }
        else
        {
	        //  Set the values that were passed in for baud and parity.
		    dcb.BaudRate = dwBaud;
		    dcb.Parity = bParity;
	        dcb.fNull=false;

	        dcb.fOutX=false;
	        dcb.fInX=false;
	        dcb.ErrorChar=false;


		    if (!SetCommState(m_hComHandle, &dcb))
            {
			    CloseHandle(m_hComHandle);
			    m_hComHandle = INVALID_HANDLE_VALUE;
			}
		    else
            {
		        // Set comm port timeout values.
				timeouts.ReadIntervalTimeout = 0;
				timeouts.ReadTotalTimeoutMultiplier = 5;
			    timeouts.ReadTotalTimeoutConstant = dwTimeout;
				timeouts.WriteTotalTimeoutMultiplier = 0;
				timeouts.WriteTotalTimeoutConstant = 0;

        		// Set Buffer size */
	            SetupComm ( m_hComHandle, 1000, 1000 );

			    if (!SetCommTimeouts(m_hComHandle, &timeouts))
                {
				    CloseHandle(m_hComHandle);
					m_hComHandle = INVALID_HANDLE_VALUE;
				}
			}

		}
	}

	return m_hComHandle;
}


//close the com port
bool CSerial::CloseComPort()
{
	if(m_hComHandle != INVALID_HANDLE_VALUE)
	{
		if(CloseHandle(m_hComHandle) != 0)
        {
            return true;;
        }
        else
			return false;   
    }
    else
	{
		return false;
	}
}

//writing to the com port
int CSerial::ComWrite(unsigned char *data,int nBytes)
{
    unsigned long nWritten;

	if(WriteFile(m_hComHandle,data,nBytes,&nWritten,NULL))
    {
        if(nWritten < 1)
			return nWritten;
        else
			return nWritten;
    }
    else
    {
		return -1;
	}

}

//reading from the com port
int CSerial::ComRead(unsigned char *response,int toRead)
{
	unsigned long nRead = 0;

	if(ReadFile(m_hComHandle,response,toRead,&nRead,NULL))
    {
        if(nRead < 1)
			return nRead;
        else
			return nRead;
	}
    else
    {
		return -1;      
    }
}

//flushing the com port
void CSerial::Flush()
{
	PurgeComm(m_hComHandle, PURGE_RXCLEAR|PURGE_TXCLEAR);
}

bool CSerial::Close()
{
	PurgeComm(m_hComHandle, PURGE_RXABORT|PURGE_TXABORT);

	return CloseHandle(m_hComHandle);
}

