#ifndef CIRCLE_H
#define CIRCLE_H

#include "../4DyuchiGRX_Common/typedef.h"

class CCircle
{
	VECTOR4*			m_pv4Point;
	WORD*				m_pIndex;
	DWORD				m_dwPicesNum;
public:
	WORD*				GetIndexList() {return m_pIndex;}
	VECTOR4*			GetPointList() {return m_pv4Point;}
	DWORD				GetVerticesNum() {return m_dwPicesNum;}
	DWORD				GetLinesNum() {return m_dwPicesNum;}

	void				Initialize(DWORD dwPicesNum);
	void				BuildMesh(VECTOR2* pv2Point,float fRs);
	CCircle();
	~CCircle();
};

#endif 