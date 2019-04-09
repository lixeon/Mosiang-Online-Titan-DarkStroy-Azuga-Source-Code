// MPRegistDialog.h: interface for the CMPRegistDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPREGISTDIALOG_H__C460C404_03F2_43F7_B548_9AB6EAD0D395__INCLUDED_)
#define AFX_MPREGISTDIALOG_H__C460C404_03F2_43F7_B548_9AB6EAD0D395__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "./Interface/cIconDialog.h"

class cTextArea;
class cStatic;
class CMugongBase;

class CMPRegistDialog  : public cIconDialog
{
	cTextArea * m_MugongInfo;
	cTextArea * m_PracticeInfo;
	cStatic * m_Fee;
	cIconDialog * m_pMugongIconDlg;
public:
	CMPRegistDialog();
	~CMPRegistDialog();
	void Linking();
	virtual void SetActive( BOOL val );
	virtual BOOL FakeMoveIcon(LONG mouseX, LONG mouseY, cIcon * icon);
	void SetSuryunMugongInfo(char* MugongName, BYTE Sung);
	void SetPracticeInfo(BYTE Sung, DWORD limitime, int Kind, int num, MONEYTYPE Fee);
	void AddLink(cIcon* picon);
	CMugongBase* GetMugong();
};

#endif // !defined(AFX_MPREGISTDIALOG_H__C460C404_03F2_43F7_B548_9AB6EAD0D395__INCLUDED_)
