// TargetList.h: interface for the CTargetList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TARGETLIST_H__6FC275E7_6E73_45D5_8CAA_47EC776FE041__INCLUDED_)
#define AFX_TARGETLIST_H__6FC275E7_6E73_45D5_8CAA_47EC776FE041__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTargetData;

#define MAX_TARGETLISTBUFFER_LENGTH	8192
class CTargetList  
{
	WORD BufLen;
	char Buf[MAX_TARGETLISTBUFFER_LENGTH];

public:
	CTargetList();
	~CTargetList();

	void Clear();
	CTargetData* GetTargetData(int PositionOfBuffer);
	void SetBufLen(WORD Len);
	WORD GetBufLen();
	
	WORD GetTotalMsgLen();

	WORD Serialize(char* pOutBuf);
};

#endif // !defined(AFX_TARGETLIST_H__6FC275E7_6E73_45D5_8CAA_47EC776FE041__INCLUDED_)
