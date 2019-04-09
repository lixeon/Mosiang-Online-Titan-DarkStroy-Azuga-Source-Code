#ifndef VARIABLE_BUFFER_H
#define VARIABLE_BUFFER_H

#include "../4DyuchiGRX_Common/stdafx.h"

class CVariableBuffer
{
	char*				m_pBuffer;
	DWORD				m_dwMaxBufferSize;
	DWORD				m_dwCurrentBufferSize;
public:
	BOOL				Initialize(DWORD dwMaxBufferSize);
	char*				AllocBuffer(DWORD dwSize);
	void				Clear()				{m_dwCurrentBufferSize = 0;}
	char*				GetBufferPtr()		{return m_pBuffer;}

	CVariableBuffer();
	~CVariableBuffer();
};

#endif

