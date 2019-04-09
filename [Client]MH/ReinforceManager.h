// ReinforceManager.h: interface for the CReinforceManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REINFORCEMANAGER_H__D6D406B7_E71B_44AA_AEA4_4980B19C85A8__INCLUDED_)
#define AFX_REINFORCEMANAGER_H__D6D406B7_E71B_44AA_AEA4_4980B19C85A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define REINFORCEMGR USINGTON(CReinforceManager)

struct sITEM_REINFORCE_INFO;
struct sITEM_RAREREINFORCE_INFO;

#define MATERIAL_TOTALGRAVITY_MAX	100

enum eItemOptionKind{ eIOK_Normal, eIOK_Rare, };

class CReinforceManager  
{
public:
	CReinforceManager();
	virtual ~CReinforceManager();

	void Release();

	sITEM_REINFORCE_INFO * GetReinforceItemInfo(WORD wItemIdx);
	BOOL LoadReinforceItemInfo();

	void SetHeadPos();
	sITEM_REINFORCE_INFO * GetData();
	
	//SW051021 수정
	BOOL CheckValidMaterial( WORD wReinforceItemIdx, WORD wMaterialItemIdx, WORD eItemOption = eIOK_Normal );
	BOOL CheckReinforceItemWithMeterial( DWORD ItemPos, DWORD MaterialItemIdx, WORD eItemOption = eIOK_Normal );

	//SW051021
	sITEM_RAREREINFORCE_INFO* GetRareReinforceItemInfo(WORD wItemIdx);
	BOOL LoadRareReinnforceItemInfo();
	BOOL CheckMaterialTotalGravity(WORD wMaterialItemIdx, WORD wMaterialCount, WORD eItemOption);

	int GetRareMaterialTotalGravity() { return m_dwRareMaterialTotalGravity; }
	void SetRareMaterialTotalGravity(DWORD value) { m_dwRareMaterialTotalGravity = value; }

	int GetTargetOptionKind() { return m_dwTargetItemOptionKind; }
	void SetTatgetOptionKind(DWORD OptKind) { m_dwTargetItemOptionKind = OptKind; }

protected:
	CYHHashTable<sITEM_REINFORCE_INFO> m_ReinforceItemInfoList;
	CYHHashTable<sITEM_RAREREINFORCE_INFO> m_RareReinforceItemInfoList;

	DWORD m_dwRareMaterialTotalGravity;	//레어 아이템 강화 재료의 총 비중 (100 이하여야 한다.)
	DWORD m_dwTargetItemOptionKind;
};

EXTERNGLOBALTON(CReinforceManager)

#endif // !defined(AFX_REINFORCEMANAGER_H__D6D406B7_E71B_44AA_AEA4_4980B19C85A8__INCLUDED_)
