//#define _WIN32_WINNT 0x0500

#include "../4DyuchiNET_Common/stdafx.h"
#include "que.h"
#include "define.h"

CRITICAL_SECTION g_csWrite;


CMsgQue::CMsgQue(DWORD dwMaxBufferSize)
{
	// 컨스트럭터
	// 멤버 변수 초기화 및 버퍼로 쓸 메모리 할당

	m_bWaitFlag = TRUE;
	m_dwOffset = 0;
	m_dwBufferPosition = 0;
	m_dwMsgNum = 0;
	m_dwMaxBufferSize = dwMaxBufferSize;
	m_pBuffer = new char[dwMaxBufferSize];
	memset(m_pBuffer,0,dwMaxBufferSize);
}



MSG_CONTAINER* GetMsgContainer(CMsgQue* pMsgQue)
{
	//
	// 메시지 컨테이너의 포인터를 돌려주는 함수.인자로 받은 이중포인터를 통해 넘긴다.두번째 인자는 메시지 컨테이너의 인덱스.
	//
//	DWORD					m_dwMsgNum;				// 쌓여있는 메시지 갯수
//	char*					m_pBuffer;
//	DWORD					m_dwOffset;				// 현재 메시지를 긁을 포인터의 위치
//	DWORD					m_dwBufferPosition;		// 현재 버퍼 수위.
//	DWORD					m_dwMaxBufferSize;		// 최대 버퍼 사이즈

	MSG_CONTAINER*	pContainer;
	__asm
	{
		mov			ebx,dword ptr[pMsgQue]
		mov			edx,dword ptr[ebx+8]			; load m_dwOffset
		mov			eax,dword ptr[ebx+4]			; load m_pBuffer
		add			eax,edx
		
		mov			ecx,dword ptr[eax+8]			; load msg's length
		add			ecx,SIZE_MSG_CONTAINER
		add			edx,ecx
		mov			dword ptr[ebx+8],edx			; write m_dwOffset
		

		mov			dword ptr[pContainer],eax
	}
	return pContainer;

//	*ppmc = (MSG_CONTAINER*)(m_pBuffer+m_dwOffset);
//	m_dwOffset += (*ppmc)->GetContainerSize();

	
}

CMsgQue::~CMsgQue()
{
	// 디스터럭터
	
	if (m_pBuffer)
	{
		delete [] m_pBuffer;
		m_pBuffer = NULL;
	}
}


BOOL PushMsg(CMsgQue* pMsgQue,DWORD dwHeader,WSABUF* pBuf,DWORD dwBufNum,DWORD dwTotalMsgSize,DWORD dwFlag,DWORD* pdwMsgNum)
{
//	class CMsgQue

//	DWORD					m_dwMsgNum;				// 쌓여있는 메시지 갯수
//	char*					m_pBuffer;
//	DWORD					m_dwOffset;				// 현재 메시지를 긁을 포인터의 위치
//	DWORD					m_dwBufferPosition;		// 현재 버퍼 수위.
//	DWORD					m_dwMaxBufferSize;		// 최대 버퍼 사이즈
//	CRITICAL_SECTION		m_csAlloc;

	char*	pEntry = NULL;
	char*	pDest = NULL;
	DWORD	dwSize = 0;
	BOOL	bResult;
	CRITICAL_SECTION* pcs;

	__asm
	{
		xor			ecx,ecx
		mov			dword ptr[bResult],ecx				; return value 

		mov			ebx,dword ptr[pMsgQue]
		mov			edx,SIZE_MSG_CONTAINER
		mov			eax,dword ptr[dwBufNum]		
		mul			edx

		add			eax,dword ptr[dwTotalMsgSize]		
		mov			dword ptr[dwSize],eax				; must alloc memory as this size


		;
		; enter critical section
		;
		mov			eax,offset g_csWrite				; critical section ptr
		mov			dword ptr[pcs],eax					; backup critical section ptr

		push		eax
		call		dword ptr[EnterCriticalSection]

		mov			eax,dword ptr[ebx+12]				; load m_dwBufferPosition;
		mov			esi,dword ptr[dwSize]				; need memory bytes
		mov			ecx,eax								; backup m_dwBufferPosition
		add			eax,esi

		cmp			eax,dword ptr[ebx+16]				; m_dwBufferPosition + dwSize >= m_dwMaxBufferSize
		jl			lb_push

		mov			eax,dword ptr[pcs]					; restore critical section ptr
		push		eax
		call		dword ptr[LeaveCriticalSection]
		jmp			lb_return


lb_push:
		mov			dword ptr[ebx+12],eax				; write m_dwBufferPosition

		mov			edx,dword ptr[ebx+4]				; m_pBuffer
		
		add			edx,ecx								; pEntry = m_pBuffer + m_dwBufferPosition
		mov			dword ptr[pEntry],edx				;

		mov			edi,dword ptr[pdwMsgNum]			; load will be writed dwMsgNum
		mov			edx,dword ptr[dwBufNum]				; m_dwMsgNum += dwBufNum;
		add			edx,dword ptr[ebx]
		mov			dword ptr[edi],edx
		mov			dword ptr[ebx],edx					;
		

		;
		; leave critical section
		;
		mov			eax,dword ptr[pcs]					; restore critical section ptr
		push		eax
		call		dword ptr[LeaveCriticalSection]

		;
		; write msg que
		;
//		DWORD						dwHeader;			// 8비트가 실제 헤더영역,24비트가 인덱스 .handle과 동일.
//		DWORD						dwSize;				// 메시지 컨테이너 사이즈
//		DWORD						dwMsgLength;		// 메시지 길이
//		DWORD						dwReserved;			// 범용적으로 쓰도록 남겨둔 4바이트.여기선 udp에서 사용한다.

		mov			edi,dword ptr[pEntry]				; will be copied msg to this ptr
		mov			eax,dword ptr[pBuf]					; src WSABUF ptr
		movd		mm1,dword ptr[dwHeader]				; load header
		mov			edx,dword ptr[dwBufNum]				; loa dwBufNum
	
lb_loop:

//		struct WSABUF
//	    u_long      len;
//	    char FAR *  buf;
		mov			ecx,dword ptr[dwFlag]
		mov			dword ptr[edi+12],ecx
		
		movd		dword ptr[edi],mm1					; write header
		mov			dword ptr[edi+4],SIZE_MSG_CONTAINER	; write msg container's size
		mov			dword ptr[edi+8],0					; wirte msg length

		or			eax,eax
		jz			lb_return_true						; in this case DISCONNECT event

		mov			esi,dword ptr[eax+4]				; msg src ptr
		mov			ecx,dword ptr[eax]					; msg length
		
		mov			dword ptr[edi+8],ecx				; wirte msg length
		

		add			edi,SIZE_MSG_CONTAINER
		;
		; wirte msg
		;
		; esi = src , edi = dest, ecx = length
			
		or			ecx,ecx
		jz			lb_cpy_end

		test		ecx,1
		jz			lb_cpy_2
		movsb
		dec			ecx
		jz			lb_cpy_end

lb_cpy_2:
		test		ecx,2
		jz			lb_cpy_4
		movsw
		sub			ecx,2
		jz			lb_cpy_end
		
lb_cpy_4:
		shr			ecx,2
		rep			movsd
		
lb_cpy_end:
		add			eax,8								; src WSABUF ptr offset move
		dec			edx									; dwBufNum--
		jnz			lb_loop

		emms

lb_return_true:
		inc			dword ptr[bResult]

	}
lb_return:
	return bResult;

}

BOOL CMsgQue::PushMsg(DWORD dwHeader,WSABUF* pBuf,DWORD dwBufNum,DWORD dwTotalMsgSize,DWORD dwFlag,DWORD* pdwMsgNum)
{
	char*	pEntry = NULL;
	char*	pDest = NULL;
	DWORD	dwSize = 0;
		
	// 이 메지시를 모두 쓸수 있는만큼의 메모리를 할당 받는다.먼저 메모리 계산
	DWORD size = SIZE_MSG_CONTAINER*dwBufNum + dwTotalMsgSize;
	

	EnterCriticalSection(&g_csWrite);

	pEntry = m_pBuffer + m_dwBufferPosition;

	if (m_dwBufferPosition + size > m_dwMaxBufferSize)
	{
		LeaveCriticalSection(&g_csWrite);
		return FALSE;
	}

	m_dwBufferPosition += size;
	m_dwMsgNum += dwBufNum;
	*pdwMsgNum = m_dwMsgNum;

	LeaveCriticalSection(&g_csWrite);

	pDest = pEntry;
	for (DWORD i=0; i<dwBufNum; i++)
	{
		((MSG_CONTAINER*)pDest)->m_dwSize = 16;
		((MSG_CONTAINER*)pDest)->m_dwHeader.m_dwHeader = dwHeader;
		((MSG_CONTAINER*)pDest)->m_dwMsgLength = (pBuf+i)->len;
		memcpy(((MSG_CONTAINER*)pDest)->m_pMsg,(pBuf+i)->buf,(pBuf+i)->len);
		pDest += (SIZE_MSG_CONTAINER + (pBuf+i)->len);
	}
	return TRUE;
}
