// cScriptManager.cpp: implementation of the cScriptManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cScriptManager.h"

#include "../GlobalEventFunc.h"
#include "../WindowIDEnum.h"
#include "cResourceManager.h"

#include "../CharacterDialog.h"
#include "../MugongSuryunDialog.h"
#include "../MugongDialog.h"
#include "../SuryunDialog.h"
#include "../InventoryExDialog.h"
#include "../ChatDialog.h"
//SW050718
//#include "../cJackpotDialog.h"
#include "../MoneyDlg.h"
#include "../DealDialog.h"
#include "../QuickDialog.h"
#include "../PartyMemberDlg.h"
#include "../PartyBtnDlg.h"
#include "../PartyCreateDlg.h"
#include "../PartyInviteDlg.h"
#include "../MousePointer.h"
#include "../MiniMapDlg.h"
#include "../BigMapDlg.h"
#include "../PyogukDialog.h"
#include "../FriendDialog.h"
#include "../NoteDialog.h"
#include "../MiniNoteDialog.h"
#include "../MiniFriendDialog.h"
#include "../ReviveDialog.h"
#include "../MacroDialog.h"
#include "../ChatOptionDialog.h"
#include "../OptionDialog.h"
#include "../ExitDialog.h"
#include "../ExchangeDialog.h"
#include "../CharStateDialog.h"
#include "../MenuSlotDialog.h"
#include "../PKLootingDialog.h"
#include "../StreetStall.h"
// LYJ 051017 구입노점상 추가
#include "../StallKindSelectDlg.h"
#include "../StreetBuyStall.h"
#include "../BuyRegDialog.h"

#include "../NpcScriptDialog.h"
#include "../HelpDialog.h"
#include "../HelperSpeechDlg.h"
#include "../MainBarDialog.h"
#include "../ObjectGuagen.h"
#include "../cComboBoxEx.h"
#include "../CharMakeDialog.h"
#include "../MonsterGuageDlg.h"
#include "../PrivateWarehouseDialog.h"
#include "../MunpaMarkDialog.h"
#include "../cDivideBox.h"
#include "../LoadingDlg.h"
#include "../UpgradeDlg.h"
#include "../MixDialog.h"
#include "../TitanPartsMakeDlg.h"	// magi82 - Titan(070112)
#include "../TitanMixDlg.h"			// magi82 - Titan(070119)
#include "../TitanRepairDlg.h"		// 2007. 9. 11. CBH
#include "../TitanRecallDlg.h"		// 2007. 9. 13. CBH
#include "../TitanUpgradeDlg.h"		// magi82 - Titan(070119)
#include "../TitanBreakDlg.h"		// magi82 - Titan(070119)
#include "../TitanGuageDlg.h"		// magi82 - Titan(070305)
//#include "../TitanMugongMixDlg.h"	// magi82 - Titan(070611) 타이탄 무공변환 주석처리
#include "../TitanRegisterDlg.h"	// magi82 - Titan(070320)
#include "../TitanDissolutionDlg.h"	// magi82 - Titan(070321)
#include "../TitanChangePreView.h"
#include "../TitanInventoryDlg.h"
#include "../ReinforceDlg.h"
#include "../ChannelDialog.h"
#include "../ServerListDialog.h"
#include "../WantNpcDialog.h"
#include "../WantRegistDialog.h"
#include "../QuestTotalDialog.h"
#include "../QuestDialog.h"
#include "../WantedDialog.h"
#include "../JournalDialog.h"
#include "../MPRegistDialog.h"
#include "../MPMissionDialog.h"
#include "../MPNoticeDialog.h"
#include "../MPGuageDialog.h"
#include "../ChinaAdviceDlg.h"
#include "../BailDialog.h"
#include "../EventNotifyDialog.h"
#include "../MallNoticeDialog.h"
#include "../DissolutionDialog.h"
#include "../GuildCreateDialog.h"
#include "../GuildDialog.h"
#include "../GuildInviteDialog.h"
#include "../GuildMarkDialog.h"
#include "../GuildLevelupDialog.h"
#include "../GuildNickNameDialog.h"
#include "../GuildRankDialog.h"
#include "../GuildWarehouseDialog.h"
#include "../ItemShopDialog.h"
#include "../ItemShopGridDialog.h"
#include "../ItemShopInven.h"
#include "../MoveDialog.h"
#include "../PointSaveDialog.h"
#include "../SkillPointRedist.h"
#include "../SkillPointNotify.h"
//#include "../MainDialog.h"
//#include "../WearedExDialog.h"

#include "../ShoutDialog.h"
#include "../ShoutchatDialog.h"
#include "../ChaseDialog.h"
#include "../ChaseinputDialog.h"
#include "../NameChangeDialog.h"
#include "../NameChangeNotifyDlg.h"

// Guild Tournament
#include "../GTRegistDialog.h"
#include "../GTRegistcancelDialog.h"
#include "../GTStandingDialog.h"
#include "../GTBattleListDialog.h"
#include "../GTScoreInfoDialog.h"

#include "../ItemManager.h"
//////////
//MurimNet
#include "../MNPlayRoomDialog.h"
#include "../MNChannelDialog.h"
#include "../GuildFieldWarDialog.h"
#include "../PartyWarDialog.h"
#include "../SeigeWarDialog.h"

// Character Change
#include "../CharChangeDlg.h"
#include "../SealDialog.h"
#include "../ChangeJobDialog.h"
#include "../ReinforceResetDlg.h"
#include "../GuageDialog.h"
#include "../RareCreateDialog.h"
// 06. 02. 강화 재료 안내 인터페이스 추가 - 이영준
#include "../ReinforceDataGuideDlg.h"
// 06. 02. 내정보창 팁보기 추가 - 이영준
#include "../TipBrowserDlg.h"
#include "../KeySettingTipDlg.h"


// Pet
#include "../PetStateDlg.h"
#include "../PetStateMiniDlg.h"
#include "../PetInventoryDlg.h"
#include "../PetUpgradeDialog.h"
#include "../PetRevivalDialog.h"

#include "../GuildNoteDlg.h"
#include "../UnionNoteDlg.h"
#include "../GuildNoticeDlg.h"

#include "../SkillOptionChangeDlg.h"
#include "../SkillOptionClearDlg.h"

#include "../GuildJoinDialog.h"

// ScreenShot
#include "../ScreenShotDlg.h"

#include "../GuildPlusTimeDialog.h"
#include "../GuildMunhaDialog.h"
#include "../GuildTraineeDialog.h"

#include "../SurvivalCountDialog.h"
#include "../UniqueItemCurseCancellationDlg.h"
#include "../UniqueItemMixDlg.h"

#include "../SOSDialog.h"
#include "../IntroReplayDlg.h"
#include "../UniqueItemMixProgressBardlg.h"		// 2007. 10. 22. CBH
#include "../TitanMixProgressBardlg.h"		// 2007. 10. 22. CBH
#include "../TitanPartsProgressBardlg.h"		// 2007. 10. 23. CBH

#include "../SkinSelectDialog.h"	// 2007. 12. 4. CBH
#include "../CostumeSkinSelectDialog.h"	// 2008. 1. 16. CBH

#include "../SkillPointResetDlg.h"

#include "../NumberPadDialog.h"	// 2008. 3. 17. CBH
#include "../StreetStallItemViewDlg.h"

#include "../StallFindDlg.h"	// 노점검색 추가 by Stiner(8)
#include "../AutoNoteDlg.h"		// AutoNote
#include "../AutoAnswerDlg.h"

#include "FortWarDialog.h"

#include "../PartyMatchingDlg.h"	// 2008. 5. 23. CBH

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(cScriptManager);
cScriptManager::cScriptManager()
{	
}

cScriptManager::~cScriptManager()
{
	ReleaseParseTypeData();
	ReleaseImagePathTable();
}

void cScriptManager::ReleaseParseTypeData()
{
	m_ParseTypeTable.SetPositionHead();
	PARSE_TYPE_INFO* pParseTypeInfo = NULL;
	while(pParseTypeInfo = m_ParseTypeTable.GetData())
	{
		m_ParseTypeTable.StartGetMultiData(pParseTypeInfo->nKeyValue);
		PARSE_TYPE_INFO* pParseTypeSubInfo = NULL;
		while( pParseTypeSubInfo = m_ParseTypeTable.GetMultiData() )
		{
			m_ParseTypeTable.Remove(pParseTypeSubInfo->nKeyValue);
			SAFE_DELETE(pParseTypeSubInfo);
		}		
	}
	m_ParseTypeTable.RemoveAll();
}

void cScriptManager::ReleaseImagePathTable()
{
	m_ImageHardPath.SetPositionHead();
	sIMAGHARDPATH* pImageHardPath = NULL;
	while(pImageHardPath = m_ImageHardPath.GetData())
	{		
		SAFE_DELETE(pImageHardPath);
	}	
	m_ImageHardPath.RemoveAll();

	m_ItemHardPath.SetPositionHead();
	sIMAGHARDPATH* pItemHardPath = NULL;
	while(pItemHardPath = m_ItemHardPath.GetData())
	{		
		SAFE_DELETE(pItemHardPath);
	}	
	m_ItemHardPath.RemoveAll();

	m_MugongHardPath.SetPositionHead();
	sIMAGHARDPATH* pMugongHardPath = NULL;
	while(pMugongHardPath = m_MugongHardPath.GetData())
	{		
		SAFE_DELETE(pMugongHardPath);
	}	
	m_MugongHardPath.RemoveAll();

	m_AbilityHardPath.SetPositionHead();
	sIMAGHARDPATH* pAbilityHardPath = NULL;
	while(pAbilityHardPath = m_AbilityHardPath.GetData())
	{		
		SAFE_DELETE(pAbilityHardPath);
	}	
	m_AbilityHardPath.RemoveAll();

	m_BuffHardPath.SetPositionHead();	
	sIMAGHARDPATH* pBuffHardPath = NULL;
	while(pBuffHardPath = m_BuffHardPath.GetData())
	{		
		SAFE_DELETE(pBuffHardPath);
	}	
	m_BuffHardPath.RemoveAll();

	m_MiniMapHardPath.SetPositionHead();
	sIMAGHARDPATH* pMiniMapHardPath = NULL;
	while(pMiniMapHardPath = m_MiniMapHardPath.GetData())
	{		
		SAFE_DELETE(pMiniMapHardPath);
	}	
	m_MiniMapHardPath.RemoveAll();

	m_JackPotHardPath.SetPositionHead();
	sIMAGHARDPATH* pJackPotHardPath = NULL;
	while(pJackPotHardPath = m_JackPotHardPath.GetData())
	{		
		SAFE_DELETE(pJackPotHardPath);
	}	
	m_JackPotHardPath.RemoveAll();
}

char * cScriptManager::GetMsg( int idx )
{
	return RESRCMGR->GetMsg(idx);
}
void cScriptManager::GetImage( CMHFile * fp, cImage * pImage )
{
	ASSERT(pImage);
	char buff[255];
	int	idx = 0;
	cImageRect rect = {0,0,0,0};

	fp->GetString(buff);
	if(buff[0] != '(')		return ;
	idx						= fp->GetInt();
	rect.left				= fp->GetLong();
	rect.top				= fp->GetLong();
	rect.right				= fp->GetLong();
	rect.bottom				= fp->GetLong();
	fp->GetString(buff);
	if(buff[0] != ')')		return ;

	GetImage( idx, pImage, &rect );	
}

void cScriptManager::GetImage( int idx, cImage * pImage, cImageRect * rect )
{
	ASSERT(pImage);

	pImage->SetSpriteObject(RESRCMGR->GetImageInfo(idx));
	IMAGE_NODE* pNode = RESRCMGR->GetInfo(idx);
	if(pNode)
		pImage->SetImageSrcSize(&pNode->size);
	pImage->SetImageSrcRect(rect);
}


void cScriptManager::InitScriptManager()
{
	m_ImageHardPath.Initialize(150);
	m_ItemHardPath.Initialize(1200);
	m_MugongHardPath.Initialize(100);
	m_AbilityHardPath.Initialize(100);
    m_BuffHardPath.Initialize(200);
	m_MiniMapHardPath.Initialize(40);
    m_JackPotHardPath.Initialize(10);

	CMHFile file;
	sIMAGHARDPATH* pPath = NULL;
	int index = 0;
	if( file.Init( FILE_IMAGE_HARD_PATH, "rb" ) == TRUE )
	{
		while( !file.IsEOF() )
		{
			pPath = new sIMAGHARDPATH;
			index = file.GetInt();
			pPath->idx = file.GetInt();
			pPath->left = file.GetLong();
			pPath->top = file.GetLong();
			pPath->right = file.GetLong();
			pPath->bottom = file.GetLong();

			if( m_ImageHardPath.GetData( index ) )
			{
				SAFE_DELETE(pPath);
				continue;
			}

			m_ImageHardPath.Add( pPath, index );
		}

		file.Release();
	}
	if( file.Init( FILE_IMAGE_ITEM_PATH, "rb" ) == TRUE )
	{
		while( !file.IsEOF() )
		{
			pPath = new sIMAGHARDPATH;
			index = file.GetInt();
			pPath->idx = file.GetInt();
			pPath->left = file.GetLong();
			pPath->top = file.GetLong();
			pPath->right = file.GetLong();
			pPath->bottom = file.GetLong();

			if( m_ItemHardPath.GetData( index ) )
			{
				SAFE_DELETE(pPath);
				continue;
			}

			m_ItemHardPath.Add( pPath, index );
		}

		file.Release();
	}
	if( file.Init( FILE_IMAGE_MUGONG_PATH, "rb" ) == TRUE )
	{
		while( !file.IsEOF() )
		{
			pPath = new sIMAGHARDPATH;
			index = file.GetInt();
			pPath->idx = file.GetInt();
			pPath->left = file.GetLong();
			pPath->top = file.GetLong();
			pPath->right = file.GetLong();
			pPath->bottom = file.GetLong();

			if( m_MugongHardPath.GetData( index ) )
			{
				SAFE_DELETE(pPath);
				continue;
			}

			m_MugongHardPath.Add( pPath, index );
		}

		file.Release();
	}
	if( file.Init( FILE_IMAGE_ABILITY_PATH, "rb" ) == TRUE )
	{
		while( !file.IsEOF() )
		{
			pPath = new sIMAGHARDPATH;
			index = file.GetInt();
			pPath->idx = file.GetInt();
			pPath->left = file.GetLong();
			pPath->top = file.GetLong();
			pPath->right = file.GetLong();
			pPath->bottom = file.GetLong();

			if( m_AbilityHardPath.GetData( index ) )
			{
				SAFE_DELETE(pPath);
				continue;				
			}

			m_AbilityHardPath.Add( pPath, index );
		}

		file.Release();
	}
	if( file.Init( FILE_IMAGE_BUFF_PATH, "rb" ) == TRUE )
	{		
		while( !file.IsEOF() )
		{
			pPath = new sIMAGHARDPATH;
			index = file.GetInt();
			pPath->idx = file.GetInt();
			pPath->left = file.GetLong();
			pPath->top = file.GetLong();
			pPath->right = file.GetLong();
			pPath->bottom = file.GetLong();

			if( m_BuffHardPath.GetData( index ) )
			{
				SAFE_DELETE(pPath);
				continue;
			}

			m_BuffHardPath.Add( pPath, index );
		}

		file.Release();
	}
	if( file.Init( FILE_IMAGE_MINIMAP_PATH, "rb" ) == TRUE )
	{
		while( !file.IsEOF() )
		{
			pPath = new sIMAGHARDPATH;
			index = file.GetInt();
			pPath->idx = file.GetInt();
			pPath->left = file.GetLong();
			pPath->top = file.GetLong();
			pPath->right = file.GetLong();
			pPath->bottom = file.GetLong();

			if( m_MiniMapHardPath.GetData( index ) )
			{
				SAFE_DELETE(pPath);
				continue;
			}

			m_MiniMapHardPath.Add( pPath, index );
		}

		file.Release();
	}
	if( file.Init( FILE_IMAGE_JACKPOT_PATH, "rb" ) == TRUE )
	{
		while( !file.IsEOF() )
		{
			pPath = new sIMAGHARDPATH;
			index = file.GetInt();
			pPath->idx = file.GetInt();
			pPath->left = file.GetLong();
			pPath->top = file.GetLong();
			pPath->right = file.GetLong();
			pPath->bottom = file.GetLong();

			if( m_JackPotHardPath.GetData( index ) )
			{
				SAFE_DELETE(pPath);
				continue;
			}

			m_JackPotHardPath.Add( pPath, index );
		}

		file.Release();
	}
}


void cScriptManager::GetImage( int hard_idx, cImage * pImage , ePATH_FILE_TYPE type )
{
	sIMAGHARDPATH* pData = NULL;
	switch( type )
	{
	case PFT_HARDPATH:	pData = m_ImageHardPath.GetData( hard_idx );	break;
	case PFT_ITEMPATH:	pData = m_ItemHardPath.GetData( hard_idx );	break;
	case PFT_MUGONGPATH:	pData = m_MugongHardPath.GetData( hard_idx );	break;
	case PFT_ABILITYPATH:	pData = m_AbilityHardPath.GetData( hard_idx );	break;
	case PFT_BUFFPATH:	pData = m_BuffHardPath.GetData( hard_idx );	break;
	case PFT_MINIMAPPATH:	pData = m_MiniMapHardPath.GetData( hard_idx );	break;
	case PFT_JACKPOTPATH:	pData = m_JackPotHardPath.GetData( hard_idx );	break;
	}

	if( pData == NULL )
	{
		pImage = NULL;
		return;
	}

	cImageRect rect = {0,0,0,0};
	rect.right = pData->right;
	rect.top = pData->top;
	rect.left = pData->left;
	rect.bottom = pData->bottom;

	GetImage( pData->idx, pImage, &rect );
}


/*
void cScriptManager::GetImage( int hard_idx, cImage * pImage , ePATH_FILE_TYPE type )
{
	CMHFile fp;
	//--------------------------------------
#ifdef _FILE_BIN_
	char msg[255];
	switch( type )
	{
	case PFT_HARDPATH:
		fp.Init( FILE_IMAGE_HARD_PATH, "rb" );
		break;
	case PFT_ITEMPATH:
		fp.Init( FILE_IMAGE_ITEM_PATH, "rb" );
		break;
	case PFT_MUGONGPATH:
		fp.Init( FILE_IMAGE_MUGONG_PATH, "rb" );
		break;
	case PFT_ABILITYPATH:
		fp.Init( FILE_IMAGE_ABILITY_PATH, "rb");
		break;
	case PFT_BUFFPATH:
		fp.Init( FILE_IMAGE_BUFF_PATH, "rb");
		break;
	case PFT_MINIMAPPATH:
		fp.Init( FILE_IMAGE_MINIMAP_PATH, "rb");
		break;
	case PFT_JACKPOTPATH:
		fp.Init( FILE_IMAGE_JACKPOT_PATH, "rb");
		break;
	}
	for( int i = 0 ; i < hard_idx ; ++i )
	{
		fp.GetLine(msg, 255);
	}
	fp.GetInt();
//	fp.Seek(1);
#else
	char msg[255];
	switch( type )
	{
	case PFT_HARDPATH:
		fp.Init( FILE_IMAGE_HARD_PATH, "rt" );
		break;
	case PFT_ITEMPATH:
		fp.Init( FILE_IMAGE_ITEM_PATH, "rt" );
		break;
	case PFT_MUGONGPATH:
		fp.Init( FILE_IMAGE_MUGONG_PATH, "rt" );
		break;
	case PFT_ABILITYPATH:
		fp.Init( FILE_IMAGE_ABILITY_PATH, "rt");
		break;
	case PFT_BUFFPATH:
		fp.Init( FILE_IMAGE_BUFF_PATH, "rt");
		break;
	case PFT_MINIMAPPATH:
		fp.Init( FILE_IMAGE_MINIMAP_PATH, "rt");
		break;
	case PFT_JACKPOTPATH:
		fp.Init( FILE_IMAGE_JACKPOT_PATH, "rt");
		break;
	}
	for( int i = 0 ; i < hard_idx ; ++i )
	{
		fp.GetLine(msg, 255);
	}
	fp.GetInt();
	fp.Seek(1);
#endif
	//--------------------------------------
	
	int	idx = 0;
	cImageRect rect = {0,0,0,0};
	idx						= fp.GetInt();
	rect.left				= fp.GetLong();
	rect.top				= fp.GetLong();
	rect.right				= fp.GetLong();
	rect.bottom				= fp.GetLong();
	
	if( rect.right - rect.left == 0 || rect.top - rect.bottom == 0 )
		return;

	GetImage( idx, pImage, &rect );
}*/



cWindow * cScriptManager::GetDlgInfoFromFile(char * filePath, char* mode)
{
	CMHFile fp;
	if(!fp.Init(filePath, mode))
		return NULL;
	char buff[256]={0,};
	cWindow * dlg = NULL;


	while(1)
	{
		fp.GetString(buff);
		if(fp.IsEOF())
			break;
		if(buff[0] == '@')
		{
			fp.GetLineX(buff, 256);
			continue;
		}
				
		////////// 2007. 5. 18. CBH - Parse Type List 관련 처리 추가 /////////
		PARSE_TYPE eParseType = GetParseType(buff);		
		////////////////////////////////////////////////////////////////

		{			
			////////////////////////////////////////////////////////////////
			/// 2007. 5. 21. CBH - CMD_ST, CMD_CS -> swich, case 문으로 변경   ///
			////////////////////////////////////////////////////////////////
			switch(eParseType)
			{
			case eDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cDialog), &fp);
					else
						__asm int 3;
					break;
				}				
			case eCHARGUAGEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuageDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eLISTDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cListDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eLISTDLGEX:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cListDialogEx), &fp);
					else
						__asm int 3;
					break;
				}
			case eCHARINFODLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CCharacterDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eMUGONGSURYUNDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMugongSuryunDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eMAINDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMainBarDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eINVENTORYDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CInventoryExDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eCHATDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CChatDialog), &fp);
					else
						__asm int 3;
					break;
				}				
			//SW050722 attach to minimap
			//		//SW050718
			//		CMD_CS("$JACKPOT")
			//			if((fp.GetString())[0] == '{')
			//				dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cJackpotDialog), &fp);
			//			else
			//				__asm int 3;
			// LYJ 051017 구입노점상 추가
			case eSTALLOPTIONDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CStallKindSelectDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eBUYSTALLDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CStreetBuyStall), &fp);
					else
						__asm int 3;
					break;
				}
			case eBUYREGDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CBuyRegDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eSTREETSTALLDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CStreetStall), &fp);
					else
						__asm int 3;
					break;
				}
			case eNPCSCRIPTDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cNpcScriptDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eHELPDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cHelpDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eCHARMAKEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cCharMakeDlg *)GetInfoFromFile((cWindow*)(new cCharMakeDlg), &fp);
					else
						__asm int 3;
					break;
				}				
			case eHELPERSPEECHDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cHelperSpeechDlg *)GetInfoFromFile((cWindow*)(new cHelperSpeechDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eMONEYDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMoneyDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eDEALDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CDealDialog), &fp);
					else
						__asm int 3;	
					break;
				}
			case eQUICKDIALOG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CQuickDialog), &fp);
					else
						__asm int 3;	
					break;
				}
			case ePA_MEMBER1DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CPartyMemberDlg *)GetInfoFromFile((cWindow*)(new CPartyMemberDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case ePA_MEMBER2DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CPartyMemberDlg *)GetInfoFromFile((cWindow*)(new CPartyMemberDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case ePA_MEMBER3DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CPartyMemberDlg *)GetInfoFromFile((cWindow*)(new CPartyMemberDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case ePA_MEMBER4DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CPartyMemberDlg *)GetInfoFromFile((cWindow*)(new CPartyMemberDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case ePA_MEMBER5DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CPartyMemberDlg *)GetInfoFromFile((cWindow*)(new CPartyMemberDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case ePA_MEMBER6DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CPartyMemberDlg *)GetInfoFromFile((cWindow*)(new CPartyMemberDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case ePA_BTNDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CPartyBtnDlg *)GetInfoFromFile((cWindow*)(new CPartyBtnDlg), &fp);
					else
						__asm int 3;	
					break;
				}
			case ePA_CREATEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CPartyCreateDlg *)GetInfoFromFile((cWindow*)(new CPartyCreateDlg), &fp);
					else
						__asm int 3;			
					break;
				}
			case ePA_INVITEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CPartyInviteDlg *)GetInfoFromFile((cWindow*)(new CPartyInviteDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eANI:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cAni *)GetInfoFromFile((cWindow*)(new cAni), &fp);
					else
						__asm int 3;	
					break;
				}

			case eMOUSEPOINTER:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CMousePointer *)GetInfoFromFile((cWindow*)(new CMousePointer), &fp);
					else
						__asm int 3;
					break;
				}
			case ePYOGUKDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CPyogukDialog *)GetInfoFromFile((cWindow*)(new CPyogukDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eFRIENDDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CFriendDialog *)GetInfoFromFile((cWindow*)(new CFriendDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eNOTEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CNoteDialog *)GetInfoFromFile((cWindow*)(new CNoteDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eMINNOTEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CMiniNoteDialog *)GetInfoFromFile((cWindow*)(new CMiniNoteDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eMINFRIENDDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CMiniFriendDialog *)GetInfoFromFile((cWindow*)(new CMiniFriendDialog), &fp);
					else
						__asm int 3;	
					break;
				}
			case eREVIVEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CReviveDialog*)GetInfoFromFile((cWindow*)(new CReviveDialog), &fp);
					else
						__asm int 3;			
					break;
				}
			case eCHANNELDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CChannelDialog*)GetInfoFromFile((cWindow*)(new CChannelDialog), &fp);
					else
						__asm int 3;			
					break;
				}
			case eWANTNPCDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CWantNpcDialog*)GetInfoFromFile((cWindow*)(new CWantNpcDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eWANTREGISTDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CWantRegistDialog*)GetInfoFromFile((cWindow*)(new CWantRegistDialog), &fp);
					else
						__asm int 3;
					break;
				}				
			/////		
			//KES
			case eEXITDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = GetInfoFromFile((cWindow*)(new CExitDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eMACRODLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = GetInfoFromFile((cWindow*)(new CMacroDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eOPTIONDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = GetInfoFromFile((cWindow*)(new COptionDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eEXCHANGEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = GetInfoFromFile((cWindow*)(new CExchangeDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eCHARSTATEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = GetInfoFromFile((cWindow*)(new CCharStateDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eMENUSLOTDIALOG:
				{
					if((fp.GetString())[0] == '{')
						dlg = GetInfoFromFile((cWindow*)(new CMenuSlotDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case ePKLOOTINGDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = GetInfoFromFile((cWindow*)(new CPKLootingDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eCNADVICEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = GetInfoFromFile((cWindow*)(new CChinaAdviceDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eDISSOLUTIONDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = GetInfoFromFile((cWindow*)(new CDissolutionDialog), &fp);
					else
						__asm int 3;
					break;
				}					
			//////////
			//MurimNet
			case eMNPLAYROOMDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = GetInfoFromFile((cWindow*)(new CMNPlayRoomDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eMNCHANNELDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = GetInfoFromFile((cWindow*)(new CMNChannelDialog), &fp);
					else
						__asm int 3;			
					break;
				}
			///////////////////////////////////////
			case eMINIMAPDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMiniMapDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eBIGMAPDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CBigMapDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eMONSTERGUAGEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMonsterGuageDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eDIVIDEBOX:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cDivideBox), &fp);
					else
						__asm int 3;
					break;
				}
			case eLOADINGDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CLoadingDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eUPGRADEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CUpgradeDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eMIXDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMixDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eREINFORCEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CReinforceDlg), &fp);
					else
						__asm int 3;
					break;
				}
			// JSD-------------------------------------------------------------------------------------
			case eSERVERLISTDLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CServerListDialog), &fp );
					else
						__asm int 3;
					break;
				}
			case eQUESTTOTALDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CQuestTotalDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eMPREGISTDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMPRegistDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eMPMISSIONDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMPMissionDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eMPNOTICEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMPNoticeDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eMPGUAGEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMPGuageDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eBAILDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CBailDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eGMOPENTALKDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CEventNotifyDialog), &fp);
					else				
						__asm int 3;
					break;
				}
			case eITEM_MALLDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMallNoticeDialog), &fp);
					else				
						__asm int 3;
					break;
				}
			case eGUILDCREATEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildCreateDialog), &fp);
					else
						__asm int 3;	
					break;
				}
			case eGUILDUNIONCREATEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CGuildUnionCreateDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eGUILDDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildDialog), &fp);
					else
						__asm int 3;	
					break;
				}
			case eGUILDINVITEDlG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildInviteDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eGUILDMARKDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildMarkDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eGUILDLEVELUPDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildLevelUpDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eGUILDNICKNAMEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildNickNameDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eGUILDRANKDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildRankDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eGUILDWAREHOUSEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildWarehouseDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eITEMALLBASE:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CItemShopDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eSAVEMOVEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMoveDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eCHANGSAVEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CPointSaveDialog), &fp);
					else
						__asm int 3;
					break;
				}
			// guildfieldwar --------------------------------------------------------------------------------
			case eGFWARDECLAREDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGFWarDeclareDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eGFWARRESULTDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGFWarResultDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eGFWARINFODLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGFWarInfoDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eGUILDWARINFODLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildWarInfoDlg), &fp);
					else
						__asm int 3;
					break;
				}
			//------------------------------------------------------------------------------------------------
			case eSKILLPOINTAGAINDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSkillPointRedist), &fp);
					else
						__asm int 3;
					break;
				}
			case eSKILLPOINTOPDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSkillPointNotify), &fp);
					else
						__asm int 3;
					break;
				}
			// partywar -----------------------------
			case ePARTYWARDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CPartyWarDialog), &fp);
					else
						__asm int 3;
					break;
				}
			//---------------------------------------
			case eCHALLWORLDCREATEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CShoutDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eCHATALLDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CShoutchatDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eSEECHASEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CChaseDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eCHASEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CChaseinputDialog), &fp);
					else
						__asm int 3;			
					break;
				}
			case eCHNAMECHANGEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CNameChangeDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eOPCHNAMECHANGEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CNameChangeNotifyDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eGDTENTRYDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGTRegistDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eGDTENTRYCANCELDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGTRegistcancelDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eGDTOURNAMENTDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGTStandingDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eGDTVIEWDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGTBattleListDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eEVENTMAPCOUNTDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGTScoreInfoDialog), &fp);
					else
						__asm int 3;			
					break;
				}
			// seigewar
			case eSEIGEWARTIMEREGDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSWTimeRegDlg), &fp);
					else
						__asm int 3;		
					break;
				}
			case eSEIGEWARPROTECTREGDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSWProtectRegDlg), &fp);
					else
						__asm int 3;	
					break;
				}
			case eSEIGEWARINFODLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSWInfoDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eSWSTARTTIMEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSWStartTimeDialog), &fp);
					else
						__asm int 3;
					break;
				}
			// Character change
			case eCHARCHANGEEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CCharChangeDlg), &fp);
					else
						__asm int 3;
					break;
				}
			// ItemSeal
			case eITEMLOCKDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSealDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eSWPROFITDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSWProfitDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eENGRAVEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSWEngraveDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eSWCOUNTDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSWTimeDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eCHANGEJOBDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CChangeJobDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eRFDefaultDlg:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CReinforceResetDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eRareCreateDlg:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CRareCreateDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case ePETSTATEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CPetStateDlg), &fp);
					else
						_asm int 3;
					break;
				}
			case ePETSTATEMINIDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CPetStateMiniDlg), &fp);
					else
						_asm int 3;
					break;
				}
			case ePETINVENTORYDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CPetInventoryDlg), &fp);
					else
						_asm int 3;
					break;
				}
			case ePETUPGRADEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CPetUpgradeDialog), &fp);
					else
						_asm int 3;
					break;
				}
			case ePETREVIVALDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CPetRevivalDialog), &fp);
					else
						_asm int 3;
					break;
				}
			// 06. 02. 강화 재료 안내 인터페이스 추가 - 이영준
			case eRFDATAGUIDEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CReinforceDataGuideDlg), &fp);
					else
						__asm int 3;
					break;
				}
			// 06. 02. 내정보창 팁보기 추가 - 이영준
			case eTIPBROWSERDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTipBrowserDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eKSDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CKeySettingTipDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eGUILDNOTEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CGuildNoteDlg *)GetInfoFromFile((cWindow*)(new CGuildNoteDlg), &fp);
					else
						__asm int 3;
					break;
				}
			}
		}
		{
			switch(eParseType)
			{
			case eALLYNOTEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CUnionNoteDlg *)GetInfoFromFile((cWindow*)(new CUnionNoteDlg), &fp);
					else
						__asm int 3;
					break;
				}
				// 06. 03. 문파공지 - 이영준
			case eGUILDNOTICEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CGuildNoticeDlg *)GetInfoFromFile((cWindow*)(new CGuildNoticeDlg), &fp);
					else
						__asm int 3;
					break;
				}	
			case eGUILDPLUSTIMEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildPlusTimeDlg), &fp);
					else
						__asm int 3;
					break;
				}
				/////////////////////////////////////////////////////////////
				// 06. 06. 2차 전직 - 이영준
				// 무공 변환
			case eSkillTransDlg:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CSkillOptionChangeDlg *)GetInfoFromFile((cWindow*)(new CSkillOptionChangeDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eTDefaultDlg:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CSkillOptionClearDlg *)GetInfoFromFile((cWindow*)(new CSkillOptionClearDlg), &fp);
					else
						__asm int 3;
					break;
				}
				/////////////////////////////////////////////////////////////
			case eSCREENSHOTDLG:  //스크린샷
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CScreenShotDlg), &fp);
					else
						__asm int 3;
					break;
				}
				//SW060713 문하생
			case eJOINOPTIONDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CGuildJoinDialog), &fp);
					else
						_asm int 3;
					break;
				}
				//SW061019 서바이벌모드
			case eSURVCOUNTERDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CSurvivalCountDialog), &fp);
					else
						_asm int 3;
					break;
				}
			case eGUILDTRAINEEINFODLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CGuildMunhaDialog), &fp);
					else
						_asm int 3;
					break;
				}
			case eGUILDTRAINEEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CGuildTraineeDialog), &fp);
					else
						_asm int 3;
					break;
				}
				// magi82 - Titan(070112) ////////////////////////////////////////////////
			case eTITANPARTSMAKEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanPartsMakeDlg), &fp);
					else
						__asm int 3;
					break;
				}
				//////////////////////////////////////////////////////////////////////////

				// magi82 - Titan(070119) ////////////////////////////////////////////////
			case eTITANMIXDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanMixDlg), &fp);
					else
						__asm int 3;
					break;
				}			
			case eTITANUPGRADEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanUpgradeDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eTITANBREAKDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanBreakDlg), &fp);
					else
						__asm int 3;
					break;
				}
				//////////////////////////////////////////////////////////////////////////
				// magi82 - Titan(070212) ////////////////////////////////////////////////
			case eTITANPARTSCHANGEPREVIEWDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanChangePreViewDlg), &fp);
					else
						__asm int 3;
					break;
				}
				//////////////////////////////////////////////////////////////////////////
				// magi82 - Titan(070222) ////////////////////////////////////////////////
			case eTITAN_INVENTORY_DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanInventoryDlg), &fp);
					else
						__asm int 3;
					break;
				}
				//////////////////////////////////////////////////////////////////////////
				// magi82 - Titan(070305) ////////////////////////////////////////////////
			case eTITAN_GUAGE_DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanGuageDlg), &fp);
					else
						__asm int 3;
					break;
				}
				//////////////////////////////////////////////////////////////////////////
				// magi82 - Titan(070611) 타이탄 무공변환 주석처리
				/*
			case eTITAN_MUGONGMIX_DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanMugongMixDlg), &fp);
					else
						__asm int 3;
					break;
				}
				*/
				// magi82 - Titan(070320) ////////////////////////////////////////////////
			case eTITAN_USE_DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanRegisterDlg), &fp);
					else
						__asm int 3;
					break;
				}
				//////////////////////////////////////////////////////////////////////////
				// magi82 - Titan(070321) ////////////////////////////////////////////////
			case eTITAN_BONGIN_DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanDissolutionDlg), &fp);
					else
						__asm int 3;
					break;
				}
				//////////////////////////////////////////////////////////////////////////
				// magi82 - UniqueItem(070703) ////////////////////////////////////////////////
			case eUNIQUEITEM_CURSECANCELLATION_DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CUniqueItemCurseCancellationDlg), &fp);
					else
						__asm int 3;
					break;
				}
				//////////////////////////////////////////////////////////////////////////
				// magi82 - UniqueItem(070709) ////////////////////////////////////////////////
			case eUNIQUEITEM_MIX_DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CUniqueItemMixDlg), &fp);
					else
						__asm int 3;
					break;
				}
				//////////////////////////////////////////////////////////////////////////
				// magi82 - SOS(070724) ////////////////////////////////////////////////
			case eSOS_DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSOSDlg), &fp);
					else
						__asm int 3;
					break;
				}
				//////////////////////////////////////////////////////////////////////////
				// magi82 - Intro(070802)
			case eINTROREPLAY_DLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CIntroReplayDlg), &fp );
					else
						__asm int 3;
					break;
				}
				//////////////////////////////////////////////////////////////////////////
				// 2007. 9. 11. CBH - 타이탄 수리창
			case eTITANREPAIR_DLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CTitanRepairDlg), &fp );
					else
						__asm int 3;
					break;
				}
				/////////////////////////////////////////////////////////////////////////////
				// 2007. 9. 13. CBH - 타이탄 소환 게이지
			case eTITANRECALL_DLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CTitanRecallDlg), &fp );
					else
						__asm int 3;
					break;
				}
				/////////////////////////////////////////////////////////////////////////////
				// 2007. 10. 19. CBH - 유니크 아이탬 조합 게이지
			case eUNIQUEITEMMIX_PROGRESSBAR_DLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CUniqueItemMixProgressBarDlg), &fp );
					else
						__asm int 3;
					break;
				}
				/////////////////////////////////////////////////////////////////////////////
				// 2007. 10. 22. CBH - 타이탄 조합 게이지
			case eTITANMIX_PROGRESSBAR_DLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CTitanMixProgressBarDlg), &fp );
					else
						__asm int 3;
					break;
				}
				/////////////////////////////////////////////////////////////////////////////
				// 2007. 10. 23. CBH - 타이탄 파츠 조합 게이지
			case eTITANPARTS_PROGRESSBAR_DLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CTitanPartsProgressBarDlg), &fp );
					else
						__asm int 3;
					break;
				}
				/////////////////////////////////////////////////////////////////////////////
				// 2007. 12. 4. CBH - 스킨 선택 다이얼로그
			case eSKIN_SELECT_DLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CSkinSelectDialog), &fp );
					else
						__asm int 3;
					break;
				}
				/////////////////////////////////////////////////////////////////////////////
				// 2008. 1. 16. CBH - 코스튬 스킨 선택 다이얼로그
			case eCOSTUMESKIN_SELECT_DLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CCostumeSkinSelectDialog), &fp );
					else
						__asm int 3;
					break;
				}
				/////////////////////////////////////////////////////////////////////////////
				// magi82(42) - 샵아이템 추가(수련치 초기화)
			case eSKILL_POINT_RESET_DLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CSkillPointResetDlg), &fp );
					else
						__asm int 3;
					break;
				}
				// 2008. 3. 17. CBH - 보안번호 다이얼로그
			case eNUMBERPAD_DLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CNumberPadDialog), &fp );
					else
						__asm int 3;
					break;
				}
				/////////////////////////////////////////////////////////////////////////////
				// magi82(47)
			case eSTALLVIEWDLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CStreetStallItemViewDlg), &fp );
					else
						__asm int 3;
					break;
				}
				//////////////////////////////////////////////////////////////////////////
				// 노점상 검색 추가 by Stiner(8)
			case eSTALLFINDDLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CStallFindDlg), &fp );
					else
						__asm int 3;
					break;
				}
			// autonote
			case eAUTONOTEDLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CAutoNoteDlg), &fp );
					else
						__asm int 3;
					break;
				}
			case eAUTOANSWERDLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CAutoAnswerDlg), &fp );
					else
						__asm int 3;
					break;
				}

			case ePARTYMATCHING_DLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CPartyMatchingDlg), &fp );
					else
						__asm int 3;
					break;
				}
			// 요새전
			case eFWENGRAVEDLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CFWEngraveDialog), &fp );
					else
						__asm int 3;
					break;
				}
			case eFWTIMEDLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CFWTimeDialog), &fp );
					else
						__asm int 3;
					break;
				}
			case eFWWAREHOUSEDLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CFWWareHouseDialog), &fp );
					else
						__asm int 3;
					break;
				}

			default:
				{
					break;
				}
			}
		}
	}

/*	
	while(1)
	{
		fp.GetString(buff);
		if(fp.IsEOF())
			break;
		if(buff[0] == '@')
		{
			fp.GetLineX(buff, 256);
			continue;
		}
		{		
			CMD_ST(buff)
				CMD_CS("$DLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$CHARGUAGEDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuageDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$LISTDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cListDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$LISTDLGEX")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cListDialogEx), &fp);
				else
					__asm int 3;
			CMD_CS("$CHARINFODLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CCharacterDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$MUGONGSURYUNDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMugongSuryunDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$MAINDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMainBarDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$INVENTORYDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CInventoryExDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$CHATDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CChatDialog), &fp);
				else
					__asm int 3;
			//SW050722 attach to minimap
			//		//SW050718
			//		CMD_CS("$JACKPOT")
			//			if((fp.GetString())[0] == '{')
			//				dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cJackpotDialog), &fp);
			//			else
			//				__asm int 3;
			// LYJ 051017 구입노점상 추가
			CMD_CS("$STALLOPTIONDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CStallKindSelectDlg), &fp);
				else
					__asm int 3;
			CMD_CS("$BUYSTALLDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CStreetBuyStall), &fp);
				else
					__asm int 3;
			CMD_CS("$BUYREGDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CBuyRegDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$STREETSTALLDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CStreetStall), &fp);
				else
					__asm int 3;
			CMD_CS("$NPCSCRIPTDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cNpcScriptDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$HELPDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cHelpDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$CHARMAKEDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cCharMakeDlg *)GetInfoFromFile((cWindow*)(new cCharMakeDlg), &fp);
				else
					__asm int 3;
			CMD_CS("$HELPERSPEECHDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cHelperSpeechDlg *)GetInfoFromFile((cWindow*)(new cHelperSpeechDlg), &fp);
				else
					__asm int 3;
			CMD_CS("$MONEYDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMoneyDlg), &fp);
				else
					__asm int 3;
			CMD_CS("$DEALDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CDealDialog), &fp);
				else
					__asm int 3;	
			CMD_CS("$QUICKDIALOG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CQuickDialog), &fp);
				else
					__asm int 3;	
			CMD_CS("$PA_MEMBER1DLG")
				if((fp.GetString())[0] == '{')
					dlg = (CPartyMemberDlg *)GetInfoFromFile((cWindow*)(new CPartyMemberDlg), &fp);
				else
					__asm int 3;
			CMD_CS("$PA_MEMBER2DLG")
				if((fp.GetString())[0] == '{')
					dlg = (CPartyMemberDlg *)GetInfoFromFile((cWindow*)(new CPartyMemberDlg), &fp);
				else
					__asm int 3;
			CMD_CS("$PA_MEMBER3DLG")
				if((fp.GetString())[0] == '{')
					dlg = (CPartyMemberDlg *)GetInfoFromFile((cWindow*)(new CPartyMemberDlg), &fp);
				else
					__asm int 3;
			CMD_CS("$PA_MEMBER4DLG")
				if((fp.GetString())[0] == '{')
					dlg = (CPartyMemberDlg *)GetInfoFromFile((cWindow*)(new CPartyMemberDlg), &fp);
				else
					__asm int 3;
			CMD_CS("$PA_MEMBER5DLG")
				if((fp.GetString())[0] == '{')
					dlg = (CPartyMemberDlg *)GetInfoFromFile((cWindow*)(new CPartyMemberDlg), &fp);
				else
					__asm int 3;
			CMD_CS("$PA_MEMBER6DLG")
				if((fp.GetString())[0] == '{')
					dlg = (CPartyMemberDlg *)GetInfoFromFile((cWindow*)(new CPartyMemberDlg), &fp);
				else
					__asm int 3;
			CMD_CS("$PA_BTNDLG")
				if((fp.GetString())[0] == '{')
					dlg = (CPartyBtnDlg *)GetInfoFromFile((cWindow*)(new CPartyBtnDlg), &fp);
				else
					__asm int 3;			
			CMD_CS("$PA_CREATEDLG")
				if((fp.GetString())[0] == '{')
					dlg = (CPartyCreateDlg *)GetInfoFromFile((cWindow*)(new CPartyCreateDlg), &fp);
				else
					__asm int 3;			
			CMD_CS("$PA_INVITEDLG")
				if((fp.GetString())[0] == '{')
					dlg = (CPartyInviteDlg *)GetInfoFromFile((cWindow*)(new CPartyInviteDlg), &fp);
				else
					__asm int 3;

			CMD_CS("$ANI")
				if((fp.GetString())[0] == '{')
					dlg = (cAni *)GetInfoFromFile((cWindow*)(new cAni), &fp);
				else
					__asm int 3;	
			CMD_CS("$MOUSEPOINTER")
				if((fp.GetString())[0] == '{')
					dlg = (CMousePointer *)GetInfoFromFile((cWindow*)(new CMousePointer), &fp);
				else
					__asm int 3;
			CMD_CS("$PYOGUKDLG")
				if((fp.GetString())[0] == '{')
					dlg = (CPyogukDialog *)GetInfoFromFile((cWindow*)(new CPyogukDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$FRIENDDLG")
				if((fp.GetString())[0] == '{')
					dlg = (CFriendDialog *)GetInfoFromFile((cWindow*)(new CFriendDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$NOTEDLG")
				if((fp.GetString())[0] == '{')
					dlg = (CNoteDialog *)GetInfoFromFile((cWindow*)(new CNoteDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$MINNOTEDLG")
				if((fp.GetString())[0] == '{')
					dlg = (CMiniNoteDialog *)GetInfoFromFile((cWindow*)(new CMiniNoteDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$MINFRIENDDLG")
				if((fp.GetString())[0] == '{')
					dlg = (CMiniFriendDialog *)GetInfoFromFile((cWindow*)(new CMiniFriendDialog), &fp);
				else
					__asm int 3;	
			CMD_CS("$REVIVEDLG")
				if((fp.GetString())[0] == '{')
					dlg = (CReviveDialog*)GetInfoFromFile((cWindow*)(new CReviveDialog), &fp);
				else
					__asm int 3;			
			CMD_CS("$CHANNELDLG")
				if((fp.GetString())[0] == '{')
					dlg = (CChannelDialog*)GetInfoFromFile((cWindow*)(new CChannelDialog), &fp);
				else
					__asm int 3;			
			CMD_CS("$WANTNPCDLG")
				if((fp.GetString())[0] == '{')
					dlg = (CWantNpcDialog*)GetInfoFromFile((cWindow*)(new CWantNpcDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$WANTREGISTDLG")
				if((fp.GetString())[0] == '{')
					dlg = (CWantRegistDialog*)GetInfoFromFile((cWindow*)(new CWantRegistDialog), &fp);
				else
					__asm int 3;
			/////		
			//KES
			CMD_CS("$EXITDLG")
				if((fp.GetString())[0] == '{')
					dlg = GetInfoFromFile((cWindow*)(new CExitDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$MACRODLG")
				if((fp.GetString())[0] == '{')
					dlg = GetInfoFromFile((cWindow*)(new CMacroDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$OPTIONDLG")
				if((fp.GetString())[0] == '{')
					dlg = GetInfoFromFile((cWindow*)(new COptionDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$EXCHANGEDLG")
				if((fp.GetString())[0] == '{')
					dlg = GetInfoFromFile((cWindow*)(new CExchangeDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$CHARSTATEDLG")
				if((fp.GetString())[0] == '{')
					dlg = GetInfoFromFile((cWindow*)(new CCharStateDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$MENUSLOTDIALOG")
				if((fp.GetString())[0] == '{')
					dlg = GetInfoFromFile((cWindow*)(new CMenuSlotDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$PKLOOTINGDLG")
				if((fp.GetString())[0] == '{')
					dlg = GetInfoFromFile((cWindow*)(new CPKLootingDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$CNADVICEDLG")
				if((fp.GetString())[0] == '{')
					dlg = GetInfoFromFile((cWindow*)(new CChinaAdviceDlg), &fp);
				else
					__asm int 3;
			CMD_CS("$DISSOLUTIONDLG")
				if((fp.GetString())[0] == '{')
					dlg = GetInfoFromFile((cWindow*)(new CDissolutionDialog), &fp);
				else
					__asm int 3;
			//////////
			//MurimNet
			CMD_CS("$MNPLAYROOMDLG")
				if((fp.GetString())[0] == '{')
					dlg = GetInfoFromFile((cWindow*)(new CMNPlayRoomDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$MNCHANNELDLG")
				if((fp.GetString())[0] == '{')
					dlg = GetInfoFromFile((cWindow*)(new CMNChannelDialog), &fp);
				else
					__asm int 3;			
			///////////////////////////////////////
			CMD_CS("$MINIMAPDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMiniMapDlg), &fp);
				else
					__asm int 3;
			CMD_CS("$BIGMAPDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CBigMapDlg), &fp);
				else
					__asm int 3;
			CMD_CS("$MONSTERGUAGEDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMonsterGuageDlg), &fp);
				else
					__asm int 3;
			CMD_CS("$DIVIDEBOX")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cDivideBox), &fp);
				else
					__asm int 3;
			CMD_CS("$LOADINGDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CLoadingDlg), &fp);
				else
					__asm int 3;
			CMD_CS("$UPGRADEDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CUpgradeDlg), &fp);
				else
					__asm int 3;
			CMD_CS("$MIXDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMixDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$REINFORCEDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CReinforceDlg), &fp);
				else
					__asm int 3;
			// JSD-------------------------------------------------------------------------------------
			CMD_CS("$SERVERLISTDLG")
				if( (fp.GetString())[0] == '{' )
					dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CServerListDialog), &fp );
				else
					__asm int 3;

			CMD_CS("$QUESTTOTALDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CQuestTotalDialog), &fp);
				else
					__asm int 3;

			CMD_CS("$MPREGISTDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMPRegistDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$MPMISSIONDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMPMissionDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$MPNOTICEDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMPNoticeDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$MPGUAGEDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMPGuageDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$BAILDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CBailDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$GMOPENTALKDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CEventNotifyDialog), &fp);
				else				
					__asm int 3;
			CMD_CS("$ITEM_MALLDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMallNoticeDialog), &fp);
				else				
					__asm int 3;
			CMD_CS("$GUILDCREATEDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildCreateDialog), &fp);
				else
					__asm int 3;	
			CMD_CS("$GUILDUNIONCREATEDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CGuildUnionCreateDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$GUILDDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildDialog), &fp);
				else
					__asm int 3;	
			CMD_CS("$GUILDINVITEDlG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildInviteDialog), &fp);
				else
					__asm int 3;

			CMD_CS("$GUILDMARKDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildMarkDialog), &fp);
				else
					__asm int 3;

			CMD_CS("$GUILDLEVELUPDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildLevelUpDialog), &fp);
				else
					__asm int 3;

			CMD_CS("$GUILDNICKNAMEDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildNickNameDialog), &fp);
				else
					__asm int 3;

			CMD_CS("$GUILDRANKDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildRankDialog), &fp);
				else
					__asm int 3;

			CMD_CS("$GUILDWAREHOUSEDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildWarehouseDialog), &fp);
				else
					__asm int 3;

			CMD_CS("$ITEMALLBASE")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CItemShopDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$SAVEMOVEDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMoveDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$CHANGSAVEDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CPointSaveDialog), &fp);
				else
					__asm int 3;

			// guildfieldwar --------------------------------------------------------------------------------
			CMD_CS("$GFWARDECLAREDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGFWarDeclareDlg), &fp);
				else
					__asm int 3;
			CMD_CS("$GFWARRESULTDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGFWarResultDlg), &fp);
				else
					__asm int 3;
			CMD_CS("$GFWARINFODLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGFWarInfoDlg), &fp);
				else
					__asm int 3;
			CMD_CS("$GUILDWARINFODLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildWarInfoDlg), &fp);
				else
					__asm int 3;
			//------------------------------------------------------------------------------------------------
			CMD_CS("$SKILLPOINTAGAINDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSkillPointRedist), &fp);
				else
					__asm int 3;
			CMD_CS("$SKILLPOINTOPDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSkillPointNotify), &fp);
				else
					__asm int 3;
			// partywar -----------------------------
			CMD_CS("$PARTYWARDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CPartyWarDialog), &fp);
				else
					__asm int 3;
			//---------------------------------------
			CMD_CS("$CHALLWORLDCREATEDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CShoutDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$CHATALLDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CShoutchatDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$SEECHASEDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CChaseDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$CHASEDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CChaseinputDialog), &fp);
				else
					__asm int 3;			
			CMD_CS("$CHNAMECHANGEDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CNameChangeDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$OPCHNAMECHANGEDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CNameChangeNotifyDlg), &fp);
				else
					__asm int 3;
			CMD_CS("$GDTENTRYDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGTRegistDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$GDTENTRYCANCELDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGTRegistcancelDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$GDTOURNAMENTDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGTStandingDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$GDTVIEWDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGTBattleListDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$EVENTMAPCOUNTDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGTScoreInfoDialog), &fp);
				else
					__asm int 3;			

			// seigewar
			CMD_CS("$SEIGEWARTIMEREGDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSWTimeRegDlg), &fp);
				else
					__asm int 3;		
			CMD_CS("$SEIGEWARPROTECTREGDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSWProtectRegDlg), &fp);
				else
					__asm int 3;	
			CMD_CS("$SEIGEWARINFODLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSWInfoDlg), &fp);
				else
					__asm int 3;
			CMD_CS("$SWSTARTTIMEDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSWStartTimeDialog), &fp);
				else
					__asm int 3;

			// Character change
			CMD_CS("$CHARCHANGEEDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CCharChangeDlg), &fp);
				else
					__asm int 3;

			// ItemSeal
			CMD_CS("$ITEMLOCKDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSealDialog), &fp);
				else
					__asm int 3;

			CMD_CS("$SWPROFITDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSWProfitDlg), &fp);
				else
					__asm int 3;

			CMD_CS("$ENGRAVEDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSWEngraveDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$SWCOUNTDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSWTimeDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$CHANGEJOBDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CChangeJobDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$RFDefaultDlg")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CReinforceResetDlg), &fp);
				else
					__asm int 3;
			CMD_CS("$RareCreateDlg")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CRareCreateDialog), &fp);
				else
					__asm int 3;
			CMD_CS("$PETSTATEDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CPetStateDlg), &fp);
				else
					_asm int 3;
			CMD_CS("$PETSTATEMINIDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CPetStateMiniDlg), &fp);
				else
					_asm int 3;
			CMD_CS("$PETINVENTORYDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CPetInventoryDlg), &fp);
				else
					_asm int 3;
			CMD_CS("$PETUPGRADEDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CPetUpgradeDialog), &fp);
				else
					_asm int 3;
			CMD_CS("$PETREVIVALDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CPetRevivalDialog), &fp);
				else
					_asm int 3;
			// 06. 02. 강화 재료 안내 인터페이스 추가 - 이영준
			CMD_CS("$RFDATAGUIDEDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CReinforceDataGuideDlg), &fp);
				else
					__asm int 3;
			// 06. 02. 내정보창 팁보기 추가 - 이영준
			CMD_CS("$TIPBROWSERDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTipBrowserDlg), &fp);
				else
					__asm int 3;
			CMD_CS("$KSDLG")
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CKeySettingTipDlg), &fp);
				else
					__asm int 3;
			CMD_CS("$GUILDNOTEDLG")
				if((fp.GetString())[0] == '{')
					dlg = (CGuildNoteDlg *)GetInfoFromFile((cWindow*)(new CGuildNoteDlg), &fp);
				else
					__asm int 3;
			CMD_EN
		}		
		{
			CMD_ST(buff)

				CMD_CS("$ALLYNOTEDLG")
				if((fp.GetString())[0] == '{')
					dlg = (CUnionNoteDlg *)GetInfoFromFile((cWindow*)(new CUnionNoteDlg), &fp);
				else
					__asm int 3;
				// 06. 03. 문파공지 - 이영준
				CMD_CS("$GUILDNOTICEDLG")
				if((fp.GetString())[0] == '{')
					dlg = (CGuildNoticeDlg *)GetInfoFromFile((cWindow*)(new CGuildNoticeDlg), &fp);
				else
					__asm int 3;
				CMD_CS("$GUILDPLUSTIMEDLG")//Add 060803 by wonju
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildPlusTimeDlg), &fp);
					else
						__asm int 3;
				/////////////////////////////////////////////////////////////
				// 06. 06. 2차 전직 - 이영준
				// 무공 변환
				CMD_CS("$SkillTransDlg")
					if((fp.GetString())[0] == '{')
						dlg = (CSkillOptionChangeDlg *)GetInfoFromFile((cWindow*)(new CSkillOptionChangeDlg), &fp);
					else
						__asm int 3;
				CMD_CS("$TDefaultDlg")
					if((fp.GetString())[0] == '{')
						dlg = (CSkillOptionClearDlg *)GetInfoFromFile((cWindow*)(new CSkillOptionClearDlg), &fp);
					else
						__asm int 3;
				/////////////////////////////////////////////////////////////
				CMD_CS("$SCREENSHOTDLG") //스크린샷
				if((fp.GetString())[0] == '{')
					dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CScreenShotDlg), &fp);
				else
					__asm int 3;
				//SW060713 문하생
				CMD_CS("$JOINOPTIONDLG")
					if((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CGuildJoinDialog), &fp);
					else
						_asm int 3;
					//SW061019 서바이벌모드
				CMD_CS("$SURVCOUNTERDLG")
					if((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CSurvivalCountDialog), &fp);
					else
						_asm int 3;
				CMD_CS("$GUILDTRAINEEINFODLG")
					if((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CGuildMunhaDialog), &fp);
					else
						_asm int 3;
				CMD_CS("$GuildTraineeDLG")
					if((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CGuildTraineeDialog), &fp);
					else
						_asm int 3;
				// magi82 - Titan(070112) ////////////////////////////////////////////////
				CMD_CS("$TITANPARTSMAKEDLG")
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanPartsMakeDlg), &fp);
					else
						__asm int 3;
				//////////////////////////////////////////////////////////////////////////

				// magi82 - Titan(070119) ////////////////////////////////////////////////
				CMD_CS("$TitanMixDLG")
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanMixDlg), &fp);
					else
						__asm int 3;
				CMD_CS("$TitanPartsChangeDLG")
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanPartsChangeDlg), &fp);
					else
						__asm int 3;
				CMD_CS("$TitanUpgradeDLG")
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanUpgradeDlg), &fp);
					else
						__asm int 3;
				CMD_CS("$TitanBreakDLG")
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanBreakDlg), &fp);
					else
						__asm int 3;
				//////////////////////////////////////////////////////////////////////////
				// magi82 - Titan(070212) ////////////////////////////////////////////////
				CMD_CS("$TitanPartsChangePreviewDLG")
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanChangePreViewDlg), &fp);
					else
						__asm int 3;
				//////////////////////////////////////////////////////////////////////////
				// magi82 - Titan(070222) ////////////////////////////////////////////////
				CMD_CS("$titan_inventory_Dlg")
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanInventoryDlg), &fp);
					else
						__asm int 3;
				//////////////////////////////////////////////////////////////////////////
				// magi82 - Titan(070305) ////////////////////////////////////////////////
				CMD_CS("$Titan_guage_DLG")
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanGuageDlg), &fp);
					else
						__asm int 3;
				//////////////////////////////////////////////////////////////////////////
				// magi82 - Titan(070305) ////////////////////////////////////////////////
				CMD_CS("$titan_MugongMix_Dlg")
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanMugongMixDlg), &fp);
					else
						__asm int 3;
				//////////////////////////////////////////////////////////////////////////
				// magi82 - Titan(070320) ////////////////////////////////////////////////
				CMD_CS("$titan_Use_Dlg")
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanRegisterDlg), &fp);
					else
						__asm int 3;
				//////////////////////////////////////////////////////////////////////////
				// magi82 - Titan(070321) ////////////////////////////////////////////////
				CMD_CS("$titan_Bongin_Dlg")
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanDissolutionDlg), &fp);
					else
						__asm int 3;
				//////////////////////////////////////////////////////////////////////////

			CMD_EN
		}
	}
*/	
		
	fp.Release();
	
	return dlg;
}
cWindow * cScriptManager::GetCtrlInfo(cWindow * wnd, char * buff, CMHFile * fp)
{	
	
	////////// 2007. 5. 18. CBH - Parse Type List 관련 처리 추가 /////////	
	PARSE_TYPE eParseType = GetParseType(buff);

	//LONG lID;
	//if(eParseType == -1)
	//{
	//	lID = wnd->GetID();
	//}
	////////////////////////////////////////////////////////////////
	
	switch(eParseType)
	{
	case eBTN:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cButton), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}	
	case eEDITBOX:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cEditBox), fp);
			ASSERT(wnd);
			wnd->Add(window);
			//((cDialog*)wnd)->Add(window);
			break;
		}
	case eSTATIC:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cStatic), fp);
			ASSERT(wnd);
			wnd->Add(window);
			//((cDialog*)wnd)->Add(window);
			break;
		}
	case ePUSHUPBTN:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cPushupButton), fp);
			ASSERT(wnd);
			wnd->Add(window);
			//((cDialog*)wnd)->Add(window);
			break;
		}
	case eCHECKBOX:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cCheckBox), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eCOMBOBOX:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cComboBox), fp);
			ASSERT(wnd);
			wnd->Add(window);
			//((cDialog*)wnd)->Add(window);
			break;
		}
	case eCOMBOBOXEX:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cComboBoxEx), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eSPIN:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cSpin), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eLISTCTRL:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cListCtrl), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eLISTCTRLEX:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cListCtrlEx), fp);
			ASSERT(wnd);
			wnd->Add(window);
			//((cDialog*)wnd)->Add(window);
			break;
		}
	case eICONDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cIconDialog), fp);
			ASSERT(wnd);
			wnd->Add(window);
			//((cDialog*)wnd)->Add(window);
			break;
		}
	case eICONGRIDDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cIconGridDialog), fp);
			ASSERT(wnd);
			wnd->Add(window);
			//((cDialog*)wnd)->Add(window);
			break;
		}
	case eLISTDLGEX:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cListDialogEx), fp);
			ASSERT(wnd);
			wnd->Add(window);
			//((cDialog*)wnd)->Add(window);
			break;
		}
	case eLISTDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cListDialog), fp);
			ASSERT(wnd);
			wnd->Add(window);
			//((cDialog*)wnd)->Add(window);
			break;
		}
	case eWEAREDDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CWearedExDialog), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eLIST:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cList), fp);
			ASSERT(wnd);
			wnd->Add(window);
			//((cDialog*)wnd)->Add(window);
			break;
		}
	case eGUAGEBAR:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cGuageBar), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eANI:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cAni), fp);
			ASSERT(wnd);
			wnd->Add(window);
		}
	case eGUAGE:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cGuage), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eTEXTAREA:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cTextArea), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eGUAGEN:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cGuagen), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eGUAGENE:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CObjectGuagen), fp);
 			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	//KES 030821 추가
	case eDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cDialog), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	//SW051112 무쌍모드
	case eCHARGUAGEDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CGuageDialog), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	//HS추가
	case eMUNPAMARKDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CMunpaMarkDialog), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case ePRIVATEWAREHOUSEDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CPrivateWarehouseDialog), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eMUGONGDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CMugongDialog), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eSURYUNDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CSuryunDialog), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eQUESTDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CQuestDialog), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eWANTEDDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CWantedDialog), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eJOURNALDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CJournalDialog), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eITEMSHOPGRIDDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CItemShopGridDialog), fp);
			ASSERT(wnd);
			wnd->Add(window); 
			break;
		}
	case eSHOPITEMINVENGRID:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CItemShopInven), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}	
	default:
		{	
			break;
		}
	}

/*	
		CMD_ST(buff)
		CMD_CS("$BTN")
		cWindow * window = GetInfoFromFile((cWindow*)(new cButton), fp);
	ASSERT(wnd);
	wnd->Add(window);
	CMD_CS("$EDITBOX")
		cWindow * window = GetInfoFromFile((cWindow*)(new cEditBox), fp);
	ASSERT(wnd);
	wnd->Add(window);
	//((cDialog*)wnd)->Add(window);
	CMD_CS("$STATIC")
		cWindow * window = GetInfoFromFile((cWindow*)(new cStatic), fp);
	ASSERT(wnd);
	wnd->Add(window);
	//((cDialog*)wnd)->Add(window);
	CMD_CS("$PUSHUPBTN")
		cWindow * window = GetInfoFromFile((cWindow*)(new cPushupButton), fp);
	ASSERT(wnd);
	wnd->Add(window);
	//((cDialog*)wnd)->Add(window);
	CMD_CS("$CHECKBOX")
		cWindow * window = GetInfoFromFile((cWindow*)(new cCheckBox), fp);
	ASSERT(wnd);
	wnd->Add(window);
	CMD_CS("$COMBOBOX")
		cWindow * window = GetInfoFromFile((cWindow*)(new cComboBox), fp);
	ASSERT(wnd);
	wnd->Add(window);
	//((cDialog*)wnd)->Add(window);
	CMD_CS("$COMBOBOXEX")
		cWindow * window = GetInfoFromFile((cWindow*)(new cComboBoxEx), fp);
	ASSERT(wnd);
	wnd->Add(window);
	CMD_CS("$SPIN")
		cWindow * window = GetInfoFromFile((cWindow*)(new cSpin), fp);
	ASSERT(wnd);
	wnd->Add(window);
	CMD_CS("$LISTCTRL")
		cWindow * window = GetInfoFromFile((cWindow*)(new cListCtrl), fp);
	ASSERT(wnd);
	wnd->Add(window);
	CMD_CS("$LISTCTRLEX")
		cWindow * window = GetInfoFromFile((cWindow*)(new cListCtrlEx), fp);
	ASSERT(wnd);
	wnd->Add(window);
	//((cDialog*)wnd)->Add(window);
	CMD_CS("$ICONDLG")
		cWindow * window = GetInfoFromFile((cWindow*)(new cIconDialog), fp);
	ASSERT(wnd);
	wnd->Add(window);
	//((cDialog*)wnd)->Add(window);
	CMD_CS("$ICONGRIDDLG")
		cWindow * window = GetInfoFromFile((cWindow*)(new cIconGridDialog), fp);
	ASSERT(wnd);
	wnd->Add(window);
	//((cDialog*)wnd)->Add(window);
	CMD_CS("$LISTDLGEX")
		cWindow * window = GetInfoFromFile((cWindow*)(new cListDialogEx), fp);
	ASSERT(wnd);
	wnd->Add(window);
	//((cDialog*)wnd)->Add(window);
	CMD_CS("$LISTDLG")
		cWindow * window = GetInfoFromFile((cWindow*)(new cListDialog), fp);
	ASSERT(wnd);
	wnd->Add(window);
	//((cDialog*)wnd)->Add(window);
	CMD_CS("$WEAREDDLG")
		cWindow * window = GetInfoFromFile((cWindow*)(new CWearedExDialog), fp);
	ASSERT(wnd);
	wnd->Add(window);
	CMD_CS("$LIST")
		cWindow * window = GetInfoFromFile((cWindow*)(new cList), fp);
	ASSERT(wnd);
	wnd->Add(window);
	//((cDialog*)wnd)->Add(window);
	CMD_CS("$GUAGEBAR")
		cWindow * window = GetInfoFromFile((cWindow*)(new cGuageBar), fp);
	ASSERT(wnd);
	wnd->Add(window);
	CMD_CS("$ANI")
		cWindow * window = GetInfoFromFile((cWindow*)(new cAni), fp);
	ASSERT(wnd);
	wnd->Add(window);
	CMD_CS("$GUAGE")
		cWindow * window = GetInfoFromFile((cWindow*)(new cGuage), fp);
	ASSERT(wnd);
	wnd->Add(window);
	CMD_CS("$TEXTAREA")
		cWindow * window = GetInfoFromFile((cWindow*)(new cTextArea), fp);
	ASSERT(wnd);
	wnd->Add(window);
	CMD_CS("$GUAGEN")
		cWindow * window = GetInfoFromFile((cWindow*)(new cGuagen), fp);
	ASSERT(wnd);
	wnd->Add(window);
	CMD_CS("$GUAGENE")
		cWindow * window = GetInfoFromFile((cWindow*)(new CObjectGuagen), fp);
	ASSERT(wnd);
	wnd->Add(window);
	//KES 030821 추가
	CMD_CS("$DLG")
		cWindow * window = GetInfoFromFile((cWindow*)(new cDialog), fp);
	ASSERT(wnd);
	wnd->Add(window);
	//SW051112 무쌍모드
	CMD_CS("$CHARGUAGEDLG")
		cWindow * window = GetInfoFromFile((cWindow*)(new CGuageDialog), fp);
	ASSERT(wnd);
	wnd->Add(window);
	//HS추가
	CMD_CS("$MUNPAMARKDLG")
		cWindow * window = GetInfoFromFile((cWindow*)(new CMunpaMarkDialog), fp);
	ASSERT(wnd);
	wnd->Add(window);
	CMD_CS("$PRIVATEWAREHOUSEDLG")
		cWindow * window = GetInfoFromFile((cWindow*)(new CPrivateWarehouseDialog), fp);
	ASSERT(wnd);
	wnd->Add(window);

	CMD_CS("$MUGONGDLG")
		cWindow * window = GetInfoFromFile((cWindow*)(new CMugongDialog), fp);
	ASSERT(wnd);
	wnd->Add(window);
	CMD_CS("$SURYUNDLG")
		cWindow * window = GetInfoFromFile((cWindow*)(new CSuryunDialog), fp);
	ASSERT(wnd);
	wnd->Add(window);
	CMD_CS("$QUESTDLG")
		cWindow * window = GetInfoFromFile((cWindow*)(new CQuestDialog), fp);
	ASSERT(wnd);
	wnd->Add(window);
	CMD_CS("$WANTEDDLG")
		cWindow * window = GetInfoFromFile((cWindow*)(new CWantedDialog), fp);
	ASSERT(wnd);
	wnd->Add(window);
	CMD_CS("$JOURNALDLG")
		cWindow * window = GetInfoFromFile((cWindow*)(new CJournalDialog), fp);
	ASSERT(wnd);
	wnd->Add(window);
	CMD_CS("$ITEMSHOPGRIDDLG")
		cWindow * window = GetInfoFromFile((cWindow*)(new CItemShopGridDialog), fp);
	ASSERT(wnd);
	wnd->Add(window); 
	CMD_CS("$SHOPITEMINVENGRID")
		cWindow * window = GetInfoFromFile((cWindow*)(new CItemShopInven), fp);
	ASSERT(wnd);
	wnd->Add(window);

	CMD_EN
*/	
	
	return wnd;
}
cWindow * cScriptManager::GetInfoFromFile(cWindow * wnd, CMHFile * fp)
{
	cPoint point={0,0,0,0};
	cPoint listBarPoint={0,0,0,0};
	char toolTipMsg[256]={0,};
	char buff[256]={0,};
	char funcName[256]={0,};
	char IDName[256]={0,};
	char staticText[256]={0,};
	char imagePath[256]={0,};
	RGBCOLOR color={0,0,0};
	BOOL fMovable = TRUE;
	BOOL fActive = TRUE;
	BOOL fPush = FALSE;
	cImage BasicImage;
	cImage OverImage;
	cImage ListOverImage;
	cImage PressImage	;
	cImage ToolTipImage	;
	cImage TopImage		;
	cImage MiddleImage	;
	cImage DownImage	;
	cImage HeadImage	;
	cImage BodyImage	;
	cImage ListBarImage	;
	cImage FocusImage	;
	cImage SelectedImage;

	cImage AniSpriteImage	;
	cImage GuagePieceImage	;
	cImage CheckImage		;
	cImage CheckBoxImage	;
	cImage selectedBGImage	;
	cImage dragoverBGImage	;
	cImage guageEffectPieceImage	;

	cImage BalloonImage[10];
	FLOAT  BalloonBorder = 8;

	BOOL	bChecked = FALSE;
	WORD	HeadHei = 0;
	WORD	BodyHei = 0;
	int		nLeftMargin = 0;
	int		nTopMargin = 0;

	WORD EditBoxSize = 0;
	WORD EditBoxEableTypingSize = 0;
	BOOL fSecret = FALSE;
	RGBCOLOR activeTextColor={0,0,0};
	RGBCOLOR nonactiveTextColor={0,0,0};
	RGBCOLOR fgColor={0,0,0};
	RGBCOLOR comboTextColor={0,0,0};
	char szBtnText[256]={0,};
	RGBCOLOR btnTextBasicCol={0,0,0};
	RGBCOLOR btnTextOverCol={0,0,0};
	RGBCOLOR btnTextPressCol={0,0,0};
	BYTE alphaDepth = 255;
	cCoord coord;
	RECT captionRect={0,0,0,0};
	RECT textRect={0,0,0,0};
	RECT imgSrcRect={0,0,0,0};
	BYTE cols=0;
	BYTE rows=0;
	BYTE tabNum=0;
	BYTE middleNum=0;
	WORD topHei = 0;
	WORD middleHei = 0;
	WORD downHei = 0;
	LONG minValue = 0;
	LONG maxValue = 100;
	LONG spinUnit = 10;
	BOOL lineUpward = TRUE;
	int	 interval = 0;
	WORD maxLine = 0;
	LONG listMaxHeight = 15;
	BYTE minMiddleNum=0;
	BYTE maxMiddleNum=0;
	WORD gridBorderX = 0;
	WORD gridBorderY = 0;
	cPoint gridPos ={0,0,0,0};
	cCoord validXY={0,0};
	BOOL vertical = FALSE;
	BOOL fSpriteloop = TRUE;
	WORD travelingType = 0;

	float guageMinVal=0;
	float guageMaxVal=0;
	float guageCurVal=0;
	cCoord guagePos = {0,0};
	WORD selectComboIdx = 0;

	BOOL bAutoClose = FALSE;
	cCoord textXY = {5,6};
	WORD	fontIdx = 0;
	BOOL bReadOnly = FALSE;
	LONG limitLineNum = 20;
	int	soundIdx = -1;
	WORD	wOption = 0;
//KES 030916
	int	nLimitBytes = 256;
	int nTextAlign = TXT_LEFT | TXT_TOP;
	BYTE btnTextAniX = 1;
	BYTE btnTextAniY = 1;

	BOOL bAutoScroll = TRUE;
	BOOL bPassive = FALSE;

	int tt_idx = -1;
//
	cImageScale scale={1,1};
	cImageScale ListScale={1,1};

	float guageWidth = 0.f;
	float guagePieceWidth = 0.f;
	float guageEffectPieceWidth = 0.f;
	float guagePieceHeightScaleY = 1.f;
	float guageEffectPieceHeightScaleY = 1.f;
	DWORD	guageEstTime = 0;

	LONG		shadowTextX=0;
	LONG		shadowTextY=0;
	RGBCOLOR	dwShadowColor={10,10,10};
	BOOL		bShadow = FALSE;


	while(1)
	{
		fp->GetString(buff);
		if(buff[0] == '}'|| fp->IsEOF())
			break;
		if(buff[0] == '{')
			continue;
		if(buff[0] == '$')	
		{
			GetCtrlInfo(wnd, buff, fp);
			continue;
		}
		if(buff[0] == '@')
		{
			fp->GetLineX(buff, 256);
			continue;
		}
		
		////////// 2007. 5. 18. Parse Type List 관련 처리 추가 /////////		
		PARSE_TYPE eParseType = GetParseType(buff);

		//LONG lID;
		//if(eParseType == -1)
		//{
		//	lID = wnd->GetID();
		//}
		////////////////////////////////////////////////////////////////
		
		switch(eParseType)
		{
		case ePOINT:
			{
				point.x = fp->GetDword();
				point.y = fp->GetDword();
				point.w = fp->GetDword();
				point.h = fp->GetDword();

				wnd->cWindow::SetAbsXY(point.x, point.y);
				wnd->cWindow::SetRelXY(point.x, point.y);
				break;
			}
		//SW061111 저해상도옵션
		case ePOINT_:
			{
				if( GAMERESRCMNGR->IsLowResolution() )
				{
					point.x = fp->GetDword();
					point.y = fp->GetDword();
					point.w = fp->GetDword();
					point.h = fp->GetDword();
					//CheckPosForResolution(&point);

					wnd->cWindow::SetAbsXY(point.x, point.y);
					wnd->cWindow::SetRelXY(point.x, point.y);
				}
				break;
			}
		case eFUNC:
			{
				fp->GetString(funcName);
				break;
			}
		case eID:
			{
				fp->GetString(IDName);
				break;
			}
		case eFONTIDX:
			{
				fontIdx = fp->GetWord();
				break;
			}
		case eAUTOCLOSE:
			{
				bAutoClose = fp->GetBool();
				break;
			}
		case eTOOLTIPIMAGE:
			{
				GetImage(fp, &ToolTipImage);
				break;
			}
		case eTOOLTIPMSG:
			{
				strcpy( toolTipMsg, GetMsg(fp->GetInt()));
				break;
			}
		case eTOOLTIPCOL:
			{
				color.r = fp->GetByte();
				color.g = fp->GetByte();
				color.b = fp->GetByte();
				break;
			}
		case eTEXTXY:
			{
				textXY.x = fp->GetDword();
				textXY.y = fp->GetDword();
				break;
			}
		case eMOVEABLE:
			{
				fMovable = fp->GetBool();
				break;
			}
		case eACTIVE:
			{
				fActive = fp->GetBool();
				break;
			}
		case eBASICIMAGE:
			{
				GetImage(fp, &BasicImage);
				break;
			}
		//SW061111 저해상도옵션
		case eBASICIMAGE_:
			{
				if( GAMERESRCMNGR->IsLowResolution() )
				{
					GetImage(fp, &BasicImage);
				}
				break;
			}
		case eOVERIMAGE:
			{
				GetImage(fp, &OverImage);
				break;
			}
		case eLISTOVERIMAGE:
			{
				GetImage(fp, &ListOverImage);
				break;
			}
		case eSELECTIMAGE:
			{
				GetImage(fp, &SelectedImage);
				break;
			}
		case ePRESSIMAGE:
			{
				GetImage(fp, &PressImage);
				break;
			}
		case eFOCUSIMAGE:
			{
				GetImage(fp, &FocusImage);
				break;
			}
		case eBTNTEXT:
			{
				strcpy( szBtnText, GetMsg(fp->GetInt()) );
				btnTextBasicCol.r = fp->GetByte();
				btnTextBasicCol.g = fp->GetByte();
				btnTextBasicCol.b = fp->GetByte();

				btnTextOverCol.r = fp->GetByte();
				btnTextOverCol.g = fp->GetByte();
				btnTextOverCol.b = fp->GetByte();

				btnTextPressCol.r = fp->GetByte();
				btnTextPressCol.g = fp->GetByte();
				btnTextPressCol.b = fp->GetByte();
				break;
			}
		case eBTNTEXTANI:
			{
				btnTextAniX	= fp->GetByte();
				btnTextAniY	= fp->GetByte();
				break;
			}
		case eIMAGESRCRECT:
			{
				imgSrcRect.left =  fp->GetLong();
				imgSrcRect.top = fp->GetLong();
				imgSrcRect.right = fp->GetLong();
				imgSrcRect.bottom = fp->GetLong();
				break;
			}
		case eALPHA:
			{
				alphaDepth = fp->GetByte();
				break;
			}
		case eEDITSIZE:
			{
				EditBoxSize = fp->GetWord();
				EditBoxEableTypingSize = fp->GetWord();
				break;
			}
		case eSPINSIZE:
			{
				EditBoxSize = fp->GetWord();
				EditBoxEableTypingSize = fp->GetWord();
				break;
			}
		case eSECRET:
			{
				fSecret = fp->GetBool();
				break;
			}
		case eTEXTCOLOR:	
			{
				activeTextColor.r = fp->GetByte();
				activeTextColor.g = fp->GetByte();
				activeTextColor.b = fp->GetByte();
				nonactiveTextColor.r = fp->GetByte();
				nonactiveTextColor.g = fp->GetByte();
				nonactiveTextColor.b = fp->GetByte();
				break;
			}
		case eCOORD:
			{
				coord.x = fp->GetDword();
				coord.y = fp->GetDword();
				break;
			}
		case eFGCOLOR:
			{
				fgColor.r = fp->GetByte();
				fgColor.g = fp->GetByte();
				fgColor.b = fp->GetByte();
				break;
			}
		case eTEXT:
			{
				int msg_idx = fp->GetInt();
				strcpy( staticText, GetMsg(msg_idx) );
				break;
			}
		case eITEMTOOLTIP:
			{
				tt_idx = fp->GetInt();
				break;
			}
		case eSHADOW:
			{
				bShadow = fp->GetBool();
				break;
			}
		case eSHADOWCOLOR:
			{
				dwShadowColor.r = fp->GetByte();
				dwShadowColor.g = fp->GetByte();
				dwShadowColor.b = fp->GetByte();
				break;
			}
		case eSHADOWTEXTXY:
			{
				shadowTextX = fp->GetLong();
				shadowTextY = fp->GetLong();
				break;
			}
		case eMESSAGE:
			{
				int msg_idx =  fp->GetInt();
				strcpy( staticText, GetMsg(msg_idx) );
				fgColor.r = fp->GetByte();
				fgColor.g = fp->GetByte();
				fgColor.b = fp->GetByte();
				break;
			}
		case eTEXTRECT:
			{
				textRect.left =  fp->GetLong();
				textRect.top = fp->GetLong();
				textRect.right = fp->GetLong();
				textRect.bottom = fp->GetLong();
				break;
			}
		case ePUSHUP:
			{
				fPush = fp->GetBool();
				break;
			}
		case eVALIDXY:
			{
				validXY.x = fp->GetDword();
				validXY.y = fp->GetDword();
				break;
			}
		case eCAPTIONRECT:
			{
				captionRect.left = fp->GetLong();
				captionRect.top = fp->GetLong();
				captionRect.right = fp->GetLong();
				captionRect.bottom = fp->GetLong();
				break;
			}
		case eICONCELLNUM:
			{
				cIconDialog * win = (cIconDialog *)wnd;
				win->SetCellNum(fp->GetByte());				
				break;
			}
		case eICONCELL:
			{
				RECT iconCellRect={0,0,0,0};
				iconCellRect.left = fp->GetLong();
				iconCellRect.top = fp->GetLong();
				iconCellRect.right = fp->GetLong();
				iconCellRect.bottom = fp->GetLong();
				cIconDialog * win = (cIconDialog *)wnd;
				win->AddIconCell(&iconCellRect);
				break;
			}
		case eSELECTOPTION:
			{
				wOption = fp->GetWord();
				break;
			}
		case eCELLRECT:
			{
				RECT CellRect={0,0,0,0};
				CellRect.left = fp->GetLong();
				CellRect.top = fp->GetLong();
				CellRect.right = fp->GetLong();
				CellRect.bottom = fp->GetLong();
				cIconGridDialog * win = (cIconGridDialog *)wnd;
				win->SetCellRect(&CellRect);
				break;
			}
		case eLIMITLINENUM:
			{
				limitLineNum = fp->GetLong();
				break;
			}
		//KES 030916
		case eLIMITBYTES:
			{
				nLimitBytes = fp->GetInt();
				break;
			}
		case eTEXTALIGN:
			{
				nTextAlign = fp->GetInt();
				if( nTextAlign == 0 )	nTextAlign = TXT_LEFT;
				else if( nTextAlign == 1 )	nTextAlign = TXT_RIGHT;
				else if( nTextAlign == 2 )	nTextAlign = TXT_CENTER;
				break;
			}
		case eAUTOSCROLL:
			{
				bAutoScroll = fp->GetByte();
				break;
			}
		case ePASSIVE:
			{
				bPassive = fp->GetByte();
				break;
			}
//
		case eTABNUM:
			{
				cTabDialog * win = (cTabDialog *)wnd;
				tabNum = fp->GetByte();
				win->InitTab(tabNum);
				break;
			}
		case eICONCELLBGIMAGE:
			{
				GetImage(fp, &selectedBGImage);
				break;
			}
		case eDRAGOVERBGIMAGE:
			{
				GetImage(fp, &dragoverBGImage);
				break;
			}
		case eCOLS:
			{
				cols = fp->GetByte();
				break;
			}
		case eROWS:
			{
				rows = fp->GetByte();
				break;
			}
		case eMIDDLENUM:
			{
				middleNum = fp->GetByte();
				break;
			}
		case eTOPIMAGE:
			{
				GetImage(fp, &TopImage);
				topHei = fp->GetWord();
				break;
			}
		case eTOPHEIGHT:
			{
				topHei = fp->GetWord();
				break;
			}
		case eDOWNHEIGHT:
			{
				downHei = fp->GetWord();
				break;
			}
		case eMIDDELIMAGE:
			{
				GetImage(fp, &MiddleImage);
				middleHei = fp->GetWord();
				break;
			}
		case eMIDDLEHEIGHT:
			{
				middleHei = fp->GetWord();
				break;
			}
		case eDOWNIMAGE:
			{
				GetImage(fp, &DownImage);
				downHei = fp->GetWord();
				break;
			}
		case eCOMBOTEXTCOLOR:
			{
				comboTextColor.r = fp->GetByte();
				comboTextColor.g = fp->GetByte();
				comboTextColor.b = fp->GetByte();
				break;
			}
		case eSELECTCOMBOINDEX:
			{
				selectComboIdx = fp->GetWord();
				break;
			}
		case eREADONLY:
			{
				bReadOnly = fp->GetBool();
				break;
			}
		case eADDSTRING:
			{
				int msg_idx =  fp->GetInt();
				char msg2[255]={0,};
				strcpy( msg2, GetMsg(msg_idx) );
				RGBCOLOR comboListColor={0,0,0};
				comboListColor.r = fp->GetByte();
				comboListColor.g = fp->GetByte();
				comboListColor.b = fp->GetByte();

				switch(wnd->GetType())
				{
				case WT_LIST:
					{
						cList * win = (cList *)wnd;
						ITEM* pItem = new ITEM;
						strcpy(pItem->string, msg2);
						pItem->rgb =  RGB_HALF(comboListColor.r, comboListColor.g, comboListColor.b);
						win->AddItem(pItem);
					}
					break;
				case WT_LISTDIALOG:
					{
						cListDialog * win = (cListDialog *)wnd;
						win->AddItem(msg2, RGB_HALF(comboListColor.r, comboListColor.g, comboListColor.b));
					}
					break;
				case WT_COMBOBOX:
					{
						cComboBox * win = (cComboBox *)wnd;
						ITEM* pItem = new ITEM;
						strcpy(pItem->string, msg2);
						pItem->rgb =  RGB_HALF(comboListColor.r, comboListColor.g, comboListColor.b);
						win->AddItem(pItem);
					}
					break;
				}
				break;
			}
		case eINITCOMBOLIST:
			{
				cComboBox * win = (cComboBox *)wnd;
				WORD listWidth = fp->GetWord();
				win->SetMaxLine(fp->GetWord());
				GetImage(fp, &TopImage);
				topHei = fp->GetWord();
				GetImage(fp, &MiddleImage);
				middleHei = fp->GetWord();
				GetImage(fp, &DownImage);
				downHei = fp->GetWord();
				GetImage(fp, &ListOverImage);
				win->InitComboList(listWidth, &TopImage, topHei, &MiddleImage, middleHei, &DownImage, downHei, &ListOverImage);
				break;
			}
		// LISTCTRL에서 사용하는 속성
		case eCREATE:
			{
				cListCtrl * win = (cListCtrl *)wnd;
				win->Create(fp->GetInt());
				break;
			}
		case eHEADIMAGE:
			{
				GetImage(fp, &HeadImage);
				break;
			}
		case eBODYIMAGE:
			{
				GetImage(fp, &BodyImage);
				break;
			}
		case eHEADHEIGHT:
			{
				HeadHei = fp->GetWord();
				break;
			}
		case eBODYHEIGHT:
			{
				BodyHei = fp->GetWord();
				break;
			}
		case eINSERTCOLUMN:
			{
				cListCtrl * win = (cListCtrl *)wnd;
				int col_idx =  fp->GetInt();
				int msg_idx =  fp->GetInt();
				char msg2[255]={0,};
				strcpy( msg2, GetMsg(msg_idx) );
				int colSize = fp->GetInt();
				win->InsertColoumn(col_idx, msg2, colSize);
				break;
			}
		case eSETITEMTEXT:
			{
				cListCtrl * win = (cListCtrl *)wnd;
				int col_idx =  fp->GetInt();
				int row_idx =  fp->GetInt();
				int msg_idx =  fp->GetInt();
				char msg2[255]={0,};
				strcpy( msg2, GetMsg(msg_idx) );
				win->SetItemText(col_idx, row_idx, msg2);
				break;
			}
		case eSPINMINMAX:
			{
				minValue = fp->GetLong();
				maxValue = fp->GetLong();
				break;
			}
		case eSPINUNIT:
			{
				spinUnit = fp->GetLong();
				break;
			}
		case eLINEUPWARD:
			{
				lineUpward = fp->GetBool();
				break;
			}
		case eLISTBARINTERVAL:
			{
				interval = fp->GetInt();
				break;
			}
		case eLISTBARPOINT:
			{
				listBarPoint.x = fp->GetDword();
				listBarPoint.y = fp->GetDword();
				listBarPoint.w = fp->GetDword();
				listBarPoint.h = fp->GetDword();
				break;
			}
		case eLISTBARIMAGE:
			{
				GetImage(fp, &ListBarImage);
				//listBarPoint = fp->
				break;
			}
		case eLISTMAXLINE:
			{
				maxLine = fp->GetWord();
				break;
			}
		case eLINEHEIGHT:
			{
				listMaxHeight = fp->GetLong();
				break;
			}
		case eBTNCLICKSOUND:
			{
				soundIdx = fp->GetInt();
				break;
			}
		case eMINMAXMIDDLENUM:
			{
				minMiddleNum = fp->GetByte();
				maxMiddleNum = fp->GetByte();
				break;
			}
		case eINITGRID:
			{
				gridPos.x = fp->GetDword();
				gridPos.y = fp->GetDword();
				gridPos.w = fp->GetDword();
				gridPos.h = fp->GetDword();
				gridBorderX = fp->GetWord();
				gridBorderY = fp->GetWord();
				break;
			}
		case eINITGUAGEBAR:
			{
				cGuageBar * win = (cGuageBar *)wnd;
				interval = fp->GetLong();
				vertical = fp->GetBool();
				win->InitGuageBar(interval, vertical);
				break;
			}
		case eMAXSPRITE:
			{
				cAni * win = (cAni *)wnd;
				win->SetMaxSprite(fp->GetWord());
				break;
			}
		case eSPRITELOOP:
			{
				fSpriteloop = fp->GetBool();
				break;
			}
		case eTRAVELINGTYPE:
			{
				travelingType = fp->GetWord();
				break;
			}
		case eADDSPRITE:
			{
				cAni * win = (cAni *)wnd;
				GetImage(fp, &AniSpriteImage);
				WORD delayTime = fp->GetWord();
				win->AddSprite(&AniSpriteImage, delayTime);
				break;
			}
		case eGUAGEPIECEIMAGE:
			{
				//cGuage * win = (cGuage *)wnd;
				GetImage(fp, &GuagePieceImage);
				//win->SetPieceImage(&GuagePieceImage);
				break;
			}
		case eGUAGEMINVALUE:
			{
				guageMinVal = fp->GetFloat();
				break;
			}
		case eGUAGEMAXVALUE:
			{
				guageMaxVal = fp->GetFloat();
				break;
			}
		case eGUAGECURVALUE:
			{
				guageCurVal = fp->GetFloat();
				break;
			}
		case eGUAGENEVALUE:
			{
				guageCurVal		= fp->GetFloat();
				guageEstTime	= fp->GetDword();
				break;
			}
		case eGUAGEPOS:
			{
				guagePos.x = fp->GetLong();
				guagePos.y = fp->GetLong();
				break;
			}
		case eCHECKBOXIMAGE:
			{
				GetImage(fp, &CheckBoxImage);
				break;
			}
		case eCHECKIMAGE:
			{
				GetImage(fp, &CheckImage);
				break;
			}
		case eSETCHECK:
			{
				bChecked = fp->GetBool();
				break;
			}
		case eSCALE:
			{
				scale.x = fp->GetFloat();
				scale.y = fp->GetFloat();
				break;
			}
		case eLISTSCALE:
			{
				ListScale.x = fp->GetFloat();
				ListScale.y = fp->GetFloat();
				break;
			}
		case eGUAGEWIDTH:
			{
				guageWidth = fp->GetFloat();
				break;
			}
		case eGUAGEPIECEWIDTH:
			{
				guagePieceWidth = fp->GetFloat();
				break;
			}
		case eGUAGEEFFECTPIECEIMAGE:
			{
				GetImage(fp, &guageEffectPieceImage);
				break;
			}
		case eGUAGEEFFECTPIECEWIDTH:
			{
				guageEffectPieceWidth = fp->GetFloat();
				break;
			}
		case eGUAGEPIECEHEIGHTSCALE:
			{
				guagePieceHeightScaleY = fp->GetFloat();
				break;
			}
		case eGUAGEEFFECTPIECEHEIGHTSCALE:
			{
				guageEffectPieceHeightScaleY = fp->GetFloat();
				break;
			}
		case eBALLOONIMAGETOP:
			{
				GetImage(fp, &BalloonImage[0]);
				GetImage(fp, &BalloonImage[1]);
				GetImage(fp, &BalloonImage[2]);
				break;
			}
		case eBALLOONIMAGEMIDDLE:
			{
				GetImage(fp, &BalloonImage[3]);
				GetImage(fp, &BalloonImage[4]);
				GetImage(fp, &BalloonImage[5]);
				break;
			}
		case eBALLOONIMAGEBOTTOM:
			{
				GetImage(fp, &BalloonImage[6]);
				GetImage(fp, &BalloonImage[7]);
				GetImage(fp, &BalloonImage[8]);
				break;
			}
		case eBALLOONIMAGETAIL:
			{
				GetImage(fp, &BalloonImage[9]);
				break;
			}
		case eBALLOONIMAGEBORDER:
			{
				BalloonBorder = fp->GetFloat();
				break;
			}
		default:
			{				
				break;
			}
		}
	}

/*
while(1)
{
	fp->GetString(buff);
	if(buff[0] == '}'|| fp->IsEOF())
		break;
	if(buff[0] == '{')
		continue;
	if(buff[0] == '$')	
	{
		GetCtrlInfo(wnd, buff, fp);
		continue;
	}
	if(buff[0] == '@')
	{
		fp->GetLineX(buff, 256);
		continue;
	}

	CMD_ST(buff)
		CMD_CS("#POINT")
		point.x = fp->GetDword();
	point.y = fp->GetDword();
	point.w = fp->GetDword();
	point.h = fp->GetDword();

	wnd->cWindow::SetAbsXY(point.x, point.y);
	wnd->cWindow::SetRelXY(point.x, point.y);
	//SW061111 저해상도옵션
	CMD_CS("#POINT_")
		if( GAMERESRCMNGR->IsLowResolution() )
		{
			point.x = fp->GetDword();
			point.y = fp->GetDword();
			point.w = fp->GetDword();
			point.h = fp->GetDword();
			//CheckPosForResolution(&point);

			wnd->cWindow::SetAbsXY(point.x, point.y);
			wnd->cWindow::SetRelXY(point.x, point.y);
		}
		CMD_CS("#FUNC")
			fp->GetString(funcName);
		CMD_CS("#ID")
			fp->GetString(IDName);
		CMD_CS("#FONTIDX")
			fontIdx = fp->GetWord();
		CMD_CS("#AUTOCLOSE")
			bAutoClose = fp->GetBool();
		CMD_CS("#TOOLTIPIMAGE")
			GetImage(fp, &ToolTipImage);
		CMD_CS("#TOOLTIPMSG")
			strcpy( toolTipMsg, GetMsg(fp->GetInt()));
		CMD_CS("#TOOLTIPCOL")
			color.r = fp->GetByte();
		color.g = fp->GetByte();
		color.b = fp->GetByte();
		CMD_CS("#TEXTXY")
			textXY.x = fp->GetDword();
		textXY.y = fp->GetDword();
		CMD_CS("#MOVEABLE")
			fMovable = fp->GetBool();
		CMD_CS("#ACTIVE")
			fActive = fp->GetBool();
		CMD_CS("#BASICIMAGE")
			GetImage(fp, &BasicImage);
		//SW061111 저해상도옵션
		CMD_CS("#BASICIMAGE_")
			if( GAMERESRCMNGR->IsLowResolution() )
			{
				GetImage(fp, &BasicImage);
			}
			CMD_CS("#OVERIMAGE")
				GetImage(fp, &OverImage);
			CMD_CS("#LISTOVERIMAGE")
				GetImage(fp, &ListOverImage);
			CMD_CS("#SELECTIMAGE")
				GetImage(fp, &SelectedImage);
			CMD_CS("#PRESSIMAGE")
				GetImage(fp, &PressImage);
			CMD_CS("#FOCUSIMAGE")
				GetImage(fp, &FocusImage);
			CMD_CS("#BTNTEXT")
				strcpy( szBtnText, GetMsg(fp->GetInt()) );
			btnTextBasicCol.r = fp->GetByte();
			btnTextBasicCol.g = fp->GetByte();
			btnTextBasicCol.b = fp->GetByte();

			btnTextOverCol.r = fp->GetByte();
			btnTextOverCol.g = fp->GetByte();
			btnTextOverCol.b = fp->GetByte();

			btnTextPressCol.r = fp->GetByte();
			btnTextPressCol.g = fp->GetByte();
			btnTextPressCol.b = fp->GetByte();
			CMD_CS("#BTNTEXTANI")
				btnTextAniX	= fp->GetByte();
			btnTextAniY	= fp->GetByte();
			CMD_CS("#IMAGESRCRECT")
				imgSrcRect.left =  fp->GetLong();
			imgSrcRect.top = fp->GetLong();
			imgSrcRect.right = fp->GetLong();
			imgSrcRect.bottom = fp->GetLong();
			CMD_CS("#ALPHA")
				alphaDepth = fp->GetByte();
			CMD_CS("#EDITSIZE");
			EditBoxSize = fp->GetWord();
			EditBoxEableTypingSize = fp->GetWord();
			CMD_CS("#SPINSIZE");
			EditBoxSize = fp->GetWord();
			EditBoxEableTypingSize = fp->GetWord();
			CMD_CS("#SECRET")
				fSecret = fp->GetBool();
			CMD_CS("#TEXTCOLOR")
				activeTextColor.r = fp->GetByte();
			activeTextColor.g = fp->GetByte();
			activeTextColor.b = fp->GetByte();
			nonactiveTextColor.r = fp->GetByte();
			nonactiveTextColor.g = fp->GetByte();
			nonactiveTextColor.b = fp->GetByte();
			CMD_CS("#COORD")
				coord.x = fp->GetDword();
			coord.y = fp->GetDword();
			CMD_CS("#FGCOLOR")
				fgColor.r = fp->GetByte();
			fgColor.g = fp->GetByte();
			fgColor.b = fp->GetByte();
			CMD_CS("#TEXT")
				int msg_idx = fp->GetInt();
			strcpy( staticText, GetMsg(msg_idx) );
			CMD_CS("#ITEMTOOLTIP")
				tt_idx = fp->GetInt();
			CMD_CS("#SHADOW")
				bShadow = fp->GetBool();
			CMD_CS("#SHADOWCOLOR")
				dwShadowColor.r = fp->GetByte();
			dwShadowColor.g = fp->GetByte();
			dwShadowColor.b = fp->GetByte();
			CMD_CS("#SHADOWTEXTXY")
				shadowTextX = fp->GetLong();
			shadowTextY = fp->GetLong();
			CMD_CS("#MESSAGE")
				int msg_idx =  fp->GetInt();
			strcpy( staticText, GetMsg(msg_idx) );
			fgColor.r = fp->GetByte();
			fgColor.g = fp->GetByte();
			fgColor.b = fp->GetByte();
			CMD_CS("#TEXTRECT")
				textRect.left =  fp->GetLong();
			textRect.top = fp->GetLong();
			textRect.right = fp->GetLong();
			textRect.bottom = fp->GetLong();
			CMD_CS("#PUSHUP")
				fPush = fp->GetBool();
			CMD_CS("#VALIDXY")
				validXY.x = fp->GetDword();
			validXY.y = fp->GetDword();
			CMD_CS("#CAPTIONRECT")
				captionRect.left = fp->GetLong();
			captionRect.top = fp->GetLong();
			captionRect.right = fp->GetLong();
			captionRect.bottom = fp->GetLong();
			CMD_CS("#ICONCELLNUM")
				cIconDialog * win = (cIconDialog *)wnd;
			win->SetCellNum(fp->GetByte());				
			CMD_CS("#ICONCELL")
				RECT iconCellRect={0,0,0,0};
			iconCellRect.left = fp->GetLong();
			iconCellRect.top = fp->GetLong();
			iconCellRect.right = fp->GetLong();
			iconCellRect.bottom = fp->GetLong();
			cIconDialog * win = (cIconDialog *)wnd;
			win->AddIconCell(&iconCellRect);
			CMD_CS("#SELECTOPTION")
				wOption = fp->GetWord();
			CMD_CS("#CELLRECT")
				RECT CellRect={0,0,0,0};
			CellRect.left = fp->GetLong();
			CellRect.top = fp->GetLong();
			CellRect.right = fp->GetLong();
			CellRect.bottom = fp->GetLong();
			cIconGridDialog * win = (cIconGridDialog *)wnd;
			win->SetCellRect(&CellRect);
			CMD_CS("#LIMITLINENUM")
				limitLineNum = fp->GetLong();
			//KES 030916
			CMD_CS("#LIMITBYTES")
				nLimitBytes = fp->GetInt();
			CMD_CS("#TEXTALIGN")
				nTextAlign = fp->GetInt();
			if( nTextAlign == 0 )	nTextAlign = TXT_LEFT;
			else if( nTextAlign == 1 )	nTextAlign = TXT_RIGHT;
			else if( nTextAlign == 2 )	nTextAlign = TXT_CENTER;
			CMD_CS("#AUTOSCROLL")
				bAutoScroll = fp->GetByte();
			CMD_CS("#PASSIVE")
				bPassive = fp->GetByte();
			//		
			CMD_CS("#TABNUM")
				cTabDialog * win = (cTabDialog *)wnd;
			tabNum = fp->GetByte();
			win->InitTab(tabNum);
			CMD_CS("#ICONCELLBGIMAGE")
				GetImage(fp, &selectedBGImage);
			CMD_CS("#DRAGOVERBGIMAGE")
				GetImage(fp, &dragoverBGImage);
			CMD_CS("#COLS")
				cols = fp->GetByte();
			CMD_CS("#ROWS")
				rows = fp->GetByte();
			CMD_CS("#MIDDLENUM")
				middleNum = fp->GetByte();
			CMD_CS("#TOPIMAGE")
				GetImage(fp, &TopImage);
			topHei = fp->GetWord();
			CMD_CS("#TOPHEIGHT")
				topHei = fp->GetWord();
			CMD_CS("#DOWNHEIGHT")
				downHei = fp->GetWord();
			CMD_CS("#MIDDELIMAGE")		// 이거 철자 바꿔야 되염 ^^;;		
				GetImage(fp, &MiddleImage);
			middleHei = fp->GetWord();
			CMD_CS("#MIDDLEHEIGHT")
				middleHei = fp->GetWord();
			CMD_CS("#DOWNIMAGE")
				GetImage(fp, &DownImage);
			downHei = fp->GetWord();
			CMD_CS("#COMBOTEXTCOLOR")
				comboTextColor.r = fp->GetByte();
			comboTextColor.g = fp->GetByte();
			comboTextColor.b = fp->GetByte();
			CMD_CS("#SELECTCOMBOINDEX")
				selectComboIdx = fp->GetWord();
			CMD_CS("#READONLY")
				bReadOnly = fp->GetBool();
			CMD_CS("#ADDSTRING")
				int msg_idx =  fp->GetInt();
			char msg2[255]={0,};
			strcpy( msg2, GetMsg(msg_idx) );
			RGBCOLOR comboListColor={0,0,0};
			comboListColor.r = fp->GetByte();
			comboListColor.g = fp->GetByte();
			comboListColor.b = fp->GetByte();

			switch(wnd->GetType())
			{
			case WT_LIST:
				{
					cList * win = (cList *)wnd;
					ITEM* pItem = new ITEM;
					strcpy(pItem->string, msg2);
					pItem->rgb =  RGB_HALF(comboListColor.r, comboListColor.g, comboListColor.b);
					win->AddItem(pItem);
				}
				break;
			case WT_LISTDIALOG:
				{
					cListDialog * win = (cListDialog *)wnd;
					win->AddItem(msg2, RGB_HALF(comboListColor.r, comboListColor.g, comboListColor.b));
				}
				break;
			case WT_COMBOBOX:
				{
					cComboBox * win = (cComboBox *)wnd;
					ITEM* pItem = new ITEM;
					strcpy(pItem->string, msg2);
					pItem->rgb =  RGB_HALF(comboListColor.r, comboListColor.g, comboListColor.b);
					win->AddItem(pItem);
				}
				break;
			}

			CMD_CS("#INITCOMBOLIST")
				cComboBox * win = (cComboBox *)wnd;
			WORD listWidth = fp->GetWord();
			win->SetMaxLine(fp->GetWord());
			GetImage(fp, &TopImage);
			topHei = fp->GetWord();
			GetImage(fp, &MiddleImage);
			middleHei = fp->GetWord();
			GetImage(fp, &DownImage);
			downHei = fp->GetWord();
			GetImage(fp, &ListOverImage);
			win->InitComboList(listWidth, &TopImage, topHei, &MiddleImage, middleHei, &DownImage, downHei, &ListOverImage);
			// LISTCTRL에서 사용하는 속성
			CMD_CS("#CREATE")
				cListCtrl * win = (cListCtrl *)wnd;
			win->Create(fp->GetInt());
			CMD_CS("#HEADIMAGE")
				GetImage(fp, &HeadImage);
			CMD_CS("#BODYIMAGE")
				GetImage(fp, &BodyImage);
			CMD_CS("#HEADHEIGHT")
				HeadHei = fp->GetWord();
			CMD_CS("#BODYHEIGHT")
				BodyHei = fp->GetWord();
			CMD_CS("#INSERTCOLUMN")
				cListCtrl * win = (cListCtrl *)wnd;
			int col_idx =  fp->GetInt();
			int msg_idx =  fp->GetInt();
			char msg2[255]={0,};
			strcpy( msg2, GetMsg(msg_idx) );
			int colSize = fp->GetInt();
			win->InsertColoumn(col_idx, msg2, colSize);
			CMD_CS("#SETITEMTEXT")
				cListCtrl * win = (cListCtrl *)wnd;
			int col_idx =  fp->GetInt();
			int row_idx =  fp->GetInt();
			int msg_idx =  fp->GetInt();
			char msg2[255]={0,};
			strcpy( msg2, GetMsg(msg_idx) );
			win->SetItemText(col_idx, row_idx, msg2);
			CMD_CS("#SPINMINMAX")
				minValue = fp->GetLong();
			maxValue = fp->GetLong();
			CMD_CS("#SPINUNIT")
				spinUnit = fp->GetLong();
			CMD_CS("#LINEUPWARD")
				lineUpward = fp->GetBool();
			CMD_CS("#LISTBARINTERVAL")
				interval = fp->GetInt();
			CMD_CS("#LISTBARPOINT")
				listBarPoint.x = fp->GetDword();
			listBarPoint.y = fp->GetDword();
			listBarPoint.w = fp->GetDword();
			listBarPoint.h = fp->GetDword();
			CMD_CS("#LISTBARIMAGE")
				GetImage(fp, &ListBarImage);
			//listBarPoint = fp->
			CMD_CS("#LISTMAXLINE")
				maxLine = fp->GetWord();
			CMD_CS("#LINEHEIGHT")
				listMaxHeight = fp->GetLong();
			CMD_CS("#BTNCLICKSOUND")
				soundIdx = fp->GetInt();
			CMD_CS("#MINMAXMIDDLENUM")
				minMiddleNum = fp->GetByte();
			maxMiddleNum = fp->GetByte();
			CMD_CS("#INITGRID")
				gridPos.x = fp->GetDword();
			gridPos.y = fp->GetDword();
			gridPos.w = fp->GetDword();
			gridPos.h = fp->GetDword();
			gridBorderX = fp->GetWord();
			gridBorderY = fp->GetWord();
			CMD_CS("#INITGUAGEBAR")
				cGuageBar * win = (cGuageBar *)wnd;
			interval = fp->GetLong();
			vertical = fp->GetBool();
			win->InitGuageBar(interval, vertical);
			CMD_CS("#MAXSPRITE")
				cAni * win = (cAni *)wnd;
			win->SetMaxSprite(fp->GetWord());
			CMD_CS("#SPRITELOOP")
				fSpriteloop = fp->GetBool();
			CMD_CS("#TRAVELINGTYPE")
				travelingType = fp->GetWord();
			CMD_CS("#ADDSPRITE")
				cAni * win = (cAni *)wnd;
			GetImage(fp, &AniSpriteImage);
			WORD delayTime = fp->GetWord();
			win->AddSprite(&AniSpriteImage, delayTime);
			CMD_CS("#GUAGEPIECEIMAGE")
				//			cGuage * win = (cGuage *)wnd;
				GetImage(fp, &GuagePieceImage);
			//			win->SetPieceImage(&GuagePieceImage);
			CMD_CS("#GUAGEMINVALUE")
				guageMinVal = fp->GetFloat();
			CMD_CS("#GUAGEMAXVALUE")
				guageMaxVal = fp->GetFloat();
			CMD_CS("#GUAGECURVALUE")
				guageCurVal = fp->GetFloat();
			CMD_CS("#GUAGENEVALUE")
				guageCurVal		= fp->GetFloat();
			guageEstTime	= fp->GetDword();
			CMD_CS("#GUAGEPOS")
				guagePos.x = fp->GetLong();
			guagePos.y = fp->GetLong();
			CMD_CS("#CHECKBOXIMAGE")
				GetImage(fp, &CheckBoxImage);
			CMD_CS("#CHECKIMAGE")
				GetImage(fp, &CheckImage);
			CMD_CS("#SETCHECK")
				bChecked = fp->GetBool();
			CMD_CS("#SCALE")
				scale.x = fp->GetFloat();
			scale.y = fp->GetFloat();
			CMD_CS("#LISTSCALE")
				ListScale.x = fp->GetFloat();
			ListScale.y = fp->GetFloat();
			CMD_CS("#GUAGEWIDTH")
				guageWidth = fp->GetFloat();
			CMD_CS("#GUAGEPIECEWIDTH")
				guagePieceWidth = fp->GetFloat();
			CMD_CS("#GUAGEEFFECTPIECEIMAGE")
				GetImage(fp, &guageEffectPieceImage);
			CMD_CS("#GUAGEEFFECTPIECEWIDTH")
				guageEffectPieceWidth = fp->GetFloat();
			CMD_CS("#GUAGEPIECEHEIGHTSCALE")
				guagePieceHeightScaleY = fp->GetFloat();
			CMD_CS("#GUAGEEFFECTPIECEHEIGHTSCALE")
				guageEffectPieceHeightScaleY = fp->GetFloat();
			CMD_CS("#BALLOONIMAGETOP")
				GetImage(fp, &BalloonImage[0]);
			GetImage(fp, &BalloonImage[1]);
			GetImage(fp, &BalloonImage[2]);
			CMD_CS("#BALLOONIMAGEMIDDLE")
				GetImage(fp, &BalloonImage[3]);
			GetImage(fp, &BalloonImage[4]);
			GetImage(fp, &BalloonImage[5]);
			CMD_CS("#BALLOONIMAGEBOTTOM")
				GetImage(fp, &BalloonImage[6]);
			GetImage(fp, &BalloonImage[7]);
			GetImage(fp, &BalloonImage[8]);
			CMD_CS("#BALLOONIMAGETAIL")
				GetImage(fp, &BalloonImage[9]);
			CMD_CS("#BALLOONIMAGEBORDER")
				BalloonBorder = fp->GetFloat();
			CMD_EN
}
*/
	// 모든 윈도우에 등록
	wnd->SetAutoClose(bAutoClose);
	wnd->SetFontIdx(fontIdx);
	wnd->SetImageSrcRect(&imgSrcRect);
	wnd->SetScale(&scale);
	wnd->OnCreate(fActive);

	SWINDOWTYPE(wnd->GetType())
	/*CASE(WT_DIVIDEBOX)
		cDivideBox * win = (cDivideBox *)wnd;
	*/	
	CASE(WT_GUAGE)
		cGuage * win = (cGuage *)wnd;
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->InitGuage(guagePos.x,guagePos.y);
		win->SetMinValue(guageMinVal);
		win->SetMaxValue(guageMaxVal);
		win->SetCurValue(guageCurVal);
		win->SetPieceImage(&GuagePieceImage);
		
	BREAK
	CASE(WT_GUAGEN)
		cGuagen * win = (cGuagen *)wnd;
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetGuageImagePos(guagePos.x,guagePos.y);
		win->SetValue(guageCurVal);
		win->SetPieceImage(&GuagePieceImage);
		win->SetGuageWidth(guageWidth);
		win->SetGuagePieceWidth(guagePieceWidth);
	BREAK
	CASE(WT_GUAGENE)
		CObjectGuagen * win = (CObjectGuagen *)wnd;
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetGuageImagePos(guagePos.x,guagePos.y);
		win->SetValue(guageCurVal, guageEstTime);
		win->SetPieceImage(&GuagePieceImage);
		win->SetGuageWidth(guageWidth);
		win->SetGuagePieceWidth(guagePieceWidth);
		win->SetGuagePieceHeightScale(guagePieceHeightScaleY);


		win->SetGuageEffectPieceWidth(guageEffectPieceWidth);
		win->SetEffectPieceImage(&guageEffectPieceImage);
		win->SetGuageEffectPieceHeightScale(guageEffectPieceHeightScaleY);
	BREAK
	CASE(WT_ANI)
		cAni * win = (cAni *)wnd;
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetValidXY(validXY.x,validXY.y);
		win->SetLoop(fSpriteloop);
		win->SetTravelingType(travelingType);
		win->SetAlpha(alphaDepth);
	BREAK
	CASE(WT_SPIN)
		cSpin * win = (cSpin *)wnd;
		if(FUNCSEARCH(funcName) == -1)
			win->Init(point.x, point.y, point.w, point.h, &BasicImage, NULL, IDSEARCH(IDName));
		else
			win->Init(point.x, point.y, point.w, point.h, &BasicImage, g_mt_func[FUNCSEARCH(funcName)]._func, IDSEARCH(IDName));
		win->SetValidXY(validXY.x,validXY.y);
		win->InitSpin(EditBoxSize, EditBoxEableTypingSize);
		win->SetActiveTextColor(RGB_HALF(activeTextColor.r,activeTextColor.g,activeTextColor.b));
		win->SetNonactiveTextColor(RGB_HALF(nonactiveTextColor.r,nonactiveTextColor.g,nonactiveTextColor.b));
		win->SetMinMax(minValue, maxValue);
		win->SetUnit(spinUnit);
		win->SetAlpha(alphaDepth);
	BREAK
	CASE(WT_STATIC)
		cStatic * win = (cStatic *)wnd;
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetValidXY(validXY.x,validXY.y);
		win->SetTextXY(textXY.x, textXY.y);
		win->SetStaticText(staticText);
		win->SetAlign( nTextAlign );
		win->SetFGColor(RGB_HALF(fgColor.r, fgColor.g, fgColor.b));

		win->SetShadowTextXY(shadowTextX, shadowTextY);
		win->SetShadowColor(RGB_HALF(dwShadowColor.r, dwShadowColor.g, dwShadowColor.b));
		win->SetShadow(bShadow);

		win->SetToolTip(toolTipMsg, RGBA_MAKE(color.r,color.g,color.b, 255), &ToolTipImage);

		if( tt_idx >= 0 )
		{
			cImage imgToolTip;
			SCRIPTMGR->GetImage( 63, &imgToolTip, PFT_HARDPATH );
			win->SetToolTip( "", RGBA_MAKE(255, 255, 255, 255), &imgToolTip, TTCLR_DEFAULT );
			ITEMMGR->AddItemDescriptionToolTip( win, tt_idx, TTTC_DEFAULT, 0 );
		}

	BREAK
	CASE(WT_PUSHUPBUTTON)
		cPushupButton * win = (cPushupButton *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		if(FUNCSEARCH(funcName) == -1)
			win->Init(point.x, point.y, point.w, point.h, &BasicImage, &OverImage, &PressImage, NULL, IDSEARCH(IDName));
		else	
			win->Init(point.x, point.y, point.w, point.h, &BasicImage, &OverImage, &PressImage, g_mt_func[FUNCSEARCH(funcName)]._func, IDSEARCH(IDName));
		win->SetToolTip(toolTipMsg, RGBA_MAKE(color.r,color.g,color.b, 255), &ToolTipImage);		
		win->SetPush(fPush);
		win->SetText(szBtnText, RGB_HALF(btnTextBasicCol.r,btnTextBasicCol.g,btnTextBasicCol.b), RGB_HALF(btnTextOverCol.r,btnTextOverCol.g,btnTextOverCol.b), RGB_HALF(btnTextPressCol.r,btnTextPressCol.g,btnTextPressCol.b)); 
		win->SetTextXY(textXY.x,textXY.y);
		win->SetAlpha(alphaDepth);
		win->SetTextAni(btnTextAniX, btnTextAniY );
		win->SetAlign( nTextAlign );
		win->SetShadowTextXY(shadowTextX, shadowTextY);
		win->SetShadowColor(RGB_HALF(dwShadowColor.r, dwShadowColor.g, dwShadowColor.b));
		win->SetShadow(bShadow);
		win->SetPassive(bPassive);
	BREAK
	CASE(WT_COMBOBOX)
		cComboBox * win = (cComboBox *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, NULL, IDSEARCH(IDName));
		win->SetComboTextColor(RGB_HALF(comboTextColor.r, comboTextColor.g, comboTextColor.b));
		win->SetAlpha(alphaDepth);
		win->SelectComboText(selectComboIdx);

		win->SetOverImageScale(ListScale.x, ListScale.y); // over image 추가
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
		//win->Init()
	BREAK
	CASE(WT_COMBOBOXEX)
		cComboBoxEx * win = (cComboBoxEx *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, NULL, IDSEARCH(IDName));
		win->SetComboTextColor(RGB_HALF(comboTextColor.r, comboTextColor.g, comboTextColor.b));
		win->SetAlpha(alphaDepth);
		win->SelectComboText(selectComboIdx);
		win->SetOverImageScale(ListScale.x, ListScale.y); // over image 추가
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
		//win->Init()
	BREAK
	CASE(WT_BUTTON)
		cButton * win = (cButton *)wnd;
		//btn->Init(point.x, point.y, point.w, point.h, pBtnImageBasic, pBtnImageOver, pBtnImagePress, mt_func[MT_FUNCSEARCH(funcName)], MT_IDSEARCH(IDName));
		if(FUNCSEARCH(funcName) == -1)
			win->Init(point.x, point.y, point.w, point.h, &BasicImage, &OverImage, &PressImage, NULL, IDSEARCH(IDName));
		else 
			win->Init(point.x, point.y, point.w, point.h, &BasicImage, &OverImage, &PressImage, g_mt_func[FUNCSEARCH(funcName)]._func, IDSEARCH(IDName));

		if(FUNCSEARCH(funcName) == -1)
		{
			int a = 0;
		}

		win->SetToolTip(toolTipMsg, RGBA_MAKE(color.r,color.g,color.b, 255), &ToolTipImage);
		if(*szBtnText != 0)
		win->SetText(szBtnText, RGB_HALF(btnTextBasicCol.r,btnTextBasicCol.g,btnTextBasicCol.b), RGB_HALF(btnTextOverCol.r,btnTextOverCol.g,btnTextOverCol.b), RGB_HALF(btnTextPressCol.r,btnTextPressCol.g,btnTextPressCol.b)); 
		win->SetTextXY(textXY.x,textXY.y);
		win->SetAlpha(alphaDepth);
		win->SetTextAni(btnTextAniX, btnTextAniY );
		win->SetAlign( nTextAlign );

		win->SetShadowTextXY(shadowTextX, shadowTextY);
		win->SetShadowColor(RGB_HALF(dwShadowColor.r, dwShadowColor.g, dwShadowColor.b));
		win->SetShadow(bShadow);

		if( soundIdx != -1 )
			win->SetClickSound( soundIdx );
	BREAK
	CASE(WT_CHECKBOX)	
		cCheckBox * win = (cCheckBox *)wnd;
		if(FUNCSEARCH(funcName) == -1)
			win->Init(point.x, point.y, point.w, point.h, &BasicImage, &CheckBoxImage, &CheckImage, NULL, IDSEARCH(IDName));
		else
			win->Init(point.x, point.y, point.w, point.h, &BasicImage, &CheckBoxImage, &CheckImage, g_mt_func[FUNCSEARCH(funcName)]._func, IDSEARCH(IDName));
		win->SetChecked(bChecked);
		win->SetCheckBoxMsg(staticText, RGB_HALF(fgColor.r,fgColor.g,fgColor.b));
	BREAK
	CASE(WT_EDITBOX)
		cEditBox * win = (cEditBox *)wnd;
		//editBox->Init(point.x, point.y, point.w, point.h, &BasicImage, MT_IDSEARCH(IDName));
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, &FocusImage, IDSEARCH(IDName));
		win->InitEditbox(EditBoxSize, EditBoxEableTypingSize);
		win->SetAlign( nTextAlign );
		win->SetToolTip(toolTipMsg, RGBA_MAKE(color.r,color.g,color.b, 255), &ToolTipImage);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
		//win->SetcbFunc(mt_func[FUNCSEARCH(funcName)]);
		win->SetSecret(fSecret);
		win->SetActiveTextColor(RGB_HALF(activeTextColor.r,activeTextColor.g,activeTextColor.b));
		win->SetNonactiveTextColor(RGB_HALF(nonactiveTextColor.r,nonactiveTextColor.g,nonactiveTextColor.b));
		win->SetAlpha(alphaDepth);
		win->SetReadOnly(bReadOnly);
	BREAK
	CASE(WT_GUAGEBAR)
		cGuageBar * win = (cGuageBar *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->InitValue(guageMinVal, guageMaxVal, guageCurVal);
		win->SetAlpha(alphaDepth);
		//if(-1 != FUNCSEARCH(funcName))
		//	win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_TEXTAREA)
		cTextArea * win = (cTextArea *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->InitTextArea(&textRect, nLimitBytes, &TopImage, topHei, &MiddleImage, middleHei, &DownImage, downHei );
		win->SetLimitLine( limitLineNum );	//inittextarea보다 아래에 있어야한다.
		win->SetAlpha(alphaDepth);
		win->SetReadOnly(bReadOnly);
		win->SetMiddleScale( ListScale );
		win->SetScriptText(staticText);
		
		//if(-1 != FUNCSEARCH(funcName))
		//	win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_LIST)
		cList * win = (cList *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &TopImage, IDSEARCH(IDName));
		win->InitList(maxLine, &textRect);
		win->SetLineUpward(lineUpward);
		win->SetListBarImage(&ListBarImage);
		win->SetListBar(listBarPoint.x, listBarPoint.y, listBarPoint.w, listBarPoint.h, interval);
		win->SetMaxLine(maxLine);
		win->SetAlpha(alphaDepth);
		
		//if(-1 != FUNCSEARCH(funcName))
		//	win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_LISTCTRL)
		cListCtrl * win = (cListCtrl *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->InitListCtrl(cols, rows);
		win->InitListCtrlImage(&HeadImage, HeadHei, &BodyImage, BodyHei, &ListOverImage );
		win->SetOverImageScale(ListScale.x, ListScale.y); // over image 추가
		win->SetAlpha(alphaDepth);
		win->SetSelectOption( wOption );
	BREAK
	CASE(WT_LISTCTRLEX)
		cListCtrlEx * win = (cListCtrlEx *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->InitListCtrlEx( cols, middleNum, middleHei, nLeftMargin, nTopMargin );
		win->InitListCtrlExImage( &HeadImage, &BodyImage, &DownImage, &SelectedImage, &ListOverImage );
		win->SetAlpha(alphaDepth);
	BREAK
	CASE(WT_LISTDIALOG)
		cListDialog * win = (cListDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->SetMinMaxMiddleNum(minMiddleNum, maxMiddleNum);
		win->Init(point.x, point.y, point.w, &TopImage, topHei, &MiddleImage, middleHei, &DownImage, downHei, &ListOverImage, middleNum, IDSEARCH(IDName));
		win->SetLineHeight(listMaxHeight);
		win->InitList(maxLine, &textRect);
		win->SetMaxLine(maxLine);
		win->SetOverImageScale(ListScale.x, ListScale.y); // over image 추가
		//win->SetListBarImage(&ListBarImage);
		//win->SetListBar(listBarPoint.x, listBarPoint.y, listBarPoint.w, listBarPoint.h, interval);
		win->SetAlpha(alphaDepth);
		win->SetAutoScroll( bAutoScroll );

		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
		//dlg->Init(point.x, point.y, point.w, point.h, &BasicImage, MT_IDSEARCH(IDName));
		//win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		//win->SetMovable(fMovable);
		//win->SetCaptionRect(&captionRect);
	BREAK
	CASE(WT_CHARMAKEDIALOG)
		cCharMakeDlg * win = (cCharMakeDlg *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		//dlg->Init(point.x, point.y, point.w, point.h, &BasicImage, MT_IDSEARCH(IDName));
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_DIALOG)
		cDialog * win = (cDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		//dlg->Init(point.x, point.y, point.w, point.h, &BasicImage, MT_IDSEARCH(IDName));
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_TABDIALOG)
		cTabDialog * win = (cTabDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_CHATDIALOG)
		CChatDialog * win = (CChatDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);		
	BREAK
	CASE(WT_ICONDIALOG)
		cIconDialog * win = (cIconDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		//win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetIconCellBGImage(&selectedBGImage);
		win->SetDragOverBGImage(&dragoverBGImage);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_WEAREDDIALOG)
		CWearedExDialog * win = (CWearedExDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		//win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetIconCellBGImage(&selectedBGImage);
		win->SetDragOverBGImage(&dragoverBGImage);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_ICONGRIDDIALOG)
		cIconGridDialog * win = (cIconGridDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);	
		//dlg->Init(point.x, point.y, point.w, point.h, &BasicImage, MT_IDSEARCH(IDName));
		win->InitDialog(point.x, point.y, point.w, point.h, &BasicImage, cols, rows, IDSEARCH(IDName));
		//win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->InitGrid(gridPos.x,gridPos.y,gridPos.w,gridPos.h,gridBorderX,gridBorderY);
		win->SetAlpha(alphaDepth);
		win->SetIconCellBGImage(&selectedBGImage);
		win->SetDragOverBGImage(&dragoverBGImage);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_MONSTERGUAGEDLG)
		cDialog * win = (cDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		//dlg->Init(point.x, point.y, point.w, point.h, &BasicImage, MT_IDSEARCH(IDName));
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_QUICKDIALOG)
		CQuickDialog * win = (CQuickDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);		
		//dlg->Init(point.x, point.y, point.w, point.h, &BasicImage, MT_IDSEARCH(IDName));
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		//win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_CHARINFODIALOG)
		CCharacterDialog * win = (CCharacterDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_MUGONGSURYUNDIALOG)
		CMugongSuryunDialog * win = (CMugongSuryunDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_MUGONGDIALOG)
		CMugongDialog * win = (CMugongDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetIconCellBGImage(&selectedBGImage);
		win->SetDragOverBGImage(&dragoverBGImage);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_SURYUNDIALOG)
		CSuryunDialog * win = (CSuryunDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_MAINDIALOG)
		CMugongDialog * win = (CMugongDialog *)wnd;	
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_INVENTORYDIALOG)
		CInventoryExDialog * win = (CInventoryExDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		//win->InitDialog(point.x, point.y, point.w, point.h, &BasicImage, cols, rows, IDSEARCH(IDName));
		//win->InitItemGrid(eItemGrid_Inventory,MAX_INVENTORY_NUM);
		win->SetMovable(fMovable);
		//win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName))
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_HELPERSPEECHDIALOG)
		cHelperSpeechDlg* win = (cHelperSpeechDlg*)wnd;
		win->SetValidXY(validXY.x, validXY.y);
		win->Init(point.x, point.y, point.w, point.h, listMaxHeight, IDSEARCH(IDName) );
		win->Linking();
		win->InitBalloon( &BalloonImage[0], &BalloonImage[2], &BalloonImage[6], &BalloonImage[8], &BalloonImage[3],
			   &BalloonImage[5], &BalloonImage[1], &BalloonImage[7], &BalloonImage[4], &BalloonImage[9], BalloonBorder);
		win->SetTextRect(&textRect);
		win->SetCaptionRect(&captionRect);
		win->SetMovable(fMovable);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_STREETSTALLDIALOG)
		CStreetStall * win = (CStreetStall *)wnd;	
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		//win->InitDialog(point.x, point.y, point.w, point.h, &BasicImage, cols, rows, IDSEARCH(IDName));
		//win->InitItemGrid(eItemGrid_Inventory,MAX_INVENTORY_NUM);
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_STREETBUYSTALLDIALOG)
		CStreetBuyStall * win = (CStreetBuyStall *)wnd;	
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		//win->InitDialog(point.x, point.y, point.w, point.h, &BasicImage, cols, rows, IDSEARCH(IDName));
		//win->InitItemGrid(eItemGrid_Inventory,MAX_INVENTORY_NUM);
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_NPCSCRIPDIALOG)
		cNpcScriptDialog * win = (cNpcScriptDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		//win->InitDialog(point.x, point.y, point.w, point.h, pBasicImage, cols, rows, IDSEARCH(IDName));
		//win->InitItemGrid(eItemGrid_Inventory,MAX_INVENTORY_NUM);
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_HELPDIALOG)
		cHelpDialog * win = (cHelpDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		//win->InitDialog(point.x, point.y, point.w, point.h, pBasicImage, cols, rows, IDSEARCH(IDName));
		//win->InitItemGrid(eItemGrid_Inventory,MAX_INVENTORY_NUM);
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_MONEYDIALOG)
		CMoneyDlg * win = (CMoneyDlg *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_DEALDIALOG)
		CDealDialog * win = (CDealDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		//win->InitDialog(point.x, point.y, point.w, point.h, &BasicImage, cols, rows, IDSEARCH(IDName));
		//win->InitItemGrid(eItemGrid_Inventory,MAX_INVENTORY_NUM);
		win->SetMovable(fMovable);
		//win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName))
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_MUNPAMARKDLG)
		CMunpaMarkDialog * win = (CMunpaMarkDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_PYOGUKDLG)
		CPyogukDialog * win = (CPyogukDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK		
	CASE(WT_NOTEDLG)
		CNoteDialog * win = (CNoteDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK		
	CASE(WT_MININOTEDLG)
		CMiniNoteDialog * win = (CMiniNoteDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_FRIENDDLG)
		CFriendDialog * win = (CFriendDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK		
	CASE(WT_MINIFRIENDDLG)
		CMiniFriendDialog * win = (CMiniFriendDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK		
	CASE(WT_WANTNPCDIALOG)
		CWantNpcDialog * win = (CWantNpcDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);

	BREAK
	CASE(WT_WANTREGISTDIALOG)
		CWantRegistDialog * win = (CWantRegistDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);

	BREAK
	CASE(WT_QUESTTOTALDIALOG)
		CQuestTotalDialog * win = (CQuestTotalDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_QUESTDIALOG)
		CQuestDialog * win = (CQuestDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetIconCellBGImage(&selectedBGImage);
		win->SetDragOverBGImage(&dragoverBGImage);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_WANTEDDIALOG)
		CWantedDialog * win = (CWantedDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);

	BREAK
	CASE(WT_JOURNALDIALOG)
		CJournalDialog * win = (CJournalDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);

	BREAK
	CASE(WT_PKLOOTINGDLG)
		CPKLootingDialog* win = (CPKLootingDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, -1);
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_MPREGISTDIALOG)
		CMPRegistDialog* win = (CMPRegistDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_MPMISSIONDLG)
		CMPMissionDialog* win = (CMPMissionDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_MPNOTICEDIALOG)
		CMPNoticeDialog* win = (CMPNoticeDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_MPGUAGEDLG)
		CMPGuageDialog * win = (CMPGuageDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_PARTYCREATEDLG)
		CPartyCreateDlg * win = (CPartyCreateDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_PARTYINVITEDLG)
		CPartyInviteDlg * win = (CPartyInviteDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GUILDCREATEDLG)
		CGuildCreateDialog * win = (CGuildCreateDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);		
	BREAK
	CASE(WT_GUILDDLG)
		CGuildDialog * win = (CGuildDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);		
	BREAK
	CASE(WT_GUILDINVITEDLG)
		CGuildInviteDialog * win = (CGuildInviteDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);		
	BREAK
	CASE(WT_GUILDMARKDLG)
		CGuildMarkDialog * win = (CGuildMarkDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);		
	BREAK
	CASE(WT_GUILDLEVELUPDLG)
		CGuildLevelUpDialog * win = (CGuildLevelUpDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);		
	BREAK
	CASE(WT_GUILDNICKNAMEDLG)
		CGuildNickNameDialog * win = (CGuildNickNameDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);		
	BREAK
	CASE(WT_GUILDRANKDLG)
		CGuildRankDialog * win = (CGuildRankDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);		
	BREAK
	CASE(WT_GUILDWAREHOUSEDLG)
		CGuildWarehouseDialog * win = (CGuildWarehouseDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GUILDUNIONCREATEDLG)
		CGuildUnionCreateDialog * win = (CGuildUnionCreateDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_ITEMSHOPDIALOG)
		CItemShopDialog * win = (CItemShopDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_MOVEDIALOG)
		CMoveDialog * win = (CMoveDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
//		win->InitListCtrl(cols, rows);
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
// guildfieldwar-------------------------------------------------------
	CASE(WT_GUILDFIELDWAR_DECLARE_DLG)
		CGFWarDeclareDlg* win = (CGFWarDeclareDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GUILDFIELDWAR_RESULT_DLG)
		CGFWarResultDlg* win = (CGFWarResultDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GUILDFIELDWAR_INFO_DLG)
		CGFWarInfoDlg* win = (CGFWarInfoDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GUILDWAR_INFO_DLG)
		CGuildWarInfoDlg* win = (CGuildWarInfoDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
//---------------------------------------------------------------------
	CASE(WT_SKPOINTDIALOG)
		CSkillPointRedist * win = (CSkillPointRedist*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_SKPOINTNOTIFYDIALOG)
		CSkillPointNotify * win = (CSkillPointNotify*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
// partywar-----------------------
	CASE(WT_PARTYWAR_DLG)
		CPartyWarDialog* win = (CPartyWarDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK		
	CASE(WT_SHOUT_DLG)
		CShoutDialog* win = (CShoutDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK		
	CASE(WT_SHOUTCHAT_DLG)
		CShoutchatDialog* win = (CShoutchatDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK		
	CASE(WT_CHASE_DLG)
		CChaseDialog* win = (CChaseDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK		
	CASE(WT_CHASEINPUT_DLG)
		CChaseinputDialog* win = (CChaseinputDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK		
	CASE(WT_NAMECHANGE_DLG)
		CNameChangeDialog* win = (CNameChangeDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK		
	CASE(WT_NAMECHANGENOTIFY_DLG)
		CNameChangeNotifyDlg* win = (CNameChangeNotifyDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GTREGIST_DLG)
		CGTRegistDialog* win = (CGTRegistDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GTREGISTCANCEL_DLG)
		CGTRegistcancelDialog* win = (CGTRegistcancelDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GTSTANDING_DLG)
		CGTStandingDialog* win = (CGTStandingDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GTBATTLELIST_DLG)
		CGTBattleListDialog* win = (CGTBattleListDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GTSCOREINFO_DLG)
		CGTScoreInfoDialog* win = (CGTScoreInfoDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_SEIGEWAR_TIMEREG_DLG)
		CSWTimeRegDlg* win = (CSWTimeRegDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_SEIGEWAR_PROTECTREG_DLG)
		CSWProtectRegDlg* win = (CSWProtectRegDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_SEIGEWAR_INFO_DLG)
		CSWInfoDlg* win = (CSWInfoDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_CHARCHANGE_DLG)
		CCharChangeDlg* win = (CCharChangeDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_ITEM_SEAL_DLG)
		CSealDialog* win = (CSealDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK	
	CASE(WT_SEIGEWAR_PROFIT_DLG)
		CSWProfitDlg* win = (CSWProfitDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_SIEGEWAR_ENGRAVE_DLG)
		CSWEngraveDialog* win = (CSWEngraveDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_SIEGEWAR_TIME_DLG)
		CSWTimeDialog* win = (CSWTimeDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK		
	CASE(WT_ITEM_CHANGEJOB_DLG)
		CChangeJobDialog* win = (CChangeJobDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK	
	CASE(WT_SW_STARTTIME_DLG)
		CSWStartTimeDialog* win = (CSWStartTimeDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_ITEM_REINFORCERESET_DLG)
		CReinforceResetDlg* win = (CReinforceResetDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_ITEM_RARECREATE_DLG)
		CRareCreateDialog* win = (CRareCreateDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_SCREENSHOT_DLG)
		CScreenShotDlg* win = (CScreenShotDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GUILDMUNHADLG)
		CGuildMunhaDialog* win = (CGuildMunhaDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK

	EWINDOWTYPE	

	return wnd;
}

void cScriptManager::CheckPosForResolution( cPoint* pPos )
{
	int right = pPos->x + pPos->w;
	int bottom = pPos->y + pPos->h;

	int Wgap = right - LOWRSLTN_W;
	int Hgap = bottom - LOWRSLTN_H;

	if( Wgap > 0 )
	{
		pPos->x -= Wgap;
	}
	if( Hgap > 0 )
	{
		pPos->y -= Hgap;
	}

	if( pPos->x < 0 || Wgap > LOWRSLTN_W )
	{
		pPos->x = 0;
	}
	if( pPos->y < 0 || Hgap > LOWRSLTN_H )
	{
		pPos->y = 0;
	}
}

/////////////////// 2007. 5. 18. CBH - Parse Type 관련 함수 추가 ///////////////
BOOL cScriptManager::InitParseTypeData()
{
	int nMaxNum = eMAX_PARSETYPE_NUM;

	m_ParseTypeTable.Initialize(nMaxNum); //ParseType저장 테이블 초기화
	
	CMHFile fp;
	PARSE_TYPE_INFO* pParseTypeInfo;

	if(!fp.Init("./image/ParseTypeList.bin", "rb"))
		return FALSE;
	
	while(!fp.IsEOF())
	{
		pParseTypeInfo = NULL;
		pParseTypeInfo = new PARSE_TYPE_INFO;

		pParseTypeInfo->nType = fp.GetInt();
		SafeStrCpy(pParseTypeInfo->cpTypeName, fp.GetString(), MAX_PARSE_TYPE_NAME_LENGTH+1 );		
		int nKeyValue = ChangeHashKey(pParseTypeInfo->cpTypeName, nMaxNum);
		pParseTypeInfo->nKeyValue = nKeyValue;
		if(nKeyValue < 0)
		{
			return FALSE;
		}
		m_ParseTypeTable.Add(pParseTypeInfo, nKeyValue);
	}
	
	return TRUE;
}

int cScriptManager::ChangeHashKey(const char* cpTypeName, int nMaxNum)
{
	int nKeyValue = -1;
	int nValue = 0;

	if(nMaxNum < 1)
	{
		return -1;
	}
	
	int nStrSize = strlen(cpTypeName);
	nKeyValue = ((cpTypeName[0] + cpTypeName[1] + nStrSize) * (cpTypeName[nStrSize] + cpTypeName[nStrSize-1])) % nMaxNum;

	if(nKeyValue > nMaxNum)
	{
		return -1;
	}

	return nKeyValue;
}

PARSE_TYPE cScriptManager::GetParseType(const char* str)
{
	int nMaxNum = eMAX_PARSETYPE_NUM;

	int nKeyValue = ChangeHashKey(str, nMaxNum);

	if(nKeyValue < 0)
	{
		return (PARSE_TYPE)-1;
	}		
	
	PARSE_TYPE_INFO* pParseTypeList = m_ParseTypeTable.GetData(nKeyValue);
	
	if(pParseTypeList == NULL)
	{
		return (PARSE_TYPE)-1;
	}	

	//같은 키값의 중복 되는 데이터 처리
	if(strcmp(pParseTypeList->cpTypeName, str) != 0)
	{
		PARSE_TYPE_INFO* pListTemp = NULL;
		pListTemp = pParseTypeList;
		m_ParseTypeTable.SetPositionHead();
		m_ParseTypeTable.StartGetMultiData(nKeyValue);

		while(strcmp(pListTemp->cpTypeName, str) != 0)
		{				
			pListTemp = m_ParseTypeTable.GetMultiData();

			if(pListTemp == NULL)
			{
				return (PARSE_TYPE)-1;
			}
		}
		pParseTypeList = pListTemp;		
	}

	return (PARSE_TYPE)pParseTypeList->nType;
}
///////////////////////////////////////////////////////////////////////////////////////

