// cRITEMEx.h: interface for the cRITEMEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRITEMEX_H__6DC147FD_86F8_44F5_A4B2_35D1767E1D83__INCLUDED_)
#define AFX_CRITEMEX_H__6DC147FD_86F8_44F5_A4B2_35D1767E1D83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INTERFACE\cReportItem.h"

class cRITEMEx : public RITEM  
{
public:
	cRITEMEx(WORD num):RITEM(num) {}
	virtual ~cRITEMEx() {}
	
	DWORD dwID;

};

#endif // !defined(AFX_CRITEMEX_H__6DC147FD_86F8_44F5_A4B2_35D1767E1D83__INCLUDED_)
