#include "write_memory_handle.h"

CWriteMemoryHandle::CWriteMemoryHandle()
{
	m_dwMemObjNum = 0;
	m_dwReservedBytes = 0;
	m_pMemObjHead = NULL;
	m_pMemObjWrite = NULL;	
	m_pMemObjTail = NULL;
	m_dwUsageBytes = 0;
	m_pMemory = NULL;
	m_dwMaxMemorySize = 0;
}
void CWriteMemoryHandle::InitializeMemory()
{
	m_dwMaxMemorySize = m_dwReservedBytes = 64*m_dwMemObjNum;
	m_dwUsageBytes = 0;
	m_pMemObjHead = m_pMemory;
	m_pMemObjWrite = m_pMemory;
	m_pMemObjTail = m_pMemory + (m_dwMemObjNum-1);
	InitializeMemoryObjectChain(m_pMemory,m_dwMemObjNum);
}
BOOL CWriteMemoryHandle::Push(char* msg,DWORD len)
{

//	DWORD				m_dwMemObjNum;
//	DWORD				m_dwReservedBytes;
//	MEMORY_OBJECT*		m_pMemObjHead;
//	MEMORY_OBJECT*		m_pMemObjWrite;		// 마지막으로 push한 memory object
//	MEMORY_OBJECT*		m_pMemObjTail;
//	DWORD				m_dwUsageBytes;

//	MEMORY_OBJECT*		m_pNext;
//	MEMORY_OBJECT*		m_pPrv;
//	BYTE				m_bReservedBytes;
//	BYTE				m_bUsageBytes;
//	BYTE				m_bReadPosition;		// WSABUF에 넣을때 사용할 엔트리 포지션 인덱스.메모리 오브젝트 헤드만 해당.
//	BYTE				m_bWritePosition;
//	char				m_pBuffer[MEMORY_BLOCK_SIZE];
	
	__asm
	{

		xor			eax,eax

		mov			ecx,[len]							; load length
		mov			ebx,dword ptr[this]					; load this handle
		mov			edx,dword ptr[ebx+4]				; load this reserved size

		cmp			ecx,edx								; len > reserved size : return false
		ja			lb_false
		

		sub			dword ptr[ebx+4],ecx				; m_dwReservedBytes -= len
		add			dword ptr[ebx+20],ecx				; m_dwUsagBytes += len
		mov			edx,dword ptr[ebx+12]				; load m_pMemObjWrite

lb_loop:		
		xor			eax,eax
			
		movzx		esi,byte ptr[edx+8]					

		cmp			ecx,esi								; len > mem obj's reserved bytes
		jbe			lb_cpy

lb_reset_length:

		sub			ecx,esi
		mov			eax,ecx								; set next will be copied bytes
		mov			ecx,esi

lb_cpy:

		mov			edi,edx								; load dest ptr
		add			edi,12
		movzx		esi,byte ptr[edx+11]				; add write position
		add			edi,esi
		
		sub			byte ptr[edx+8],cl					; mem obj's reserved bytes -= ecx
		add			byte ptr[edx+9],cl					; mem obj's usaged bytes += ecx
		add			byte ptr[edx+11],cl					; mem obj's write postion += ecx

		mov			esi,dword ptr[msg]					; load src ptr

		; cpy routine
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
		
		;

lb_cpy_end:
		or			eax,eax								; check bytes that copied more
		jz			lb_true

		mov			edx,dword ptr[edx]					; mem = mem->pNext
		
		mov			ecx,eax								; restore reserved length
		mov			dword ptr[msg],esi					
		jmp			lb_loop

	
lb_true:
		mov			dword ptr[ebx+12],edx				; write m_pMemObjWrite
	}
	return TRUE;

lb_false:
		return FALSE;
}
void CWriteMemoryHandle::OnWrite(DWORD dwSize)
{
	
//	DWORD				m_dwMemObjNum;
//	DWORD				m_dwReservedBytes;
//	MEMORY_OBJECT*		m_pMemObjHead;
//	MEMORY_OBJECT*		m_pMemObjWrite;		// 마지막으로 push한 memory object
//	MEMORY_OBJECT*		m_pMemObjTail;
//	DWORD				m_dwUsageBytes;
//	DWORD				m_dwMaxMemorySize;


//struct MEMORY_OBJECT
//{
//	MEMORY_OBJECT*		m_pNext;
//	MEMORY_OBJECT*		m_pPrv;
//	BYTE				m_bReservedBytes;
//	BYTE				m_bUsageBytes;
//	BYTE				m_bReadPosition;		// WSABUF에 넣을때 사용할 엔트리 포지션 인덱스.메모리 오브젝트 헤드만 해당.
//	BYTE				m_bWritePosition;
//	char				m_pBuffer[MEMORY_BLOCK_SIZE];
	__asm
	{
		
		mov				ebx,dword ptr[this]
		mov				ecx,dword ptr[dwSize]


		mov				eax,MASK_INIT_NULL
		movd			mm0,eax

		mov				edx,dword ptr[ebx+8]			; load m_pMemObjHead
		mov				eax,dword ptr[ebx+16]			; load m_pMemObjTail
		mov				edi,dword ptr[ebx+20]			; load m_dwUsageBytes
		sub				edi,ecx
		movd			mm1,edi
		mov				dword ptr[ebx+20],edi

lb_loop:
		movzx			edi,byte ptr[edx+9]				; load m_pMemObjHead->m_dwUsagedBytes
		cmp				ecx,edi							; length that will be skip <= mem obj's usaged bytes
		jb				lb_set_length					; length < usaged bytes goto lb_set_length

		movzx			esi,byte ptr[edx+11]			; load write position
		add				dword ptr[ebx+4],esi			; m_dwReservedBytes += usaged bytes

		movd			dword ptr[edx+8],mm0			; initialize mem object
		mov				dword ptr[eax],edx				; m_pMemObjTail->pNext = m_pMemObjHead
		mov				dword ptr[edx+4],eax			; m_pMemObjHead->pPrv = m_pMemObjTail
		mov				eax,dword ptr[edx]				; load m_pMemObjHead->pNext
		mov				dword ptr[edx],0				; m_pMemObjHead->pNext = NULL

		mov				dword ptr[eax+4],0				; reset m_pMemObjHead->pNext->pPrv = NULL
		
		xchg			eax,edx
		sub				ecx,edi
		jnz				lb_loop

lb_set_length:
		; edx = m_pMemObjHead,	eax = m_pMemObjTail
		add				byte ptr[edx+10],cl				; m_pMemObjHead->m_dwReadPosition += ecx
		sub				byte ptr[edx+9],cl				; m_pMemObjHead->m_dwUsagedBytes -= ecx

lb_set_obj:
		mov				dword ptr[ebx+8],edx			; write m_pMemObjHead
		mov				dword ptr[ebx+16],eax			; m_pMemObjTail

		movd			ecx,mm1							; restore m_dwUsageBytes
		or				ecx,ecx
		jnz				lb_end
		mov				dword ptr[ebx+12],edx			; write m_pMemObjWrite

lb_end:
		
		emms
		
	}
}

// virtula send
