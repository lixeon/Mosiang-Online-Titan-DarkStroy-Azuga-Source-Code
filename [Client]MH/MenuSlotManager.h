// MenuSlotManager.h: interface for the CMenuSlotManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MENUSLOTMANAGER_H__7E9778D7_1C83_4B30_B3C5_436FB47AA91C__INCLUDED_)
#define AFX_MENUSLOTMANAGER_H__7E9778D7_1C83_4B30_B3C5_436FB47AA91C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IndexGenerator.h"

#define MENUSLOTMGR USINGTON(CMenuSlotManager)

class cIcon;
class CMenuIcon;

class CMenuSlotManager  
{
protected:
	CIndexGenerator		m_IconIndexCreator;

public:
	CMenuSlotManager();
	virtual ~CMenuSlotManager();

	void Init();
	void Release();

	BOOL LoadMenuIcon( char* strFile );
	BOOL SaveMenuIcon( char* strFile );

	BOOL AddMenuIcon( WORD wPage, WORD wPos, cIcon* pIcon );
	BOOL DeleteMenuIcon( CMenuIcon* pMenuIcon );
	BOOL ExchangeMenuIcon( CMenuIcon* pMenuIcon1, CMenuIcon* pMenuIcon2 );
	BOOL MoveMenuIcon( WORD wPage, WORD wPos, CMenuIcon* pMenuIcon );
		
	BOOL UseMenuIcon( CMenuIcon* pMenuIcon );	
	
};

EXTERNGLOBALTON(CMenuSlotManager)

#endif // !defined(AFX_MENUSLOTMANAGER_H__7E9778D7_1C83_4B30_B3C5_436FB47AA91C__INCLUDED_)
