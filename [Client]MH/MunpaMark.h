// MunpaMark.h: interface for the CMunpaMark class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUNPAMARK_H__07EA7B3F_EDB5_4D65_882A_1F1B709381F5__INCLUDED_)
#define AFX_MUNPAMARK_H__07EA7B3F_EDB5_4D65_882A_1F1B709381F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include ".\cImageSelf.h"

class CMunpaMark  
{
	cImageSelf m_Image;
	
public:
	CMunpaMark();
	virtual ~CMunpaMark();

	BOOL Init(char* strFileName);
	void Release();

	void Render(VECTOR2* pPos);
};

#endif // !defined(AFX_MUNPAMARK_H__07EA7B3F_EDB5_4D65_882A_1F1B709381F5__INCLUDED_)
