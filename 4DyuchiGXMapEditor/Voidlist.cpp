/*===========================================================================//
 *
 *  Copyright (C) 1997 Hicom Corporation.Ltd. All Rights Reserved.
 *
 *
 *  File   : voidlist.cpp
 *  Content: Double Linkedlist for 'void' type pointer
 *  Program: Lee Jon suk
 *
 ****************************************************************************/
#include "voidlist.h"


//---------------------------------------------------------------------------
// 인  수: 없음                                                             |
// 반환값: 없음                                                             |
// 설  명: 생성자                                                           |
//---------------------------------------------------------------------------
CVoidList::CVoidList()
{
    m_nCount = 0;
    m_bFreeData = 1;
    m_pNodeHead = m_pNodeTail = NULL;
}


//---------------------------------------------------------------------------
// 인  수: 없음                                                             |
// 반환값: 없음                                                             |
// 설  명: 파괴자                                                           |
//---------------------------------------------------------------------------
CVoidList::~CVoidList()
{
    RemoveAll();
}


//---------------------------------------------------------------------------
// 인  수: 없음                                                             |
// 반환값: 없음                                                             |
// 설  명: 모든 노드의 메모리를 해제한다.                                   |
//---------------------------------------------------------------------------
void CVoidList::RemoveAll()
{
    // destroy elements
    CNode* pNode;
    CNode* pNext;

    pNode = m_pNodeHead;
    while(pNode != NULL)
    {
        pNext = pNode->pNext;
        FreeNode(pNode);
        pNode = pNext;
    }
    m_nCount = 0;
    m_pNodeHead = m_pNodeTail = NULL;
}



/////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------
// 인  수: newElement:자료 포인터                                           |
// 반환값: 새로운 자료의 Position Pointer                                   |
// 설  명: 자료를 Head위치에 삽입하여 Head를 newElement로 교체한다.         |
//---------------------------------------------------------------------------
POSITION_ CVoidList::AddHead(void* newElement)
{
    CNode* pNewNode = NewNode(NULL, m_pNodeHead);
    pNewNode->data = newElement;
    if (m_pNodeHead != NULL)
        m_pNodeHead->pPrev = pNewNode;
    else
        m_pNodeTail = pNewNode;
    m_pNodeHead = pNewNode;
    return (POSITION_) pNewNode;
}


//---------------------------------------------------------------------------
// 인  수: newElement:자료 포인터                                           |
// 반환값: 새로운 자료의 Position Pointer                                   |
// 설  명: 자료를 Tail위치에 삽입하여 Tail을 newElement로 교체한다.         |
//---------------------------------------------------------------------------
POSITION_ CVoidList::AddTail(void* newElement)
{
    CNode* pNewNode = NewNode(m_pNodeTail, NULL);
    pNewNode->data = newElement;
    if (m_pNodeTail != NULL)
        m_pNodeTail->pNext = pNewNode;
    else
        m_pNodeHead = pNewNode;
    m_pNodeTail = pNewNode;
    return (POSITION_) pNewNode;
}


//---------------------------------------------------------------------------
// 인  수: pNewList:다른 리스트                                             |
// 반환값: 없음                                                             |
// 설  명: 다른 리스트의 Tail값을 리스트의 Head에 삽입한다.                 |
//---------------------------------------------------------------------------
void CVoidList::AddHead(CVoidList* pNewList)
{
    if (pNewList == NULL) return;

    // add a list of same elements to head (maintain order)
    POSITION_ pos = pNewList->GetTailPosition();
    while (pos != NULL)
        AddHead(pNewList->GetPrev(pos));
}


//---------------------------------------------------------------------------
// 인  수: pNewList:다른 리스트                                             |
// 반환값: 없음                                                             |
// 설  명: 다른 리스트의 Head값을 리스트의 Tail에 삽입한다.                 |
//---------------------------------------------------------------------------
void CVoidList::AddTail(CVoidList* pNewList)
{
    if (pNewList == NULL) return;

    // add a list of same elements
    POSITION_ pos = pNewList->GetHeadPosition();
    while (pos != NULL)
        AddTail(pNewList->GetNext(pos));
}


//---------------------------------------------------------------------------
// 인  수: 없음                                                             |
// 반환값: 다음 노드의 자료                                                 |
// 설  명: Head노드를 삭제한다.                                             |
//---------------------------------------------------------------------------
void* CVoidList::RemoveHead()
{
    if (m_pNodeHead == NULL) 
		return NULL;    // don't call on empty list !!!

    CNode* pOldNode = m_pNodeHead;
    if (pOldNode == NULL) return NULL;
    void* returnValue = pOldNode->data;

    m_pNodeHead = pOldNode->pNext;
    if (m_pNodeHead != NULL)
        m_pNodeHead->pPrev = NULL;
    else
        m_pNodeTail = NULL;
    FreeNode(pOldNode);
    return returnValue;
}


//---------------------------------------------------------------------------
// 인  수: 없음                                                             |
// 반환값: 다음 노드의 자료                                                 |
// 설  명: Tail노드를 삭제한다.                                             |
//---------------------------------------------------------------------------
void* CVoidList::RemoveTail()
{
    if (m_pNodeTail == NULL) return NULL;    // don't call on empty list !!!

    CNode* pOldNode = m_pNodeTail;
    if (pOldNode == NULL) return NULL;
    void* returnValue = pOldNode->data;

    m_pNodeTail = pOldNode->pPrev;
    if (m_pNodeTail != NULL)
        m_pNodeTail->pNext = NULL;
    else
        m_pNodeHead = NULL;
    FreeNode(pOldNode);
    
	return returnValue;
}


//---------------------------------------------------------------------------
// 인  수: position:삽입될 위치, newElement:삽입될 자료                     |
// 반환값: 삽입된 자료의 위치                                               |
// 설  명: position앞에 새로운 자료를 삽입한다.                             |
//---------------------------------------------------------------------------
POSITION_ CVoidList::InsertBefore(POSITION_ position, void* newElement)
{
    if (position == NULL)
        return AddHead(newElement); // insert before nothing -> head of the list

    // Insert it before position
    CNode* pOldNode = (CNode*) position;
    CNode* pNewNode = NewNode(pOldNode->pPrev, pOldNode);
    pNewNode->data = newElement;

    if (pOldNode->pPrev != NULL)
    {
        pOldNode->pPrev->pNext = pNewNode;
    }
    else
    {
        if (pOldNode != m_pNodeHead) return NULL;
        m_pNodeHead = pNewNode;
    }
    pOldNode->pPrev = pNewNode;
    return (POSITION_) pNewNode;
}


//---------------------------------------------------------------------------
// 인  수: position:삽입될 위치, newElement:삽입될 자료                     |
// 반환값: 삽입된 자료의 위치                                               |
// 설  명: position뒤에 새로운 자료를 삽입한다.                             |
//---------------------------------------------------------------------------
POSITION_ CVoidList::InsertAfter(POSITION_ position, void* newElement)
{
    if (position == NULL)
        return AddTail(newElement); // insert after nothing -> tail of the list

    // Insert it before position
    CNode* pOldNode = (CNode*) position;
    CNode* pNewNode = NewNode(pOldNode, pOldNode->pNext);
    pNewNode->data = newElement;

    if (pOldNode->pNext != NULL)
    {
        pOldNode->pNext->pPrev = pNewNode;
    }
    else
    {
        if (pOldNode != m_pNodeTail) return NULL;
        m_pNodeTail = pNewNode;
    }
    pOldNode->pNext = pNewNode;
    return (POSITION_) pNewNode;
}


//---------------------------------------------------------------------------
// 인  수: position:삭제될 위치                                             |
// 반환값: 없음                                                             |
// 설  명: positon에 있는 노드를 삭제한다.                                  |
//---------------------------------------------------------------------------
void CVoidList::RemoveAt(POSITION_ position)
{
    if (position == NULL) return;

    CNode* pOldNode = (CNode*) position;

    // remove pOldNode from list
    if (pOldNode == m_pNodeHead)
        m_pNodeHead = pOldNode->pNext;
    else
        pOldNode->pPrev->pNext = pOldNode->pNext;
    if (pOldNode == m_pNodeTail)
        m_pNodeTail = pOldNode->pPrev;
    else
        pOldNode->pNext->pPrev = pOldNode->pPrev;
    FreeNode(pOldNode);
}


/////////////////////////////////////////////////////////////////////////////
// slow operations

//---------------------------------------------------------------------------
// 인  수: nIndex:찾아야 할 Zero기본 색인                                   |
// 반환값: 찾아낸 위치 NULL이면 nInex가 색인 범위를 초과했음                |
// 설  명: 색인을 사용하여 자료를 찾는다.                                   |
//---------------------------------------------------------------------------
POSITION_ CVoidList::FindIndex(int nIndex) const
{
    if (nIndex < 0) return NULL;

    if (nIndex >= m_nCount)
        return NULL;        // went too far

    register CNode* pNode = m_pNodeHead;
    while (nIndex--)
        pNode = pNode->pNext;
    return (POSITION_) pNode;
}


//---------------------------------------------------------------------------
// 인  수: searchValue:찾아야 자료 포인터, startAfter:여기 다음부터 찾는다. |
// 반환값: 찾아낸 위치 NULL이면 못 찾은 것이다.                             |
// 설  명: 리스트에서 자료를 검색한다.                                      |
//---------------------------------------------------------------------------
POSITION_ CVoidList::Find(void* searchValue, POSITION_ startAfter) const
{
    register CNode* pNode = (CNode*) startAfter;

    if (pNode == NULL)
        pNode = m_pNodeHead;        // start at head
    else
        pNode = pNode->pNext;       // start after the one specified

    for (; pNode != NULL; pNode = pNode->pNext)
        if (pNode->data == searchValue)
            return (POSITION_) pNode;
    return NULL;
}



//---------------------------------------------------------------------------
// 인  수: pPrev:전노드, pNext:다음노드                                     |
// 반환값: 생성된 노드                                                      |
// 설  명: 노드를 새로 생성하고 노드의  가지를 설정한다.                    |
//---------------------------------------------------------------------------

//CVoidList::CNode*
CNode* 
CVoidList::NewNode(CNode* pPrev, CNode* pNext)
{
    CNode* pNode;

    pNode = new CNode();
	if (pNode == NULL) return NULL;

    pNode->pPrev = pPrev;
    pNode->pNext = pNext;
    pNode->data  = NULL;
    m_nCount++;
    if (m_nCount <= 0)
    {
        FreeNode(pNode);
        return NULL; // make sure we don't overflow
    }

    return pNode;
}



//---------------------------------------------------------------------------
// 인  수: pNode: 삭제될 노드                                               |
// 반환값: 없음                                                             |
// 설  명: 노드를 하나 삭제한다.                                            |
//---------------------------------------------------------------------------
//void CVoidList::FreeNode(CVoidList::CNode* pNode)
void CVoidList::FreeNode(CNode* pNode)
{
    if (pNode == NULL) return;
    FreeData((void*)(pNode->data));
	delete pNode;

    m_nCount--;
}


//---------------------------------------------------------------------------
// 인  수: pData: 삭제될 데이타                                             |
// 반환값: 없음                                                             |
// 설  명: 노드내 데이타를 삭제한다.(virtual 함수로 void*가 아닌 다른       |
//         클래스의 리스트로 구성할 경우 다시 만들어 져야 한다.)            |
//         이 함수는 순수가상함수이다.                                      |
//---------------------------------------------------------------------------
// Proto Type : void CVoidList::FreeData(void* pData)
void CVoidList::FreeData(void* pData)
{
	if (pData == NULL) return;
	delete pData;
}


//In
void CSectionList::FreeData( void* pData )
{
	if( pData == NULL ) return;

	Section_Info* pSectionInfo = (Section_Info*)pData;
	delete pSectionInfo;
	pSectionInfo = NULL;
}

void CNpcInfoList::FreeData( void* pData )
{
	if( pData == NULL ) return;

	Npc_Info*	pNpcInfo = (Npc_Info*)pData;
	delete pNpcInfo;
	pNpcInfo = NULL;
}
//