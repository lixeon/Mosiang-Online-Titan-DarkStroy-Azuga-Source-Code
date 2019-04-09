#include "performance.h"

__declspec(naked) void BeginAnalyze(PERFORMANCE_CONTEXT* pCtx)
{
	__asm
	{
		push		ecx

		rdtsc
		mov			ecx,dword ptr[esp+4+4]
		mov			dword ptr[ecx],eax
		mov			dword ptr[ecx+4],edx

		pop			ecx

		ret

	}

	
}
__declspec(naked) void EndAnalyze(PERFORMANCE_CONTEXT* pCtx)
{

//	DWORD			dwPrvTick;			// 16
//	DWORD			dwFrameCount;		// 20
//	DWORD			dwAvgFrame;			// 24

	__asm
	{
		push		eax
		push		ecx
		push		edx
		
		rdtsc		
		mov			ecx,dword ptr[esp+4+12]
		sub			eax,dword ptr[ecx]
		sub			edx,dword ptr[ecx+4]
		mov			dword ptr[ecx+8],eax
		mov			dword ptr[ecx+12],edx
		
		push		ecx
		call		dword ptr[GetTickCount]
		pop			ecx
		
		mov			edx,eax						; backup tick count
		sub			eax,dword ptr[ecx+16]
		cmp			eax,1000
		jl			lb_inc_frame

		mov			dword ptr[ecx+16],edx		; dwPrvTick = GetTickCount();
		mov			eax,dword ptr[ecx+20]		
		xor			edx,edx
		mov			dword ptr[ecx+24],eax
		mov			dword ptr[ecx+20],edx
		

		jmp			lb_return

lb_inc_frame:
		inc			dword ptr[ecx+20]

lb_return:
		pop			edx
		pop			ecx
		pop			eax

		ret
	}
}