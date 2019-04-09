// MouseCursor.h: interface for the CMouseCursor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOUSECURSOR_H__59181BEB_8F90_4C69_BC7C_A4B386585E29__INCLUDED_)
#define AFX_MOUSECURSOR_H__59181BEB_8F90_4C69_BC7C_A4B386585E29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define __SYSTEMCURSOR__

enum eCURSOR
{
	eCURSOR_DEFAULT,
	eCURSOR_ATTACK,
	eCURSOR_ITEMSELECT,
	eCURSOR_ITEMDRAGGING,
	eCURSOR_DEALER,//	eCURSOR_ONOBJECT,
	eCURSOR_CANTMOVE,
	eCURSOR_DEFAULTCHEAT,
	eCURSOR_TITANREPAIR,	// magi82 - Titan(070511)
	eCURSOR_MINING,			//2007. 7. 11. CBH - 채광 커서 추가
	eCURSOR_COLLECTION,		//2007. 7. 11. CBH - 채집 커서 추가
	eCURSOR_HUNT,			//2007. 7. 11. CBH - 사냥 커서 추가

//	eCURSOR_ONGROUND,
//	eCURSOR_CARET,
	eCURSOR_MAX,
};

#define CURSOR USINGTON(CMouseCursor)

class CMouseCursor  
{

	HCURSOR m_hCursor[eCURSOR_MAX];
	eCURSOR m_curCursor;
	RECT	m_rcOldClip;
	RECT	m_rcClip;

	BOOL	m_bActive;

	BOOL	m_bCheatCursor;

public:

//	//MAKESINGLETON(CMouseCursor)
	CMouseCursor();
	virtual ~CMouseCursor();

	void Init( HINSTANCE hInstance );
	void Release();
	
	void Process();
	
	void SetCursor(eCURSOR cursor);
	eCURSOR GetCursor() { return m_curCursor; }
	
	void SetActive( BOOL bActive );

	void SetCheatMode( BOOL bCheatMode );
};

EXTERNGLOBALTON(CMouseCursor)

#endif // !defined(AFX_MOUSECURSOR_H__59181BEB_8F90_4C69_BC7C_A4B386585E29__INCLUDED_)
