// QuestRegenMgr.h: interface for the CQuestRegenMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTREGENMGR_H__853ECDEE_ED49_4F08_ADA2_48A28C74DEC5__INCLUDED_)
#define AFX_QUESTREGENMGR_H__853ECDEE_ED49_4F08_ADA2_48A28C74DEC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMHFile;
class CPlayer;

struct QRPOS
{
	VECTOR3		vPos;
	WORD		wRadius;
};

class CQuestRegenInfo
{
protected:
	BYTE	m_bCondition;	
	WORD	m_wMonsterCount;
	WORD	m_wMonsterKind;
	WORD	m_wRadius;
	VECTOR3	m_vOnePos;
	QRPOS*	m_pPos;

public:
	CQuestRegenInfo();
	virtual ~CQuestRegenInfo();

	void	LoadData( CMHFile* pFile );
	void	MonsterRegen( CPlayer* pPlayer );
};

#define QUESTREGENMGR	USINGTON(CQuestRegenMgr)

class CQuestRegenMgr  
{
protected:
	CYHHashTable<CQuestRegenInfo>	m_QRITable;

public:
	CQuestRegenMgr();
	virtual ~CQuestRegenMgr();

	void	LoadData();
	void	MonsterRegen( CPlayer* pPlayer, DWORD dwRegenType );
};

EXTERNGLOBALTON(CQuestRegenMgr);

#endif // !defined(AFX_QUESTREGENMGR_H__853ECDEE_ED49_4F08_ADA2_48A28C74DEC5__INCLUDED_)
