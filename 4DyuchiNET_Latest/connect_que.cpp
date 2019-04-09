#include "connect_que.h"
#include "pre_connect.h"
#include "network.h"

CConnectQue::CConnectQue()
{
	m_pItemBuffer = NULL;
	m_dwItemNum = 0;
	m_dwMaxItemNum = 0;
	
}
BOOL CConnectQue::PushConnectItem(CONNECT_ITEM* pItem)
{
	if (m_dwItemNum >= m_dwMaxItemNum)
		return FALSE;

	memcpy(m_pItemBuffer+m_dwItemNum,pItem,sizeof(CONNECT_ITEM));
	m_dwItemNum++;
	
	return TRUE;
}
CONNECT_ITEM* CConnectQue::GetConnectItem(DWORD dwIndex)
{
	if (dwIndex >= m_dwItemNum)
		return NULL;


	return m_pItemBuffer+dwIndex;
}

BOOL CConnectQue::Initialize(DWORD dwMaxConnectAtSameTime)
{
	m_dwMaxItemNum = dwMaxConnectAtSameTime;
	m_pItemBuffer = new CONNECT_ITEM[dwMaxConnectAtSameTime];
	memset(m_pItemBuffer,0,sizeof(CONNECT_ITEM)*m_dwMaxItemNum);

	return TRUE;
}

CConnectQue::~CConnectQue()
{
	if (m_pItemBuffer)
	{
		delete [] m_pItemBuffer;
		m_pItemBuffer = NULL;
	}

}
