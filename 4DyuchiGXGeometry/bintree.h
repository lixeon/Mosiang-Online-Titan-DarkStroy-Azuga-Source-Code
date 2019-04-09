#ifndef BINTREE_H
#define BINTREE_H

#include "../4DyuchiGRX_Common/stdafx.h"


struct NODE
{
	DWORD	dwKey;
	NODE*	pLeft;
	NODE*	pRight;
	void*	pItem;
};


class CBinTree
{	
	NODE*	m_pRootNode;

	// node pool
	DWORD	m_dwMaxNodeNum;
	NODE*	m_pNodeList;
	DWORD	m_dwAllocNodeNum;

	NODE*	AllocNode();

public:
	CBinTree();
	~CBinTree();

	BOOL	Initialize(DWORD dwMaxNodeNum);

	BOOL	InsertNode(DWORD dwKey,void* pItem);
	int		SearchItem(void** ppPrv,void** ppCur,DWORD dwKey);
	NODE*	SearchNode(DWORD dwKey);

};


#endif

