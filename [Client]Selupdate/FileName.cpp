// FileName.cpp: implementation of the CFileName class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileName.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileName::CFileName(CFileName* pParent,CYHFileFind* pFileFind)
{
	m_pParent = pParent;
	if(pFileFind)
		m_FileName = pFileFind->GetFileName();
	else
		m_FileName = "";

	m_bIsDirectory = FALSE;
}

CFileName::~CFileName()
{

}

CStrClass CFileName::GetFileName()
{
	return m_FileName;
}

CStrClass CFileName::GetFullPathName()
{
	CStrClass fullname = "";
	if(m_pParent)
		fullname += m_pParent->GetFullPathName();
	return fullname + m_FileName;
}