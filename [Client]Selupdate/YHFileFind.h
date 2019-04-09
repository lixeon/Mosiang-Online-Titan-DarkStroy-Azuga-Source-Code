// YHFileFind.h: interface for the CYHFileFind class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YHFILEFIND_H__2E0D1DAA_2417_4F30_92F0_432B45C71159__INCLUDED_)
#define AFX_YHFILEFIND_H__2E0D1DAA_2417_4F30_92F0_432B45C71159__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StrClass.h"

class CYHFileFind  
{
	HANDLE m_hFileFind;
	WIN32_FIND_DATA file;
public:
	CYHFileFind();
	virtual ~CYHFileFind();

	BOOL FindFile(LPCTSTR scFilter = NULL);
	BOOL FindNextFile();

	BOOL IsDirectory();
	CStrClass GetFileName();

};

#endif // !defined(AFX_YHFILEFIND_H__2E0D1DAA_2417_4F30_92F0_432B45C71159__INCLUDED_)
