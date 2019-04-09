#ifndef _CMSGBOX_H
#define _CMSGBOX_H


#include "./interface/cDialog.h"
//#include "./interface/cStatic.h"
//#include "./interface/cTextArea.h"

class cStatic;
class cTextArea;

#define MAX_MB_MSG	128

enum eMBType
{
	MBT_NOBTN = 0,
	MBT_OK,
	MBT_YESNO,
	MBT_CANCEL,
	MBT_COUNT,
};


enum eMB_BTN_ID
{
	MB_BTN_OK = 0,
	MB_BTN_YES,
	MB_BTN_NO,
	MB_BTN_CANCEL,
	MB_BTN_COUNT,
};


class cMsgBox : public cDialog
{

protected:

// °øÅë »ç¿ë.
	static cImage	m_DlgImg;
	static cImage	m_BtnImgBasic;
	static cImage	m_BtnImgOver;
	static cImage	m_BtnImgPress;
	static char		m_BtnText[MB_BTN_COUNT][32];		// 3->¹öÆ° Á¾·ù °³¼ö

	static DWORD	m_dwBtnTextColor[3];	//basic, over, press
	static BOOL		m_bInit;
///////////////


protected:
	cTextArea*		m_pMsg;
//	cStatic*		m_pMsgText;
	DWORD			m_dwMsgColor;
	cButton*		m_pButton[2];

	void (*m_cbMBFunc)(LONG lId, void * p, DWORD we);
	
	int				m_nDefaultBtn;
	int				m_nMBType;

// LBS 03.10.09 Ãß°¡ È®Àå µ¥ÀÌÅÍ//////////////////////////////
//	void*			m_pData;
//////////////////////////////////////////////////////////////
	DWORD			m_dwParam;
////////

public:
	cMsgBox();
	~cMsgBox();

	virtual DWORD ActionEvent( CMouse * mouseInfo );
	virtual DWORD ActionKeyboardEvent( CKeyboard * keyInfo );
	void SetMBFunc( cbFUNC cbMBFunc ) { m_cbMBFunc = cbMBFunc; }

	static void InitMsgBox();
	static BOOL IsInit() { return m_bInit; }
	void MsgBox( LONG lId, int nMBType, char* strMsg, cbFUNC cbMBFunc = NULL );
	
	void SetDefaultBtn( int nDefaultBtn ) { m_nDefaultBtn = nDefaultBtn; }

	BOOL PressDefaultBtn();
	BOOL ForcePressButton( LONG lBtnID );
	void ForceClose();

// LBS 03.10.09
	DWORD GetParam() { return m_dwParam; }
	void SetParam( DWORD dwParam )  { m_dwParam = dwParam; }
};



#endif





