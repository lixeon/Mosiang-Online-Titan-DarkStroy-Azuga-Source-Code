#include "GXMapObject.h"
#include "../4DyuchiGXGFunc/global.h"
#include "executive.h"


//	GLOBAL_FUNC_DLL DWORD			__stdcall	QBHSelect(QBHASH_HANDLE pHash,DWORD OUT* pItems,DWORD dwMaxItemNum,DWORD dwKeyData);
//GLOBAL_FUNC_DLL void*			__stdcall	QBHInsert(QBHASH_HANDLE pHash,DWORD dwItem,DWORD dwKeyData);


void CGXMapObject::Cleanup()
{
	m_dwIndex = 0xffffffff;
	m_pHashHandle = NULL;
	m_dwClipperIndex = 0xffffffff;
	m_dwRenderFrameCount = 0xffffffff;
	m_dwPickTypeFlag = PICK_TYPE_DEFAULT;
	m_dwIndexInInitialTable = 0xffffffff;
}


BOOL CGXMapObject::SetID(DWORD dwID)	
{
	QBHASH_HANDLE	pHash = m_pExecutive->GetIDHash();
	DWORD	dwItem;

	BOOL	bResult = FALSE;


	if (dwID == m_dwID)
		goto lb_true;
	
	if (dwID >= 0xfffffff0)
		goto lb_return;

	void* pSearchHandle;
	if (QBHSelect(pHash,&pSearchHandle,&dwItem,1,dwID))
	{
		MessageBox(NULL,"conflict of GXObject's UID","Error",MB_OK);
		goto lb_return;
	}

	pSearchHandle = QBHInsert(pHash,(DWORD)this,dwID);
	if (!pSearchHandle)
		goto lb_return;


	if (m_pHashHandle)
	{	// 아이디를 이미 가지고 있으면 반납하고..
		QBHDelete(pHash,m_pHashHandle);
		m_pHashHandle = NULL;
	}
	m_pHashHandle = pSearchHandle;
	m_dwID = dwID;

lb_true:
	bResult = TRUE;

lb_return:
	return bResult;
}
BOOL CGXMapObject::IsRenderable()
{ 
	BOOL	bResult;
	
	if (m_dwScheduleFlag & SCHEDULE_FLAG_NOT_RENDER)
		bResult = FALSE;
	else
		bResult = TRUE;

lb_return:
	return bResult;

}
void CGXMapObject::ReleaseID()
{
	if (m_pHashHandle)
	{
		QBHASH_HANDLE	pHash = m_pExecutive->GetIDHash();
 		QBHDelete(pHash,m_pHashHandle);
		m_dwID = 0xffffffff;
		m_pHashHandle = NULL;
	}
}


void CGXMapObject::Duplcate(CGXMapObject* pDestGXMObj)
{
	pDestGXMObj->m_dwScheduleFlag = m_dwScheduleFlag;
	pDestGXMObj->m_dwRenderFlag = m_dwRenderFlag;
	pDestGXMObj->m_dwAlphaFlag = m_dwAlphaFlag;
	pDestGXMObj->m_dwPropertyFlag = m_dwPropertyFlag;
	pDestGXMObj->m_pExecutive = m_pExecutive;
	
}