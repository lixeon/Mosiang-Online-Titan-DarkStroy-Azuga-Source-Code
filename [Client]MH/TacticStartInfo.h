// TacticStartInfo.h: interface for the CTacticInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TACTICINFO_H__0C34BE25_3D2E_431F_9393_A97E6527D799__INCLUDED_)
#define AFX_TACTICINFO_H__0C34BE25_3D2E_431F_9393_A97E6527D799__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CHero;
class CTacticStartInfo  
{	
	TACTICSTART_INFO m_Info;
	
public:
	CTacticStartInfo(TACTICSTART_INFO* pInfo);
	virtual ~CTacticStartInfo();

	BOOL IsTacticableState(CHero* pHero);
	BOOL IsTacticablePosition(CHero* pHero);
	
	WORD GetTacticID();

	DWORD GetLimitTime();
	BYTE GetTacticPlayerNum();
	WORD GetReduceNaeRyuk();
	int GetTacticStartEffectNum();
	
	void GetTacticMemberPosition(VECTOR3* pPos,TACTIC_TOTALINFO* pTotalInfo);
};

#endif // !defined(AFX_TACTICINFO_H__0C34BE25_3D2E_431F_9393_A97E6527D799__INCLUDED_)
