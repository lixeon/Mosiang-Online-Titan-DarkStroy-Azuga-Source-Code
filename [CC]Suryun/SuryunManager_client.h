// SuryunManager.h: interface for the CSuryunManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SURYUNMANAGER_H__0D37128D_3D3A_458E_959C_D66C4D25E0CD__INCLUDED_)
#define AFX_SURYUNMANAGER_H__0D37128D_3D3A_458E_959C_D66C4D25E0CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SURYUNMGR	CSuryunManager::GetInstance()

#include "SuryunDefine.h"

class CHero;
class CSuryunManager  
{
	CSuryunManager();

	CYHHashTable<SURYUNMISSIONINFO> m_MissionInfo;
	CYHHashTable<SURYUNMONSTERINFO> m_MonsterInfo;
	CYHHashTable<SURYUNLEVELCHANGE> m_ChangeLevelTable;
	
	WORD GoSuryunChk();
	LEVELTYPE GetChangedLevel(LEVELTYPE adjustedLevel,BYTE sung);
public:
	MAKESINGLETON(CSuryunManager);

	~CSuryunManager();

	void NetworkMsgParse(BYTE Protocol,void* pMsg);
	void GoSuryunMapSyn();
	void LoadSuryunInfo();
 	void LoadMonsterInfo();
	MONEYTYPE GetSuryunFee(SURYUNINFO* pInfo);

	SURYUNMISSIONINFO* GetMissionInfo(WORD MugongIdx) { return m_MissionInfo.GetData(MugongIdx); }
	SURYUNMONSTERINFO* GetMonsterInfo(LEVELTYPE Level) { return m_MonsterInfo.GetData(Level); }

	BOOL NeedSuryun(WORD MugongIdx,BYTE Sung,DWORD Exp);
	void SetActiveDlg(BOOL val);
};

#endif // !defined(AFX_SURYUNMANAGER_H__0D37128D_3D3A_458E_959C_D66C4D25E0CD__INCLUDED_)
