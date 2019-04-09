// DirectoryName.h: interface for the CDirectoryName class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIRECTORYNAME_H__D3740CAD_3F86_4191_B878_7DB72CC63ABE__INCLUDED_)
#define AFX_DIRECTORYNAME_H__D3740CAD_3F86_4191_B878_7DB72CC63ABE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileName.h"
#include <list>

using namespace std;

#define GETNEXTFILENAME_END			0
#define GETNEXTFILENAME_FILE		1
#define GETNEXTFILENAME_DIRECTORY	2

class CDirectoryName : public CFileName
{
	list<CFileName*> m_ChildList;

	// Position °ü·Ã
	BOOL m_bReturnSelf;
	list<CFileName*>::iterator m_Iter;

public:
	CDirectoryName(CFileName* pParent,CYHFileFind* pFileFind);
	virtual ~CDirectoryName();

	void LoadChild();

	void SetPositionHead();
	DWORD GetNextFileName(CStrClass* pFileNameOut);
};

#endif // !defined(AFX_DIRECTORYNAME_H__D3740CAD_3F86_4191_B878_7DB72CC63ABE__INCLUDED_)
