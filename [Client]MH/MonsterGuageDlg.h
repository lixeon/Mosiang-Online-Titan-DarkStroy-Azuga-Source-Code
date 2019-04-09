// MonsterGuageDlg.h: interface for the CMonsterGuageDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONSTERGUAGEDLG_H__3B4654AD_257E_42F7_9996_C2C8E89E535E__INCLUDED_)
#define AFX_MONSTERGUAGEDLG_H__3B4654AD_257E_42F7_9996_C2C8E89E535E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"

class cStatic;
class cPushupButton;
class CObjectGuagen;
class CMonster;

enum MonsterGuageMode
{
	eMonGuage_Monster,
	eMonGuage_Character,
	// 06. 01. 타겟팅창 인터페이스 변경 - 이영준
	eMonGuage_Npc,
	eMonGuage_Pet,

	eMonGuage_Max
};

class CMonsterGuageDlg : public cDialog  
{
public:
	CMonsterGuageDlg();
	virtual ~CMonsterGuageDlg();

	void	SetMonsterName( char * szMonsterName );
	void	SetMonsterLife( DWORD curVal, DWORD maxVal, int type=-1 );
	void	SetMonsterLife( CMonster * pMonster );
	void	SetMonsterShield( DWORD curVal, DWORD maxVal, int type=-1 );
	void	SetMonsterShield( CMonster * pMonster );
	void	SetGuildUnionName(char* GuildName, char* GuildUnionName);
	void	SetActiveMonGuageMode(int mode, BOOL bActive);
	// 06. 01. 타겟팅창 인터페이스 변경 - 이영준
	void	SetNpcName( char * szNpcName );

	void	Linking();
	void	ShowMonsterGuageMode(int mode);
	virtual void Render();

	CMonster*	GetCurMonster() { return m_pCurMonster; }
	
	// Imsi
	void	SetObjectType( DWORD Type )		{	m_Type = Type;	}
	DWORD	GetObjectType()					{	return m_Type;	}

	void SetMonsterNameColor(DWORD dwColor); //2007. 10. 31. CBH - 몬스터 정보창 다이얼로그 몬스터 이름 색 변경 추가

protected:

	cStatic * m_pName;
	cStatic * m_pLifeText;
	CObjectGuagen * m_pLifeGuage;
	cStatic * m_pShieldText;
	CObjectGuagen * m_pShieldGuage;
	cStatic * m_pGuildName;
	cStatic * m_pGuildUnionName;
	// 06. 01. 타겟팅창 인터페이스 변경 - 이영준
	cStatic * m_pNpcName;
	
	cPtrList m_MonGuageCtlListArray[eMonGuage_Max];
	int m_CurMode;
	CMonster * m_pCurMonster;				// 현재 표시중인 몬스터..
	DWORD	m_Type;							// ObjectType
};

#endif // !defined(AFX_MONSTERGUAGEDLG_H__3B4654AD_257E_42F7_9996_C2C8E89E535E__INCLUDED_)
