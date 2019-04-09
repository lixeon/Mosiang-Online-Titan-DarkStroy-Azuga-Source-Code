// StateParam.h: interface for the CStateParam class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATEPARAM_H__2C2D47E2_2037_4509_91AF_00A46F634B7F__INCLUDED_)
#define AFX_STATEPARAM_H__2C2D47E2_2037_4509_91AF_00A46F634B7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStateParam
{
public:
	CStateParam():
	stateNew(0),
	stateCur(0),
	stateOld(0),
	stateStartTime(0),
	stateEndTime(0),
	stateMidTime(0),
	bStateUpdate(FALSE){}

	void SetState(DWORD dwState)
	{
		stateCur=dwState;
	}
	DWORD	stateNew;
	DWORD	stateCur;			// objectState¿Í °°?Ì ¾µ·Á°í ÇÔ//m_BaseObjectInfo.ObjectState
	DWORD	stateOld;
	DWORD	stateStartTime;
	DWORD	stateEndTime;
	DWORD	stateMidTime;
	BOOL	bStateUpdate;
};

#endif // !defined(AFX_STATEPARAM_H__2C2D47E2_2037_4509_91AF_00A46F634B7F__INCLUDED_)
