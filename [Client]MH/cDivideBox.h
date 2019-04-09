// cDivideBox.h: interface for the cDivideBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CDIVIDEBOX_H__E7789D66_4B67_41B1_8CAC_AFB48DC1B0D4__INCLUDED_)
#define AFX_CDIVIDEBOX_H__E7789D66_4B67_41B1_8CAC_AFB48DC1B0D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"
//#include "INTERFACE\cSpin.h"

class cSpin;

class cDivideBox : public cDialog  
{
public:
	cDivideBox();
	virtual ~cDivideBox();

	void CreateDivideBox( LONG x, LONG y, LONG ID, cbDivideFUNC cbFc1, cbDivideFUNC cbFc2, void * vData1, void * vData2, char* strTitle );
	virtual DWORD ActionEvent( CMouse * mouseInfo );
	virtual DWORD ActionKeyboardEvent( CKeyboard* keyInfo );
	void SetValue( DWORD val );

//	void SetPos( int nPos ) { m_nPos = nPos; }
	LONG GetValue();

	void SetMaxValue( DWORD val );
	void SetMinValue( DWORD val );

	void ExcuteDBFunc( DWORD we );
	void ChangeKind( int nKind );
	
public:
	
	cbDivideFUNC	m_cbDivideFunc;
	cbDivideFUNC	m_cbCancelFunc;
	cButton			* m_pOKBtn;
	cButton			* m_pCancelBtn;
	cSpin			* m_pSpin;	
	void			* m_vData1;
	void			* m_vData2;
	
//	int				m_nPos;				 // 나눠지는 아이템의 위치를 가진다.
	
};

#endif // !defined(AFX_CDIVIDEBOX_H__E7789D66_4B67_41B1_8CAC_AFB48DC1B0D4__INCLUDED_)
