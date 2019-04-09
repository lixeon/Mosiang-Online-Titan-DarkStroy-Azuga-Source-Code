#pragma once

#include "../4DyuchiGRX_common/stdafx.h"

struct CLOCK
{
	DWORD			dwLO;
	DWORD			dwHI;
};

struct PERFORMANCE_CONTEXT
{
	CLOCK			dwClock;			// 0
	CLOCK			dwUsagedClock;		// 8
	DWORD			dwPrvTick;			// 16
	DWORD			dwFrameCount;		// 20
	DWORD			dwAvgFrame;			// 24

};

void BeginAnalyze(PERFORMANCE_CONTEXT* pCtx);
void EndAnalyze(PERFORMANCE_CONTEXT* pCtx);
