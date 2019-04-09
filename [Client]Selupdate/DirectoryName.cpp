// DirectoryName.cpp: implementation of the CDirectoryName class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DirectoryName.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDirectoryName::CDirectoryName(CFileName* pParent,CYHFileFind* pFileFind)
: CFileName(pParent,pFileFind)
{
	int len = m_FileName.GetLength();
	if(len != 0)
	{
		if(m_FileName.GetAt(len-1) != '\\')
		{
			m_FileName += "\\";
		}
	}
	m_bIsDirectory = TRUE;
}

CDirectoryName::~CDirectoryName()
{
	CFileName* pChild;
	list<CFileName*>::iterator iter;
	for(iter = m_ChildList.begin();iter != m_ChildList.end(); ++iter)
	{
		pChild = *iter;
		delete pChild;
	}
	m_ChildList.clear();
}

void CDirectoryName::LoadChild()
{
	CYHFileFind ff;
	CFileName* pChild;

	BOOL bHasNext;
	ff.FindFile();
	while(bHasNext = ff.FindNextFile())
	{
		CStrClass fname = ff.GetFileName();
		if(fname == "." || fname == "..")
			continue;

		if(ff.IsDirectory())
		{
			pChild = new CDirectoryName(this,&ff);
		}
		else
		{
			pChild = new CFileName(this,&ff);
		}

		m_ChildList.push_back(pChild);		
	}

	list<CFileName*>::iterator iter;
	for(iter = m_ChildList.begin();iter != m_ChildList.end(); ++iter)
	{
		pChild = *iter;
		if(pChild->IsDirectory())
		{
			SetCurrentDirectory(pChild->GetFileName());
			((CDirectoryName*)pChild)->LoadChild();
			SetCurrentDirectory("..");
		}
	}
}


void CDirectoryName::SetPositionHead()
{
	// Position Set
	m_bReturnSelf = FALSE;
	m_Iter = m_ChildList.begin();

	// Child Directory Position Set
	CFileName* pChild;
	list<CFileName*>::iterator iter;
	for(iter = m_ChildList.begin();iter != m_ChildList.end(); ++iter)
	{		
		pChild = *iter;
		if(pChild->IsDirectory())
		{
			((CDirectoryName*)pChild)->SetPositionHead();
		}
	}
}
DWORD CDirectoryName::GetNextFileName(CStrClass* pFileNameOut)
{
	if(m_bReturnSelf == FALSE)
	{
		*pFileNameOut = GetFullPathName();
		m_bReturnSelf = TRUE;
		return GETNEXTFILENAME_DIRECTORY;
	}

	if(m_Iter == m_ChildList.end())
		return GETNEXTFILENAME_END;

	CFileName* pChild = *m_Iter;
	if(pChild->IsDirectory())
	{
		DWORD rt;
		if( rt = ((CDirectoryName*)pChild)->GetNextFileName(pFileNameOut) )
			return rt;
		else
		{
			++m_Iter;
			return GetNextFileName(pFileNameOut);			
		}
	}

	*pFileNameOut = pChild->GetFullPathName();
	++m_Iter;
	return GETNEXTFILENAME_FILE;
}