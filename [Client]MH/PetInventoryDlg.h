#pragma once
//#include "d:\darkstory\main_dev\[client]mh\interface\ctabdialog.h"
#include "./interface/cTabDialog.h"
#include "PetWearedExDialog.h"

class CPetWearedExDialog;
class cIconGridDialog;
class CItem;
class cStatic;
class cPushupButton;

class CPetInventoryDlg : public cTabDialog
{
	cImage					m_LockImage;

	MSG_ITEM_DIVIDE_SYN		m_MsgDivide;
	CPetWearedExDialog*		m_pPetWearedDlg;
	cIconGridDialog*		m_pIconGridDlg[TAB_PET_INVENTORY_NUM];
	
	//펫인벤 유효 탭 갯수
	DWORD					m_dwValidTabNum;

	//펫인벤 정보 초기화 상태
	BOOL					m_bItemInfoInit;

	//아이콘 드래그 & 탭이동
	DWORD					m_BtnPushstartTime;	
	DWORD					m_BtnPushDelayTime;
	BOOL					m_bPushTabWithMouseOver;

public:
	CPetInventoryDlg(void);
	virtual ~CPetInventoryDlg(void);

	void	SetItemInfoInit(BOOL bVal)	{	m_bItemInfoInit = bVal;	}
	BOOL	IsItemInfoInit()	{	return m_bItemInfoInit;	}

	virtual DWORD ActionEvent(CMouse * mouseInfo);

	void	Linking();
	void	Render();
	void	SetActive(BOOL bVal);
	void	Add(cWindow * window);
	void	OnActionEvent(LONG lId, void *p, DWORD we);

	BOOL	AddItem(ITEMBASE* pItemBase);
	BOOL	AddItem(CItem* pItem);
	BOOL	DeleteItem(POSTYPE Pos,CItem** ppItem);

	void	MoveItem(POSTYPE FromPos, POSTYPE ToPos);
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);
	BOOL	FakeMoveItem( LONG mouseX, LONG mouseY, CItem * pFromItem );
	BOOL	FakeGeneralItemMove( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem);
	BOOL	GetPositionForXYRef(LONG x, LONG y, WORD& pos);
	BOOL	IsDivideFunctionKey();

	CPetWearedExDialog*	GetPetWearedDlg() {	return m_pPetWearedDlg; }

	//탭 유무효화 확인 & 아이템,슬롯 락 처리
	void	ApplyValidInvenTab(DWORD dwValidTabNum);

	CItem*	GetItemForPos(POSTYPE absPos);
	POSTYPE GetRelativePosition( POSTYPE absPos);
	WORD	GetPetIvenTabIndex( POSTYPE absPos );

private:
	static void OnFakeItemDivideOk( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 );
	static void OnFakeItemDivideCancel( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 );

	BOOL	FakeItemDivide( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem );
	BOOL	FakeItemCombine( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem );
	BOOL	CanBeMoved(CItem* pItem,POSTYPE pos);


};
