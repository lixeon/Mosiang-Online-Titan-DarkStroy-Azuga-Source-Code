#pragma once
#include "object.h"
#include "GameResourceStruct.h"

class CTitan :
	public CObject
{
	TITAN_TOTALINFO		m_TitanTotalInfo;

	BASE_TITAN_LIST*	m_pSInfo;

	void	InitTitan(TITAN_TOTALINFO* pTotalInfo);
public:
	CTitan(void);
	~CTitan(void);

	friend class CObjectManager;
	friend class CTitanManager;

	TITAN_TOTALINFO*	GetTitanTotalInfo()	{	return &m_TitanTotalInfo;	}
	virtual	void	SetLife(DWORD dwLife);
	DWORD	GetLife();
	virtual	void	SetNaeRyuk(DWORD dwNaeRyuk);
	DWORD	GetNaeRyuk();
};
