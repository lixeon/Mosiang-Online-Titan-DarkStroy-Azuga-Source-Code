#ifndef __POINTERLIST_INCLUDE__
#define __POINTERLIST_INCLUDE__


#ifndef NULL
#define NULL (0)
#endif

#include "windows.h"


typedef void* POSITION_;

struct CNode
{
	CNode*  pNext;
	CNode*  pPrev;
	void*    data;
};

class CVoidList
{
public:
/*
	struct CNode
	{
		CNode*  pNext;
		CNode*  pPrev;
		void*    data;
	};
*/
    CVoidList();

// Attributes (head and tail)
    // count of elements
    int     GetCount() const
                { return m_nCount; }
    int     IsEmpty() const // RetVal: TRUE or FALSE
                { return m_nCount == 0; }

    // peek at head or tail
    void*    GetHead() const
                { if (m_pNodeHead == NULL) return NULL;
                    return m_pNodeHead->data; }
    void*    GetTail() const
                { if (m_pNodeTail == NULL) return NULL;
                    return m_pNodeTail->data; }

// Operations
    // get head or tail (and remove it) - don't call on empty list !
    void*    RemoveHead();
    void*    RemoveTail();

    // add before head or after tail
    POSITION_ AddHead(void* newElement);
    POSITION_ AddTail(void* newElement);

    // add another list of elements before head or after tail
    void    AddHead(CVoidList* pNewList);
    void    AddTail(CVoidList* pNewList);

    // remove all elements
    void    RemoveAll();

    // iteration
    POSITION_ GetHeadPosition() const
                { return (POSITION_) m_pNodeHead; }
    POSITION_ GetTailPosition() const
                { return (POSITION_) m_pNodeTail; }

    void*     SwapData(POSITION_& rPosition1,
                      POSITION_& rPosition2)
    {
        void* pData;
        CNode* pNode1, *pNode2;
        pNode1 = (CNode*) rPosition1;
        pNode2 = (CNode*) rPosition2;
        if (pNode1 == NULL || pNode2 == NULL) return NULL;

        pData = pNode1->data;
        pNode1->data = pNode2->data;
        pNode2->data = pData;

        return pData; 
    }
    void*    GetNext(POSITION_& rPosition) const // return *Position++
                { CNode* pNode = (CNode*) rPosition;
                    if (pNode == NULL) return NULL;
                    rPosition = (POSITION_) pNode->pNext;
                    return pNode->data; }
    void*    GetPrev(POSITION_& rPosition) const // return *Position--
                { CNode* pNode = (CNode*) rPosition;
                    if (pNode == NULL) return NULL;
                    rPosition = (POSITION_) pNode->pPrev;
                    return pNode->data; }

    // getting/modifying an element at a given position
    void*   GetAt(POSITION_ position) const
                { CNode* pNode = (CNode*) position;
                    if (pNode == NULL) return NULL;
                    return pNode->data; }
    void    SetAt(POSITION_ pos, void* newElement)
                { CNode* pNode = (CNode*) pos;
                    if (pNode == NULL) return;
                    pNode->data = newElement; }
    void    RemoveAt(POSITION_ position);

    // inserting before or after a given position
    POSITION_ InsertBefore(POSITION_ position, void* newElement);
    POSITION_ InsertAfter(POSITION_ position, void* newElement);

    // helper functions (note: O(n) speed)
    POSITION_ Find(void* searchValue, POSITION_ startAfter = NULL) const;
                        // defaults to starting at the HEAD
                        // return NULL if not found
    POSITION_ FindIndex(int nIndex) const;
                        // get the 'nIndex'th element (may return NULL)
	virtual void    FreeNode(CNode*);

// Implementation
protected:
    CNode*  m_pNodeHead;
    CNode*  m_pNodeTail;
    int     m_nCount;


	CNode*  NewNode(CNode*, CNode*);
protected:

	virtual void    FreeData(void* pData);

public:
    unsigned char m_bFreeData;
    ~CVoidList();
};


//In
#pragma pack(push, 1)

struct Section_Info
{
	WORD	wSectionNum;
	RECT	rcSection;
	LPWORD	pwTNum;

	Section_Info()
	{
		memset( this, 0, sizeof( Section_Info ) );
	}

	~Section_Info()
	{
		if( pwTNum )
		{
			delete[] pwTNum;
			pwTNum = NULL;
		}
	}
};

struct Npc_Info
{
	WORD	wNpcID;
	WORD	wPosX;
	WORD	wPosZ;

	//------
};

#pragma pack(pop)

class CSectionList : public CVoidList
{

protected:
	virtual void FreeData( void* pData );
};

class CNpcInfoList : public CVoidList
{
protected:
	virtual void FreeData( void* pData );
};
//
#endif
