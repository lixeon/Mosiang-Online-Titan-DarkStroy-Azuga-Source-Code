// GMNotifyManager.h: interface for the CGMNotifyManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GMNOTIFYMANAGER_H__8AB34475_4213_4ADE_B123_5DF6F67CA06F__INCLUDED_)
#define AFX_GMNOTIFYMANAGER_H__8AB34475_4213_4ADE_B123_5DF6F67CA06F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define NOTIFYMGR USINGTON(CGMNotifyManager)


#include "PtrList.h"


#define MAX_NOTIFYMSG_LENGTH			127
#define MAX_NOTIFYMSG_PER_LINE			64
#define MAX_NOTIFYMSG_LINE				5
#define NOTIFYMSG_LAST_TIME				10000

#define NOTIFYMSG_DEFAULT_X				200
#define NOTIFYMSG_DEFAULT_Y				100
#define NOTIFYMSG_DEFAULT_WIDTH			624
#define NOTIFYMSG_DEFAULT_LINESPACE		10
#define NOTIFYMSG_DEFAULT_FONT			0

#define NOTIFYMSG_RSLTN_X				100
#define NOTIFYMSG_RSLTN_Y				78

struct sGMNOTIFY
{
	DWORD	dwReceiveTime;
	DWORD	dwColor;
	int		nStrLen;
	char	strMsg[MAX_NOTIFYMSG_LENGTH+1];
};

enum eNOTIFY_CLASS
{
	eNTC_DEFAULT,
	eNTC_EMERGENCY,
	eNTC_REMAINTIME,	//ºô¸µ
	eNTC_TRACKING,
	eNTC_JACKPOT,
	eNTC_SURVIVAL,
	eNTC_GMCHAT,	//gm chatting color
	eNTC_ALERT,
	eNTC_COUNT,
};

const DWORD dwNotifyColor[eNTC_COUNT] = {

	RGBA_MAKE(0, 255, 60, 0),
	RGBA_MAKE(0, 255, 60, 0),
	RGBA_MAKE(0, 255, 60, 0),
	RGBA_MAKE(255, 20, 20, 255),	//eNTC_TRACKING
	RGBA_MAKE(0, 240, 255, 0),		//eNTC_JACKPOT
	RGBA_MAKE(0, 240, 255, 0),		//eNTC_SURVIVAL
	RGBA_MAKE( 255, 0, 50, 255 ),	//eNTC_GMCHAT
	RGBA_MAKE(255, 0, 50, 255),		//eNTC_ALERT
};

class CGMNotifyManager  
{
protected:

	cPtrList	m_ListNotify;
	int			m_nMaxLine;

	RECT		m_rcPos;
	int			m_nLineSpace;
	int			m_nFontIdx;
	
//EventNotify
	BOOL		m_bUseEventNotify;
	char		m_strEventNotifyTitle[32];
	char		m_strEventNotifyContext[128];
//
	BOOL		m_bNotifyChanged;
//	BOOL		m_bNotifyActive;
	
	BOOL		m_ApplyEventList[eEvent_Max];

protected:
	void PushMsg( char* pMsg, DWORD dwColor );
	void PopMsg();

public:
	CGMNotifyManager();
	virtual ~CGMNotifyManager();

	void Init( int nMaxLine );
	void Release();
	void AddMsg( char* pMsg, int nClass = eNTC_DEFAULT );
	void Render();

	void SetPosition( LONG lx, LONG ly, LONG lWidth );
	void SetFont( int nFontIdx )		{ m_nFontIdx = nFontIdx; }
	void SetLineSpace( int nLineSpace ) { m_nLineSpace = nLineSpace; }
	
//EventNotify
	void SetEventNotifyStr( char* pStrTitle, char* pStrContext );
	void SetEventNotify( BOOL bUse );
	BOOL IsEventNotifyUse()				{ return m_bUseEventNotify; }
	char* GetEventNotifyTitle()			{ return m_strEventNotifyTitle; }
	char* GetEventNotifyContext()		{ return m_strEventNotifyContext; }

	void SetEventNotifyChanged( BOOL bChanged ) { m_bNotifyChanged = bChanged; }
	BOOL IsEventNotifyChanged()					{ return m_bNotifyChanged; }

	void ResetEventApply()					{	memset( m_ApplyEventList, 0, sizeof(BOOL)*eEvent_Max );		}
	void SetEventApply( DWORD EventIdx )	{	m_ApplyEventList[EventIdx] = TRUE;		}
	BOOL IsApplyEvent( DWORD EventIdx )		{	return m_ApplyEventList[EventIdx];		}

//	void SetNotifyActive( BOOL bActive )		{ m_bNotifyActive = bActive; }
//	BOOL IsNotifyActive()						{ return m_bNotifyActive; }
//
};

EXTERNGLOBALTON(CGMNotifyManager)

#endif // !defined(AFX_GMNOTIFYMANAGER_H__8AB34475_4213_4ADE_B123_5DF6F67CA06F__INCLUDED_)
