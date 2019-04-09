// TacticStartInfo.h: interface for the CTacticStartInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TACTICINFO_H__5B755A08_7A56_4545_BCEC_1C8D5D43D8E2__INCLUDED_)
#define AFX_TACTICINFO_H__5B755A08_7A56_4545_BCEC_1C8D5D43D8E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTacticStartInfo  
{
	TACTICSTART_INFO m_Info;
		
public:
	CTacticStartInfo(TACTICSTART_INFO* pInfo);
	virtual ~CTacticStartInfo();
	
	BOOL IsTacticableState(CPlayer* pPlayer);
	BOOL IsTacticablePosition(CPlayer* pPlayer,float AngleDeg);
	
	WORD GetTacticID();
	char* GetTacticName();

	DWORD GetLimitTime();
	WORD GetTacticPlayerNum();
	WORD GetReduceNaeRyuk();
	WORD GetSuccessNaeRyuk(int TacticLevel);
	int GetTacticStartEffectNum();
	
	void GetTacticMemberPosition(VECTOR3* pPos,TACTIC_TOTALINFO* pTotalInfo);
};

#endif // !defined(AFX_TACTICINFO_H__5B755A08_7A56_4545_BCEC_1C8D5D43D8E2__INCLUDED_)
