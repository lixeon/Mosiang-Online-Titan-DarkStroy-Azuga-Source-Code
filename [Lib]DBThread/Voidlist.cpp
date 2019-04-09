#include "stdafx.h"
#include "voidlist.h"


//---------------------------------------------------------------------------
// ÀÎ  ¼ö: ¾øÀ½                                                             |
// ¹ÝÈ¯°ª: ¾øÀ½                                                             |
// ¼³  ¸í: »ý¼ºÀÚ                                                           |
//---------------------------------------------------------------------------
CVoidList::CVoidList()
{
    m_nCount = 0;
    m_bFreeData = 1;
    m_pNodeHead = m_pNodeTail = NULL;
}


//---------------------------------------------------------------------------
// ÀÎ  ¼ö: ¾øÀ½                                                             |
// ¹ÝÈ¯°ª: ¾øÀ½                                                             |
// ¼³  ¸í: ÆÄ±«ÀÚ                                                           |
//---------------------------------------------------------------------------
CVoidList::~CVoidList()
{
    RemoveAll();
}
void CVoidList::RemoveList()
{
	m_nCount = 0;
    m_pNodeHead = NULL;
	m_pNodeTail = NULL;
}



//---------------------------------------------------------------------------
// ÀÎ  ¼ö: ¾øÀ½                                                             |
// ¹ÝÈ¯°ª: ¾øÀ½                                                             |
// ¼³  ¸í: ¸ðµç ³ëµåÀÇ ¸Þ¸ð¸®¸¦ ÇØÁ¦ÇÑ´Ù.                                   |
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


//---------------------------------------------------------------------------
// ÀÎ  ¼ö: pPrev:Àü³ëµå, pNext:´ÙÀ½³ëµå                                     |
// ¹ÝÈ¯°ª: »ý¼ºµÈ ³ëµå                                                      |
// ¼³  ¸í: ³ëµå¸¦ »õ·Î »ý¼ºÇÏ°í ³ëµåÀÇ  °¡Áö¸¦ ¼³Á¤ÇÑ´Ù.                    |
//---------------------------------------------------------------------------
CVoidList::CNode*
CVoidList::NewNode(CVoidList::CNode* pPrev, CVoidList::CNode* pNext)
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
// ÀÎ  ¼ö: pData: »èÁ¦µÉ µ¥ÀÌÅ¸                                             |
// ¹ÝÈ¯°ª: ¾øÀ½                                                             |
// ¼³  ¸í: ³ëµå³» µ¥ÀÌÅ¸¸¦ »èÁ¦ÇÑ´Ù.(virtual ÇÔ¼ö·Î void*°¡ ¾Æ´Ñ ´Ù¸¥       |
//         Å¬·¡½ºÀÇ ¸®½ºÆ®·Î ±¸¼ºÇÒ °æ¿ì ´Ù½Ã ¸¸µé¾î Á®¾ß ÇÑ´Ù.)            |
//         ÀÌ ÇÔ¼ö´Â ¼ø¼ö°¡»óÇÔ¼öÀÌ´Ù.                                      |
//---------------------------------------------------------------------------
// Proto Type : void CVoidList::FreeData(void* pData)


//---------------------------------------------------------------------------
// ÀÎ  ¼ö: pNode: »èÁ¦µÉ ³ëµå                                               |
// ¹ÝÈ¯°ª: ¾øÀ½                                                             |
// ¼³  ¸í: ³ëµå¸¦ ÇÏ³ª »èÁ¦ÇÑ´Ù.                                            |
//---------------------------------------------------------------------------
void CVoidList::FreeNode(CVoidList::CNode* pNode)
{
    if (pNode == NULL) return;
    FreeData((void*)(pNode->data));
    delete pNode;

    m_nCount--;
}


/////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------
// ÀÎ  ¼ö: newElement:ÀÚ·á Æ÷ÀÎÅÍ                                           |
// ¹ÝÈ¯°ª: »õ·Î¿î ÀÚ·áÀÇ Position Pointer                                   |
// ¼³  ¸í: ÀÚ·á¸¦ HeadÀ§Ä¡¿¡ »ðÀÔÇÏ¿© Head¸¦ newElement·Î ±³Ã¼ÇÑ´Ù.         |
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
// ÀÎ  ¼ö: newElement:ÀÚ·á Æ÷ÀÎÅÍ                                           |
// ¹ÝÈ¯°ª: »õ·Î¿î ÀÚ·áÀÇ Position Pointer                                   |
// ¼³  ¸í: ÀÚ·á¸¦ TailÀ§Ä¡¿¡ »ðÀÔÇÏ¿© TailÀ» newElement·Î ±³Ã¼ÇÑ´Ù.         |
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
// ÀÎ  ¼ö: pNewList:´Ù¸¥ ¸®½ºÆ®                                             |
// ¹ÝÈ¯°ª: ¾øÀ½                                                             |
// ¼³  ¸í: ´Ù¸¥ ¸®½ºÆ®ÀÇ Tail°ªÀ» ¸®½ºÆ®ÀÇ Head¿¡ »ðÀÔÇÑ´Ù.                 |
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
// ÀÎ  ¼ö: pNewList:´Ù¸¥ ¸®½ºÆ®                                             |
// ¹ÝÈ¯°ª: ¾øÀ½                                                             |
// ¼³  ¸í: ´Ù¸¥ ¸®½ºÆ®ÀÇ Head°ªÀ» ¸®½ºÆ®ÀÇ Tail¿¡ »ðÀÔÇÑ´Ù.                 |
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
// ÀÎ  ¼ö: ¾øÀ½                                                             |
// ¹ÝÈ¯°ª: ´ÙÀ½ ³ëµåÀÇ ÀÚ·á                                                 |
// ¼³  ¸í: Head³ëµå¸¦ »èÁ¦ÇÑ´Ù.                                             |
//---------------------------------------------------------------------------
void* CVoidList::RemoveHead()
{
    if (m_pNodeHead == NULL) return NULL;    // don't call on empty list !!!

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
// ÀÎ  ¼ö: ¾øÀ½                                                             |
// ¹ÝÈ¯°ª: ´ÙÀ½ ³ëµåÀÇ ÀÚ·á                                                 |
// ¼³  ¸í: Tail³ëµå¸¦ »èÁ¦ÇÑ´Ù.                                             |
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
// ÀÎ  ¼ö: position:»ðÀÔµÉ À§Ä¡, newElement:»ðÀÔµÉ ÀÚ·á                     |
// ¹ÝÈ¯°ª: »ðÀÔµÈ ÀÚ·áÀÇ À§Ä¡                                               |
// ¼³  ¸í: position¾Õ¿¡ »õ·Î¿î ÀÚ·á¸¦ »ðÀÔÇÑ´Ù.                             |
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
// ÀÎ  ¼ö: position:»ðÀÔµÉ À§Ä¡, newElement:»ðÀÔµÉ ÀÚ·á                     |
// ¹ÝÈ¯°ª: »ðÀÔµÈ ÀÚ·áÀÇ À§Ä¡                                               |
// ¼³  ¸í: positionµÚ¿¡ »õ·Î¿î ÀÚ·á¸¦ »ðÀÔÇÑ´Ù.                             |
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
// ÀÎ  ¼ö: position:»èÁ¦µÉ À§Ä¡                                             |
// ¹ÝÈ¯°ª: ¾øÀ½                                                             |
// ¼³  ¸í: positon¿¡ ÀÖ´Â ³ëµå¸¦ »èÁ¦ÇÑ´Ù.                                  |
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

//---------------------------------------------------------------------------
// ÀÎ  ¼ö: position:»èÁ¦µÉ À§Ä¡                                             |
// ¹ÝÈ¯°ª: ¾øÀ½                                                             |
// ¼³  ¸í: positon¿¡ ÀÖ´Â ³ëµå¸¦ ¸®½ºÆ®¿¡¼­ »«´Ù. 
// add by slowboat 2001.1.11                                 |
//---------------------------------------------------------------------------
void CVoidList::RemoveAtFromList(POSITION_ position)
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

	m_nCount--;
    delete pOldNode;

 //   FreeNode(pOldNode);
}

/////////////////////////////////////////////////////////////////////////////
// slow operations

//---------------------------------------------------------------------------
// ÀÎ  ¼ö: nIndex:Ã£¾Æ¾ß ÇÒ Zero±âº» »öÀÎ                                   |
// ¹ÝÈ¯°ª: Ã£¾Æ³½ À§Ä¡ NULLÀÌ¸é nInex°¡ »öÀÎ ¹üÀ§¸¦ ÃÊ°úÇßÀ½                |
// ¼³  ¸í: »öÀÎÀ» »ç¿ëÇÏ¿© ÀÚ·á¸¦ Ã£´Â´Ù.                                   |
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
// ÀÎ  ¼ö: searchValue:Ã£¾Æ¾ß ÀÚ·á Æ÷ÀÎÅÍ, startAfter:¿©±â ´ÙÀ½ºÎÅÍ Ã£´Â´Ù. |
// ¹ÝÈ¯°ª: Ã£¾Æ³½ À§Ä¡ NULLÀÌ¸é ¸ø Ã£Àº °ÍÀÌ´Ù.                             |
// ¼³  ¸í: ¸®½ºÆ®¿¡¼­ ÀÚ·á¸¦ °Ë»öÇÑ´Ù.                                      |
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
