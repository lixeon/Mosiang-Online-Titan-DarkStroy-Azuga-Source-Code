// STRPath.cpp: implementation of the CSTRPath class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "STRPath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define LinearNumber(x, y, row)		x*row+y

CSTRPath::CSTRPath()
{
	m_pSTRNodePool = new CMemoryPoolTempl<CSTRNode>;
	m_pSTRNodePool->Init(1000, 500,"CSTRNode");
	m_pStackNodePool = new CMemoryPoolTempl<CStackNode>;
	m_pStackNodePool->Init(1000, 500,"CStackNode");
	m_pOpenList = NULL;
	m_pClosedList = NULL;
	m_pStack = NULL;
	m_pCurBestNode = NULL;
/*
	m_pOpenList = new CYHHashTable<CSTRNode>;
	m_pClosedList = new CYHHashTable<CSTRNode>;
	m_pOpenList->Initialize(500);
	m_pClosedList->Initialize(500);*/

	m_wCurDepth = 0;
}

CSTRPath::~CSTRPath()
{
	Release();

/*
	if(m_pOpenList)
	{
		m_pOpenList->RemoveAll();
		delete m_pOpenList;
	}
	if(m_pClosedList)
	{
		m_pClosedList->RemoveAll();
		delete m_pClosedList;
	}*/

	if(m_pStackNodePool)
	{
		m_pStackNodePool->Release();
		delete m_pStackNodePool;
	}

	if(m_pSTRNodePool)
	{
		m_pSTRNodePool->Release();
		delete m_pSTRNodePool;
	}
}
void CSTRPath::Release()
{
	CSTRNode * temp = NULL, * temp2 = NULL;
	if (m_pOpenList) 
	{
		while (m_pOpenList) 
		{
			temp = m_pOpenList->next;
			FreeNode(m_pOpenList);
			m_pOpenList = temp;
		}
		m_pOpenList = NULL;
	}

	if (m_pClosedList) 
	{
		while (m_pClosedList) 
		{
			temp = m_pClosedList->next;
			FreeNode(m_pClosedList);
			m_pClosedList = temp;
		}
		m_pClosedList = NULL;
	}
/*
	if(m_pOpenList)
	{
		m_pOpenList->RemoveAll();
	}
	if(m_pClosedList)
	{
		m_pClosedList->RemoveAll();
	}*/

}
void CSTRPath::FreeNode(CSTRNode * freeNode)
{
	freeNode->Release();
	m_pSTRNodePool->Free(freeNode);
}
CSTRNode * CSTRPath::AllocNode()
{
	CSTRNode * tmp = m_pSTRNodePool->Alloc();
	tmp->Init();
	return tmp;
}
void CSTRPath::PreInit(CSTRINFO * pSTRINFO, int sx, int sy, int dx, int dy)
{
	Release();
/*
	pSTRINFO->m_srcX = sx;
	pSTRINFO->m_srcY = sy;
	pSTRINFO->m_DestX = dx;
	pSTRINFO->m_DestY = dy;
	pSTRINFO->m_TargetCellNumber = LinearNumber(dx, dy, pSTRINFO->m_nWidth);*/
	
	m_srcX = sx;
	m_srcY = sy;
	m_DestX = dx;
	m_DestY = dy;
	m_TargetCellNumber = LinearNumber(dx, dy, m_nWidth);
	
	CSTRNode * NewNode = AllocNode();
	NewNode->SetXY(sx, sy);
	NewNode->Setghf(0, abs(dx-sx) + abs(dy-sy), abs(dx-sx) + abs(dy-sy));
	NewNode->cellNumber = LinearNumber(sx, sy, m_nWidth);

	// add open list
	//ASSERT(!m_pOpenList->GetData(NewNode->cellNumber));
	//m_pOpenList->Add(NewNode, NewNode->cellNumber);
	
	// open list 에서 add
	m_pOpenList = NewNode;
	++m_wCurDepth;
}

BOOL CSTRPath::GetBestNode()
{
	m_pCurBestNode = m_pOpenList;
	if(!m_pCurBestNode) 
		return FALSE;

	// open list 에서 remove
	ASSERT(m_wCurDepth > 0);
	m_pOpenList = m_pOpenList->next;
	if(m_pOpenList)
		--m_wCurDepth;
	ASSERT(m_wCurDepth >= 0);
	CSTRNode * tmp = m_pCurBestNode;
	tmp->next = m_pClosedList;
	m_pClosedList = tmp;

	return TRUE;
}


BOOL CSTRPath::FindPath(VECTOR3 * pSrcPos, VECTOR3 * pDestPos, VECTOR3 * pWayPointBuf, WORD buffCount, BYTE& wWayPointNum, WORD wDepth)
{
	m_wCurDepth = 0;
	m_wLimiteDepth = wDepth;

	PreInit(NULL, (int)(pSrcPos->x/50), (int)(pSrcPos->z/50), (int)(pDestPos->x/50), (int)(pDestPos->z/50));

	while(GetBestNode())
	{
		if(m_pCurBestNode->cellNumber == m_TargetCellNumber)
			break;

		if(m_wLimiteDepth <= m_wCurDepth)
			break;

		Traversal(m_pCurBestNode);
	}

	if(!m_pCurBestNode)
		return FALSE;

	CalcWayPoint(pWayPointBuf, buffCount, wWayPointNum);

	return TRUE;
}

void CSTRPath::CalcWayPoint(VECTOR3 * pWayPointBuf, WORD buffCount, BYTE& wWayPointNum)
{
	int dex = 0;
	int dey = 0;

	// 임시- openlist에서 point결정???
	WORD wtmpWayPointNum = 0;

	CSTRNode * cur = m_pCurBestNode;
	CSTRNode * next = m_pCurBestNode->parent;
	ASSERT(next);
	while(next)
	{
		if(dex != next->cellX - cur->cellX || dey != next->cellY - cur->cellY)
		{
			tmpWayPoint[wtmpWayPointNum].x  = (float)(cur->cellX*50);
			tmpWayPoint[wtmpWayPointNum].y  = 0;
			tmpWayPoint[wtmpWayPointNum++].z  = (float)(cur->cellY*50);
			dex = next->cellX - cur->cellX;
			dey = next->cellY - cur->cellY;
		}
		cur = next;
		next = cur->parent;
	}
	// 임시- openlist에서 point결정???
	int ri = wtmpWayPointNum - 1;
	for(int i = 0 ; i < wtmpWayPointNum && i < buffCount ; ++i, --ri)
	{
		pWayPointBuf[i] = tmpWayPoint[ri];
	}
	wWayPointNum = i;
	
}

void CSTRPath::Traversal(CSTRNode * parent)
{
	CSTRNode child;
	int x = parent->cellX, y = parent->cellY;

	for( int i = -1 ; i < 2 ; ++i )
	{
		for( int j = -1 ; j < 2 ; ++j ) 
		{
			child.cellX = x+i;
			child.cellY = y+j;
			if (i == 0 && j == 0 || !IsValidNode(child.cellX, child.cellY, NULL)) 
				continue;
			TraversalChild(parent, &child);
		}
	}
}

CSTRNode * CSTRPath::GetInList(CSTRNode * pList, int key)
{
	//* 해쉬로 검색하는 방법 : 열린 목록, 닫힌 목록
	while (pList) 
	{
		if (pList->cellNumber == key) 
			return pList;
		pList = pList->next;
	//	return NULL;
	}
	return NULL;
}

DWORD CSTRPath::CostNode()
{
	return 1;
}

void CSTRPath::TraversalChild(CSTRNode * parent, CSTRNode * child)
{
	int x = child->cellX;
	int y = child->cellY;
	int g = parent->goal + CostNode();
	int num = LinearNumber(x, y, m_nWidth);

	CSTRNode * check = NULL;


	if (check = GetInList(m_pOpenList, num)) 
	{
		parent->child[parent->childNum++] = check;


		if(g < check->goal)
		{
			check->parent = parent;
			check->goal = g;
			check->fithness = g + check->heuristic;
		} 
	}
	else if (check = GetInList(m_pClosedList, num)) 
	{
		parent->child[parent->childNum++] = check;

		if(g < check->goal)
		{
			check->parent = parent;
			check->goal = g;
			check->fithness = g + check->heuristic;
			SpreadNode(check);
		} 
	} 
	else 
	{
		// 메모리 풀 사용
		CSTRNode * newnode = AllocNode();
		newnode->SetXY(x, y);
		newnode->parent			= parent;
		newnode->goal			= g;
		newnode->heuristic		= abs(x-m_DestX) + abs(y-m_DestY);
		newnode->fithness		= newnode->goal + newnode->heuristic;
		newnode->cellNumber		= LinearNumber(x, y, m_nWidth);
		newnode->next = NULL;

		AddToOpenList(newnode);

		parent->child[parent->childNum++] = newnode;
	}	
}

void CSTRPath::AddToOpenList(CSTRNode * addNode)
{
	CSTRNode * node = m_pOpenList;
	CSTRNode * prev = NULL;

	if (!m_pOpenList) 
	{
		// open list 에서 add
		m_pOpenList = addNode;
		m_pOpenList->next = NULL;
		++m_wCurDepth;
		return;
	}

	while(node) 
	{
		if( addNode->fithness > node->fithness ) 
		{
			prev = node;
			node = node->next;
			// open list 에서 add
		} 
		else 
		{
			if(prev) 
			{
				// open list 에서 add
				prev->next = addNode;
				addNode->next = node;
			} 
			else 
			{
				// open list 에서 add
				CSTRNode * temp = m_pOpenList;
				m_pOpenList = addNode;
				m_pOpenList->next = temp;
			}
			++m_wCurDepth;
			return;
		}
	}

	prev->next = addNode;
	++m_wCurDepth;
}


void CSTRPath::SpreadNode(CSTRNode * node)
{
	int g = node->goal;
	int cnum = node->childNum;

	//* 자식 노드 모두 업데이트
	CSTRNode * child = NULL;
	for( int i = 0 ; i < cnum ; ++i ) 
	{
		child = node->child[i];
		if( g+1 < child->goal ) 
		{
			child->goal = g+1;
			child->fithness = child->goal + child->heuristic;
			child->parent = node;
			
			Push(child);
		}
	}

	//* 자식의 자식들 업데이트
	CSTRNode * parent;
	while(m_pStack) 
	{
		parent = Pop();
		cnum = parent->childNum;
		for( int i = 0 ; i < cnum ; ++i ) 
		{
			child = parent->child[i];
			
			if( parent->goal+1 < child->goal ) 
			{
				child->goal = parent->goal + CostNode();
				child->fithness = child->goal + child->heuristic;
				child->parent = parent;

				Push(child);
			}
		}
	}
}

void CSTRPath::Push(CSTRNode * node)
{

	if (!m_pStack) 
	{
		m_pStack = m_pStackNodePool->Alloc();
		m_pStack->data = node;
		m_pStack->next = NULL;
	} 
	else 
	{
		CStackNode * temp = m_pStackNodePool->Alloc();
		temp->data = node;
		temp->next = m_pStack;
		m_pStack = temp;
	}
}

CSTRNode * CSTRPath::Pop() 
{
	CSTRNode * data = m_pStack->data;
	CStackNode * temp = m_pStack;
	m_pStack = temp->next;

	m_pStackNodePool->Free(temp);

	return data;
}
