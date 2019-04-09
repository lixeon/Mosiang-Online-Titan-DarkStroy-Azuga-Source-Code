#pragma once

#include "../4DyuchiNET_Common/typedef.h"

class CNetwork;

#pragma pack (push,1)

struct CONNECT_ITEM
{	
	SOCKET				socket;
	CNetwork*			pNetwork;

	CONNECTSUCCESSFUNC	SuccessConnect;
	CONNECTFAILFUNC		FailConnect;
	void*				pExt;

	SOCKADDR_IN		addr;
	
};
#pragma pack(pop)


#define CONNECT_ATOM_SIZE			20

class CConnectQue
{
	CONNECT_ITEM*		m_pItemBuffer;
	DWORD				m_dwItemNum;
	DWORD				m_dwMaxItemNum;
public:
	BOOL				Initialize(DWORD dwMaxConnectAtSameTime);
	BOOL				PushConnectItem(CONNECT_ITEM* pItem);
	CONNECT_ITEM*		GetConnectItem(DWORD index);
	DWORD				GetConnectItemNum()	{return m_dwItemNum;}
	void				Clear()		{ m_dwItemNum = 0;}
	CConnectQue();
	~CConnectQue();
};

