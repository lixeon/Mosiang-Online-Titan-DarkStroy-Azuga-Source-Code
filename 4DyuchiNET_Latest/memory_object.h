#pragma once

#define	MEMORY_BLOCK_SIZE 64

#include "../4DyuchiNET_Common/stdafx.h"


// 메모리 오브젝트.64바이트 단위로 만들자.

#define MASK_INIT_NULL		0x00000040	// 스트럭쳐의 수치값을 초기화하기 위한 마스크.
#define MASK_INIT_FULL		0x40004000	// 스트럭쳐의 수치값을 초기화하기 위한 마스크.

#define	MASK_RESERVED_BYTES	0x000000ff
#define	MASK_USAGED_BYTES	0x0000ff00
#define MASK_READ_POSITION	0x00ff0000
#define MASK_WRITE_POSITION	0xff000000

struct MEMORY_OBJECT
{
	MEMORY_OBJECT*		m_pNext;
	MEMORY_OBJECT*		m_pPrv;

	BYTE				m_bReservedBytes;
	BYTE				m_bUsageBytes;
	BYTE				m_bReadPosition;		// WSABUF에 넣을때 사용할 엔트리 포지션 인덱스.메모리 오브젝트 헤드만 해당.
	BYTE				m_bWritePosition;
	
	char				m_pBuffer[MEMORY_BLOCK_SIZE];
};
#ifdef _NEED_CONFIRM
	extern void (*InitializeMemoryObjectChain)(MEMORY_OBJECT* pMemory,DWORD dwNum);
#else
	void InitializeMemoryObjectChain(MEMORY_OBJECT* pMemory,DWORD dwNum);
#endif






#define SIZE_MEMORY_OBJECT	76
