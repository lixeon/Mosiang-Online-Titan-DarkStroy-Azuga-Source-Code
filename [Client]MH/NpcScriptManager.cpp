#include "stdafx.h"
#include "NpcScriptManager.h"
#include "cPage.h"
#include "cDialogueList.h"
#include "cHyperTextList.h"
#include "GameIn.h"
#include "Npc.h"
#include "ObjectManager.h"
#include "PyogukDialog.h"
#include "MugongSuryunDialog.h"
#include "ChatManager.h"
#include "WindowIDEnum.h"

#include "NpcScriptDialog.h"
#include "InventoryExDialog.h"
#include "MPRegistDialog.h"
#include "GuildLevelUpDialog.h"
#include "GuildMarkDialog.h"
#include "GuildWarehouseDialog.h"
#include "GuildCreateDialog.h"
#include "objectstatemanager.h"
#include "WantedManager.h"
#include "GuildManager.h"
#include "BailDialog.h"
#include "WantedDialog.h"
#include "./Interface/cWindowManager.h"
#include "cMsgBox.h"
#include "AbilityManager.h"
#include "ItemShopDialog.h"
#include "MHMap.h"
#include "MoveManager.h"
// Guild Tournament
#include "GTRegistDialog.h"
#include "GTRegistcancelDialog.h"
#include "GTBattleListDialog.h"
// guildunion
#include "GuildUnion.h"
#include "GuildTraineeDialog.h"

// seigewar
#include "SeigeWarDialog.h"
#include "SiegeWarMgr.h"

#include "SkillOptionChangeDlg.h"

#include "TitanPartsMakeDlg.h"
#include "TitanMixDlg.h"
#include "TitanUpgradeDlg.h"
#include "TitanInventoryDlg.h"
#include "TitanRepairDlg.h"
#include "TitanBreakDlg.h"
#include "TitanRegisterDlg.h"
#include "TitanDissolutionDlg.h"
//#include "TitanMugongMixDlg.h"	// magi82 - Titan(070611) 타이탄 무공변환 주석처리
#include "UniqueItemMixDlg.h"

#include "TitanManager.h"
#include "bobusangmanager.h"
#include "item.h"


GLOBALTON(cNpcScriptManager)
cNpcScriptManager::cNpcScriptManager()
{
	m_nRegistNpcCount = 0;
	m_pDialogue = NULL;
	m_pHyperText = NULL;

	SetSelectedNpc(NULL);
}

cNpcScriptManager::~cNpcScriptManager()
{
	DeleteAllScriptInfo();
	SAFE_DELETE( m_pDialogue );
	SAFE_DELETE( m_pHyperText );
}

void cNpcScriptManager::Init()
{
	LoadNpcScriptInfo();

	m_pDialogue = new cDialogueList;
#ifdef _FILE_BIN_
	m_pDialogue->LoadDialogueListFile(NPC_DIALOGUE_PATH, "rb");
#else
	m_pDialogue->LoadDialogueListFile(NPC_DIALOGUE_PATH);
#endif

	m_pHyperText = new cHyperTextList;
#ifdef _FILE_BIN_
	m_pHyperText->LoadHyperTextFormFile(NPC_HYPERTEXT_PATH, "rb");
#else
	m_pHyperText->LoadHyperTextFormFile(NPC_HYPERTEXT_PATH);
#endif
}


void cNpcScriptManager::AddNpcScriptInfo(DWORD dwNpcId, cPage* pPage)
{
	m_NpcScriptList[dwNpcId].AddTail(pPage);
}

void cNpcScriptManager::DeleteAllScriptInfo()
{
	for(int i=0;i<MAX_NPC_COUNT;++i)
	{
		PTRLISTSEARCHSTART(m_NpcScriptList[i],cPage*,pPage)
			delete pPage;
		PTRLISTSEARCHEND

			m_NpcScriptList[i].RemoveAll();
	}
}

cPage* cNpcScriptManager::GetMainPage( DWORD dwNpcId )
{
	if( m_NpcScriptList[dwNpcId].IsEmpty() ) return NULL;

	return (cPage*)m_NpcScriptList[dwNpcId].GetHead();
}

cPage* cNpcScriptManager::GetPage( DWORD dwNpcId, DWORD dwPageId )
{
	PTRLISTSEARCHSTART(m_NpcScriptList[dwNpcId],cPage*,pPage)
		if( pPage->GetPageId() == dwPageId )
			return pPage;
	PTRLISTSEARCHEND

		return NULL;
}


void cNpcScriptManager::LoadNpcScriptInfo()
{
	CMHFile fp;
#ifdef _FILE_BIN_
	if(!fp.Init(NPC_SCRIPT_PATH, "rb"))
		return;
#else
	if(!fp.Init(NPC_SCRIPT_PATH, "rt"))
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
			CMD_CS("$NPC")
			if((fp.GetString())[0] == '{')
			{
				LoadPageInfo(NULL, &fp);
				++m_nRegistNpcCount;
			}
			else
				__asm int 3;
		CMD_EN
	}
}

void cNpcScriptManager::LoadPageInfo(cPage* pPage, CMHFile* fp)
{
	// 다시한번 확인 및 주석 처리요!!
	char buff[256]={0,};

	static DWORD dwNpcId; // Npc에대한 Page정보가 완료 될때까지 유지 가능해야 한다.
	DWORD dwPageId;
	int nDialogueCount = 0;
	int nHyperLinkCount = 0;
	DWORD dwDialogueId;
	HYPERLINK sHyper;

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
				AddNpcScriptInfo( dwNpcId, pNewPage );
				LoadPageInfo(pNewPage, fp);
			}
			CMD_CS("#NPCID")
				dwNpcId = fp->GetDword();
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
				//			if( sHyper.wLinkType == emLink_Page || sHyper.wLinkType == emLink_Open 
				//				|| sHyper.wLinkType == emLink_Quest )
				sHyper.dwData = fp->GetDword();
				pPage->AddHyperLink( &sHyper );
				CMD_EN
	}
}

void cNpcScriptManager::StartNpcScript( CNpc* pNpc )
{
	// 기존에 존재하면 기존것을 삭제하고 새로 연다.
	if( m_pCurSelectedNpc )
	{
		GAMEIN->GetNpcScriptDialog()->EndDialog();
		SetSelectedNpc(NULL);
	}

	SetSelectedNpc(pNpc);
	GAMEIN->GetNpcScriptDialog()->OpenDialog( pNpc->GetNpcUniqueIdx() );
}

void cNpcScriptManager::StartNpcBusiness(DWORD dwData, DWORD dwParam)
{
	if( m_pCurSelectedNpc == NULL && dwParam == 0 ) 
	{
		ASSERT(0);

		if( HERO->GetState() == eObjectState_Deal )
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

		return;
	}

	if( m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == DEALER_ROLE)
	{
		switch( dwData )
		{
		case 0:		// 거래
			{
				MSG_WORD msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_DEALER_SYN;
				msg.dwObjectID = HEROID;
				msg.wData = m_pCurSelectedNpc->GetNpcUniqueIdx();
				NETWORK->Send(&msg, sizeof(msg));
			}
			break;
		case 1:		// 유니크 조합
			{
				GAMEIN->GetUniqueItemMixDlg()->SetActive( TRUE );	// magi82(1) - UniqueItem(071022)
			}
			break;
		}
	}
	//SW070626 보부상NPC
	else if( m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == BOBUSANG_ROLE )
	{
		CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemLike( BOBUSANG_ITEM_TICKET );

		if( !pItem )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1639) );
			GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);
			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

			return;
		}
		else
		{
			MSG_WORD msg;
			msg.Category = MP_BOBUSANG;
			msg.Protocol = MP_BOBUSANG_ADD_GUEST_SYN;
			msg.dwObjectID = HEROID;
			msg.wData = m_pCurSelectedNpc->GetNpcUniqueIdx();
			NETWORK->Send(&msg, sizeof(msg));
		}
	}
	else if( m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == CASTLE_ROLE )
	{
		switch( dwData )
		{
		case 0:		// 세율관리
			{
				if( HERO->GetGuildIdx() == 0 || HERO->GetGuildMemberRank() != GUILD_MASTER )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(726) );
					return;
				}
				SWPROFIT->SendProfitInfoSyn();
			}
			break;
		case 1:
		case 2:
		case 3:		// 외성문
		case 4:
		case 5:
		case 6:		// 내성문
			{	
				if( HERO->GetGuildIdx() == 0 || HERO->GetGuildMemberRank() != GUILD_MASTER )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(726) );
					return;
				}
				DWORD dwMoney = 0;
				switch( dwData )
				{
				case 1:		dwMoney = 1000000;		break;
				case 2:		dwMoney = 2000000;		break;
				case 3:		dwMoney = 3000000;		break;
				case 4:		dwMoney = 1500000;		break;
				case 5:		dwMoney = 2500000;		break;				
				case 6:		dwMoney = 4000000;		break;
				}
				if( HERO->GetMoney() < dwMoney )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(117) );
					return;
				}
				MSG_DWORD Msg;
				Msg.Category = MP_SIEGEWAR;
				Msg.Protocol = MP_SIEGEWAR_UPGRADE_GATE_SYN;
				Msg.dwObjectID = HEROID;
				Msg.dwData = dwData;
				NETWORK->Send( &Msg, sizeof(Msg) );
				return;
			}
			break;
		}		
	}
	else if( (m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == CHANGGO_ROLE) || dwParam == eNpcParam_ShowpPyoguk )
	{
		if(dwData == 0) //창고 보기
		{	
			if(GAMEIN->GetPyogukDialog()->IsItemInit())
			{
				if( !GAMEIN->GetInventoryDialog()->IsActive() )
					GAMEIN->GetInventoryDialog()->SetActive(TRUE);
				GAMEIN->GetPyogukDialog()->SetActive(TRUE);
				if(HERO->GetPyogukNum() == 0)
				{
#ifdef _JAPAN_LOCAL_
					if( HERO->GetExtraPyogukSlot() == 0 )					
						GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_NoWare);
					else
					{
						GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_PyogukWare4);
						GAMEIN->GetPyogukDialog()->AddPyogukMode(ePyogukMode_PyogukWare4);
					}
#elif defined _HK_LOCAL_
					if( HERO->GetExtraPyogukSlot() == 0 )					
						GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_NoWare);
					else
					{
						GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_PyogukWare4);
						GAMEIN->GetPyogukDialog()->AddPyogukMode(ePyogukMode_PyogukWare4);
					}
#elif defined _TL_LOCAL_
					if( HERO->GetExtraPyogukSlot() == 0 )					
						GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_NoWare);
					else
					{
						GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_PyogukWare4);
						GAMEIN->GetPyogukDialog()->AddPyogukMode(ePyogukMode_PyogukWare4);
					}
#else

					GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_NoWare);
#endif	// _JAPAN_LOCAL_
				}
				else
				{
					GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_PyogukWare1);
					GAMEIN->GetPyogukDialog()->AddPyogukMode(ePyogukMode_PyogukWare1);
				}
			}
			else
			{
				if(HERO->GetPyogukNum() == 0)
				{
#ifdef _JAPAN_LOCAL_
					if( HERO->GetExtraPyogukSlot() == 0 )
					{
						if( !GAMEIN->GetInventoryDialog()->IsActive() )
							GAMEIN->GetInventoryDialog()->SetActive(TRUE);
						GAMEIN->GetPyogukDialog()->SetActive(TRUE);
						GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_NoWare);						
					}
					else
					{
						if( dwParam == eNpcParam_ShowpPyoguk || MAP->IsVillage() == TRUE )
						{						
							MSG_WORD2 msg;
							msg.Category = MP_ITEM;
							msg.Protocol = MP_ITEM_PYOGUK_ITEM_INFO_SYN;
							msg.dwObjectID = HEROID;						
							dwParam==eNpcParam_ShowpPyoguk ? msg.wData1=79 : msg.wData1=m_pCurSelectedNpc->GetNpcUniqueIdx();
							msg.wData2 = eNpcParam_ShowpPyoguk;
							NETWORK->Send(&msg, sizeof(msg));
						}
						else
							return;
					}
#elif defined _HK_LOCAL_
					if( HERO->GetExtraPyogukSlot() == 0 )
					{
						if( !GAMEIN->GetInventoryDialog()->IsActive() )
							GAMEIN->GetInventoryDialog()->SetActive(TRUE);
						GAMEIN->GetPyogukDialog()->SetActive(TRUE);
						GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_NoWare);						
					}
					else
					{
						if( dwParam == eNpcParam_ShowpPyoguk || MAP->IsVillage() == TRUE )
						{						
							MSG_WORD2 msg;
							msg.Category = MP_ITEM;
							msg.Protocol = MP_ITEM_PYOGUK_ITEM_INFO_SYN;
							msg.dwObjectID = HEROID;						
							dwParam==eNpcParam_ShowpPyoguk ? msg.wData1=79 : msg.wData1=m_pCurSelectedNpc->GetNpcUniqueIdx();
							msg.wData2 = eNpcParam_ShowpPyoguk;
							NETWORK->Send(&msg, sizeof(msg));
						}
						else
							return;
					}
#elif defined _TL_LOCAL_
					if( HERO->GetExtraPyogukSlot() == 0 )
					{
						if( !GAMEIN->GetInventoryDialog()->IsActive() )
							GAMEIN->GetInventoryDialog()->SetActive(TRUE);

						GAMEIN->GetPyogukDialog()->SetActive(TRUE);
						GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_NoWare);						
					}
					else
					{
						if( dwParam == eNpcParam_ShowpPyoguk || MAP->IsVillage() == TRUE )
						{						
							MSG_WORD2 msg;
							msg.Category = MP_ITEM;
							msg.Protocol = MP_ITEM_PYOGUK_ITEM_INFO_SYN;
							msg.dwObjectID = HEROID;						
							dwParam==eNpcParam_ShowpPyoguk ? msg.wData1=79 : msg.wData1=m_pCurSelectedNpc->GetNpcUniqueIdx();
							msg.wData2 = eNpcParam_ShowpPyoguk;
							NETWORK->Send(&msg, sizeof(msg));
						}
						else
							return;
					}
#else
					if( !GAMEIN->GetInventoryDialog()->IsActive() )
						GAMEIN->GetInventoryDialog()->SetActive(TRUE);
					GAMEIN->GetPyogukDialog()->SetActive(TRUE);
					GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_NoWare);
#endif	// _JAPAN_LOCAL_

				}
				else
				{
					if( dwParam == eNpcParam_ShowpPyoguk || MAP->IsVillage() == TRUE )
					{						
						MSG_WORD2 msg;
						msg.Category = MP_ITEM;
						msg.Protocol = MP_ITEM_PYOGUK_ITEM_INFO_SYN;
						msg.dwObjectID = HEROID;						
						dwParam==eNpcParam_ShowpPyoguk ? msg.wData1=79 : msg.wData1=m_pCurSelectedNpc->GetNpcUniqueIdx();
						msg.wData2 = eNpcParam_ShowpPyoguk;
						NETWORK->Send(&msg, sizeof(msg));
					}
					else
						return;
				}
			}

			OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);
		}	
		else if(dwData == 1) //가격표/구입
		{
			GAMEIN->GetPyogukDialog()->SetPyogukListInfo();
			GAMEIN->GetPyogukDialog()->SetActive(TRUE);			
			GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_PyogukListInfo);
			GAMEIN->GetPyogukDialog()->AddPyogukMode(ePyogukMode_PyogukListInfo);
		}
		else if(dwData == 2)
		{
			if(HERO->GetGuildIdx() == 0)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(35));
				return;
			}
			if(GUILDMGR->GetGuildLevel() < GUILD_3LEVEL)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(723));
				return;
			}
			if(GUILDMGR->CanUseWarehouse() == FALSE)
				return;
			GAMEIN->GetGuildWarehouseDlg()->SetActive(TRUE);	
		}
		else if(dwData == 3)			// 아이템몰창고
		{
			MSGBASE msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_SHOPITEM_INFO_SYN;
			msg.dwObjectID = HERO->GetID();
			NETWORK->Send(&msg, sizeof(msg));
		}
		else if(dwData == 4)			// 공성창고
		{
			MSGBASE msg;
			msg.Category = MP_FORTWAR;
			msg.Protocol = MP_FORTWAR_SIEGEWAREHOUSE_INFO_SYN;
			msg.dwObjectID = HEROID;
			NETWORK->Send(&msg, sizeof(msg));

			if( !GAMEIN->GetInventoryDialog()->IsActive() )
				GAMEIN->GetInventoryDialog()->SetActive(TRUE);
		}
		else if(dwData == 5)			// 1요새창고
		{
			MSG_INT msg;
			msg.Category = MP_FORTWAR;
			msg.Protocol = MP_FORTWAR_WAREHOUSE_INFO_SYN;
			msg.dwObjectID = HEROID;
			msg.nData = FORTWARWAREIDX_1;
			NETWORK->Send(&msg, sizeof(msg));

			if( !GAMEIN->GetInventoryDialog()->IsActive() )
				GAMEIN->GetInventoryDialog()->SetActive(TRUE);
		}
		else if(dwData == 6)			// 2요새창고
		{
			MSG_INT msg;
			msg.Category = MP_FORTWAR;
			msg.Protocol = MP_FORTWAR_WAREHOUSE_INFO_SYN;
			msg.dwObjectID = HEROID;
			msg.nData = FORTWARWAREIDX_2;
			NETWORK->Send(&msg, sizeof(msg));

			if( !GAMEIN->GetInventoryDialog()->IsActive() )
				GAMEIN->GetInventoryDialog()->SetActive(TRUE);
		}
		else if(dwData == 7)			// 3요새창고
		{
			MSG_INT msg;
			msg.Category = MP_FORTWAR;
			msg.Protocol = MP_FORTWAR_WAREHOUSE_INFO_SYN;
			msg.dwObjectID = HEROID;
			msg.nData = FORTWARWAREIDX_3;
			NETWORK->Send(&msg, sizeof(msg));

			if( !GAMEIN->GetInventoryDialog()->IsActive() )
				GAMEIN->GetInventoryDialog()->SetActive(TRUE);
		}
	}
	//SW060920 문파배틀시 창고소환주문서 사용불가로 인한 위치 수정.// NpcJob을 초기화하지 않기에 위치변경으로 처리.
	else if( m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == MUNPA_ROLE)
	{
		switch( dwData )
		{
		case 0:
			{
				if(HERO->GetGuildIdx())
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(28));
					return;
				}
				if((ABILITYMGR->GetAbilityLevel(ABILITYINDEX_LEADERSHIP, HERO->GetAbilityGroup()) < 1) || (HERO->GetLevel() < GUILD_CREATELEVEL))
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(41));
					return;
				}
				if(GUILDMGR->CanEntryGuild(HERO->GetGuildEntryDate()) == FALSE)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(59), HERO->GetGuildEntryDate());
					return;
				}
				cDialog* pDlg = (cDialog*)GAMEIN->GetGuildCreateDlg();
				pDlg->SetActive(TRUE);
			}
			break;
		case 1:
			{
				if(HERO->GetGuildIdx() == 0)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(35));
					return;
				}
				else if(HERO->GetGuildMemberRank() == GUILD_MASTER)
				{
					WINDOWMGR->MsgBox( MBI_GUILD_BREAKUP, MBT_YESNO, CHATMGR->GetChatMsg( 302 ) );
				}
				else
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(726));
					return;
				}
			}
			break;
		case 2:
			{
				if(HERO->GetGuildIdx() == 0)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(35));
					return;
				}
				else if(HERO->GetGuildMemberRank() != GUILD_MASTER)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(726));
					return;
				}
				else
				{
					GAMEIN->GetGuildLevelUpDlg()->SetActive(TRUE);
				}	
			}
			break;
		case 3:
			{
				if(GUILDMGR->GetGuildLevel() < GUILD_2LEVEL)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(723));
					return;
				}
				if(HERO->GetGuildIdx() && (HERO->GetGuildMemberRank() == GUILD_MASTER))
				{
					GAMEIN->GetGuildMarkDlg()->ShowGuildMark();
				}
				else
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(113));
					return;
				}	
			}
			break;
		case 4:		// create guildunion
			{
				if( GUILDUNION->CheckGuildUnionCondition( 0 ) )
					GAMEIN->GetGuildUnionCreateDlg()->SetActive( TRUE );
				else
					return;
			}
			break;
		case 5:		// destroy guildunion
			{
				if( GUILDUNION->CheckGuildUnionCondition( 1 ) )
					WINDOWMGR->MsgBox( MBI_GUILD_UNION_DESTROY, MBT_YESNO, CHATMGR->GetChatMsg( 1126 ) );
				else
					return;
			}
			break;
		case 6:		// regist guildunion mark
			{
				if( GUILDUNION->CheckGuildUnionCondition( 5 ) )
					GAMEIN->GetGuildMarkDlg()->ShowGuildUnionMark();
				else
					return;
			}
			break;
		case 7:		// seigewar time regist
			{
				if(HERO->GetGuildIdx() == 0)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(35));
					return;
				}
				else if(HERO->GetGuildMemberRank() != GUILD_MASTER)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(726));
					return;
				}
				GAMEIN->GetSWTimeRegDlg()->SetActive( TRUE );
			}
			break;
		case 8:		// seigewar protectregist
			{
				if(HERO->GetGuildIdx() == 0)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(35));
					return;
				}
				else if(HERO->GetGuildMemberRank() != GUILD_MASTER)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(726));
					return;
				}

				MSGBASE Msg;
				Msg.Category = MP_SIEGEWAR;
				Msg.Protocol = MP_SIEGEWAR_PROPOSALLIST_SYN;
				Msg.dwObjectID = HEROID;
				NETWORK->Send( &Msg, sizeof(Msg) );
			}
			break;
		case 9:		// 공성전 관전하기
			{	
				WINDOWMGR->MsgBox( MBI_SW_OBSERVER_SYN, MBT_YESNO, CHATMGR->GetChatMsg(1146) );
				return;
			}
			break;
		case 10:	// seigewar info
			{
				MSGBASE Msg;
				Msg.Category = MP_SIEGEWAR;
				Msg.Protocol = MP_SIEGEWAR_GUILDLIST_SYN;
				Msg.dwObjectID = HEROID;
				NETWORK->Send( &Msg, sizeof(Msg) );
			}
			break;
		case 11:	// enter deffense team
			{
				if(HERO->GetGuildIdx() == 0)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(35));
					return;
				}

				MSG_DWORD2 Msg;
				Msg.Category = MP_SIEGEWAR;
				Msg.Protocol =  MP_SIEGEWAR_MOVEIN_SYN;
				Msg.dwObjectID = HEROID;
				Msg.dwData1 = HERO->GetGuildIdx();
				Msg.dwData2 = 0;
				NETWORK->Send( &Msg, sizeof(Msg) );		

				return;
			}
			break;
		case 12:	// enter offense team
			{
				if(HERO->GetGuildIdx() == 0)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(35));
					return;
				}

				MSG_DWORD2 Msg;
				Msg.Category = MP_SIEGEWAR;
				Msg.Protocol =  MP_SIEGEWAR_MOVEIN_SYN;
				Msg.dwObjectID = HEROID;
				Msg.dwData1 = HERO->GetGuildIdx();
				Msg.dwData2 = 1;
				NETWORK->Send( &Msg, sizeof(Msg) );	

				return;
			}
			break;
		case 13:	// 수성 참여신청
			{
				if(HERO->GetGuildIdx() == 0)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(35));
					return;
				}
				else if(HERO->GetGuildMemberRank() != GUILD_MASTER)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(726));
					return;
				}

				WINDOWMGR->MsgBox( MBI_SW_PROPOSAL_SYN, MBT_YESNO, CHATMGR->GetChatMsg(1058) );
				return;
			}
			break;
		case 14:	// 공성 참여신청
			{
				if(HERO->GetGuildIdx() == 0)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(35));
					return;
				}
				else if(HERO->GetGuildMemberRank() != GUILD_MASTER)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(726));
					return;
				}

				WINDOWMGR->MsgBox( MBI_SW_ATTACKREGIST_SYN, MBT_YESNO, CHATMGR->GetChatMsg(1059) );		
				return;
			}
			break;
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
			{
				if(HERO->GetGuildIdx() == 0)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(35));
					return;
				}
				if(HERO->GetGuildMemberRank() != GUILD_MASTER)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(726));
					return;
				}

				if( GUILDMGR->CheckChangeLocation( dwData ) )
				{
					// magi82(37) 맵 속성 데이터
					WINDOWMGR->MsgBox( MBI_GUILD_CHANGE_LOCATION, MBT_YESNO, CHATMGR->GetChatMsg(1200), MAP->GetMapName( GUILDMGR->GetTempLocation() ) );
					//					return;
				}
				else
					return;
			}
			break;
		case 20:
			{
				MSGBASE Msg;
				Msg.Category = MP_SIEGEWAR;
				Msg.Protocol = MP_SIEGEWAR_TIMEINFO_SYN;
				Msg.dwObjectID = HEROID;
				NETWORK->Send( &Msg, sizeof(Msg) );				
			}
			break;
		case 21:			// 문하생 -> 문파소개글 등록
			{
				if( HERO->GetGuildMemberRank() != GUILD_MASTER )
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(726));
					return;
				}

				if( GUILDMGR->GetGuild()->GetLevel() < MAX_GUILD_LEVEL )
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(723));
					return;
				}

				MSG_DWORD msg;
				ZeroMemory( &msg, sizeof(MSG_DWORD) );
				SetProtocol( &msg, MP_GUILD, MP_GUILD_MUNPA_REGIST_SYN );
				msg.dwObjectID = HEROID;
				msg.dwData = HERO->GetCharacterTotalInfo()->MunpaID;
				NETWORK->Send( &msg, sizeof(msg) );

				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState( HERO,eObjectState_Deal );
			}
			break;
		case 22:			// 문하생 -> 문파소개글 수정
			{
				if( HERO->GetGuildMemberRank() != GUILD_MASTER )
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(726));
					return;
				}

				//GAMEIN->GetGuildCreateDlg()->SetGuildName();
				//GAMEIN->GetGuildCreateDlg()->GetGuildIntro();
				GAMEIN->GetGuildCreateDlg()->SetActive( TRUE );
			}
			break;
		case 23:		// 문하생 -> 문파소개창
			{
				// magi82 - 문하생 관련(서버에서 메세지 받은다음에 다이얼로그창 액티브 070119
				GAMEIN->GetGuildTraineeDialog()->SendPage(1, TRUE);
			}
			break;
		case 24:		// 문하생 -> 문파소개글 취소
			{
				if( HERO->GetGuildMemberRank() != GUILD_MASTER )
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(726));
					return;
				}

				WINDOWMGR->MsgBox( MBI_MUNPADELETESEL, MBT_YESNO, CHATMGR->GetChatMsg(1493) );	// magi82 - 문하생관련(070124)

				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState( HERO,eObjectState_Deal );
			}
			break;
		}
	}
	else if( m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == WANTED_ROLE)
	{
		//전에 클릭한게 늦게올 수도 있네
		if(GAMEIN->GetWantedDialog()->IsActive() == TRUE)
			return;
		if(dwData == 0) //창고 보기
			WANTEDMGR->SortWantedListSyn(1);
		else if(dwData == 1)
			GAMEIN->GetBailDialog()->Open();
		//
	}
	else if( m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == SURYUN_ROLE)
	{
		if( dwData == eSuryunNpc_Suryun )		// 수련하기
		{
			// magi82 - Titan(070612)
			if( TITANMGR->GetCurRidingTitan() == NULL )
			{
				GAMEIN->GetMPRegistDialog()->SetActive(TRUE);
				if(GAMEIN->GetMugongSuryunDialog()->IsActive() == FALSE)
					GAMEIN->GetMugongSuryunDialog()->SetActive(TRUE);
			}
			else
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1557));

				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
			}
		}
		else if( dwData == eSuryunNpc_About_GuildTournament )		// 문파토너먼트 알아보기
		{
			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
		}
		else if( dwData == eSuryunNpc_Regist_GuildTournament )		// 문파토너먼트 참가신청하기
		{
			MSGBASE msg;
			msg.Category = MP_GTOURNAMENT;
			msg.Protocol = MP_GTOURNAMENT_REGISTCOUNT;
			msg.dwObjectID = HEROID;
			NETWORK->Send( &msg, sizeof(msg) );
		}
		else if( dwData == eSuryunNpc_Cancel_GuildTournament )		// 문파토너먼트 신청 취소하기
		{
			if( GAMEIN->GetGTRegistcancelDlg() )
				GAMEIN->GetGTRegistcancelDlg()->SetActive( TRUE );
		}
		else if( dwData == eSuryunNpc_Show_GuildStand )				// 문파토너먼트 대진표 보기
		{
			MSG_WORD msg;
			msg.Category = MP_GTOURNAMENT;
			msg.Protocol = MP_GTOURNAMENT_STANDINGINFO_SYN;
			msg.wData = 1;
			msg.dwObjectID = HEROID;
			NETWORK->Send( &msg, sizeof(msg) );
		}
		else if( dwData == eSuryunNpc_Enter_GuildTournament )		// 문파토너먼트 입장하기
		{
			MSG_DWORD msg;
			msg.Category = MP_GTOURNAMENT;
			msg.Protocol = MP_GTOURNAMENT_MOVETOBATTLEMAP_SYN;
			msg.dwObjectID = HEROID;
			msg.dwData = 0;
			NETWORK->Send( &msg, sizeof(msg) );
		}
		else if( dwData == eSuryunNpc_EnterObserver_GuildTournament )	// 관람모드로 문파토너먼트 입장하기
		{
			GAMEIN->SetGuildIdx( HERO->GetGuildIdx() );

			MSGBASE msg;
			msg.Category = MP_GTOURNAMENT;
			msg.Protocol = MP_GTOURNAMENT_BATTLEINFO_SYN;
			msg.dwObjectID = HEROID;
			NETWORK->Send( &msg, sizeof(msg) );

			//if( GAMEIN->GetGTBattleListDlg() )
			//	GAMEIN->GetGTBattleListDlg()->SetActive( TRUE );
		}
		/////////////////////////////////////////////////////////////////////
		// 06. 06. 2차 전직 - 이영준
		// 무공 변환
		else if( dwData == eSuryunNpc_SkillOption )
		{
			GAMEIN->GetSkillOptionChangeDlg()->SetActive(TRUE);

			if(GAMEIN->GetMugongSuryunDialog()->IsActive() == FALSE)
				GAMEIN->GetMugongSuryunDialog()->SetActive(TRUE);
			if(GAMEIN->GetInventoryDialog()->IsActive() == FALSE)
				GAMEIN->GetInventoryDialog()->SetActive(TRUE);
		}
		/////////////////////////////////////////////////////////////////////
	}
	else if(m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == MAPCHANGE_ROLE )
	{
		if(dwData == 0)
		{
			MSG_DWORD msg;
			msg.Category = MP_NPC;
			msg.Protocol = MP_NPC_DOJOB_SYN;
			msg.dwObjectID = HEROID;
			msg.dwData = m_pCurSelectedNpc->GetID();
			NETWORK->Send(&msg, sizeof(msg));
		}
		else
		{
			if(GAMEIN->GetNpcScriptDialog()->IsActive())
			{
				NPCSCRIPTMGR->SetSelectedNpc(NULL);
				GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);						
			}

			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
		}
	}
	else if( m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == SYMBOL_ROLE )
	{
		// 각인시작		
		if( HERO->GetGuildMemberRank() == GUILD_MASTER )
		{
			CGuild* pGuild = GUILDMGR->GetGuild();
			if( pGuild && pGuild->GetLevel() == 5 )
			{
				MSG_DWORD msg;
				SetProtocol( &msg, MP_SIEGEWAR, MP_SIEGEWAR_ENGRAVE_SYN );
				msg.dwObjectID = HEROID;
				msg.dwData = HERO->GetGuildIdx();
				NETWORK->Send( &msg, sizeof(msg) );
			}
			else
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1011) );
		}
		else
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1010) );
	}
	else if( m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == TALKER_ROLE )
	{

	}
	else if( m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == GUIDE_ROLE )
	{
		//magi82 - 문하생 가입편리시스템		
		switch( dwData )
		{
		case 0:	// 초보자 관리인의 문하생 지원창
			{
				// magi82 - 문하생 관련(서버에서 메세지 받은다음에 다이얼로그창 액티브 070119
				GAMEIN->GetGuildTraineeDialog()->SendPage(1, TRUE);
			}
			break;
		default:
			break;
		}
	}
	// magi82 - Titan(070323)
	else if( m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == TITAN_ROLE )
	{
		switch( dwData )
		{
		case eTITAN_NPC_BuyItem:	// 타이탄 아이템 구매
			{
				MSG_WORD msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_DEALER_SYN;
				msg.dwObjectID = HEROID;
				msg.wData = m_pCurSelectedNpc->GetNpcUniqueIdx();
				NETWORK->Send(&msg, sizeof(msg));
			}
			break;
		case eTITAN_NPC_TitanMix:	// 타이탄 제조
			{
				if( TITANMGR->GetCurRidingTitan() == NULL )
				{
					GAMEIN->GetTitanMixDlg()->SetActive( TRUE );
				}
				else
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1557));

					if( HERO->GetState() == eObjectState_Deal )
						OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
				}
			}
			break;
		case eTITAN_NPC_TitanUpgrade:	// 타이탄 등급업
			{
				if( TITANMGR->GetCurRidingTitan() == NULL )
				{
					GAMEIN->GetTitanUpgradeDlg()->SetActive( TRUE );
				}
				else
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1557));

					if( HERO->GetState() == eObjectState_Deal )
						OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
				}
			}
			break;
		case eTITAN_NPC_TitanPartsMake:	// 타이탄 파츠 제작(부품조합)
			{
				GAMEIN->GetTitanPartsMakeDlg()->SetActive( TRUE );
			}
			break;
		case eTITAN_NPC_TitanPartsChange:	// 타이탄 아이템 체인지
			{
				if(TITANMGR->GetRegistedTitanItemDBIdx())
				{
					if( TITANMGR->GetCurRidingTitan() == NULL )
					{						
						GAMEIN->GetTitanRepairDlg()->SetActive( TRUE );
						GAMEIN->GetInventoryDialog()->SetActive( TRUE );
						GAMEIN->GetTitanInventoryDlg()->SetActive( TRUE );
					}
					else
					{
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1557));

						if( HERO->GetState() == eObjectState_Deal )
							OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
					}
				}
				else
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1510) );

					if( HERO->GetState() == eObjectState_Deal )
						OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
				}
			}
			break;
		case eTITAN_NPC_TitanBreak:	// 타이탄 아이템 분해
			{
				GAMEIN->GetTitanBreakDlg()->SetActive( TRUE );
			}
			break;
		case eTITAN_NPC_TitanRegister:	// 타이탄 등록
			{
				if( TITANMGR->GetCurRidingTitan() == NULL )
				{
					GAMEIN->GetTitanRegisterDlg()->SetActive( TRUE );
				}
				else
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1557));

					if( HERO->GetState() == eObjectState_Deal )
						OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
				}
			}
			break;
		case eTITAN_NPC_TitanDissolution:	// 타이탄 해제
			{
				if( TITANMGR->GetCurRidingTitan() == NULL )
				{
					GAMEIN->GetTitanDissolutionDlg()->SetActive( TRUE );
				}
				else
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1557));

					if( HERO->GetState() == eObjectState_Deal )
						OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
				}
			}
			break;
			// magi82 - Titan(070611) 타이탄 무공변환 주석처리
			/*
		case eTITAN_NPC_TitanMugongMix:	// 타이탄 무공 조합
			{
				GAMEIN->GetTitanMugongMixDlg()->SetActive( TRUE );
			}
			break;
			*/
		default:
			break;
		}
	}
	else
	{
		ASSERT(0);
		if( HERO->GetState() == eObjectState_Deal )
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
		return;
	}

	GAMEIN->GetNpcScriptDialog()->DisActiveEx();
	//	GAMEIN->GetNpcScriptDialog()->EndDialog();
}
