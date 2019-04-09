// ListBoxContainer.cpp: implementation of the CListBoxContainer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ListBoxContainer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
// Various Bytes HashTable
GLOBAL_FUNC_DLL	void*			__stdcall	VBHInsert(VBHASH_HANDLE pHash,DWORD dwItem,void* pKeyData,DWORD dwSize);
GLOBAL_FUNC_DLL	BOOL			__stdcall	VBHDelete(VBHASH_HANDLE pHash,void* pBucket);


GLOBAL_FUNC_DLL	DWORD			__stdcall	VBHGetMaxBucketNum(VBHASH_HANDLE pHash);
GLOBAL_FUNC_DLL void			__stdcall	VBHDeleteAll(VBHASH_HANDLE pHash);
*/

CListBoxContainer::CListBoxContainer()
{
	memset(this,0,sizeof(CListBoxContainer));

}
BOOL CListBoxContainer::Initialize(DWORD dwMaxItemNum,DWORD dwMaxSize,CListBox* pListBox,DelFunc pDelFunc)
{
	m_pDelFunc = pDelFunc;
	m_pNameHash = VBHCreate();
	VBHInitialize(m_pNameHash,128,dwMaxSize,dwMaxItemNum);
	m_pListBox = pListBox;


	return TRUE;
}
BOOL CListBoxContainer::InsertItem(char* szName,void* pItem)
{
	BOOL	bResult = FALSE;
	if (!VBHInsert(m_pNameHash,(DWORD)pItem,szName,lstrlen(szName)))
		goto lb_return;

	m_pListBox->AddString(szName);

	bResult = TRUE;
lb_return:
	return bResult;
}
void* CListBoxContainer::GetSelectedItem()
{
	char	szText[_MAX_PATH];
	int		iTextLen;

	void*	pItem = NULL;
	int iIndex = m_pListBox->GetCurSel();
	if (iIndex == LB_ERR)
		goto lb_return;
	
	memset(szText,0,sizeof(szText));
	m_pListBox->GetText(iIndex,szText);
	iTextLen = m_pListBox->GetTextLen(iIndex);
	VBHSelect(m_pNameHash,(DWORD*)&pItem,1,szText,iTextLen);

lb_return:
	return pItem;
}
DWORD CListBoxContainer::GetMultipleSelectedItems(void* pItemList,DWORD dwMaxItemNum)
{
	int*	piIndices = new int[dwMaxItemNum];
	memset(piIndices,0xff,sizeof(int)*dwMaxItemNum);

	DWORD	dwNum = m_pListBox->GetSelItems(dwMaxItemNum,piIndices);

	char	szText[_MAX_PATH];
	int		iTextLen;

	for (DWORD i=0; i<dwNum; i++)
	{
		memset(szText,0,sizeof(szText));
		m_pListBox->GetText(piIndices[i],szText);
		iTextLen = m_pListBox->GetTextLen(piIndices[i]);
		VBHSelect(m_pNameHash,(DWORD*)((DWORD*)pItemList+i),1,szText,iTextLen);
	}
	delete [] piIndices;

	return dwNum;
	
}


void CListBoxContainer::DeleteAll()
{
	char	szText[_MAX_PATH];
	memset(szText,0,sizeof(szText));
	int		iTextLen;

	while (m_pListBox->GetCount())
	{
		m_pListBox->GetText(0,szText);
		iTextLen = m_pListBox->GetTextLen(0);
		VBHDeleteWithKey(m_pNameHash,szText,iTextLen,m_pDelFunc);

		// VBHDeleteWithKey()함수는 키(이름)가 같은 아이템을 모두 지우므로 리스트박스의 아이템과 갯수가 안맞을수 있다.
		// 따라서 리스트박스의 아이템은 몽땅 지운다.
		m_pListBox->DeleteString(0);
	}
}
void* CListBoxContainer::GetItem(int iSeqIndex)
{
	char	szText[_MAX_PATH];
	int		iTextLen;

	void*	pItem = NULL;
	
	memset(szText,0,sizeof(szText));
	if (LB_ERR == m_pListBox->GetText(iSeqIndex,szText))
		goto lb_return;

	iTextLen = m_pListBox->GetTextLen(iSeqIndex);
	VBHSelect(m_pNameHash,(DWORD*)&pItem,1,szText,iTextLen);

lb_return:
	return pItem;
}
int CListBoxContainer::GetItemNum()
{
	return m_pListBox->GetCount();
}
BOOL CListBoxContainer::DeleteSelectedItem()
{
	BOOL	bResult = FALSE;
	char	szText[_MAX_PATH];
	int		iTextLen;

	void*	pItem = NULL;
	int iIndex = m_pListBox->GetCurSel();
	if (iIndex == LB_ERR)
		goto lb_return;
	
	memset(szText,0,sizeof(szText));
	m_pListBox->GetText(iIndex,szText);
	iTextLen = m_pListBox->GetTextLen(iIndex);
	
	bResult = VBHDeleteWithKey(m_pNameHash,szText,iTextLen,m_pDelFunc);
	if (bResult)
		m_pListBox->DeleteString(iIndex);

lb_return:
	return bResult;
}
BOOL CListBoxContainer::DeleteItem(char* szName)
{
	return VBHDeleteWithKey(m_pNameHash,szName,lstrlen(szName),m_pDelFunc);
}

CListBoxContainer::~CListBoxContainer()
{
	if (m_pNameHash)
	{
		VBHRelease(m_pNameHash);
		m_pNameHash = NULL;
	}

}
