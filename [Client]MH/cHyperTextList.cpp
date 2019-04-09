#include "stdafx.h"
#include "cHyperTextList.h"
#include "../MHFile.h"


cHyperTextList::cHyperTextList()
{
	m_HyperText.Initialize(1000);
}

cHyperTextList::~cHyperTextList()
{
	m_HyperText.SetPositionHead();
	DIALOGUE* value;
	while(value = m_HyperText.GetData())
	{
		delete value;
	}
	
//	PTRLISTSEARCHSTART(m_HyperText,DIALOGUE*,Msg)
//		delete Msg;
//	PTRLISTSEARCHEND
			
	m_HyperText.RemoveAll();
}

void cHyperTextList::LoadHyperTextFormFile(char* filePath, char* mode)
{
	int nLen;
	DIALOGUE* pTemp;

	CMHFile fp;
	if(!fp.Init(filePath, mode))
		return;
	char buff[256]={0,};
	DWORD idx;

	while(1)
	{
		idx = fp.GetInt();
		if(fp.IsEOF())
			break;
		fp.GetLine( buff, 256 );
		if( nLen = strlen(buff) )
		{
			pTemp = new DIALOGUE;
			pTemp->Init();

			strcpy(pTemp->str, buff);
			m_HyperText.Add(pTemp, idx);
//			m_HyperText.AddTail(pTemp);
		}
	}
}

DIALOGUE* cHyperTextList::GetHyperText( DWORD dwIdx )
{
	return (DIALOGUE*)m_HyperText.GetData(dwIdx);
}