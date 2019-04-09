// StringLoader.h: interface for the CStringLoader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRINGLOADER_H__71C3FFE0_5D25_4A11_9934_F381F019AE67__INCLUDED_)
#define AFX_STRINGLOADER_H__71C3FFE0_5D25_4A11_9934_F381F019AE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRINGLOADER USINGTON(CStringLoader)

enum eString
{
	eString_GetExp,
};

class CStringLoader  
{
	int m_MaxLoadedString;
	StaticString* m_LoadedString;
	char lpszReturn[1024];
	
public:
	//MAKESINGLETON(CStringLoader)

	CStringLoader();
	virtual ~CStringLoader();

	BOOL Init(char* strFileName);
	void Release();

	char* GetString(DWORD StrNum, ...);

};
EXTERNGLOBALTON(CStringLoader);
#endif // !defined(AFX_STRINGLOADER_H__71C3FFE0_5D25_4A11_9934_F381F019AE67__INCLUDED_)
