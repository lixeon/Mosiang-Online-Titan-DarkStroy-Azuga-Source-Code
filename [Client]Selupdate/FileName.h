// FileName.h: interface for the CFileName class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILENAME_H__266550BD_FB88_437E_8185_613FD9D375B7__INCLUDED_)
#define AFX_FILENAME_H__266550BD_FB88_437E_8185_613FD9D375B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StrClass.h"
#include "YHFileFind.h"

class CFileName  
{
protected:
	CFileName* m_pParent;
	CStrClass m_FileName;
	BOOL m_bIsDirectory;
	
public:
	CFileName(CFileName* pParent,CYHFileFind* pFileFind);
	virtual ~CFileName();

	BOOL IsDirectory()	{ return m_bIsDirectory;	}

	CStrClass GetFileName();
	CStrClass GetFullPathName();
};

#endif // !defined(AFX_FILENAME_H__266550BD_FB88_437E_8185_613FD9D375B7__INCLUDED_)
