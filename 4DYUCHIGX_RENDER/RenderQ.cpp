#include "RenderQ.h"


CRenderQueue::CRenderQueue()
{
	memset(this,0,sizeof(CRenderQueue));
}
BOOL CRenderQueue::Initialize(DWORD dwItemSize,DWORD dwMaxItemNum)
{
	m_dwItemSize = dwItemSize;
	m_dwMaxItemNum = dwMaxItemNum;

	m_Buffer.Initialize(dwItemSize*dwMaxItemNum);
	m_pRenderItemList = new SORT_ITEM_FLOAT[dwMaxItemNum];

	return TRUE;
}
BOOL CRenderQueue::InsertItem(char* pItem,float fDistance)
{
	BOOL		bResult = FALSE;
	char*		pItemPtr;

	pItemPtr = m_Buffer.AllocBuffer(m_dwItemSize);
	if (!pItemPtr)
		goto lb_return;

	memcpy(pItemPtr,pItem,m_dwItemSize);

	m_pRenderItemList[m_dwCurrentItemNum].fCmp = fDistance;
	m_pRenderItemList[m_dwCurrentItemNum].pItem = pItemPtr;

	m_dwCurrentItemNum++;
	bResult = TRUE;

lb_return:
	return bResult;

}




CRenderQueue::~CRenderQueue()
{
	if (m_pRenderItemList)
	{
		delete [] m_pRenderItemList;
		m_pRenderItemList = NULL;
	}
}