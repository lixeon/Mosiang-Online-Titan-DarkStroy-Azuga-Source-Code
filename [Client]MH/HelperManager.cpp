#include "stdafx.h"
#include "HelperManager.h"
#include "cpage.h"
#include "cDialogueList.h"
#include "Helper.h"
#include "HelperSpeechDlg.h"
#include "./interface/cscriptmanager.h"
#include "./interface/cwindowmanager.h"
#include "mhtimemanager.h"

GLOBALTON(cHelperManager)

cHelperManager::cHelperManager()
{
	m_pDialogue = NULL;
	m_pHelper = NULL;
	m_pSpeechDlg = NULL;

	m_bInit = FALSE;
}

cHelperManager::~cHelperManager()
{
	DeleteAllPageInfo();
	SAFE_DELETE(m_pDialogue);
//	SAFE_DELETE(m_pHelper);

//	m_pSpeechDlg->RemoveAll();
}

void cHelperManager::Init()
{
	if( !m_bInit )
	{
		m_pDialogue = new cDialogueList;
#ifdef _FILE_BIN_
		m_pDialogue->LoadDialogueListFile("./Image/Helper_Msg.bin", "rb");
#else
		m_pDialogue->LoadDialogueListFile("./Image/Helper_Msg.txt");
#endif

		LoadSpeechList();

		m_bInit = TRUE;
	}

	m_pHelper = new cHelper;	// À§Ä¡ ÁÂÇ¥ ¼ÂÆÃ
	m_pHelper->SetActive( FALSE ); 
	LoadHelper();

	WINDOWMGR->AddWindow( (cWindow*)m_pHelper );

#ifdef _FILE_BIN_
//	m_pSpeechDlg = (cHelperSpeechDlg*)SCRIPTMGR->GetDlgInfoFromFile("./image/34.bin", "rb");
	m_pSpeechDlg = (cHelperSpeechDlg*)SCRIPTMGR->GetDlgInfoFromFile("./image/InterfaceScript/34.bin", "rb");
#else
	m_pSpeechDlg = (cHelperSpeechDlg*)SCRIPTMGR->GetDlgInfoFromFile("./image/34.txt");
#endif
	WINDOWMGR->AddWindow( (cWindow*)m_pSpeechDlg );
}

// ÁÖÀÇ!! windowmgr°¡ Á¾·á µÉ¶§¿¡¸¸ È£ÃâÇÑ´Ù.
void cHelperManager::Release()
{
	m_pHelper = NULL;
	m_pSpeechDlg = NULL;
}

void cHelperManager::SetActive(BOOL bActive)
{
	m_pHelper->SetActive(bActive);
	m_pSpeechDlg->SetActive(bActive);
}

void cHelperManager::AddPage( cPageBase* pPage )
{
	m_HelperSpeechList.AddTail( pPage );
}

void cHelperManager::DeleteAllPageInfo()
{
	PTRLISTSEARCHSTART(m_HelperSpeechList,cPageBase*,pPage)
		delete pPage;
	PTRLISTSEARCHEND
			
	m_HelperSpeechList.RemoveAll();
}

cPageBase* cHelperManager::GetPage( DWORD dwPageId )
{
	PTRLISTSEARCHSTART(m_HelperSpeechList, cPageBase*, pPage)
		if( pPage->GetPageId() == dwPageId )
			return pPage;
	PTRLISTSEARCHEND
			
	return NULL;
}

void cHelperManager::ShowRespect( DWORD dwPageId, BOOL bComp )
{
	if( m_pSpeechDlg->IsActive() )
	{
		ChangeSpeechDlg( dwPageId, bComp );
		return;
	}

	m_pSpeechDlg->SetHelperPos( m_pHelper->GetAbsX(), m_pHelper->GetAbsY() );
	m_pSpeechDlg->UseComponent( bComp );

	if( !m_pSpeechDlg->StartFadeOut( dwPageId ) ) return;
	m_pSpeechDlg->SetActive( TRUE );

	m_pHelper->SetActive( TRUE );
	m_pHelper->SetMotion( emHM_Stand );
}

void cHelperManager::ChangeSpeechDlg( DWORD dwPageId,  BOOL bComp )
{
	if( !m_pHelper || !m_pSpeechDlg ) return;

	m_pSpeechDlg->SetHelperPos( m_pHelper->GetAbsX(), m_pHelper->GetAbsY() );
	m_pSpeechDlg->UseComponent( bComp );

	m_pSpeechDlg->AddPage( dwPageId );
	m_pSpeechDlg->CloseDialog();

//	m_pHelper->SetActive( TRUE );
	m_pHelper->SetMotion( emHM_Stand );

	if( m_pHelper->IsGreetCheck() )
		m_pHelper->StopGreetCheck();
}

void cHelperManager::LoadSpeechList()
{
	CMHFile fp;
#ifdef _FILE_BIN_
	if(!fp.Init("./Image/HelperSpeech_List.bin", "rb"))
		return;
#else
	if(!fp.Init("./Image/HelperSpeech_List.txt", "rt"))
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
		CMD_CS("$SPEECH")
			if((fp.GetString())[0] == '{')
				LoadPageInfo(NULL, &fp);
			else
				__asm int 3;
		CMD_EN
	}
}

void cHelperManager::LoadPageInfo(cPageBase* pPage, CMHFile* fp)
{
	char buff[256]={0,};

	DWORD		dwPageId, dwNextId, dwPrevId;
	int			nDialogueCount = 0;
	int			nHyperLinkCount = 0;
	DWORD		dwDialogueId;

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
				cPageBase* pNewPage = new cPageBase; // »õ·Î¿î ÆäÀÌÁöÀÇ µî·Ï
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
		CMD_CS("#NEXTPAGE")
			dwNextId = fp->GetDword();
			pPage->SetNextPageId( dwNextId );
		CMD_CS("#PREVPAGE")
			dwPrevId = fp->GetDword();
			pPage->SetPrevPageId( dwPrevId );
//		CMD_CS("#HYPERLINK")
			//
		CMD_EN
	}
}

void cHelperManager::LoadHelper()
{
	CMHFile fp;
#ifdef _FILE_BIN_
//	if(!fp.Init("./image/Helper.bin", "rb"))
	if(!fp.Init("./image/InterfaceScript/Helper.bin", "rb"))
		return;
#else
	if(!fp.Init("./image/Helper.txt", "rt"))
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
		CMD_CS("$HELPER")
			if((fp.GetString())[0] == '{')
				LoadHelperInfo(&fp);
			else
				__asm int 3;
		CMD_EN
	}
}

void cHelperManager::LoadHelperInfo( CMHFile* fp )
{
	char buff[256]={0,};
	LONG	lX = 0;
	LONG	lY = 0;
	WORD	wid, hei;
	DWORD	greetTime;

	while(1)
	{
		fp->GetString(buff);
		if( fp->IsEOF() )
			break;
		if(buff[0] == '@')
		{
			fp->GetLineX(buff, 256);
			continue;
		}

		CMD_ST(buff)
		CMD_CS("#HELPERINFO")
			lX = fp->GetLong();
			lY = fp->GetLong();
			wid = fp->GetWord();
			hei = fp->GetWord();
			greetTime = fp->GetDword();
			m_pHelper->Init( lX, lY, wid, hei, NULL );
			m_pHelper->SetGreetTime( greetTime );
		CMD_CS("$MOTION")
			if((fp->GetString())[0] == '{')
				LoadFrameInfo( fp );
			else
				__asm int 3;
		CMD_EN
	}
}

void cHelperManager::LoadFrameInfo( CMHFile* fp )
{
	char buff[256]={0,};
	cImageRect rect = {0,0,0,0};
	int ImgIdx = 0;
	DWORD wDelay;
	WORD wMotionIdx = 0;
	int	nFrame;

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
		CMD_CS("#MOTIONINFO")
			wMotionIdx = fp->GetWord();
			nFrame = fp->GetInt();
			m_pHelper->SetMaxSprite(wMotionIdx, nFrame);
		CMD_CS("#ADDSPRITE")
			ImgIdx					= fp->GetInt();
			rect.left				= fp->GetLong();
			rect.top				= fp->GetLong();
			rect.right				= fp->GetLong();
			rect.bottom				= fp->GetLong();
			wDelay					= fp->GetWord();

			cImage Sprite;
			SCRIPTMGR->GetImage( ImgIdx, &Sprite, &rect );

			m_pHelper->AddSprite( wMotionIdx, &Sprite, wDelay );
		CMD_EN
	}
}
