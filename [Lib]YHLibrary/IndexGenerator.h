// IndexGenerator.h: interface for the CIndexGenerator class.
// Lastest update : 2002.12.29. by taiyo
//////////////////////////////////////////////////////////////////////

#ifndef __CINDEXGENERATOR_H__
#define __CINDEXGENERATOR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Windows.h>

//////////////////////////////////////////////////////////////////////
// ±âº» ½ÃÀÛ ÀÎµ¦½º´Â 1ÀÌ´Ù.
// index°¡ 0ÀÌ ³Ñ¾î ¿À¸é ¿¡·¯!
//////////////////////////////////////////////////////////////////////
class CIndexGenerator  
{
public:
	CIndexGenerator();
	virtual ~CIndexGenerator();

	void	Init(DWORD dwMaxIndexNum, DWORD startIdx=1);
	void	Release();
	DWORD	GenerateIndex();
	BOOL	ReleaseIndex(DWORD dwIndex);
private:
	DWORD * m_pIndexTable;
	DWORD front;
	DWORD rear;
	DWORD m_dwMaxIndexNum;
	DWORD m_dwStartIndex;
};

#endif // __CINDEXGENERATOR_H__
