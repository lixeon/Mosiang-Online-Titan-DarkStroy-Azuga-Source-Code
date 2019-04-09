// MapNameManager.h: interface for the CMapNameManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPNAMEMANAGER_H__E7197EE0_38FD_4C3D_B592_83AB8F7CF490__INCLUDED_)
#define AFX_MAPNAMEMANAGER_H__E7197EE0_38FD_4C3D_B592_83AB8F7CF490__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../[Lib]YHLibrary/hashtable.h"

#define MAPNAME USINGTON(CMapNameManager)

class CMapNameManager  
{
protected:
	CYHHashTable<char>	m_MapNameTable;

public:
	CMapNameManager();
	virtual ~CMapNameManager();

	void	ReadMapName();
	char*	GetMapName( DWORD dwMapNum );
};

EXTERNGLOBALTON(CMapNameManager)

#endif // !defined(AFX_MAPNAMEMANAGER_H__E7197EE0_38FD_4C3D_B592_83AB8F7CF490__INCLUDED_)
