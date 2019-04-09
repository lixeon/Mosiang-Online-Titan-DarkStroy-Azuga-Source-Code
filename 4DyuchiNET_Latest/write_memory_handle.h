#pragma once

#include "memory_object.h"
#include "dynamic_memory.h"

class CWriteMemoryHandle
{
	DWORD				m_dwMemObjNum;
	DWORD				m_dwReservedBytes;
	MEMORY_OBJECT*		m_pMemObjHead;
	MEMORY_OBJECT*		m_pMemObjWrite;		// 마지막으로 push한 memory object
	MEMORY_OBJECT*		m_pMemObjTail;
	DWORD				m_dwUsageBytes;
	DWORD				m_dwMaxMemorySize;
	MEMORY_OBJECT*		m_pMemory;
public:
	MEMORY_OBJECT*		GetMemory() {return m_pMemory;}
	void				SetMemory(MEMORY_OBJECT* pMemory,DWORD dwMemObjNum)
	{
		m_pMemory = pMemory;
		m_dwMemObjNum = dwMemObjNum;
		InitializeMemory();
	}
	DWORD				GetUsageBytes() {return m_dwUsageBytes;}
	DWORD				GetReservedBytes() {return m_dwReservedBytes;}
	void				InitializeMemory();
	DWORD				GetMaxMemorySize() {return m_dwMemObjNum*64;}

	BOOL				Push(char* msg,DWORD len);
	void				OnWrite(DWORD dwSize);
	
	MEMORY_OBJECT*		GetMemoryObject() {return m_pMemObjHead;}
	CWriteMemoryHandle(); 
};

