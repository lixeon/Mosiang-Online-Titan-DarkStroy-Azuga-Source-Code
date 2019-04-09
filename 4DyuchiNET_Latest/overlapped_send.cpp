//#define _WIN32_WINNT 0x0500

#include "overlapped_send.h"
#include "network.h"
#include "cpio.h"

COverlappedSend::COverlappedSend()
{
	Initialize();
	InitializeCriticalSectionAndSpinCount(&m_csSend,1000);
}

void COverlappedSend::Initialize()
{
	// 컨스트럭터.변수 초기화.
	// 변수를 초기화한다.
	m_dwFlag = 0;
	memset(&m_Overlapped,0,sizeof(m_Overlapped));
	m_socket = INVALID_SOCKET;
	m_dwIoType = IO_TYPE_WAIT;
	m_pNetwork = NULL;
	m_dwHandle = 0;
}

BOOL COverlappedSend::AllocBuffer(DWORD size)
{
	if (GetMaxBufferSize())
	{
		m_mhWrite.InitializeMemory();
		return TRUE;
	}

	MEMORY_OBJECT* pMemory = (MEMORY_OBJECT*)m_pNetwork->GetDMWrite()->Alloc();
	if (!pMemory)
		return FALSE;

	m_mhWrite.SetMemory(pMemory,m_pNetwork->GetDMWrite()->GetMemoryNumPerBucket());
	
	return TRUE;
}
// for assembly
/*   
void __declspec(naked) COverlappedSend::SendBuffer()
{
	MAX_WSABUF_NUM_IN_SEND_FUNC;
	
}*/

BOOL COverlappedSend::SendBuffer()
{
//	MEMORY_OBJECT*		m_pNext;
//	MEMORY_OBJECT*		m_pPrv;
//	BYTE				m_bReservedBytes;
//	BYTE				m_bUsageBytes;
//	BYTE				m_bReadPosition;		// WSABUF에 넣을때 사용할 엔트리 포지션 인덱스.메모리 오브젝트 헤드만 해당.
//	BYTE				m_bWritePosition;
//	char				m_pBuffer[MEMORY_BLOCK_SIZE];

	WSABUF	wsabuf[MAX_WSABUF_NUM_IN_SEND_FUNC];
//	MEMORY_OBJECT* pMemory = m_mhWrite.GetMemoryObject();

//	DWORD total_size;
//	total_size = m_mhWrite.GetUsageBytes();
//	if (total_size > GetMaxTransferSize())
//		total_size = GetMaxTransferSize();
/*	
	while (total_size)
	{
		wsabuf[count].buf = pMemory->m_pBuffer + pMemory->m_bReadPosition;
		wsabuf[count].len = pMemory->m_bUsageBytes;
		total_size -= pMemory->m_bUsageBytes;
		count++;
		
		pMemory = pMemory->m_pNext;
	}

seg_exit:

	SetIOType(IO_TYPE_WRITE);
	
	result = WSASend(m_socket,
		wsabuf,
		count,
		&m_dwTransferredBytes,
		0,
		&m_Overlapped,
		NULL);
*/
	int result = 0;
	DWORD count = 0;
	__asm
	{
		mov				ebx,dword ptr[this]		; base ptr
		movd			mm0,dword ptr[ebx+28]	; m_socket
		movd			mm1,ebx					; overlapped ptr
		
		mov				eax,ebx					; &m_dwTransferBytes
		add				eax,24
		movd			mm2,eax					; &m_dwTransferBytes

		mov				dword ptr[ebx+20],IO_TYPE_WRITE
		mov				eax,dword ptr[ebx+32]	; m_iMaxTransferSize
		
		add				ebx,48					; m_mhWrite ptr

  		mov				ecx,dword ptr[ebx+20]	; m_mhWrite.m_dwUsageBytes
		cmp				ecx,eax					; m_dwUsageBytes > m_iMaxTransferSize : send_size = m_iMaxTransferSize
		jbe				lb_start

		mov				ecx,eax

		
lb_start:
		mov				ebx,dword ptr[ebx+8]	; m_mhWrite.m_pMemObjHead
		lea				edi,dword ptr[wsabuf]	; wsabuf ptr
		movd			mm3,edi					; backup wsabuf
		xor				edx,edx
lb_loop:
		movzx			eax,dword ptr[ebx+10]	; pMemory->m_bReadPosition
		mov				esi,ebx
		add				esi,12					; pMemory->m_pBuffer + pMemory->m_bReadPosition;
		add				esi,eax	
			
		mov				dword ptr[edi+4],esi
		movzx			eax,byte ptr[ebx+9]		; pMemory->m_bUsageBytes
		mov				dword ptr[edi],eax		;

		inc				edx
		
		add				edi,8
		sub				ecx,eax					; discount total bytes
		jbe				lb_end

		mov				ebx,dword ptr[ebx]		; next 
		jmp				lb_loop				
			
lb_end:
		//mov				dword ptr[count],edx		
		
		movd			eax,mm0					; m_socket
		movd			esi,mm1					; overlapped ptr
		movd			ecx,mm2					; m_dwTransferredBytes
		movd			ebx,mm3					; wsabuf ptr
		emms
		;				edx						; count

		push		0
		push		esi
		push		0
		push		ecx
		push		edx
		push		ebx
		push		eax

		call		dword ptr[WSASend]

		mov			dword ptr[result],eax
//		result = WSASend(m_socket,
//		wsabuf,
//		count,
//		&m_dwTransferredBytes,
//		0,
//		&m_Overlapped,
//		NULL);
		
		
	}
	

	if (result != SOCKET_ERROR)
		goto seg_true;

	result = WSAGetLastError();
	
	if (result == ERROR_IO_PENDING)	// 이건 에러가 아니다.최초에 무사히 초기화 되었다 뜻이다.
		goto seg_true;

	if (result == WSAEWOULDBLOCK)
		goto seg_false;

	PostDisconnectEvent(m_dwHandle);
	
seg_false:
	return FALSE;

seg_true:
	return TRUE;

}
void COverlappedSend::Release()
{
	m_pNetwork->GetDMWrite()->Free((char*)m_mhWrite.GetMemory());
	DeleteCriticalSection(&m_csSend);
}

COverlappedSend::~COverlappedSend()
{
	Release();

}