#include "CheckClock.h"

/*
	unsigned int	iHigh, iLow;
	_asm 
	{
		rdtsc
		mov dword ptr[iHigh],edx
		mov dword ptr[iLow],eax
	}

	m_Clipper.ClipWithViewVolume( vv);

	_asm 
	{
		rdtsc
		sub		eax,dword ptr[iLow]
		sub		edx,dword ptr[iHigh]

	}
*/

void CheckClockBegin( unsigned int* iLow, unsigned int* iHigh)
{
	return ;
}

void CheckClockEnd( unsigned int* iLow, unsigned int* iHigh)
{
	return ;
}

unsigned int GetLowClock(void)
{
	unsigned int	iLowClock;
	_asm
	{
		rdtsc
		mov		dword ptr[iLowClock], eax
	}
	return iLowClock;
}
