// PartyIconMoveInfo.h: interface for the CPartyIconMoveInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARTYICONMOVEINFO_H__4B0968CF_730A_4CC8_A2A4_CDAA5D200D2B__INCLUDED_)
#define AFX_PARTYICONMOVEINFO_H__4B0968CF_730A_4CC8_A2A4_CDAA5D200D2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CPartyIconMoveInfo  
{
	MoveCoord	m_IconPos;
	MoveCoord	m_cPos;
	MoveCoord	m_tPos[MAX_CHARTARGETPOSBUF_SIZE];
	int			m_KyungGongIdx;
	int			m_MoveState;
	int			m_tgMaxIndex;
	int			m_tgIndex;
	DWORD		m_MoveStartTime;
	float		m_MoveTime;
	float		m_Speed;
	float		m_DirectionX;
	float		m_DirectionZ;


public:
	CPartyIconMoveInfo();
	virtual ~CPartyIconMoveInfo();

	void Process();
	void Reset();
	void SetMoveInfo(PARTYICON_MOVEINFO* pMoveInfo);
	void SetPosition(int posX, int posZ)		{ m_cPos.posX = posX, m_cPos.posZ = posZ;	}
	void SetIconPosition(int posX, int posZ)	{ m_IconPos.posX = posX, m_IconPos.posZ = posZ;	}
	void SetKoungGongIdx(int Index);
	void SetMoveState(int State);	
	void CalMoveUpdate();	
	MoveCoord GetIconPos()			{ return m_IconPos;	}

protected:
	void SetMoveSpeed();
};

#endif // !defined(AFX_PARTYICONMOVEINFO_H__4B0968CF_730A_4CC8_A2A4_CDAA5D200D2B__INCLUDED_)
