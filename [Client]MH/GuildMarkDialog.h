// GuildMarkDialog.h: interface for the CGuildMarkDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDMARKDIALOG_H__E389FD12_6720_460C_97EE_5EE57C901DAD__INCLUDED_)
#define AFX_GUILDMARKDIALOG_H__E389FD12_6720_460C_97EE_5EE57C901DAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INTERFACE\cDialog.h"
class cTextArea;
class cButton;
class CGuildMarkDialog : public cDialog  
{
	cTextArea * m_pInfoText;
	cButton*	m_pGuildMarkBtn;
	cButton*	m_pGuildUnionMarkBtn;

public:
	CGuildMarkDialog();
	virtual ~CGuildMarkDialog();
	virtual void SetActive(BOOL val);
	void Linking();
	
	void	ShowGuildMark();
	void	ShowGuildUnionMark();
};

#endif // !defined(AFX_GUILDMARKDIALOG_H__E389FD12_6720_460C_97EE_5EE57C901DAD__INCLUDED_)
