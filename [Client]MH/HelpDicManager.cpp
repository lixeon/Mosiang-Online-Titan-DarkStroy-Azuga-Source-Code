#include "stdafx.h"
#include "HelpDicManager.h"
#include "cPage.h"
#include "cDialogueList.h"
#include "cHyperTextList.h"

GLOBALTON(cHelpDicManager)

cHelpDicManager::cHelpDicManager()
{
}
	
cHelpDicManager::~cHelpDicManager()
{
	DeleteAllPageInfo();
	SAFE_DELETE( m_pDialogue );
	SAFE_DELETE( m_pHyperText );
}

void cHelpDicManager::Init()
{
	nTest = 0;

	LoadHelpDicInfo();

	m_pDialogue = new cDialogueList;
#ifdef _FILE_BIN_
	m_pDialogue->LoadDialogueListFile( HELP_DIALOGUE_PATH, "rb" );
#else
	m_pDialogue->LoadDialogueListFile( HELP_DIALOGUE_PATH );
#endif

	m_pHyperText = new cHyperTextList;
#ifdef _FILE_BIN_
	m_pHyperText->LoadHyperTextFormFile( HELP_HYPERTEXT_PATH, "rb");
#else
	m_pHyperText->LoadHyperTextFormFile( HELP_HYPERTEXT_PATH );
#endif

}

void cHelpDicManager::AddPage( cPage* pPage )
{
	m_HelpDicList.AddTail( pPage );
}

void cHelpDicManager::LoadHelpDicInfo()
{
	CMHFile fp;
#ifdef _FILE_BIN_
	if(!fp.Init(HELP_SCRIPT_PATH, "rb"))
		return;
#else
	if(!fp.Init(HELP_SCRIPT_PATH, "rt"))
		return;
#endif
	char buff[256]={0,};

	while(1)
	{
		fp.GetString(buff);
		if( fp.IsEOF() )
			break;
		if(buff[0] == '@')
		{
			fp.GetLineX(buff, 256);
			continue;
		}

		CMD_ST(buff)
		CMD_CS("$HELPDIC")
			if((fp.GetString())[0] == '{')
				LoadPageInfo(NULL, &fp);
			else
				__asm int 3;
		CMD_EN
	}
}

void cHelpDicManager::LoadPageInfo(cPage* pPage, CMHFile* fp)
{
	char buff[256]={0,};

	DWORD		dwPageId;
	int			nDialogueCount = 0;
	int			nHyperLinkCount = 0;
	DWORD		dwDialogueId;
	HYPERLINK	sHyper;

	while(1)
	{
		fp->GetString(buff);
		if(buff[0] == '}'|| fp->IsEOF())
			break;
		if(buff[0] == '@')
		{
			fp->GetLineX(buff, 256);
			continue;
		}
		CMD_ST(buff)
		CMD_CS("$PAGE")
			if((fp->GetString())[0] == '{')
			{
				cPage* pNewPage = new cPage; // 새로운 페이지의 등록
				AddPage( pNewPage );
				LoadPageInfo(pNewPage, fp);
			}
		CMD_CS("#PAGEINFO")
			dwPageId = fp->GetDword();
			nDialogueCount = fp->GetInt();
			nHyperLinkCount = fp->GetInt();
			pPage->Init( dwPageId );
		CMD_CS("#DIALOGUE")
			for(int i =0; i<nDialogueCount;++i)
			{
				dwDialogueId = fp->GetDword();
				pPage->AddDialogue( dwDialogueId );
			}
		CMD_CS("#HYPERLINK")
			sHyper.wLinkId = fp->GetWord();
			sHyper.wLinkType = fp->GetWord()+1;
			if( sHyper.wLinkType == emLink_Page )
				sHyper.dwData = fp->GetDword();
			pPage->AddHyperLink( &sHyper );
			nTest++;
		CMD_EN
	}
}


void cHelpDicManager::DeleteAllPageInfo()
{
	PTRLISTSEARCHSTART(m_HelpDicList,cPage*,pPage)
		delete pPage;
	PTRLISTSEARCHEND
			
	m_HelpDicList.RemoveAll();
}

cPage* cHelpDicManager::GetMainPage()
{
	if( m_HelpDicList.IsEmpty() ) return NULL;

	return (cPage*)m_HelpDicList.GetHead();
	
}

cPage* cHelpDicManager::GetPage( DWORD dwPageId )
{
	PTRLISTSEARCHSTART(m_HelpDicList, cPage*, pPage)
		if( pPage->GetPageId() == dwPageId )
			return pPage;
	PTRLISTSEARCHEND
			
	return NULL;
}
