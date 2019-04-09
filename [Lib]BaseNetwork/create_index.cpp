#include "create_index.h"

void InitializeIndexDesc(void* pEntry,DWORD dwNum);

CIndexCreator::CIndexCreator()
{
	memset(this,0,sizeof(CIndexCreator));
}
	
BOOL CIndexCreator::Initialize(DWORD dwMaxNum)
{
	m_dwMaxIndexNum = dwMaxNum;
	m_pIndexList = new INDEX_DESC[m_dwMaxIndexNum];
	InitializeIndexDesc(m_pIndexList,m_dwMaxIndexNum);
	m_pBaseDesc = m_pIndexList;

	m_ppIndexDescTable = new INDEX_DESC*[m_dwMaxIndexNum];
	memset(m_ppIndexDescTable,0,sizeof(INDEX_DESC*)*m_dwMaxIndexNum);

	m_dwMaxIndexNum--;

	return TRUE;
}

DWORD __declspec(naked) ICAllocIndex(CIndexCreator* pIC)
{
	__asm
	{
		enter		0,0
		
		push		esi
		push		edi
		push		ebx
		push		edx
		
		xor			eax,eax
		mov			ebx,dword ptr[pIC]
		mov			esi,dword ptr[ebx+4]		; m_pBaseDesc
		or			esi,esi
		jz			lb_return

		mov			edx,dword ptr[esi]			; result dwIndex
		mov			edi,dword ptr[ebx+8]		; m_ppIndexDescTable
		mov			eax,dword ptr[esi+4]		; pDesc->m_pNext
		mov			dword ptr[ebx+4],eax		; m_pBaseDesc = pDesc->m_pNext
		mov			eax,edx						; backup result
		shl			edx,2						; * 4
		add			edi,edx
		mov			dword ptr[edi],esi			; m_ppIndexDescTable[dwIndex] = pDesc(result)
		inc			dword ptr[ebx+12]
lb_return:

		pop			edx
		pop			ebx
		pop			edi
		pop			esi
		
		leave
		ret
	}
}

void __declspec(naked) ICFreeIndex(CIndexCreator* pIC,DWORD dwIndex)
{
	__asm
	{
		enter		0,0
		
		push		esi
		push		edi
		push		ebx
		push		edx
		push		eax

		xor			edx,edx
		mov			ebx,dword ptr[pIC]
		mov			eax,dword ptr[dwIndex]
		mov			edi,dword ptr[ebx+8]		; m_ppIndexDescTable
		shl			eax,2
		add			edi,eax
		mov			esi,dword ptr[edi]			; pDesc
		
		or			esi,esi
		jz			lb_return

		mov			dword ptr[edi],edx			; m_ppIndexDescTable[dwIndex] = NULL
		mov			edi,dword ptr[ebx+4]		; m_pBaseDesc
		
		mov			dword ptr[esi+4],edi		; pDesc->m_pNext = m_pBaseDesc
		mov			dword ptr[ebx+4],esi		; m_pBaseDesc = pDesc
		dec			dword ptr[ebx+12]
lb_return:
		
		pop			eax
		pop			edx
		pop			ebx
		pop			edi
		pop			esi

		leave
		ret
	}
}

CIndexCreator::~CIndexCreator()
{
	if (m_pIndexList)
	{
		delete [] m_pIndexList;
		m_pIndexList = NULL;
	}
	if (m_ppIndexDescTable)
	{
		delete [] m_ppIndexDescTable;
		m_ppIndexDescTable = NULL;
	}

}

void __declspec(naked) InitializeIndexDesc(void* pEntry,DWORD dwNum)
{
	__asm
	{
		enter		0,0
		
		push		esi
		push		edi
		push		eax
		push		edx
		push		ecx

		xor			eax,eax
		xor			edx,edx
		inc			eax
		mov			edi,dword ptr[pEntry]
		mov			ecx,dword ptr[dwNum]
		mov			esi,edi
		add			edi,INDEX_DESC_SIZE
		dec			ecx

lb_loop:
		mov			dword ptr[edi-8],eax
		mov			dword ptr[edi-4],edi
		inc			eax
		add			edi,INDEX_DESC_SIZE

		loop		lb_loop
		
		mov			dword ptr[edi-4],edx
		mov			dword ptr[edi-8],eax

		pop			ecx
		pop			edx
		pop			eax
		pop			edi
		pop			esi

		leave
		ret 
	}

}