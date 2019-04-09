#pragma once

#include "./Interface/cDialog.h"
#include "./Interface/cTabDialog.h"


class cStatic;
class cButton;
class CObjectGuagen;
class CItem;

class CFWEngraveDialog : public cDialog
{
protected:
	CObjectGuagen*	m_pEngraveGuagen;
	cStatic*		m_pRemaintimeStatic;

	DWORD			m_dwProcessTime;
	float			m_fBasicTime;

public:
	CFWEngraveDialog();
	virtual ~CFWEngraveDialog();

	void Linking();
	DWORD ActionEvent( CMouse * mouseInfo );
	void OnActionEvent( LONG lId, void * p, DWORD we );

	void SetActiveWithTime( BOOL val, DWORD dwTime );
};

class CFWTimeDialog : public cDialog
{
protected:
	cStatic*	m_pTimeStatic;
	cStatic*	m_pCharacterName;

	DWORD		m_dwWarTime;

public:
	CFWTimeDialog();
	virtual ~CFWTimeDialog();

	void Linking();
	DWORD ActionEvent( CMouse * mouseInfo );

	void SetActiveWithTimeName( BOOL val, DWORD dwTime, char* pName );
	void SetCharacterName( char* pName );
};

// 요새 & 공성 창고
enum eWareHouseKind
{
	eSeigeWareHouseDlg = 1,
	eFortWareHouseDlg,
};

class CFWWareHouseDialog : public cTabDialog
{
protected:
	int			m_nDialogKind;				// 어떤창고인지???
	WORD		m_wStartPos;					
	cStatic*	m_pMoney;
	CItem*		m_pTempMoveItem;
	int			m_nTempMovePos;

	BOOL m_Lock;	
	BOOL m_InitInfo;

public:
	CFWWareHouseDialog();
	virtual ~CFWWareHouseDialog();

	virtual void Add( cWindow * window );
	virtual void Render();
	void Linking();

	POSTYPE GetRelativePosition( POSTYPE absPos );
	BOOL GetPositionForXYRef( BYTE param, LONG x, LONG y, WORD& pos );
	WORD GetTabIndex( POSTYPE absPos );
	void SetFortWarWareHouseInfo( MSG_FORTWAR_WAREHOUSE_INFO* pInfo, int nKind );
    BOOL AddItem( ITEMBASE* itemBase );
	BOOL DeleteItem( POSTYPE Pos, CItem** ppItem );
	void DeleteAllItem();

	virtual BOOL FakeMoveIcon( LONG x, LONG y, cIcon* icon );
	BOOL FakeMoveItem( LONG mouseX, LONG mouseY, CItem* pFromItem );
	BOOL FakeGeneralItemMove( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem );

	void SendItemMoveSyn();

	virtual void SetActive(BOOL val);
	virtual DWORD ActionEvent(CMouse * mouseInfo);	
	void OnActionEvent(LONG lId, void * p, DWORD we);

	int GetDialogKind()		{ return m_nDialogKind; }

	void SetMoney( MONEYTYPE money );
	void PutOutMoneyMsgBox();
	void PutOutMoney();
	static void OnPutOutMoneyFromFortWarWareOk( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 );
	static void OnPutOutMoneyFromSeigeWarWareOk( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 );	
};