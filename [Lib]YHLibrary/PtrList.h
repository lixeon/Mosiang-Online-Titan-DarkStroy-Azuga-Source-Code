// PtrList.h: interface for the cPtrList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PTRLIST_H__D2A0B78A_A979_4176_A882_9DD41765AA95__INCLUDED_)
#define AFX_PTRLIST_H__D2A0B78A_A979_4176_A882_9DD41765AA95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


struct ___POSITION {};
typedef ___POSITION* PTRLISTPOS;


struct CPlex
{
	CPlex* pNext;
	void* data() { return this+1; }

	static CPlex* PASCAL Create(CPlex*& head, UINT nMax, UINT cbElement);

	void FreeDataChain();
};

class cPtrList  
{
protected:
	struct CNode
	{
		CNode* pNext;
		CNode* pPrev;
		void* data;
	};
public:

	cPtrList(int nBlockSize = 10);


	int GetCount() const { return m_nCount; }
	BOOL IsEmpty() const { return m_nCount == 0; }


	void*& GetHead();      
	void* GetHead() const;
	void*& GetTail();      
	void* GetTail() const;


	// get head or tail (and remove it) - don't call on empty list!
	void* RemoveHead();
	void* RemoveTail();

	PTRLISTPOS AddHead(void* newElement);
	PTRLISTPOS AddTail(void* newElement);


	void AddHead(cPtrList* pNewList);
	void AddTail(cPtrList* pNewList);


	void RemoveAll();


	PTRLISTPOS GetHeadPosition() const;
	PTRLISTPOS GetTailPosition() const;
	void*& GetNext(PTRLISTPOS& rPosition);		
	void* GetNext(PTRLISTPOS& rPosition) const;
	void*& GetPrev(PTRLISTPOS& rPosition);
	void* GetPrev(PTRLISTPOS& rPosition) const;


	void*& GetAt(PTRLISTPOS position);
	void* GetAt(PTRLISTPOS position) const;
	void SetAt(PTRLISTPOS pos, void* newElement);

	void RemoveAt(PTRLISTPOS& position);


	PTRLISTPOS InsertBefore(PTRLISTPOS position, void* newElement);
	PTRLISTPOS InsertAfter(PTRLISTPOS position, void* newElement);

	// helper functions (note: O(n) speed)
	PTRLISTPOS Find(void* searchValue, PTRLISTPOS startAfter = NULL) const;
	PTRLISTPOS FindIndex(int nIndex) const;
	BOOL Remove(void* pRemoveValue);


protected:
	CNode* m_pNodeHead;
	CNode* m_pNodeTail;
	int m_nCount;
	CNode* m_pNodeFree;
	struct CPlex* m_pBlocks;
	int m_nBlockSize;

	CNode* NewNode(CNode*, CNode*);
	void FreeNode(CNode*);

public:
	virtual ~cPtrList();

	// local typedefs for class templates
	//typedef void* BASE_TYPE;
	//typedef void* BASE_ARG_TYPE;

};


#define PTRLISTSEARCHSTART(List,DataType,ValueName)		{	\
				PTRLISTPOS pos = List.GetHeadPosition();	\
				DataType ValueName;					\
				while(pos)	{											\
					ValueName = (DataType)List.GetNext(pos);

#define PTRLISTSEARCHEND	}		\
				}


#endif // !defined(AFX_PTRLIST_H__D2A0B78A_A979_4176_A882_9DD41765AA95__INCLUDED_)
