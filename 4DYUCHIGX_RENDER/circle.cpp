#include "circle.h"

CCircle::CCircle()
{
	memset(this,0,sizeof(CCircle));
}
void CCircle::Initialize(DWORD dwPicesNum)
{
	DWORD	i;

	m_dwPicesNum = dwPicesNum;


	m_pv4Point = new VECTOR4[dwPicesNum];
		
	DWORD	dwIndicesNum = dwPicesNum*2;
	m_pIndex = new WORD[dwIndicesNum];

	for (i=0; i<dwPicesNum; i++)
	{
		m_pIndex[i*2+0] = i;
		m_pIndex[i*2+1] = i+1;
	}
	m_pIndex[dwIndicesNum-1] = 0;
	
	for (i=0; i<dwPicesNum; i++)
	{
		m_pv4Point[i].z = 0.1f;
		m_pv4Point[i].w = 1.0f / m_pv4Point[i].z;
	}
	


//	memset(m_pIndex,0,sizeof(WORD)*m_dwTotalIndicesNum);

}
void CCircle::BuildMesh(VECTOR2* pv2Point,float fRs)
{
	float d = PI_MUL_2 / (float)m_dwPicesNum;

	float theta = 0.0f;
	for (DWORD i=0; i<m_dwPicesNum; i++)
	{
		
		m_pv4Point[i].x = sinf(theta) * fRs + pv2Point->x;
		m_pv4Point[i].y = cosf(theta) * fRs + pv2Point->y;
		theta+=d;
	}

}
CCircle::~CCircle()
{
	if (m_pv4Point)
	{
		delete [] m_pv4Point;
		m_pv4Point = NULL;
	}
	if (m_pIndex)
	{
		delete [] m_pIndex;
		m_pIndex = NULL;
	}

}
