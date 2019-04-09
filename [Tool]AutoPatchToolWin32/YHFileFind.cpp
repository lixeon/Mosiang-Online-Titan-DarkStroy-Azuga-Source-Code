// YHFileFind.cpp: implementation of the CYHFileFind class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "YHFileFind.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CYHFileFind::CYHFileFind()
{
	m_hFileFind = NULL;
}

CYHFileFind::~CYHFileFind()
{
	if(m_hFileFind)
	{
		FindClose(m_hFileFind);
	}
}

BOOL CYHFileFind::FindFile(LPCTSTR scFilter)
{
	if(scFilter == NULL)
		m_hFileFind = ::FindFirstFile("*.*",&file);
	else
		m_hFileFind = ::FindFirstFile(scFilter,&file);

	if(m_hFileFind != NULL)
		return TRUE;
	else
		return FALSE;
}
BOOL CYHFileFind::FindNextFile()
{
	if(m_hFileFind == NULL)
		return FALSE;
	return ::FindNextFile(m_hFileFind,&file);
}

BOOL CYHFileFind::IsDirectory()
{
	if(file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		return TRUE;
	else
		return FALSE;
}
CStrClass CYHFileFind::GetFileName()
{
	if(m_hFileFind == NULL)
		return "";
	
	return file.cFileName;
}