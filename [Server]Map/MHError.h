// MHError.h: interface for the CMHError class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MHERROR_H__5A5D5DAB_979D_4697_A552_907A81211743__INCLUDED_)
#define AFX_MHERROR_H__5A5D5DAB_979D_4697_A552_907A81211743__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define MHERROR	USINGTON(CMHError)

class CMHError  
{
public:
	CMHError();
	virtual ~CMHError();

	void OutputFile(char * fileName, char * buff, int type = 1);
	char * GetStringArg(char * buff,...);
};

EXTERNGLOBALTON(CMHError);
#endif // !defined(AFX_MHERROR_H__5A5D5DAB_979D_4697_A552_907A81211743__INCLUDED_)
