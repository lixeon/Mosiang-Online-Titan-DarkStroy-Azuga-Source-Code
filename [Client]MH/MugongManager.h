// MugongManager.h: interface for the CMugongManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUGONGMANAGER_H__BDADCB0F_E3E8_4387_A6D1_C7D3439872BE__INCLUDED_)
#define AFX_MUGONGMANAGER_H__BDADCB0F_E3E8_4387_A6D1_C7D3439872BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MugongCalc.h"
#include "MugongDialog.h"
#include "IndexGenerator.h"

#define MUGONGMGR USINGTON(CMugongManager)

class CMugongManager  
{
	CMugongCalc m_MugongCalc;
	CMugongDialog * m_pMugongDlg;
	CIndexGenerator m_IconIndexCreator;
	CYHHashTable<CMugongBase> m_MugongHash;
public:
	enum eMUGONG_CANNOT_REASON
	{
		eMCUR_Usable,
		eMCUR_SungZero,
		eMCUR_Weapon,
		eMCUR_Limit,	//레벨등 제한이 걸렸다.
		eMCUR_Disable,
		// magi82 - Titan(070911) 타이탄 무공업데이트
		eMCUR_Titan,	// 타이탄 탑승상태가 아니면 무공을 못쓰게 해야함.
	};

//	//MAKESINGLETON(CMugongManager)

	CMugongManager();
	virtual ~CMugongManager();
	void Init();
	void Release();
	void SetMugongDialog(CMugongDialog * dlg){ m_pMugongDlg = dlg; }
	void NetworkMsgParse(BYTE Protocol,void* pMsg);
	void SetToolTipIcon(CMugongBase *pMugong);
	
	int GetMugongNum();

	CMugongBase * NewMugongIcon(MUGONGBASE * mugongBase);
	void DeleteMugongIcon(CMugongBase * mugong);
	//cImage* GetIconImage(WORD ItemIdx, cImage * pImage);
	void GetIconImage(WORD ItemIdx, cImage * pLowImage , cImage * pHighImage);
	/*
	/// 리소스 관리 - 현재 공격무공만 되어 있음
	WORD GetMugongWeaponForIdx(WORD idx);
	char * GetMugongNameForIdx(WORD idx);
	BYTE GetMugongNatureForIdx(WORD idx);
	WORD GetMugongEstPointForIdx(WORD idx, BYTE sung);
	*/
	CMugongBase * GetMugong(DWORD dwIdx);
	CMugongBase * GetMugongByMugongIdx(WORD MugongIdx);
	void RefreshMugong();

	eMUGONG_CANNOT_REASON	CanUse(CMugongBase * pMugong);

	BOOL CheckMugongTree(WORD MugongIdx);

	//// 2007. 7. 2. CBH - 전문스킬 검색 함수 추가
	BOOL SearchJobSkill();
	///////////////////////////////////////////////////////

private:
//	CMugongBase * NewMugong(WORD idx);
};
EXTERNGLOBALTON(CMugongManager)
#endif // !defined(AFX_MUGONGMANAGER_H__BDADCB0F_E3E8_4387_A6D1_C7D3439872BE__INCLUDED_)
