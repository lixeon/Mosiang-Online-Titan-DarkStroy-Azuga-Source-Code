// PyoGukManager.h: interface for the CPyoGukManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PYOGUKMANAGER_H__A316F76A_DB8B_4009_B69E_ECF2DCD2F0EC__INCLUDED_)
#define AFX_PYOGUKMANAGER_H__A316F76A_DB8B_4009_B69E_ECF2DCD2F0EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct PYOGUKLISTINFO
{
	BYTE MaxCellNum;
	MONEYTYPE MaxMoney;
	MONEYTYPE BuyPrice;
};

#define PYOGUKMGR CPyoGukManager::GetInstance()
class CPyoGukManager
{
	CPyoGukManager();
	PYOGUKLISTINFO m_PyogukListInfo[MAX_PYOGUKLIST_NUM];
public:
	GETINSTANCE(CPyoGukManager);
	virtual ~CPyoGukManager();
	
	
	PYOGUKLISTINFO* GetPyogukInfo(int i); //i개 일 때의 정보를 리턴
	
	void LoadPyogukList();
	void SendPyogukItemInfo(CPlayer* pPlayer);
	void NetworkMsgParse( BYTE Protocol, void* pMsg );
	void BuyPyogukSyn(CPlayer* pPlayer);
	void PutInMoneyPyoguk(CPlayer* pPlayer, MONEYTYPE Money);
	void PutOutMoneyPyoguk(CPlayer* pPlayer, MONEYTYPE Money);
	
	BOOL	CheckHack( CPlayer* pPlayer ); 
};

#endif // !defined(AFX_PYOGUKMANAGER_H__A316F76A_DB8B_4009_B69E_ECF2DCD2F0EC__INCLUDED_)
