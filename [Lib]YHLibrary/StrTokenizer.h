// StrTokenizer.h: interface for the CStrTokenizer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRTOKENIZER_H__C647B2A9_1397_48AA_A613_F5ED3A6A273B__INCLUDED_)
#define AFX_STRTOKENIZER_H__C647B2A9_1397_48AA_A613_F5ED3A6A273B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_TOKENIZER_SEPARATOR	32

// ÀÎ½ºÅÏ½º¸¦ »ç¿ëÇÏ´Â µ¿¾È ³Ñ°ÜÁØ String Àº ¹«È¿È­ µÇ¸é ¾ÈµÈ´Ù.
// ¹Ýµå½Ã ÇÑ¹ø¿¡ ÇÏ³ªÀÇ ÀÎ½ºÅÏ½º¸¸À» »ç¿ëÇØ¾ß ÇÑ´Ù.

class CStrTokenizer  
{
	char* m_ReturnData;
	char m_strSep[MAX_TOKENIZER_SEPARATOR];

public:
	CStrTokenizer(char* String,char* Sep);
	virtual ~CStrTokenizer();

	char* GetNextToken();
	char* GetNextTokenUpper();	
};

#endif // !defined(AFX_STRTOKENIZER_H__C647B2A9_1397_48AA_A613_F5ED3A6A273B__INCLUDED_)
