#include "read_memory_handle.h"

CReadMemoryHandle::CReadMemoryHandle()
{
	m_dwMaxMemorySize = 0;
	m_pMemory = NULL;
	m_dwMemoryIndex = -1;
	m_pWritePtr = NULL;
	m_pReadPtr = NULL;
	m_dwReservedBytes = 0;
	m_dwUsageBytes = 0;
}

void CReadMemoryHandle::InitializeMemory()
{
	m_dwReserved = 0;
	m_pWritePtr = m_pReadPtr = m_pMemory;
	m_dwReservedBytes = m_dwMaxMemorySize;
	m_dwUsageBytes = 0;
}

void CReadMemoryHandle::OnRead(DWORD dwSize,WSABUF* pBuf,DWORD dwBufNum,DWORD* pdwTotalMsgBytes,DWORD* pdwTotalMsgNum)
{
//	WSABUF 
//	char*	buf;
//	int		len
		
		
//	void*					m_pMemory;
//	DWORD					m_dwMemoryIndex;
//	char*					m_pWritePtr;
//	char*					m_pReadPtr;
//	DWORD					m_dwReservedBytes;
//  DWORD					m_dwUsageBytes	
	DWORD const64[2] = {0x00000001,00000000};

	__asm
	{
		pxor			mm0,mm0								; 모은 메시지 카운트할 레지스터
		movq			mm1,dword ptr[const64]				; 상수 로드
		movd			mm2,dword ptr[pBuf]					; WSABUF ptr

		mov				ebx,dword ptr[this]				
		mov				ecx,dword ptr[dwSize]
		add				dword ptr[ebx+8],ecx				; write할 포인터를 뒤로 이동.
		xor				edx,edx
		sub				dword ptr[ebx+16],ecx				; 남은 바이트.
		xor				eax,eax
		add				dword ptr[ebx+20],ecx				; 사용된 바이트.
		mov				edx,MSG_LENGTH_SIZE					; 상수 edx = 2 


lb_dispatch:
		; dispatch msg
		mov				ecx,dword ptr[dwBufNum]				; WSABUF배열의 갯수
		or				ecx,ecx
		jz				lb_end

		mov				ecx,dword ptr[ebx+20]				; 사용된 바이트 로드
		cmp				ecx,edx								; 길이 2바이트만큼도 버퍼가 차지 않았으면 

		jbe				lb_end

lb_read_length:
		sub				ecx,edx								; m_dwUsageBytes -= 2
		mov				esi,dword ptr[ebx+12]				; 읽어들일 포인터
		movzx			edi,word ptr[esi]					; 길이를 읽는다
		
		cmp				ecx,edi								; m_dwUsageBytes - 2 < length : return 
		jb				lb_end
		
		add				esi,edx								; 길이 2바이트를 읽었으므로 포인터+=2
		shl				edx,2								; 상수 edx = 8
lb_read_msg:
		
		mov				ecx,edi								; 읽어들일 바이트 수
		movd			edi,mm2								; 길이와 포인터를 넣을 WSABUF ptr

		mov				dword ptr[edi],ecx					; dest 버퍼에 길이를 넣는다.
		mov				dword ptr[edi+4],esi				; dest버퍼에 메시지의 포인터를 넣는다.

		add				esi,ecx								; read ptr += len
		add				edi,edx								; dest 버퍼+=8
		movd			mm2,edi

		
		shr				edx,2								; 상수 edx = 2
		add				eax,ecx								; 여기서 읽어들인 메시지의 총 길이.누적시킨다.
		add				ecx,edx								; (메시지 길이 +-2)	
		sub				dword ptr[ebx+20],ecx				; 사용된 바이트 -= (메시지 길이 +-2)
		mov				dword ptr[ebx+12],esi				; read ptr 재설정

		paddd			mm0,mm1								; 모은 메시지 갯수 카운트
		dec				dword ptr[dwBufNum]					; 사용 가능한 WSABUF배열 디스카운트
		jmp				lb_dispatch



lb_end:
		mov				edi,dword ptr[pdwTotalMsgBytes]		; dispatch한 메시지 총 바이트수(길이 2바이트씩 제외)
		mov				dword ptr[edi],eax
		mov				esi,dword ptr[pdwTotalMsgNum]		; dispatch한 메시지 갯수 
		movd			dword ptr[esi],mm0
		emms

	}
}

void CReadMemoryHandle::OnReadC(DWORD dwSize,WSABUF* pBuf,DWORD dwBufNum,DWORD* pdwTotalMsgBytes,DWORD* pdwTotalMsgNum)
{	
//	void*					m_pMemory;
//	DWORD					m_dwMemoryIndex;
//	char*					m_pWritePtr;
//	char*					m_pReadPtr;
//	DWORD					m_dwReservedBytes;
//  DWORD					m_dwUsageBytes	

	m_dwReserved += dwSize;
	m_pWritePtr += dwSize;
	m_dwUsageBytes += dwSize;
	m_dwReservedBytes -= dwSize;
	DWORD dwLength = 0;
	DWORD count = 0;
	DWORD size = 0;

	*pdwTotalMsgBytes = 0;
	*pdwTotalMsgNum = 0;


lb_loop:

	if (m_dwUsageBytes < 3)
		return;
	
	
	dwLength = (DWORD)(*(WORD*)m_pReadPtr);

	if (m_dwUsageBytes - 2 < dwLength )
		return;
	
	m_pReadPtr += 2;
	
	pBuf[count].buf = m_pReadPtr;
	pBuf[count].len = dwLength;

	m_pReadPtr += dwLength;

	(*pdwTotalMsgBytes) += dwLength;
	(*pdwTotalMsgNum)++;
	m_dwUsageBytes -= (2 + dwLength);



	count++;

	goto lb_loop;

}
	

void CReadMemoryHandle::ResetMemory()
{
	if (m_dwReservedBytes < 100)
	{
		m_dwReservedBytes = m_dwMaxMemorySize-m_dwUsageBytes;
		memcpy(m_pMemory,m_pReadPtr,m_dwUsageBytes);
		m_pReadPtr = m_pMemory;
		m_pWritePtr = m_pReadPtr+m_dwUsageBytes;
	}
}

CReadMemoryHandle::~CReadMemoryHandle()
{

}