// RegenPrototype.h: interface for the CRegenPrototype class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGENPROTOTYPE_H__A2C8BAC0_38AC_4AB8_85FF_5D6149B59AC5__INCLUDED_)
#define AFX_REGENPROTOTYPE_H__A2C8BAC0_38AC_4AB8_85FF_5D6149B59AC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRegenPrototype
{
public:
	CRegenPrototype():RegenType(0),ObjectKind(0),wMonsterKind(0)
	{
		vPos.x = vPos.y = vPos.z = 0;
	}

	BYTE	RegenType;
	BYTE	ObjectKind;
	WORD	wMonsterKind;
	DWORD	dwObjectID;
	VECTOR3 vPos;
	
	WORD	InitHelpType;		// 0 : no request, 1~~
	BOOL	bHearing;
	DWORD	HearingDistance;
};

class CRegenObject
{
public:
	CRegenObject():m_dwObjectID(0),m_dwSubObjectID(0),m_dwGridID(0),m_dwGroupID(0),m_pPrototype(NULL){}
	void InitPrototype(CRegenPrototype * pPrototype);
	void InitHelpType();
	BOOL IsHearing() { return m_pPrototype->bHearing; }
	BYTE GetObjectKind() { return m_pPrototype->ObjectKind;	}
	WORD GetMonsterKind() { return m_pPrototype->wMonsterKind;	}
	VECTOR3 * GetPos() { return &m_pPrototype->vPos;	}
	DWORD GetHearingDistance() { return m_pPrototype->HearingDistance;	}
	WORD GetCurHelpType() { return m_CurHelpType;	}
	void SetCurHelpType(WORD type) { m_CurHelpType = type;	}
	DWORD GetSubID() { return m_dwSubObjectID;	}
	DWORD GetGroupID();

	DWORD	m_dwObjectID;
	DWORD	m_dwSubObjectID;
	DWORD	m_dwGridID;
	DWORD	m_dwGroupID;

	WORD	m_CurHelpType;


	CRegenPrototype * m_pPrototype;
};

#endif // !defined(AFX_REGENPROTOTYPE_H__A2C8BAC0_38AC_4AB8_85FF_5D6149B59AC5__INCLUDED_)
