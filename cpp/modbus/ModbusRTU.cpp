//---------------------------------------------------------------------------

//#pragma hdrstop

#include "ModbusRTU.h"
//---------------------------------------------------------------------------
//#pragma package(smart_init)
//---------------------------------------------------------------------------

ModbusRTU::ModbusRTU()
{

}

ModbusRTU::ModbusRTU(BYTE addr)
{
    m_bAEBusAddr = addr;
}

ModbusRTU::~ModbusRTU()
{

}

int ModbusRTU::SendSerialData(BYTE bCommand, WORD wStartAddr,WORD wQuantity,BYTE bWriteLen, LPBYTE lpData)
{
    int i, ndata;
    int err = 0;
    DWORD bytes_written, bytes_read;
    BYTE acknowlege;
    BYTE bDevAddr;

    BYTE data[255 + 4];

    //bDevAddr = (BYTE)(m_bAEBusAddr);  //dev id


    if(bCommand == 0x05)    //set single coil
        ;
    else if(bCommand == 0x06)    //set single register
        ;
    else if(bCommand == 0x0F)    //set multiple coil
        ;
    else if(bCommand == 0x10)    //set multiple register
        ;
    else
        ;//function code not implemented

    return err;
}

int ModbusRTU::RecieveSerialData(BYTE bCommand, WORD wStartAddr,WORD wQuantity, LPBYTE lpRecdLength, LPBYTE lpData)
{
    int err;
    BYTE lpLen;
    BYTE bDevAddr;

    bDevAddr = (BYTE)(m_bAEBusAddr);  //dev id

    if(bCommand == 0x01)
    {
        err = SendFunc01(bDevAddr,wStartAddr,wQuantity);//
        err = RecvFunc01(lpData,&lpLen,wQuantity);
    }
    else if(bCommand == 0x03 || bCommand == 0x04 )
    {
        err = SendFunc03(bCommand, bDevAddr,wStartAddr,wQuantity);//
        err = RecvFunc03(lpData,&lpLen,wQuantity);
    }
    else if(bCommand == 0x11)
    {
        err = SendFunc17(bDevAddr);//
        err = RecvFunc17(lpData,&lpLen);
    }
    else if(bCommand == 6 || bCommand == 16 )
    {
         err = SendFunc06(bDevAddr,wStartAddr,wQuantity,lpData,bCommand);//
         err = RecvFunc06(lpData,&lpLen);
    }
     else if( bCommand == 5)
    {
         err = SendFunc05(bDevAddr,wStartAddr,wQuantity,lpData,bCommand);//
         err = RecvFunc05(lpData,&lpLen);
    }
    else
    {    ;}

    if(err == MODBUS_OK)
    {
        *lpRecdLength = lpLen - 2;

    }
    else
    {
        if(err == MODBUS_EXCEPTION)
        {
            *lpRecdLength = 3;
        }
    }
    return err;
}

int ModbusRTU::SendFunc01(BYTE bDevAddr,WORD wStartAddr,WORD wQuantity)
{
    BYTE data[255 + 4];
    WORD CRC = 0;

    data[0] = bDevAddr;  //dev id
    data[1] = 0x01;      //func code
    data[2] = (BYTE)(wStartAddr >> 8);
    data[3] = wStartAddr;
    data[4] = (BYTE)(wQuantity >> 8);
    data[5] = wQuantity;

    CRC = CRC16(data,6);
    data[6] = CRC;
    data[7] = CRC >> 8;

    return (ComWrite(data,8));
}
int ModbusRTU::SendFunc03(BYTE fc, BYTE bDevAddr,WORD wStartAddr,WORD wQuantity)
{
    BYTE data[255 + 4];
    WORD CRC = 0;

    data[0] = bDevAddr;  //dev id
    data[1] = fc;      //func code
    data[2] = (BYTE)(wStartAddr >> 8);
    data[3] = wStartAddr;
    data[4] = (BYTE)(wQuantity >> 8);
    data[5] = wQuantity;

    CRC = CRC16(data,6);
    data[6] = CRC;
    data[7] = CRC >> 8;

    return (ComWrite(data,8));
}
int ModbusRTU::SendFunc05(BYTE bDevAddr,WORD wStartAddr,WORD wOn,LPBYTE lpData,BYTE bCommand)
{
    BYTE data[255 + 10];
    WORD CRC = 0;

    data[0] = bDevAddr;  //dev id
    data[1] = bCommand;      //func code
    data[2] = (BYTE)(wStartAddr >> 8);
    data[3] = wStartAddr;
    data[4] = (BYTE)(wOn >> 8);
    data[5] = wOn&0xff;   // always 1

    CRC = CRC16(data,6);
    data[6] = CRC;
    data[7] = CRC >> 8;
    return (ComWrite(data,8));

}
int ModbusRTU::SendFunc06(BYTE bDevAddr,WORD wStartAddr,WORD wQuantity,LPBYTE lpData,BYTE bCommand)
{
    BYTE data[255 + 10];
    WORD CRC = 0;

    data[0] = bDevAddr;  //dev id
    data[1] = bCommand;      //func code
    data[2] = (BYTE)(wStartAddr >> 8);
    data[3] = wStartAddr;
    data[4] = (BYTE)(wQuantity >> 8);
    data[5] = wQuantity;   // always 1
    if(bCommand == 6)
    {

    data[4] = lpData[0];  // no lenngth in cmd 6
    data[5] = lpData[1];
    CRC = CRC16(data,6);
    data[6] = CRC;
    data[7] = CRC >> 8;
    return (ComWrite(data,8));
    } else
    {// fc 16
     data[6] = wQuantity*2; // byte count
     for(int j=0;j<wQuantity;j++)
     {
       data[7+j*2] = lpData[j*2];
       data[7+j*2 +1] = lpData[j*2+1];
     }
      CRC = CRC16(data,7 + wQuantity*2);
      data[7 +wQuantity*2] = CRC;
      data[8 +wQuantity*2] = CRC >> 8;
    }

    return (ComWrite(data,wQuantity*2 + 9));
}
int ModbusRTU::RecvFunc01(LPBYTE lpData,LPBYTE lpLen,WORD wQuantity)
{
    int err;
    int dLen = 3;//5 + (wQuantity/8) + ((wQuantity%8 == 0)?0:1);
    WORD NewCRC = 0;
    WORD OldCRC = 0;

    err = ComRead(lpData,dLen);

    if(err == 0)
    {
        return MODBUS_ERR_TIMEOUT;
    }

    if((lpData[1] & 0x80) == 0x80)
    {
        err = ComRead(&lpData[3],2); //since there will be no byte count
        dLen += 2;
    }
    else
    {
        dLen = lpData[2] + 2;
        err = ComRead(&lpData[3],dLen);
        dLen += 3;
    }

    OldCRC = lpData[dLen-1];
    OldCRC = (OldCRC<<8) | lpData[dLen-2];

    NewCRC = CRC16(lpData,dLen - 2);

    if(NewCRC == OldCRC)
    {
        if((lpData[1] & 0x80) == 0x80)
            err = MODBUS_EXCEPTION;    //send exception code
        else
        {
            err = MODBUS_OK;    //
            *lpLen = (BYTE)dLen;
        }
    }
    else
        err = MODBUS_ERR_CRC;//crc error code

    return err;
}

int ModbusRTU::RecvFunc03(LPBYTE lpData,LPBYTE lpLen,WORD wQuantity)
{
    int err;
    int dLen = 3;//5 + (wQuantity*2);
    WORD NewCRC = 0;
    WORD OldCRC = 0;

    err = ComRead(lpData,dLen);

    if(err == 0)
    {
        return MODBUS_ERR_TIMEOUT;
    }

    if((lpData[1] & 0x80) == 0x80)
    {
        err = ComRead(&lpData[3],2); //since there will be no byte count
        dLen += 2;
    }
    else
    {
        dLen = lpData[2] + 2;
        err = ComRead(&lpData[3],dLen);
        dLen += 3;
    }

    OldCRC = lpData[dLen-1];
    OldCRC = (OldCRC<<8) | lpData[dLen-2];

    NewCRC = CRC16(lpData,dLen - 2);

    if(NewCRC == OldCRC)
    {
        if((lpData[1] & 0x80) == 0x80)
        {
            err = MODBUS_EXCEPTION;    //send exception code
        }
        else
        {
            *lpLen = (BYTE)dLen;
            err = MODBUS_OK;    //
        }
    }
    else
    {
        err = MODBUS_ERR_CRC;//crc error code
    }

    return err;
}

int ModbusRTU::SendFunc17(BYTE bDevAddr)
{
    BYTE data[255 + 4];
    WORD CRC = 0;

    data[0] = bDevAddr;  //dev id
    data[1] = 0x11;      //func code
    CRC = CRC16(data,2);
    data[2] = CRC;
    data[3] = CRC >> 8;

    return (ComWrite(data,4));
}

int ModbusRTU::RecvFunc17(LPBYTE lpData,LPBYTE lpLen)
{
    int err;
    int dLen = 3;  //to get the byte count
    WORD NewCRC = 0;
    WORD OldCRC = 0;

    err = ComRead(lpData,dLen);     //extract first 3 bytes

    if(err == 0)
    {
        return MODBUS_ERR_TIMEOUT;
    }

    if((lpData[1] & 0x80) == 0x80)
    {
        err = ComRead(&lpData[3],2); //since there will be no byte count
        dLen += 2;
    }
    else
    {
        dLen = lpData[2] + 2;
        err = ComRead(&lpData[3],dLen);
        dLen += 3;
    }

    //crc verification starts
    OldCRC = lpData[dLen-1];
    OldCRC = (OldCRC<<8) | lpData[dLen-2];

    NewCRC = CRC16(lpData,dLen - 2);

    if(NewCRC == OldCRC)
    {
        if((lpData[1] & 0x80) == 0x80)
            err = MODBUS_EXCEPTION;    //send exception code
        else
        {
            err = MODBUS_OK;    //
            *lpLen = (BYTE)dLen;
        }
    }
    else
        err = MODBUS_ERR_CRC;//crc error code

    return err;
}
int ModbusRTU::RecvFunc06(LPBYTE lpData,LPBYTE lpLen)
{
    int err;
    int dLen = 3;  //to get the byte count
    WORD NewCRC = 0;
    WORD OldCRC = 0;

    err = ComRead(lpData,dLen);     //extract first 3 bytes

    if(err == 0)
    {
        return MODBUS_ERR_TIMEOUT;
    }

    if((lpData[1] & 0x80) == 0x80)
    {
       // err = ComRead(&lpData[3],2); //since there will be no byte count
       // dLen += 2;
       return MODBUS_EXCEPTION;
    }
    else
    {
       // dLen = lpData[2] + 2;
        dLen =  5;
        err = ComRead(&lpData[3],dLen);
        dLen += 3;
    }

    //crc verification starts
    OldCRC = lpData[dLen-1];
    OldCRC = (OldCRC<<8) | lpData[dLen-2];

    NewCRC = CRC16(lpData,dLen - 2);

    if(NewCRC == OldCRC)
    {
        if((lpData[1] & 0x80) == 0x80)
            err = MODBUS_EXCEPTION;    //send exception code
        else
        {
            err = MODBUS_OK;    //
            *lpLen = (BYTE)dLen;
        }
    }
    else
        err = MODBUS_ERR_CRC;//crc error code

    return err;
}

int ModbusRTU::RecvFunc05(LPBYTE lpData,LPBYTE lpLen)
{
    int err;
    int dLen = 3;  //to get the byte count
    WORD NewCRC = 0;
    WORD OldCRC = 0;

    err = ComRead(lpData,dLen);     //extract first 3 bytes

    if(err == 0)
    {
        return MODBUS_ERR_TIMEOUT;
    }

    if((lpData[1] & 0x80) == 0x80)
    {
       // err = ComRead(&lpData[3],2); //since there will be no byte count
       // dLen += 2;
       return MODBUS_EXCEPTION;
    }
    else
    {
       // dLen = lpData[2] + 2;
        dLen =  5;
        err = ComRead(&lpData[3],dLen);
        dLen += 3;
    }
//    for(int j=0;j<dLen;j++) printf(" %02X",lpData[j]);
    //crc verification starts
    OldCRC = lpData[dLen-1];
    OldCRC = (OldCRC<<8) | lpData[dLen-2];

    NewCRC = CRC16(lpData,dLen - 2);

    if(NewCRC == OldCRC)
    {
        if((lpData[1] & 0x80) == 0x80)
            err = MODBUS_EXCEPTION;    //send exception code
        else
        {
            err = MODBUS_OK;    //
            *lpLen = (BYTE)dLen;
        }
    }
    else
        err = MODBUS_ERR_CRC;//crc error code

    return err;
}

unsigned int ModbusRTU::CRC16 (unsigned char *ucpuchMsg, unsigned int uiDataLen )
{
    unsigned char ucCRCHi = 0xFF ; /* high byte of CRC initialized */
    unsigned char ucCRCLo = 0xFF ; /* low byte of CRC initialized */
    unsigned char ucIndex ; /* will index into CRC lookup table */

    while (uiDataLen--) /* pass through message buffer */
	{
		ucIndex = ucCRCLo ^ *(ucpuchMsg++); /* calculate the CRC */
//		ucpuchMsg++;
        ucCRCLo = ucCRCHi ^ auchCRCHi[ucIndex];
        ucCRCHi = auchCRCLo[ucIndex];
    }

    return ((ucCRCHi << 8) | ucCRCLo);
}
