// StateNPC.h: interface for the CStateNPC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATENPC_H__6C1AC98E_F4EC_4866_B050_856E83D22A19__INCLUDED_)
#define AFX_STATENPC_H__6C1AC98E_F4EC_4866_B050_856E83D22A19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PEACE_STATE		1		// ÆòÈ­»óÅÂ
#define ANGER_STATE		2		// ÈïºÐ»óÅÂ
#define ATTACK_STATE	3		// °ø°Ý»óÅÂ

#define UP_FLAG			1		// »óÅÂ ¾÷(ÆòÈ­->ÈïºÐ)
#define DOWN_FLAG		0		// »óÅÂ ´Ù¿î(ÈïºÐ->ÆòÈ­)

#define STATENPC_OBJ CStateNPC::GetInstance()

class CStateNPC  
{
	BYTE m_CurState;
	BYTE m_TempState;
	
public:
	GETINSTANCE(CStateNPC)
	CStateNPC();
	virtual ~CStateNPC();

	void AreaExploreProcess(DWORD NpcID);
	void ChangeState(BOOL UpNDown = FALSE);
	void SetCurState(BYTE state){m_CurState = state;}
	BYTE GetCurState(){return m_CurState;}

};

#endif // !defined(AFX_STATENPC_H__6C1AC98E_F4EC_4866_B050_856E83D22A19__INCLUDED_)
