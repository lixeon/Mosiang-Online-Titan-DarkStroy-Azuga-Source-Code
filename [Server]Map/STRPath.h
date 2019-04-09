// STRPath.h: interface for the CSTRPath class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRPATH_H__620980BD_DB7E_4812_B62C_23F31D0A8D1F__INCLUDED_)
#define AFX_STRPATH_H__620980BD_DB7E_4812_B62C_23F31D0A8D1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef BOOL (*pValidFunc)(int cellX, int cellY,CObject* pObject);

class CSTRNode
{
public:
	CSTRNode(){ Init(); }
	void Init()
	{
		memset(this, 0, sizeof(CSTRNode));
	}
	void Release()
	{
	}
	void SetXY(int x, int y)
	{
		cellX = x;
		cellY = y;
	}
	void Setghf(int g, int h, int f)
	{
		goal = g;
		heuristic = h;
		fithness = f;
	}
	int cellX;
	int cellY;

	int fithness;

	int heuristic;
	int goal;
	int cellNumber;
	int childNum;
	CSTRNode * parent;
	CSTRNode * child[8];
	CSTRNode * next;
};

class CSTRINFO
{
public:
	CSTRINFO(){}
	~CSTRINFO(){}

	//CSTRNode *	m_pOpen;			// The open list
	//CSTRNode *	m_pClosed;			// The closed list
	//CSTRNode *	m_pBest;			// The best node

	int m_nRows;
	
	int	m_srcX;
	int	m_srcY;
	int	m_DestX;
	int	m_DestY;
	int m_TargetCellNumber;				// 1-dimention number
};

struct CStackNode
{
	CSTRNode	* data;
	CStackNode	* next;
};

class CSTRPath  
{
	//CYHHashTable<CSTRNode> * m_pOpenList;
	//CYHHashTable<CSTRNode> * m_pClosedList;
	int m_nWidth;
	
	int	m_srcX;
	int	m_srcY;
	int	m_DestX;
	int	m_DestY;
	int m_TargetCellNumber;				// 1-dimention number

	//CSTRINFO STRINFO;
	int m_limitDepth;

	CMemoryPoolTempl<CSTRNode> * m_pSTRNodePool;
	CMemoryPoolTempl<CStackNode> * m_pStackNodePool;

	void PreInit(CSTRINFO * pSTROBJ, int sx, int sy, int dx, int dy);
	void FreeNode(CSTRNode * freeNode);
	CSTRNode * AllocNode();
	CSTRNode * m_pCurBestNode;
	CSTRNode * m_pOpenList;
	CSTRNode * m_pClosedList;
	CStackNode * m_pStack;
	void Traversal(CObject* pObject,CSTRNode * parent);
	void TraversalChild(CSTRNode * parent, CSTRNode * child);
	CSTRNode * GetInList(CSTRNode * pList, int key);
	void AddToOpenList(CSTRNode * addNode);

	void CalcWayPoint(VECTOR3 * pWayPointBuf, WORD buffCount, BYTE& wWayPointNum);
	DWORD CostNode();
	void SpreadNode(CSTRNode * node);

	//stack ope
	void Push(CSTRNode * addNode);
	CSTRNode * Pop();
	VECTOR3 tmpWayPoint[MAX_CHARTARGETPOSBUF_SIZE+50];
	WORD m_wCurDepth;
	WORD m_wLimiteDepth;
public:
	void SetWidth(int wid)
	{
		m_nWidth = wid;
	}
	

	CSTRPath();
	virtual ~CSTRPath();
	pValidFunc IsValidNode;
	void Release();
	BOOL FindPath(CObject* pObject,VECTOR3 * pSrcPos, VECTOR3 * pDestPos, VECTOR3 * pWayPointBuf, WORD buffCount, BYTE& wWayPointNum, WORD wDepth);
	BOOL GetBestNode();
	
};

#endif // !defined(AFX_STRPATH_H__620980BD_DB7E_4812_B62C_23F31D0A8D1F__INCLUDED_)
