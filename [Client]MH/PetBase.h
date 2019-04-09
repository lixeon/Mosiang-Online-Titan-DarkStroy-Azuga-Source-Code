// PetBase.h: interface for the CPetBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PETBASE_H__16D72518_17A7_4FED_AEAF_7AF8B7C73B3A__INCLUDED_)
#define AFX_PETBASE_H__16D72518_17A7_4FED_AEAF_7AF8B7C73B3A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object.h"


class CPetBase : public CObject
{
	DWORD		m_type;
	CPlayer*	m_pMasterPlayer;
	

public:
	CPetBase();
	virtual ~CPetBase();
	
	void SetMaseter( CPlayer* player )			{	m_pMasterPlayer = player;	}
	void SetMotionInState( BYTE State );

	float DoGetMoveSpeed();

	void OnClicked();
};

#endif // !defined(AFX_PETBASE_H__16D72518_17A7_4FED_AEAF_7AF8B7C73B3A__INCLUDED_)
