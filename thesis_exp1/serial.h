#ifndef _INCLUDE_SERIAL_
#define _INCLUDE_SERIAL_

#include <windows.h>

class COMPORT
{
	HANDLE			hCom;		//Handle of This Communication Port.
	COMMPROP		Properties;	//Performance Properties of This Communication Port
	COMMTIMEOUTS	ctmo;		//Setting of Timeout Duration
	DCB				dcb;		//Setting of Serial Connection

public:
	COMPORT( char *PORT );
	COMPORT( char *PORT, DWORD Speed );
	operator BOOL( void )
	{
		if( hCom == INVALID_HANDLE_VALUE ) return FALSE;
		return TRUE;
	}
	void loadDefalutState();
	~COMPORT() { CloseHandle( hCom ); }
	void setBaudRate( DWORD setValue )
	{
		dcb.BaudRate = setValue;
		setState();
	}
	void setStopBits( BYTE setValue )
	{
		dcb.StopBits = setValue;
		setState();
	}
	void setParity( BYTE setValue )
	{
		dcb.Parity = setValue;
		dcb.Parity == 0 ? dcb.fParity = FALSE: dcb.fParity = TRUE;
		setState();
	}
	void setState()
	{
		SetCommState(hCom, &dcb);
		SetCommTimeouts(hCom, &ctmo);
	}
	void  purgeNow( void )	{ PurgeComm( hCom, PURGE_TXABORT | PURGE_RXABORT );	}
	void  purgeBuff( void )	{ PurgeComm( hCom, PURGE_TXCLEAR | PURGE_RXCLEAR );	}
	DWORD receive( BYTE *lpBuff, DWORD BuffLength )
	{
		DWORD	ret;
		ReadFile( hCom, lpBuff, BuffLength, &ret, NULL ); //�ǂݍ���
		return ret;
	}


	DWORD transmit( BYTE *lpBuff, DWORD BuffLength )
	{
		DWORD	ret;
		WriteFile( hCom, lpBuff, BuffLength, &ret, NULL ); //�����o��
		return ret;
	}

};

COMPORT::COMPORT( char *PORT )
{
	hCom	= CreateFile(PORT, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, NULL, NULL);
	loadDefalutState();
	setState();
}

COMPORT::COMPORT( char *PORT, DWORD Speed )
{
	hCom	= CreateFile(PORT, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, NULL, NULL);
	loadDefalutState();
	setBaudRate( Speed );
	setState();
}

void COMPORT::loadDefalutState()
{
	GetCommProperties(hCom, &Properties);
	GetCommState(hCom, &dcb);

	//Default Value of This Class.
	dcb.DCBlength			= sizeof(DCB);
	dcb.BaudRate			= 9600;
	dcb.fBinary				= TRUE;		//In the NT Kernel, allways keep true;
	dcb.fParity				= FALSE;
	dcb.fOutxCtsFlow		= FALSE;
	dcb.fOutxDsrFlow		= FALSE;
	dcb.fDtrControl			= DTR_CONTROL_DISABLE;
	dcb.fDsrSensitivity		= FALSE;

	dcb.fTXContinueOnXoff	= FALSE;	/* XOFF������M���p��	*/
	dcb.fOutX				= FALSE;	/* XON/XOFF�o�̓t���[����	*/
	dcb.fInX				= FALSE;	/* XON/XOFF���̓t���[����	*/
	dcb.fErrorChar			= FALSE;	/* �G���[�ɂ��u����L��	*/
	dcb.fNull				= FALSE;	/* �k�� �X�g���b�s���O��L��	*/
	dcb.fRtsControl			= FALSE;	/* RTS�t���[����	*/
	dcb.fAbortOnError		= FALSE;	/* �G���[���ɓǂݎ��܂��͏������݂𒆎~	*/
//		dcb.fDummy2				;			/* �\��ς�	*/
//		dcb.wReserved			;			/* ���o�[�W�����ł͎g��Ȃ�	*/
//		dcb.XonLim				= ;			/* ���M�ɂ�����XON�X���b�V�����h	*/
//		dcb.XoffLim				= ;			/* ���M�ɂ�����XOFF�X���b�V�����h	*/

	dcb.ByteSize			= 8;
	dcb.Parity				= 0;
	dcb.StopBits			= ONESTOPBIT;	//This Memory Take ONESTOPBIT or ONE5STOPBITS or TWOSTOPBITS

	dcb.XonChar				= 0;			/* ���M����ю�M�ɑ΂���XON����	*/
	dcb.XoffChar			= (char)0xff;	/* ���M����ю�M�ɑ΂���XON����	*/
	dcb.ErrorChar			= 0;			/* �G���[�u������	*/
	dcb.EofChar				= 0;			/* ���͕����̏I���	*/
	dcb.EvtChar				= 0;			/* �󂯎�����C�x���g����	*/

	ctmo.ReadIntervalTimeout = 0;
	ctmo.ReadTotalTimeoutMultiplier = 0;
	ctmo.ReadTotalTimeoutConstant = 500;//50-1
	ctmo.WriteTotalTimeoutMultiplier = 0;
	ctmo.WriteTotalTimeoutConstant = 500;
}

#endif // !defined( MT_SERIAL_H )