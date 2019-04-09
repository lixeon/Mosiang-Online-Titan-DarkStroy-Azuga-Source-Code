// QuestString.cpp: implementation of the CQuestString class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "QuestString.h"

#define TEXT_DELIMITER			'^'
#define TEXT_NEWLINECHAR		'n'
#define TEXT_FLAG				'$'
#define TEXT_TAB				'	'
#define MAX_STR_LENTH			30


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestString::CQuestString()
{
}

CQuestString::~CQuestString()
{
	QString* pData = NULL;
	PTRLISTPOS pos = m_TitleList.GetHeadPosition();
	while( pos )
	{
		pData = (QString*)m_TitleList.GetNext( pos );
		if( pData )
			delete pData;
	}
	m_TitleList.RemoveAll();

	pos = m_DescList.GetHeadPosition();
	while( pos )
	{
		pData = (QString*)m_DescList.GetNext( pos );
		if( pData )
			delete pData;
	}
	m_DescList.RemoveAll();
}


void CQuestString::AddLine(char* pStr, int& line, BOOL bTitle)
{
	int len=0;
	int linelen = 0;
	char tb[128] = { 0, };
//	char linebuf[128] = { 0, };
	DWORD Color = 0;
	BOOL bHighlight = FALSE;

	while(*pStr)
	{
		tb[len]=*pStr;
		++linelen;
		if( pStr + 1 != CharNext( CharPrev( pStr, pStr + 1 ) ) )
		{
			tb[++len] = *(++pStr);
			++linelen;
		}

		// ¸ÇÃ³À½ÀÌ TabÀÏ °æ¿ì
		if(tb[0]==TEXT_TAB || tb[0] == ' ')		{	len = -1;	--linelen;	}		

		// ColorFlag°¡ ÀÖÀ» °æ¿ì
		if(tb[len] == TEXT_FLAG)
		{
			if(*(pStr+=2) == 's')
			{
				Color = QUEST_DESC_COLOR;
				bHighlight = TRUE;
			}
			else if(*(pStr) == 'e')
			{
				Color = QUEST_DESC_HIGHLIGHT;
				bHighlight = FALSE;
			}
			++pStr;
			

			// ÀÌÀüÀÇ ³»¿ë ±â·Ï
			if(len>1)
			{
				tb[len] = 0;
				
				QString* pStc = new QString;
				*pStc = tb;
				pStc->Line = line;	
				pStc->Color = Color;
				if(bTitle)		m_TitleList.AddTail(pStc);
				else			m_DescList.AddTail(pStc);

//				printf("%s\n", tb);
				
				memset(tb, 0, sizeof(tb));
				len=0;
			}
		}
		else
		{
			++len;
			++pStr;
		}

		// ÁöÁ¤µÈ ±æÀÌ¸¦ ³Ñ¾î°¬À» °æ¿ì ±â·Ï
		if(linelen > MAX_STR_LENTH)
		{
			QString* pStc = new QString;
			*pStc = tb;
			pStc->Line = line;
			if(bHighlight==FALSE)
				pStc->Color=QUEST_DESC_COLOR;
			else
				pStc->Color=QUEST_DESC_HIGHLIGHT;

			if(bTitle)		m_TitleList.AddTail(pStc);
			else			m_DescList.AddTail(pStc);
			++line;

//			printf("%s\n", tb);
			
			memset(tb, 0, sizeof(tb));			
			len=0;
			linelen=0;
		}
	}

	// À§ÀÇ °æ¿ì¿¡ ÇØ´çµÇÁö ¾Ê´Â ÀÏ¹ÝÀûÀÎ ÅØ½ºÆ®
	if(len)
	{
		QString* pStc = new QString;
		*pStc = tb;
		pStc->Line = line;
		pStc->Color = QUEST_DESC_COLOR;
		if(bTitle)		m_TitleList.AddTail(pStc);
		else			m_DescList.AddTail(pStc);

//		printf("%s\n", tb);
	}	

}