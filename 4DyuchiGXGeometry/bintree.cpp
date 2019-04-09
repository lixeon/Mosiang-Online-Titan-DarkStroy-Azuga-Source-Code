#include "bintree.h"


CBinTree::CBinTree()
{
	memset(this,0,sizeof(CBinTree));
}

NODE* CBinTree::AllocNode()
{
	NODE*	pNode = NULL;
	if (m_dwAllocNodeNum >= m_dwMaxNodeNum)
		goto lb_return;

	pNode = m_pNodeList + m_dwAllocNodeNum;
	
	pNode->pLeft = NULL;
	pNode->pRight = NULL;

	m_dwAllocNodeNum++;
lb_return:
	return pNode;

}

BOOL CBinTree::Initialize(DWORD dwMaxNodeNum)
{

	m_pNodeList = new NODE[dwMaxNodeNum];
	memset(m_pNodeList,0,sizeof(NODE)*dwMaxNodeNum);
	m_dwMaxNodeNum = dwMaxNodeNum;


	return TRUE;
}

BOOL CBinTree::InsertNode(DWORD dwKey,void* pItem)
{
	BOOL	bResult = FALSE;

	NODE*	pParent;
	NODE*	pCur = AllocNode();
	if (!pCur)
		goto lb_false;

	pCur->dwKey = dwKey;
	pCur->pItem = pItem;

	if (!m_pRootNode)
	{
		m_pRootNode = pCur;
		bResult = TRUE;
		goto lb_return;
		
	}

	pParent = m_pRootNode;
	
	
	

lb_check_left:
	if (dwKey <= pParent->dwKey)
	{
		if (!pParent->pLeft)
		{
			pParent->pLeft = pCur;
			goto lb_return;
		}
		else
		{
			pParent = pParent->pLeft;
			goto lb_check_left;
		}
	}
lb_check_right:
	if (dwKey > pParent->dwKey)
	{
		if (!pParent->pRight)
			pParent->pRight = pCur;
		else
		{
			pParent = pParent->pRight;
			goto lb_check_left;
		}
	}
	
lb_return:
	bResult = TRUE;

lb_false:
	return bResult;

}

NODE* CBinTree::SearchNode(DWORD dwKey)
{
	NODE*	pParent = m_pRootNode;
	NODE*	pResult = NULL;

lb_search_left:
	if (!pParent || pParent->dwKey == dwKey)
		goto lb_return;

	if (dwKey < pParent->dwKey)
	{
		pParent = pParent->pLeft;
		goto lb_search_left;
	}

	if (dwKey > pParent->dwKey)
	{
		pParent = pParent->pRight;
		goto lb_search_left;
	}
lb_return:
	return pParent;
}

int CBinTree::SearchItem(void** ppPrv,void** ppCur,DWORD dwKey)
{
	NODE*	pCur = m_pRootNode;
	NODE*	pTemp;
	NODE*	pPrv = NULL;
	*ppCur = NULL;
	*ppPrv = NULL;
	
	int		iResult;

lb_search_left:
	if (pCur->dwKey == dwKey)
	{
		*ppCur = pCur->pItem;
		iResult = 0;
		goto lb_return;
	}

	if (dwKey < pCur->dwKey)
	{
		if (!pCur->pLeft)
		{
			pPrv = m_pRootNode;
			goto lb_set_seq;
		}
		
		pPrv = pCur;
		pCur = pCur->pLeft;
		goto lb_search_left;
	}

	if (dwKey > pCur->dwKey)
	{
		if (!pCur->pRight)
			goto lb_set_seq;
		
		pPrv = pCur;
		pCur = pCur->pRight;
				
		goto lb_search_left;
	}
lb_set_seq:
	if (pPrv)
	{
		// 끝까지 왔으나 값이 일치하는건 없으므로 보간에 필요한 두노드의 순서를 맞춘다.
		if (pCur->dwKey < pPrv->dwKey)
		{
			pTemp = pCur;
			pCur = pPrv;
			pPrv = pTemp;
		}
		*ppCur = pCur->pItem;
		*ppPrv = pPrv->pItem;
		iResult = 1;
	}
	else
	{
		// 노드가 딸랑 한개인 경우가 해당된다.보간을 할수 없다.
		*ppCur = pCur->pItem;
		iResult = -1;
	}
lb_return:

	return iResult;
}

CBinTree::~CBinTree()
{
	if (m_pNodeList)
	{
		delete [] m_pNodeList;
		m_pNodeList = NULL;
	}

}
