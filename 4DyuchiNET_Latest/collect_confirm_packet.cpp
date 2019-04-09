#include "collect_confirm_packet.h"

#ifdef _NEED_CONFIRM
BOOL CollectCodeBytes(char* pDest,char* pSrc,DWORD dwSize)
{
	memcpy(pDest,pSrc,dwSize);
	return TRUE;
/*
	static DWORD		dwCodePos = 0;
	static DWORD		dwRecvBytesPos = 0;
	float limit_value = 200.0f;


	float	x0[4];
	float	x1[4];



	__asm
	{
		push			esi

		push			53
		pop				ecx

		xor				eax,eax
		mov				edx,dword ptr[dwSize]
		mov				esi,dword ptr[pSrc]
		mov				edi,dword ptr[pDest]
		xorps			xmm1,xmm1
		movss			xmm7,[limit_value]

lb_collect:
		movss			xmm0,[esi]
		add				esi,4
		addss			xmm1,xmm0
		comiss			xmm1,xmm7
		movss			x0,xmm0
		movss			x1,xmm1
		jb				lb_collect

		movsb
		xorps			xmm1,xmm1
		inc				eax

		loop			lb_collect
	

lb_code_size_check:
		cmp				al,53
		jnz				lb_debug

		xor				eax,eax
		inc				eax
		pop				esi
		leave
		ret

lb_debug:
		int				3
	}*/
}
#endif

