// GridUnit.h: interface for the CGridUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDUNIT_H__83C9510E_310C_456B_97AB_5033124AB8ED__INCLUDED_)
#define AFX_GRIDUNIT_H__83C9510E_310C_456B_97AB_5033124AB8ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct GridPosition
{
	WORD x;
	WORD z;
	WORD LastX;
	WORD LastZ;
};

class CGridUnit  
{
protected:
	BOOL m_bGridInited;
	BOOL m_bNeededToCalcGrid;
	DWORD m_GridID;
	GridPosition GridPos;
	
	friend class CGridSystem;
	friend class CGridTable;
	friend class CGeneralGridTable;


public:
	inline GridPosition* GetGridPosition()	{	return &GridPos;	}

	CGridUnit();
	virtual ~CGridUnit();

	virtual void SetInitedGrid()	{	m_bGridInited = TRUE;	}
	BOOL GetInitedGrid()	{	return m_bGridInited;	}
	inline DWORD GetGridID()	{	return m_GridID;	}
	inline void SetGridID(DWORD ID)	{	m_GridID = ID;	}

};

#endif // !defined(AFX_GRIDUNIT_H__83C9510E_310C_456B_97AB_5033124AB8ED__INCLUDED_)
