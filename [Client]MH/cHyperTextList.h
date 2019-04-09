#ifndef _HYPERTEXTLIST_
#define _HYPERTEXTLIST_

//#include "PtrList.h"

class cHyperTextList
{
	CYHHashTable<DIALOGUE>	m_HyperText;	
//	cPtrList	m_HyperText;
public:
	cHyperTextList();
	virtual ~cHyperTextList();

	void LoadHyperTextFormFile(char* filePath, char* mode = "rt");

	DIALOGUE* GetHyperText( DWORD dwIdx );
};

#endif