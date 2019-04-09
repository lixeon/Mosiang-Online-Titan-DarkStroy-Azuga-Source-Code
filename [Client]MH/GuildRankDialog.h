// GuildRankDialog.h: interface for the CGuildRankDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDRANKDIALOG_H__34228850_CFC8_4FAE_9A7F_DCAD9D68A695__INCLUDED_)
#define AFX_GUILDRANKDIALOG_H__34228850_CFC8_4FAE_9A7F_DCAD9D68A695__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INTERFACE\cDialog.h"

class cTextArea;
class cComboBox;
class cButton;

enum Rankmode
{
	eRankMode_Dan,
	eRankMode_Guild,
	eRankMode_Max,
};

class CGuildRankDialog  : public cDialog  
{
	cTextArea * m_pRankMemberName;
	cComboBox * m_pRankComboBox;
	cComboBox * m_pDRankComboBox;
	cButton * m_pOkBtn;
	cButton * m_pDOkBtn;

	cPtrList m_GuildRankCtrlList[eRankMode_Max];
	BYTE m_CurGuildRankMode;
public:
	CGuildRankDialog();
	virtual ~CGuildRankDialog();
	virtual void SetActive(BOOL val);
	void Linking();
	void ShowGuildRankMode(BYTE GuildLvl);
	void SetActiveGuildRankMode(int showmode,BOOL bActive);
	void SetName(char * name);
};

#endif // !defined(AFX_GUILDRANKDIALOG_H__34228850_CFC8_4FAE_9A7F_DCAD9D68A695__INCLUDED_)
