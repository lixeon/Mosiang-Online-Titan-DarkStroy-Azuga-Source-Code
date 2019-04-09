// PtrList.cpp: implementation of the cPtrList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "PtrList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define ASSERT assert
void* pNullPoint	= NULL;

//////////////////////////////////////////////////////////////////
//				CPlex	class									//
//////////////////////////////////////////////////////////////////
CPlex* PASCAL CPlex::Create(CPlex*& pHead, UINT nMax, UINT cbElement)
{
//	ASSERT(nMax > 0 && cbElement > 0);
	CPlex* p = (CPlex*) new BYTE[sizeof(CPlex) + nMax * cbElement];
	p->pNext = pHead;
	pHead = p;
	return p;
}

void CPlex::FreeDataChain()
{
	CPlex* p = this;
	while (p != NULL)
	{
		BYTE* bytes = (BYTE*) p;
		CPlex* pNext = p->pNext;
		delete[] bytes;
		p = pNext;
	}
}


cPtrList::cPtrList(int nBlockSize)
{
//	ASSERT(nBlockSize > 0);

	m_nCount = 0;
	m_pNodeHead = m_pNodeTail = m_pNodeFree = NULL;
	m_pBlocks = NULL;
	m_nBlockSize = nBlockSize;
}

void cPtrList::RemoveAll()
{
//	ASSERT_VALID(this);
//	ASSERT(this);

	m_nCount = 0;
	m_pNodeHead = m_pNodeTail = m_pNodeFree = NULL;
	m_pBlocks->FreeDataChain();
	m_pBlocks = NULL;
}

cPtrList::~cPtrList()
{
	RemoveAll();
//	ASSERT(m_nCount == 0);
}


cPtrList::CNode*
cPtrList::NewNode(cPtrList::CNode* pPrev, cPtrList::CNode* pNext)
{
	if (m_pNodeFree == NULL)
	{
		// add another block
		CPlex* pNewBlock = CPlex::Create(m_pBlocks, m_nBlockSize, sizeof(CNode));

		// chain them into free list
		CNode* pNode = (CNode*) pNewBlock->data();
		// free in reverse order to make it easier to debug
		pNode += m_nBlockSize - 1;
		for (int i = m_nBlockSize-1; i >= 0; i--, pNode--)
		{
			pNode->pNext = m_pNodeFree;
			m_pNodeFree = pNode;
		}
	}
//	ASSERT(m_pNodeFree != NULL);

	cPtrList::CNode* pNode = m_pNodeFree;
	m_pNodeFree = m_pNodeFree->pNext;
	pNode->pPrev = pPrev;
	pNode->pNext = pNext;
	m_nCount++;
//	ASSERT(m_nCount > 0);  

	pNode->data = NULL;

	return pNode;
}

void cPtrList::FreeNode(cPtrList::CNode* pNode)
{
	pNode->pNext = m_pNodeFree;
	m_pNodeFree = pNode;
	m_nCount--;
//	ASSERT(m_nCount >= 0);  


	if (m_nCount == 0)
		RemoveAll();
}

/////////////////////////////////////////////////////////////////////////////

PTRLISTPOS cPtrList::AddHead(void* newElement)
{
//	ASSERT_VALID(this);
//	ASSERT(this);

	CNode* pNewNode = NewNode(NULL, m_pNodeHead);
	pNewNode->data = newElement;
	if (m_pNodeHead != NULL)
		m_pNodeHead->pPrev = pNewNode;
	else
		m_pNodeTail = pNewNode;
	m_pNodeHead = pNewNode;
	return (PTRLISTPOS) pNewNode;
}

PTRLISTPOS cPtrList::AddTail(void* newElement)
{
//	ASSERT_VALID(this);
//	ASSERT(this);

	CNode* pNewNode = NewNode(m_pNodeTail, NULL);
	pNewNode->data = newElement;
	if (m_pNodeTail != NULL)
		m_pNodeTail->pNext = pNewNode;
	else
		m_pNodeHead = pNewNode;
	m_pNodeTail = pNewNode;
	return (PTRLISTPOS) pNewNode;
}

void cPtrList::AddHead(cPtrList* pNewList)
{
//	ASSERT_VALID(this);
//	ASSERT(this);
//	ASSERT(pNewList != NULL);
//	ASSERT_KINDOF(cPtrList, pNewList);
//	ASSERT(cPtrList, pNewList);
//	ASSERT_VALID(pNewList);
//	ASSERT(pNewList);


	PTRLISTPOS pos = pNewList->GetTailPosition();
	while (pos != NULL)
		AddHead(pNewList->GetPrev(pos));
}

void cPtrList::AddTail(cPtrList* pNewList)
{
//	ASSERT_VALID(this);
//	ASSERT(this);
//	ASSERT(pNewList != NULL);
//	ASSERT_KINDOF(cPtrList, pNewList);
//	ASSERT_VALID(pNewList);

	// add a list of same elements
	PTRLISTPOS pos = pNewList->GetHeadPosition();
	while (pos != NULL)
		AddTail(pNewList->GetNext(pos));
}

void* cPtrList::RemoveHead()
{
//	ASSERT_VALID(this);
//	ASSERT(m_pNodeHead != NULL);  
//	ASSERT(AfxIsValidAddress(m_pNodeHead, sizeof(CNode)));

	CNode* pOldNode = m_pNodeHead;
	void* returnValue = pOldNode->data;

	m_pNodeHead = pOldNode->pNext;
	if (m_pNodeHead != NULL)
		m_pNodeHead->pPrev = NULL;
	else
		m_pNodeTail = NULL;
	FreeNode(pOldNode);
	return returnValue;
}

void* cPtrList::RemoveTail()
{
//	ASSERT_VALID(this);
//	ASSERT(m_pNodeTail != NULL);  
//	ASSERT(AfxIsValidAddress(m_pNodeTail, sizeof(CNode)));
	if( this == NULL)
		return pNullPoint;
	if( m_pNodeTail == NULL)
		return pNullPoint;

	CNode* pOldNode = m_pNodeTail;
	void* returnValue = pOldNode->data;

	m_pNodeTail = pOldNode->pPrev;
	if (m_pNodeTail != NULL)
		m_pNodeTail->pNext = NULL;
	else
		m_pNodeHead = NULL;
	FreeNode(pOldNode);
	return returnValue;
}

PTRLISTPOS cPtrList::InsertBefore(PTRLISTPOS position, void* newElement)
{
//	ASSERT_VALID(this);

	if (position == NULL)
		return AddHead(newElement); 

	// Insert it before position
	CNode* pOldNode = (CNode*) position;
	CNode* pNewNode = NewNode(pOldNode->pPrev, pOldNode);
	pNewNode->data = newElement;

	if (pOldNode->pPrev != NULL)
	{
//		ASSERT(AfxIsValidAddress(pOldNode->pPrev, sizeof(CNode)));
		pOldNode->pPrev->pNext = pNewNode;
	}
	else
	{
//		ASSERT(pOldNode == m_pNodeHead);
		m_pNodeHead = pNewNode;
	}
	pOldNode->pPrev = pNewNode;
	return (PTRLISTPOS) pNewNode;
}

PTRLISTPOS cPtrList::InsertAfter(PTRLISTPOS position, void* newElement)
{
//	ASSERT_VALID(this);

	if (position == NULL)
		return AddTail(newElement);

	// Insert it before position
	CNode* pOldNode = (CNode*) position;
//	ASSERT(AfxIsValidAddress(pOldNode, sizeof(CNode)));
	CNode* pNewNode = NewNode(pOldNode, pOldNode->pNext);
	pNewNode->data = newElement;

	if (pOldNode->pNext != NULL)
	{
//		ASSERT(AfxIsValidAddress(pOldNode->pNext, sizeof(CNode)));
		pOldNode->pNext->pPrev = pNewNode;
	}
	else
	{
//		ASSERT(pOldNode == m_pNodeTail);
		m_pNodeTail = pNewNode;
	}
	pOldNode->pNext = pNewNode;
	return (PTRLISTPOS) pNewNode;
}

void cPtrList::RemoveAt(PTRLISTPOS& position)
{
//	ASSERT_VALID(this);

	CNode* pOldNode = (CNode*) position;
//	ASSERT(AfxIsValidAddress(pOldNode, sizeof(CNode)));
	if( pOldNode == NULL)
		return;

	// remove pOldNode from list
	if (pOldNode == m_pNodeHead)
	{
		m_pNodeHead = pOldNode->pNext;
		position	= (PTRLISTPOS)m_pNodeHead;
	}
	else
	{
//		ASSERT(AfxIsValidAddress(pOldNode->pPrev, sizeof(CNode)));
		pOldNode->pPrev->pNext = pOldNode->pNext;
		position	= (PTRLISTPOS)pOldNode->pPrev->pNext;
	}
	if (pOldNode == m_pNodeTail)
	{
		m_pNodeTail = pOldNode->pPrev;
	}
	else
	{
//		ASSERT(AfxIsValidAddress(pOldNode->pNext, sizeof(CNode)));
		pOldNode->pNext->pPrev = pOldNode->pPrev;
	}
	FreeNode(pOldNode);
}


/////////////////////////////////////////////////////////////////////////////

PTRLISTPOS cPtrList::FindIndex(int nIndex) const
{
//	ASSERT_VALID(this);

	if (nIndex >= m_nCount || nIndex < 0)
		return NULL;  // went too far

	CNode* pNode = m_pNodeHead;
	while (nIndex--)
	{
//		ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
		pNode = pNode->pNext;
	}
	return (PTRLISTPOS) pNode;
}

PTRLISTPOS cPtrList::Find(void* searchValue, PTRLISTPOS startAfter) const
{
//	ASSERT_VALID(this);

	CNode* pNode = (CNode*) startAfter;
	if (pNode == NULL)
	{
		pNode = m_pNodeHead;  // start at head
	}
	else
	{
//		ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
		pNode = pNode->pNext; 
	}

	for (; pNode != NULL; pNode = pNode->pNext)
		if (pNode->data == searchValue)
			return (PTRLISTPOS) pNode;
	return NULL;
}


void*& cPtrList::GetHead()
{
//	ASSERT(m_pNodeHead != NULL);
	return m_pNodeHead->data;
}

void* cPtrList::GetHead() const
{
//	ASSERT(m_pNodeHead != NULL);
	return m_pNodeHead->data;
}

void*& cPtrList::GetTail()
{
	//ASSERT(m_pNodeTail == NULL);
	if( m_pNodeTail == NULL)
	{
		return pNullPoint;
	}
	return m_pNodeTail->data;
}

void* cPtrList::GetTail() const
{
//	ASSERT(m_pNodeTail != NULL);
	return m_pNodeTail->data;
}

PTRLISTPOS cPtrList::GetHeadPosition() const
{
	return (PTRLISTPOS) m_pNodeHead;
}

PTRLISTPOS cPtrList::GetTailPosition() const
{
	return (PTRLISTPOS) m_pNodeTail;
}

void*& cPtrList::GetNext(PTRLISTPOS& rPosition) // return *Position++
{
	CNode* pNode = (CNode*) rPosition;
//	ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
	if( pNode == NULL)
		return pNullPoint;

	rPosition = (PTRLISTPOS) pNode->pNext;
	return pNode->data;
}

void* cPtrList::GetNext(PTRLISTPOS& rPosition) const // return *Position++
{
	CNode* pNode = (CNode*) rPosition;
//	ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
	rPosition = (PTRLISTPOS) pNode->pNext;
	return pNode->data;
}

void*& cPtrList::GetPrev(PTRLISTPOS& rPosition) // return *Position--
{
	CNode* pNode = (CNode*) rPosition;
//	ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
	if( pNode == NULL)
		return pNullPoint;

	rPosition = (PTRLISTPOS) pNode->pPrev;
	return pNode->data;
}

void* cPtrList::GetPrev(PTRLISTPOS& rPosition) const
{
	CNode* pNode = (CNode*) rPosition;
//	ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
	if( pNode == NULL)
		return pNullPoint;

	rPosition = (PTRLISTPOS) pNode->pPrev;
	return pNode->data;
}

void*& cPtrList::GetAt(PTRLISTPOS position)
{
	CNode* pNode = (CNode*) position;
//	ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
	return pNode->data;
}

void* cPtrList::GetAt(PTRLISTPOS position) const
{
	CNode* pNode = (CNode*) position;
//	ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
	return pNode->data;
}

void cPtrList::SetAt(PTRLISTPOS pos, void* newElement)
{
	CNode* pNode = (CNode*) pos;
//	ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
	pNode->data = newElement;
}


BOOL cPtrList::Remove(void* pRemoveValue)
{
	PTRLISTPOS findpos = NULL;
	CNode* pNode = m_pNodeHead;  // start at head
	for (; pNode != NULL; pNode = pNode->pNext)
	{
		if (pNode->data == pRemoveValue)
			findpos = (PTRLISTPOS) pNode;
	}
	if(findpos == NULL)
		return FALSE;
	
	RemoveAt(findpos);
	return TRUE;
}