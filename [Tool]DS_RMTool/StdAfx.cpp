// stdafx.cpp : source file that includes just the standard includes
//	DS_RMTool.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#include "DS_RMTool.h"
#include "DS_RMToolDoc.h"

char g_szHeroIDName[17] = {0,};


void SafeStrCpy( char* pDest, const char* pSrc, int nDestBufSize )
{
	strncpy( pDest, pSrc, nDestBufSize -1 );
	pDest[nDestBufSize -1] = 0;
}


////////////////////////////////////////////////////////////////
// Global NetworkMsg Parse
void NetworkMsgParse(BYTE Category, BYTE Protocol, void *pMsg)
{
	CDS_RMToolApp* pApp = (CDS_RMToolApp*)AfxGetApp();

	if( !pApp ) 
	{
		ASSERT(0);
		return;
	}

	switch(Category)
	{
	case MP_RMTOOL_CONNECT:
		pApp->NetworkMsgParse(Protocol, pMsg);
		break;
	case MP_RMTOOL_USER:
	case MP_RMTOOL_MUNPA:
	case MP_RMTOOL_GAMELOG:
	case MP_RMTOOL_OPERLOG:
	case MP_RMTOOL_STATISTICS:
	case MP_RMTOOL_ADMIN:
	case MP_RMTOOL_CHARACTER:
		pApp->AllotMessage(Protocol, pMsg);
		break;
	default:
		return;
	}
}
////////////////////////////////////////////////////////////////

