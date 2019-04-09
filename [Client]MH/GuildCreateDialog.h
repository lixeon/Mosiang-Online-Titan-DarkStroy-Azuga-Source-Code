// GuildCreateDialog.h: interface for the CGuildCreateDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDCREATEDIALOG_H__280DECC6_0507_4E41_9FC8_5F2FCB9417C5__INCLUDED_)
#define AFX_GUILDCREATEDIALOG_H__280DECC6_0507_4E41_9FC8_5F2FCB9417C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INTERFACE\cDialog.h"
class cStatic;
class cEditBox;
class cTextArea;
class cButton;

class CGuildCreateDialog : public cDialog  
{
	cStatic * m_pLocation;
	cEditBox * m_pGuildName;
	cTextArea * m_pIntro;
	cButton * m_OkBtn;
	cStatic * m_CaptionName;

public:
	CGuildCreateDialog();
	virtual ~CGuildCreateDialog();
	void Linking();
	virtual void SetActive(BOOL val);

	void SetMunpaName( char* strName );
	void SetMunpaIntro( char* strIntro );
};

class CGuildUnionCreateDialog : public cDialog
{
protected:
	cEditBox*	m_pNameEdit;
	cTextArea*	m_pText;
	cButton*	m_pOkBtn;
	
public:
	CGuildUnionCreateDialog();
	virtual ~CGuildUnionCreateDialog();

	virtual void	SetActive( BOOL val );
	void			Linking();
};


#endif // !defined(AFX_GUILDCREATEDIALOG_H__280DECC6_0507_4E41_9FC8_5F2FCB9417C5__INCLUDED_)
