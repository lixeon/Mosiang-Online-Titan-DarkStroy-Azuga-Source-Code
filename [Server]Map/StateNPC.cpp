// StateNPC.cpp: implementation of the CStateNPC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StateNPC.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStateNPC::CStateNPC()
{
	m_CurState = PEACE_STATE;
	m_TempState = PEACE_STATE;
}

CStateNPC::~CStateNPC()
{

}

void CStateNPC::AreaExploreProcess(DWORD NpcID)
{
	
}

void CStateNPC::ChangeState(BOOL UpNDown)
{
	m_TempState = GetCurState();
	switch(m_TempState)
	{
	case PEACE_STATE:
		{
			SetCurState(ANGER_STATE);
		}
		break;
	case ANGER_STATE:
		{
			if(UpNDown == 0)	// DownÇÃ·¡±×°¡ ¿À¸é ÆòÈ­»óÅÂ
				SetCurState(PEACE_STATE);
			else
				SetCurState(ATTACK_STATE);
		}
		break;
	case ATTACK_STATE:
		{
			SetCurState(ANGER_STATE);
		}
		break;
	}
}