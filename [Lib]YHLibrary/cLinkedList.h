// cLinkedList.h: interface for the cLinkedList class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _cLINKEDLIST_H_
#define _cLINKEDLIST_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef void* POS;

template <typename T>
class cLinkedList  
{
public:
	cLinkedList() : m_pHead(0), m_pTail(0), m_Count(0){}
	virtual ~cLinkedList(){ DeleteAll(); }
	
protected:
    typedef struct _tagNode
    {
        T           *pElement;
        _tagNode    *pPrev;
        _tagNode    *pNext;
        //_tagNode(){ pPrev=0; pElement=0; pNext=0; }
		//~_tagNode(){ if(pElement)delete pElement; }
    } NODE;
    
    NODE *m_pHead;  // Heade of linked list
    NODE *m_pTail;  // Tail of linked list
    int m_Count;  // pCurrent count of linked list


public:
    // Get element of head
    inline T* GetHead(void)
	{ 
		if( m_pHead )
			return m_pHead->pElement;

		return 0;
	}
    
    // Add to linked list head
    void AddHead(T *pElement)
    {
        NODE *newNode = new NODE;
        newNode->pPrev = 0;
        newNode->pElement = pElement;
        newNode->pNext = 0;
        
        if (0 == m_pHead)
        {
            // First element added
            m_pHead = newNode;
            m_pTail = m_pHead;
        }
        else {
            // Add new element at head
            m_pHead->pPrev = newNode;
            newNode->pNext = m_pHead;
            m_pHead = newNode;
        }
        ++m_Count;
    }
    
    // Delete head
    T* DeleteHead(void)
    {
		if (!m_pHead) return 0;

        T *pElement = 0;
        if (m_pHead->pNext == NULL)
        {
            pElement = m_pHead->pElement;
            
            delete m_pHead;
            m_pHead = m_pTail = 0;
            m_Count = 0;
        }
        else {
            NODE *pCurrent = m_pHead;
            m_pHead = m_pHead->pNext;
            m_pHead->pPrev = 0;
            
            pElement = pCurrent->pElement;
            
            delete pCurrent;
            --m_Count;
        }
        return pElement;
    }
    

    
    // Get element of tail
    inline T* GetTail(void)
	{
		if( m_pTail )
			return m_pTail->pElement;

		return 0;
	}
    
    // Add to linked list tail
    void AddTail(T *pElement)
    {
        NODE *newNode = new NODE;
        newNode->pPrev = 0;
        newNode->pElement = pElement;
        newNode->pNext = 0;
        
        if (0 == m_pTail)
        {
            // First pElement added
            m_pTail = newNode;
            m_pHead = m_pTail;
        }
        else
		{
            // Add new pElement at tail
            m_pTail->pNext = newNode;
            newNode->pPrev = m_pTail;
            m_pTail = newNode;
        }
        ++m_Count;
    }
    
    // Delete tail
    T* DeleteTail(void)
    {
		if (!m_pTail) return 0;

        T *pElement = 0;
        if (m_pTail->pPrev == NULL)
        {
            pElement = m_pTail->pElement;
            
            delete m_pTail;
            m_pTail = m_pHead= 0;
            m_Count = 0;
        }
        else
		{
            NODE *pCurrent = m_pTail;
            m_pTail = m_pTail->pPrev;
            m_pTail->pNext = 0;
            
            pElement = pCurrent->pElement;
            
            delete pCurrent;
            --m_Count;
        }
        return pElement;
    }
/*    
    // Insert element before index
    int InsertBefore(T *pElement, int index)
    {
        if (index == 0) AddHead(pElement);
        else {
            NODE *pAfter = 0;
            NODE *pCurrent = m_pHead;
            while (0 < index-- && pCurrent)
			{
                pAfter = pCurrent;
                pCurrent = pCurrent->pNext;
            }
        
            NODE *newNode = new NODE;
            newNode->pPrev = pCurrent->pPrev;
            newNode->pElement = pElement;
            newNode->pNext = pCurrent;
        
            pCurrent->pPrev = newNode;
            pAfter->pNext = newNode;
        
            m_Count++;
        }
        return 0;
    }
    
    // Insert element after index
    int InsertpAfter(T *pElement, int index)
    {
        if (index == m_Count-1) AddTail(pElement);
        else {
            NODE *pAfter = 0;
            NODE *pCurrent = m_pHead;
            while (0 < index-- && pCurrent) {
                pAfter = pCurrent;
                pCurrent = pCurrent->pNext;
            }
            
            NODE *newNode = new NODE;
            newNode->pPrev = pCurrent;
            newNode->pElement = pElement;
            newNode->pNext = pCurrent->pNext;
            
            pAfter->pPrev = newNode;
            pCurrent->pNext = newNode;
            
            m_Count++;
        }
        return 0;
    }
*/
    
    // Get first position of node
    inline POS GetFirstPos(void){ return (POS)m_pHead; }
    
    // Get last position of node
    inline POS GetLastPos(void){ return (POS)m_pTail; }
/*    
    // Pos to index
    int GetPosIndex(POS pos)
    {
        NODE *pCurrent = m_pHead;
        for (int i=0; pCurrent; i++)
        {
            if (pos == (POS)pCurrent) return i;
            pCurrent = pCurrent->pNext;
        }
        return -1;
    }
*/    
    // Index to pos
    POS GetIndexPos(int index)
    {
        NODE *pCurrent = m_pHead;
        while (0 < index-- && pCurrent)
            pCurrent = pCurrent->pNext;
        
        return (POS)pCurrent;
    }


    T* GetAtPos(POS& pos)
	{
        if (pos)
        {
            return ((NODE*)pos)->pElement;
        }

		return 0;
	}
    // Get previous position of node
    T* GetPrevPos(POS& pos)
    {
        T *pElement;
        if (pos)
        {
            pElement = ((NODE*)pos)->pElement;
            pos = (POS)((NODE*)pos)->pPrev;
        }
        else {
            pElement = 0;
//            pos = 0;
        }
        return pElement;
    }
    
    // Get next position of node
    T* GetNextPos(POS& pos)
    {
        T *pElement;
        if (pos)
        {
            pElement = ((NODE*)pos)->pElement;
            pos = (POS)((NODE*)pos)->pNext;
        }
        else {
            pElement = 0;
//            pos = 0;
        }
        return pElement;
    }

/*    
    // Delete position of node
    T* DeletePos(POS& pos)
    {
        T *pElement = 0;
        if (m_pHead == (NODE*)pos)
        {
            pElement = DeleteHead();
//            pos = (POS)m_pHead;
        }
        else if (m_pTail == (NODE*)pos)
        {
            pElement = DeleteTail();
//            pos = (POS)m_pTail;
        }
        else {
            NODE *pAfter = m_pHead;
            NODE *pCurrent = m_pHead->pNext;
            while (pCurrent != (NODE*)pos)
            {
                pAfter = pCurrent;
                pCurrent = pCurrent->pNext;
            }
            pCurrent->pNext->pPrev = pAfter;
            pAfter->pNext = pCurrent->pNext;
            pos = (POS)pAfter;
            
            pElement = pCurrent->pElement;
            
            delete pCurrent;
            m_Count--;
        }
        return pElement;
    }
*/   
    
    // Get total number of node
    inline int GetCount(void){ return m_Count; }
    
    // Get node by index number : 0base
    T* GetAt(int index)
    {
        NODE *pCurrent = m_pHead;
        while (0 < index-- && pCurrent)
            pCurrent = pCurrent->pNext;
        
		if( pCurrent )
			return pCurrent->pElement;

		return 0;
    }
    
    // Delete at
    T* DeleteAt(int index)
    {
		if( index < 0 || index >= m_Count )
			return 0;
		
        if (index == 0) return DeleteHead();
        else if (index == m_Count-1) return DeleteTail();
        else {
            NODE *pAfter = m_pHead;
            NODE *pCurrent = m_pHead->pNext;
            while (--index && pCurrent)
            {
                pAfter = pCurrent;
                pCurrent = pCurrent->pNext;
            }
			if( pCurrent )
			{
	            pCurrent->pNext->pPrev = pAfter;
		        pAfter->pNext = pCurrent->pNext;
            
			    T *pElement = pCurrent->pElement;
            
	            delete pCurrent;
		        --m_Count;
            
			    return pElement;
			}
        }
        return 0;
    }
    
    // Delete all node
    void DeleteAll(void)
    {
        NODE *pCurrent = m_pHead;
        NODE *pAfter = 0;
        while (pCurrent)
        {
            pAfter = pCurrent->pNext;
            delete pCurrent;
            pCurrent = pAfter;
        }
        m_pHead = m_pTail = 0;
        m_Count = 0;
    }

};

#endif // _cLINKEDLIST_H_
