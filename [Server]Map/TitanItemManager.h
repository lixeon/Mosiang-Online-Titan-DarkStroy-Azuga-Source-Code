// TitanItemManager.h: interface for the CTitanItemManager class.
//////////////////////////////////////////////////////////////////////

#if !defined(TITANITEMMANAGER_H)
#define TITANITEMMANAGER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define TITANITEMMGR CTitanItemManager::GetInstance()

#include "..\[CC]Header\GameResourceStruct.h"

class ITEM_MIX_INFO;
class CMHFile;

class CTitanItemManager  
{
	CYHHashTable<ITEM_MIX_INFO> m_TPMInfoList;

	CMemoryPoolTempl<TITAN_PARTS_KIND>*	m_TitanPartsKindPool;
	CYHHashTable<TITAN_PARTS_KIND>		m_TitanPartsKindTable;

	CYHHashTable<TITAN_UPGRADEINFO>		m_TitanUpgradeInfoTable;

	CYHHashTable<TITAN_BREAKINFO>		m_TitanBreakInfoTable;


	DWORD m_TitanBreakRate;	// 타이탄 분해 확률

public:
	CTitanItemManager();
	virtual ~CTitanItemManager();
	GETINSTANCE(CTitanItemManager);

	void LoadTitanMixList();
	void LoadTitanMixResultItemInfo(CMHFile * fp, ITEM_MIX_RES * pResInfo);
	ITEM_MIX_INFO * GetTitanMixItemInfo(WORD wItemIdx);

	// 파츠 속성 테이블 관련 함수
	BOOL LoadTitanPartsKindList();
	TITAN_PARTS_KIND* GetTitanPartsKind(DWORD partsIdx);

	// 타이탄 업그레이드 테이블 관련 함수
	void	LoadTitanUpgradeInfoList();
	void	LoadTitanUpgradeInfoResultItemInfo(CMHFile * fp, TITAN_UPGRADE_MATERIAL * pMaterialInfo);
	TITAN_UPGRADEINFO * GetTitanUpgradeInfoItemInfo(DWORD titanIdx);

	// 타이탄 분해 테이블 관련 함수
	void	LoadTitanBreakInfoMaterialInfo(CMHFile * fp, TITAN_BREAK_MATERIAL * pMaterialInfo);
	void	LoadTitanBreakInfoList();
	TITAN_BREAKINFO * GetTitanBreakInfoItemInfo(DWORD dwIdx);

	// 타이탄 네트워크 메세지 관련 함수들..
	// 타이탄 조합 관련
	void TitanMixAddItem( CPlayer* pPlayer, MSG_ITEM* pmsg );
	void TitanMix( CPlayer* pPlayer, MSG_TITANMIX* pmsg );

	// 타이탄 업그레이드 관련
	void TitanupgradeAddItem( CPlayer* pPlayer, MSG_ITEM* pmsg );
	void Titanupgrade( CPlayer* pPlayer, MSG_TITAN_UPGRADE_SYN* pmsg );

	// 타이탄 아이템해체 관련
	void TitanBreakAddItem( CPlayer* pPlayer, MSG_ITEM* pmsg );
	void TitanBreak( CPlayer* pPlayer, MSG_DWORD2* pmsg );

	// magi82 - Titan(070611) 타이탄 무공변환 주석처리
	// 타이탄 무공합성 관련
	//void TitanMugongChange( CPlayer* pPlayer, MSG_WORD3* pmsg );

	// 타이탄 등록/봉인 관련
	void TitanRegisterAdditem( CPlayer* pPlayer, MSG_ITEM* pmsg );
	void TitanDissolutionAdditem( CPlayer* pPlayer, MSG_ITEM* pmsg );

	// 타이탄 파츠 제작 관련
	void TitanPartsMakeAddItem( CPlayer* pPlayer, MSG_ITEM* pmsg );
	void TitanPartsMake( CPlayer* pPlayer, MSG_ITEM_MIX_SYN* pmsg );

	// 타이탄 등록 관련
	void TitanRegister( CPlayer* pPlayer, MSG_DWORD2* pmsg );

	// 타이탄 해제 관련
	void TitanCancellation( CPlayer* pPlayer, MSG_DWORD2* pmsg );
};


#endif // !defined(TITANITEMMANAGER_H)