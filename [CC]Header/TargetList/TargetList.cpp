// TargetList.cpp: implementation of the CTargetList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TargetList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTargetList::CTargetList()
{
	// YH TEST!!!!
	/*
	CTargetList list;
	list.BufLen = 7;
	list.Buf[0] = -80;
	list.Buf[1] = 0;
	list.Buf[2] = 0;
	list.Buf[3] = -123;
	list.Buf[4] = 30;
	list.Buf[5] = 0;
	list.Buf[6] = 0;
	*/
}

CTargetList::~CTargetList()
{

}

void CTargetList::Clear()
{
	BufLen = 0;
}
CTargetData* CTargetList::GetTargetData(int PositionOfBuffer)
{
	return (CTargetData*)(&Buf[PositionOfBuffer]);
}

void CTargetList::SetBufLen(WORD Len)
{
	BufLen = Len;
}
WORD CTargetList::GetBufLen()
{
	return BufLen;
}

WORD CTargetList::GetTotalMsgLen()
{
	return sizeof(CTargetList) - MAX_TARGETLISTBUFFER_LENGTH + GetBufLen();
}

WORD CTargetList::Serialize(char* pOutBuf)
{
	WORD len = GetTotalMsgLen();
	memcpy(pOutBuf,this,len);
	return len;
}