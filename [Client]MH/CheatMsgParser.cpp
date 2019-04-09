// CheatMsgParser.cpp: implementation of the CCheatMsgParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChatManager.h"
#include "CheatMsgParser.h"
#include "mhnetwork.h"
#include "objectmanager.h"
#include "Movemanager.h"
#include "ItemManager.h"
#include "DebugDlg.h"
#include "GameIn.h"
#include "MAINGAME.h"
#include "StatsCalcManager.h"
#include "PeaceWarModeManager.h"
#include "ObjectStateManager.h"
#include "./Audio/MHAudioManager.h"
#include "FilteringTable.h"
#ifdef _GMTOOL_
#include "GMToolManager.h"
#endif

#include "mhfile.h"

#include "InventoryExDialog.h"
#include "WindowIdEnum.h"
#include "./Interface/cWindowManager.h"
#include "cDivideBox.h"
#include "QuickManager.h"
#include "UserInfoManager.h"

#include "MapChange.h"
#include "EventMapInfo.h"
#include "GuildFieldWar.h"
#include "GMNotifyManager.h"
#include "EventNotifyDialog.h"

#include "GuildFieldWarDialog.h"
#include "PartyWar.h"
#include "GuildUnion.h"

#include "cJackpotManager.h"
#include "MHMap.h"

#ifdef _GMTOOL_
#include "MHCamera.h"
#endif

#include "CharacterDialog.h"
#include "SeigeWarDialog.h"
#include "SiegeWarMgr.h"

#include "PetManager.h"
#include "MugongDialog.h"

#include "SkillOptionChangeDlg.h"
#include "SkillOptionClearDlg.h"

#include "GuildManager.h"
#include "BobusangManager.h"
#include "AutoNoteDlg.h"
#include "AutoAnswerDlg.h"
#include "ChannelDialog.h"
#include "FortWarDialog.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CCheatMsgParser)

#ifdef _CHEATENABLE_

BOOL CheatFunc(char* cheat)
{
	char code[256];
	char buf[256] = {0,};
	sscanf(cheat,"%s",code);
	strupr(code);

//	//KES CHEAT!!!
/*
	if(strcmp(code,"MAPNAME")==0)
	{
		if( strcmp( USERINFOMGR->GetUserID(), "mkteam2" ) != 0 
			&& strcmp( USERINFOMGR->GetUserID(), "MKTEAM2" ) != 0
			&& strcmp( USERINFOMGR->GetUserID(), "andyamy" ) != 0 )///
			return FALSE;

		//MAPTYPE mapnum;
		sscanf(cheat,"%s %s",code,buf);

		// magi82(37) 맵 속성 데이터
		WORD wMapNum = MAP->GetMapNumForName( buf );
		//WORD mapnum = GetMapNumForName( buf );

		if( mapnum == 0 )
			return TRUE;

		MSG_NAME_DWORD2 msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_CHANGEMAP_SYN;
		msg.dwObjectID	= HEROID;
		msg.dwData1		= (DWORD)mapnum;
		msg.dwData2		= (DWORD)gChannelNum;
		msg.Name[0]		= 0;		
		NETWORK->Send( &msg, sizeof(msg) );

		return TRUE;
	}
//
*/
#ifdef _GMTOOL_
#ifndef _GMTOOL_SIMPLE_
	if( strcmp(code,"MONEYBEGETSMONEY") == 0 )
	{
		CHEATMGR->SetCheatEnable( TRUE );
		
		CHATMGR->AddMsg( CTC_CHEAT_1, "01001010010101110100101010101010010010011" );
		CHATMGR->AddMsg( CTC_CHEAT_1, "01001 [ Welcome To the Real World ] 11011" );
		CHATMGR->AddMsg( CTC_CHEAT_1, "11001101010100111010100001010010101111011" );
		
		return TRUE;
	}

	if( !CHEATMGR->IsCheatEnable() ) return FALSE;


	if(strcmp(code,"===")==0)
	{
		CObject* pObj = OBJECTMGR->GetSelectedObject();
		if( pObj == NULL ) return TRUE;

		if( !( pObj->GetObjectKind() & eObjectKind_Monster ) &&
			( pObj->GetObjectKind() != eObjectKind_Npc ) )
			return TRUE;

		if( strlen(cheat) > 4 )
		{
			TESTMSGID msg;
			msg.Category = MP_CHEAT;
			msg.Protocol = MP_CHEAT_MOB_CHAT;
			msg.dwSenderID = pObj->GetID();
			SafeStrCpy( msg.Msg, cheat+4, MAX_CHAT_LENGTH+1 );
			NETWORK->Send(&msg,msg.GetMsgLength());
		}

		return TRUE;
	}
	else if(strcmp(code,"CAMERAMODE")==0)
	{
		int onoff;
		sscanf(cheat,"%s %d",code,&onoff);

		if(onoff)
			HERO->GetEngineObject()->Show();
		else
			HERO->GetEngineObject()->HideWithScheduling();

		MSG_DWORD2 msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_SETVISIBLE;
		msg.dwObjectID = HEROID;
		msg.dwData1 = HEROID;
		msg.dwData2 = (DWORD)onoff;
		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}
	else if(strcmp(code, "JPMONEY")==0)
	{
		DWORD money;
		sscanf(cheat, "%s %d", code, &money);

		if( !money || money > 999999999 ) return FALSE;

		JACKPOTMGR->SetJPTotalMoneyTest(money);
//		MSG_DWORD msg;
//		msg.Category = MP_JACKPOT;
//		msg.Protocol = MP_JACKPOT_CHEAT_MAPTOTALMONEY;
//		msg.dwObjectID = HEROID;
//		msg.dwData = HEROID;
//		NETWORK->Send(&Msg,sizeof(msg));
		return TRUE;
	}

	else if(strcmp(code, "WANAJACKPOT")==0)
	{
//		char Name[MAX_NAME_LENGTH+1];
		DWORD	PrizeKind = 0;
		sscanf(cheat, "%s %d", code, &PrizeKind);

		if( 1 > PrizeKind || PrizeKind > 3 ) return FALSE;
		/*
		//SW050802
		//사용자를 위한.. 내부적으로 등수는 0, 1, 2 순
		*/
		PrizeKind--;

		MSG_DWORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_JACKPOT_GETPRIZE;
		msg.dwObjectID = HEROID;
		msg.dwData = PrizeKind;

		NETWORK->Send(&msg,sizeof(msg));

		return TRUE;
	}

	else if(strcmp(code, "ONOFFJACKPOT")==0)
	{
		DWORD	bOn = TRUE;
		sscanf(cheat, "%s %d", code, &bOn);

		if( !(bOn == TRUE || bOn == FALSE) ) return FALSE;

		MSG_DWORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_JACKPOT_ONOFF;
		msg.dwObjectID = HEROID;
		msg.dwData = bOn;

		NETWORK->Send(&msg,sizeof(msg));

		char* arS[2] = {"OFF", "ON"};

		CHATMGR->AddMsg( CTC_CHEAT_5, "JACKPOT ACTIVATION %s", arS[bOn] );

		return TRUE;
	}
	
	else if(strcmp(code, "JPMONEYPERMONSTER") == 0 )
	{
		DWORD	ChipsPerMon = 0;
		sscanf(cheat, "%s %d", code, &ChipsPerMon);
		
		if( ChipsPerMon < 0 || ChipsPerMon > 1000 ) return FALSE;

		MSG_DWORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_JACKPOT_MONEYPERMONSTER;
		msg.dwObjectID = HEROID;
		msg.dwData = ChipsPerMon;

		NETWORK->Send(&msg,sizeof(msg));

		CHATMGR->AddMsg( CTC_CHEAT_5, "SET JP_CHIP AS %d", ChipsPerMon );

		return TRUE;
	}
	//SW050809 강PM님 지시
	else if(strcmp(code, "JACKPOTSETTING") == 0 )
	{
		DWORD PrizeKind = 0;
		DWORD PrizeProb = 0;
		DWORD PrizePercent = 0;

		sscanf(cheat, "%s %d %d %d", code, &PrizeKind, &PrizeProb, &PrizePercent);

		if( PrizeKind < 1 || PrizeKind > 3 ) return FALSE;
		if( PrizeProb < 0 || PrizeProb > 100000000 ) return FALSE;
		if( PrizePercent < 0 || PrizePercent > 100 ) return FALSE;

		MSG_DWORD3 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_JACKPOT_PROBABILITY;
		msg.dwObjectID = HEROID;
		msg.dwData1 = PrizeKind - 1;	// 내부적으로 1등은 0...
		msg.dwData2 = PrizeProb;
		msg.dwData3 = PrizePercent;

		NETWORK->Send(&msg,sizeof(msg));

		CHATMGR->AddMsg( CTC_CHEAT_5, "SET CHANGED...OK" );

		return TRUE;
	}
	// 강PM님 지시 프로토콜 줄이기
	else if(strcmp(code, "JACKPOTCONTROL") == 0 )
	{
		char Dum[128] = {0,};
		DWORD OderFlag = 0;

		sscanf(cheat, "%s %s %d", code, Dum, &OderFlag);
		strupr(Dum);

		MSG_DWORD2 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_JACKPOT_CONTROL;
		msg.dwObjectID = HEROID;

		enum {TIMELENGTH, };	//CommonHeader 로~

		if(strcmp(Dum, "TIME") == 0 )	//Agent에 세팅
		{
			if( OderFlag < 0 || OderFlag > 3600 )	// sec 단위
				return FALSE;

			msg.dwData1 = TIMELENGTH;
			msg.dwData2 = OderFlag * 1000;

		}
		else
		{
			return FALSE;
		}

		NETWORK->Send(&msg,sizeof(msg));

		CHATMGR->AddMsg( CTC_CHEAT_5, "DO AS CONTROL...OK" );

		return TRUE;
	}
	//SW051011 강화/레어 치트
	else if(strcmp(code, "ITEMREADY")==0)
	{
		char Dum[128] = {0,};
		sscanf(cheat, "%s %s", code, Dum);
		strupr(Dum);
		if(strcmp(Dum, "THIS") == 0)	//강화 아이템 만들기
		{
			//클릭 선택된 인벤의 아이템 Idx, Pos 를 가져온다.
			CHEATMGR->m_wItemOptionKind = CCheatMsgParser::eKindOption_Reinforce;	//기존 아이템은 레어로 만들 수 없다.
			CHATMGR->AddMsg(CTC_CHEAT_2, "Now Open ReinforceDialog And Drop The Target Item in The Dialog, Keep Going");
		}
		else	//레어아이템 생성하기
		{
			//숫자화 하여 Idx 에 세팅한다.
			CHEATMGR->m_wTargetItemIdx = atoi(Dum);
			ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(atoi(Dum));
			if( !pItemInfo )
			{
				CHATMGR->AddMsg(CTC_CHEAT_2, "Your ItemIndex Number is Not Using, Check it Again");
				return TRUE;
			}
			else
			{
				CHATMGR->AddMsg(CTC_CHEAT_2, "TargetItem's Name is [%s]", pItemInfo->ItemName);
			}
			CHEATMGR->m_wItemOptionKind = CCheatMsgParser::eKindOption_Rare;
		}
		memset(&CHEATMGR->m_OptionInfo, 0, sizeof(ITEM_OPTION_INFO));	//초기화 꼭 해주어야한다.
		CHEATMGR->m_bReadyToOptionCheat = TRUE;	//이 작업을 기본으로 옵션 수치 입력을 받는다.
		
		return TRUE;
	}
	else if(strcmp(code, "OPT") == 0)
	{
		if(!CHEATMGR->m_bReadyToOptionCheat)
		{
			//"ITEMREADY 명령이 잘못되었습니다."
			CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : Invalid Input On ITEMREADY !!");
			return TRUE;
		}
		if( CHEATMGR->m_wItemOptionKind == CCheatMsgParser::eKindOption_Reinforce && !CHEATMGR->m_wTargetItemIdx )
		{
			//"강화할 아이템 등록이 안됨. 강화창을 열고 강화할 아이템을 올려 놓으세요"
			CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : Open ReinforceDialog And Then Drop The Target Item in It");
			return TRUE;
		}
		DWORD OptionValue = 0;
		sscanf(cheat, "%s %s %d", code, buf, &OptionValue);

		strupr(buf);
		if( OptionValue > 1000 )
		{
			//"제한 값은 1000 입니다."
			CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : Value Limit is 1000");
			return TRUE;
		}
//#define MAX_OPTION_KIND	19
//		char* pOptionName[MAX_OPTION_KIND] = {"G", "M", "C", "S", "CR", "NR", "HS", "MK", "MB", "CT", "MA", "HA", "TA", "GA", "SA", "MD", "HD", "TD", "GD", "SD"};

		if( strcmp(buf,"G") == 0 )
			CHEATMGR->m_OptionInfo.GenGol = (WORD)OptionValue;
		else if( strcmp(buf,"M") == 0 )
			CHEATMGR->m_OptionInfo.MinChub = (WORD)OptionValue;
		else if( strcmp(buf,"C") == 0 )
			CHEATMGR->m_OptionInfo.CheRyuk = (WORD)OptionValue;
		else if( strcmp(buf,"S") == 0 )
			CHEATMGR->m_OptionInfo.SimMek = (WORD)OptionValue;
		else if( strcmp(buf,"CR") == 0 )
			CHEATMGR->m_OptionInfo.Life = OptionValue;
		else if( strcmp(buf,"NR") == 0 )
			CHEATMGR->m_OptionInfo.NaeRyuk = (WORD)OptionValue;
		else if( strcmp(buf,"HS") == 0 )
			CHEATMGR->m_OptionInfo.Shield = OptionValue;
		else if( strcmp(buf,"PA") == 0 )
			CHEATMGR->m_OptionInfo.PhyAttack = (WORD)OptionValue;
		else if( strcmp(buf,"PD") == 0 )
			CHEATMGR->m_OptionInfo.PhyDefense = (WORD)OptionValue;
		else if( strcmp(buf,"CT") == 0 )
		{
			if(CHEATMGR->m_wItemOptionKind == CCheatMsgParser::eKindOption_Rare)
			{
			//"RareItem Do not have Critical Option"
				CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : RareItem Do not have Critical Option");
				return TRUE;
			}
			CHEATMGR->m_OptionInfo.CriticalPercent = (WORD)OptionValue;
		}
		else if( strcmp(buf,"TA") == 0 )
			CHEATMGR->m_OptionInfo.AttrAttack.SetElement_Val(ATTR_TREE, OptionValue/100.f);
		else if( strcmp(buf,"FA") == 0 )
			CHEATMGR->m_OptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE, OptionValue/100.f);
		else if( strcmp(buf,"EA") == 0 )
			CHEATMGR->m_OptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH, OptionValue/100.f);
		else if( strcmp(buf,"IA") == 0 )
			CHEATMGR->m_OptionInfo.AttrAttack.SetElement_Val(ATTR_IRON, OptionValue/100.f);
		else if( strcmp(buf,"WA") == 0 )
			CHEATMGR->m_OptionInfo.AttrAttack.SetElement_Val(ATTR_WATER, OptionValue/100.f);
		else if( strcmp(buf,"TD") == 0 )
			CHEATMGR->m_OptionInfo.AttrRegist.SetElement_Val(ATTR_TREE, OptionValue/100.f);
		else if( strcmp(buf,"FD") == 0 )
			CHEATMGR->m_OptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE, OptionValue/100.f);
		else if( strcmp(buf,"ED") == 0 )
			CHEATMGR->m_OptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH, OptionValue/100.f);
		else if( strcmp(buf,"ID") == 0 )
			CHEATMGR->m_OptionInfo.AttrRegist.SetElement_Val(ATTR_IRON, OptionValue/100.f);
		else if( strcmp(buf,"WD") == 0 )
			CHEATMGR->m_OptionInfo.AttrRegist.SetElement_Val(ATTR_WATER, OptionValue/100.f);
		else
		{
			CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : Option is Invalid");
			return TRUE;
		}

		++CHEATMGR->m_OptionCount;

		return TRUE;
	}

	else if( strcmp(code, "CLEAROPTION") == 0 )
	{
		if(CHEATMGR->m_bReadyToOptionCheat)
			memset(&CHEATMGR->m_OptionInfo, 0, sizeof(ITEM_OPTION_INFO));
		else
		{
			//"do what?"
			CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : Nothing to Clear, Do it Step by Step");
			return TRUE;
		}

		CHEATMGR->m_OptionCount = 0;

		return TRUE;
	}

	else if( strcmp(code, "WISHITEM") == 0 )
	{
		if(!CHEATMGR->m_OptionCount)
		{
			CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : There is no Selected Option, Cheat OPTION Again" );
			return TRUE;
		}
		if(!CHEATMGR->m_bReadyToOptionCheat || !CHEATMGR->m_wItemOptionKind ) 
		{
			//"NOT ENOUGH PROGRESS, CLEAR AND DO AGAIN"
			CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : Not Enough Progress, Type CLEAR And Try Again");
			return TRUE;
		}
		SEND_CHEAT_ITEM_OPTION msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_ITEM_OPTION_SYN;
		msg.dwObjectID = HEROID;
		msg.bPosition = CHEATMGR->m_ItemAbsPos;
		msg.wItemIdx = CHEATMGR->m_wTargetItemIdx;
		msg.wOptionKind = CHEATMGR->m_wItemOptionKind;

		msg.OptionInfo = CHEATMGR->m_OptionInfo;
		NETWORK->Send(&msg,sizeof(msg));

		CHEATMGR->m_bReadyToOptionCheat = FALSE;
		CHEATMGR->m_wTargetItemIdx = 0;
		CHEATMGR->m_ItemAbsPos = 0;
		CHEATMGR->m_wItemOptionKind = CCheatMsgParser::eKindOption_None;

		return TRUE;
	}
	
	//SW051112 무쌍모드
	else if(strcmp(code, "MUSSANG")==0)
	{
		MSGBASE msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_MUSSANG_READY;
		msg.dwObjectID = HEROID;
		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}

	//SW051129 Pet
	else if(strcmp(code, "PETST")==0)
	{
		CPet* pPet = PETMGR->GetCurSummonPet();
		if(!pPet)
		{
			CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : No Summoned Pet, Cheat denied" );
			return TRUE;
		}

		char szBuf[128] = {0,};

		sscanf(cheat, "%s %s", code, szBuf);
		strupr(szBuf);

		MSG_BYTE msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_PET_STAMINA;
		msg.dwObjectID = HEROID;

		if(strcmp(szBuf, "MAX") == 0)
		{
			msg.bData = 1;
		}
		else if(strcmp(szBuf, "MIN") == 0)
		{
			msg.bData = 0;
		}
		else
		{
			CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : Incorrect Arg.. , Cheat denied" );
			return TRUE;
		}

		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}

	else if(strcmp(code, "PETFR")==0)
	{
		CPet* pPet = PETMGR->GetCurSummonPet();
		if(!pPet)
		{
			CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : No Summoned Pet, Cheat denied" );
			return TRUE;
		}

		DWORD dwNum = 0;
		sscanf(cheat, "%s %d", code, &dwNum);

		if( 0 > dwNum || dwNum > 100 )
		{
			CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : Value Limit is 0 to 100");
			return TRUE;
		}

		MSG_DWORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_PET_FRIENDSHIP_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData = dwNum * 100000;	//friend max is 10000000

		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}

	else if(strcmp(code, "CLSALFEH") == 0)
	{
		CObject* TObj = CHEATMGR->GetCheatTargetObject();
		if(TObj)
		{
			if(TObj->GetObjectKind() != eObjectKind_Pet)
			{
				CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : TargetKind is not a Pet");
				return TRUE;
			}
		}
		else
		{
			CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : NonTarget" );
			return TRUE;
		}

		DWORD dwNum = 0;
		sscanf(cheat, "%s %d", code, &dwNum);

		if( 0 > dwNum || dwNum > 100 )
		{
			CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : Value Limit is 0 to 100");
			return TRUE;
		}

		MSG_DWORD2 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_PET_SELECTED_FRIENDSHIP_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData1 = TObj->GetID();
		msg.dwData2 = dwNum * 100000;	//friend max is 10000000

		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}

	else if(strcmp(code,"PSK")==0)
	{
		CPet* pPet = PETMGR->GetCurSummonPet();
		if(!pPet)
		{
			CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : No Summoned Pet, Cheat denied" );
			return TRUE;
		}

		MSGBASE msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_PET_SKILLREADY_SYN;
		msg.dwObjectID = HEROID;

		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}

	//SW060719 문파포인트
	else if(strcmp(code, "ANSVH")==0)	//문포
	{
		DWORD PlayerID = 0;
		DWORD GuildIdx = 0;
		CObject* TObj = CHEATMGR->GetCheatTargetObject();
		if(TObj)
		{
			if(TObj->GetObjectKind() != eObjectKind_Player && TObj->GetObjectKind() != eObjectKind_Npc)
			{
				CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : TargetKind is not a Player");
				return TRUE;
			}

			if(TObj->GetObjectKind() == eObjectKind_Player)
			{
				PlayerID = ((CPlayer*)TObj)->GetID();
				GuildIdx = ((CPlayer*)TObj)->GetGuildIdx();
			}
			else if(TObj->GetObjectKind() == eObjectKind_Npc)
			{
				PlayerID = HEROID;
				GuildIdx = HERO->GetGuildIdx();
			}

			if( 0 == GuildIdx )
			{
				CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : TargetPlayer is not in a guild");
				return TRUE;
			}
		}
		else
		{
			CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : NonTarget" );
			return TRUE;
		}

		DWORD GuildPoint = 0;
		sscanf(cheat, "%s %d", code, &GuildPoint);

		MSG_DWORD3 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_GUILDPOINT_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData1 = PlayerID;
		msg.dwData2 = GuildIdx;
		msg.dwData3 = GuildPoint;

		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}

	//문파사냥몬스터
	else if(strcmp(code, "ANSAHQ")==0)	//문몹
	{
		DWORD PlayerID = 0;
		DWORD GuildIdx = 0;
		CObject* TObj = CHEATMGR->GetCheatTargetObject();
		if(TObj)
		{
			if(TObj->GetObjectKind() != eObjectKind_Player && TObj->GetObjectKind() != eObjectKind_Npc)
			{
				CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : TargetKind is not a Player");
				return TRUE;
			}

			if(TObj->GetObjectKind() == eObjectKind_Player)
			{
				PlayerID = ((CPlayer*)TObj)->GetID();
				GuildIdx = ((CPlayer*)TObj)->GetGuildIdx();
			}
			else if(TObj->GetObjectKind() == eObjectKind_Npc)
			{
				PlayerID = HEROID;
				GuildIdx = HERO->GetGuildIdx();
			}

			if( 0 == GuildIdx )
			{
				CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : TargetPlayer is not in a guild");
				return TRUE;
			}
		}
		else
		{
			CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : NonTarget" );
			return TRUE;
		}

		DWORD dwGuildMonsterCount = 0;
		sscanf(cheat, "%s %d", code, &dwGuildMonsterCount);

		MSG_DWORD3 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_GUILDHUNTED_MONSTERCOUNT_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData1 = PlayerID;
		msg.dwData2 = GuildIdx;
		msg.dwData3 = dwGuildMonsterCount;

		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;		
	}

//SW061019 서바이벌모드 
	else if( strcmp(code, "SURVIVAL")==0 )
	{
		sscanf(cheat, "%s %s", code, buf);
		strupr(buf);
		MSGBASE msg;
		msg.Category = MP_SURVIVAL;

		if( strcmp(buf, "START")==0 )
		{
			msg.Protocol = MP_SURVIVAL_READY_SYN;
			msg.dwObjectID = HEROID;
			NETWORK->Send(&msg,sizeof(msg));
			return TRUE;
		}
		else if( strcmp(buf, "STOP")==0 )
		{
			msg.Protocol = MP_SURVIVAL_STOP_SYN;
			msg.dwObjectID = HEROID;
			NETWORK->Send(&msg,sizeof(msg));
			return TRUE;
		}
	}
	else if( strcmp(code, "MAPOFF") == 0 )
	{
		MSGBASE msg;
		msg.Category = MP_SURVIVAL;
		msg.Protocol = MP_SURVIVAL_MAPOFF_SYN;
		msg.dwObjectID = HEROID;
		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}
	else if( strcmp(code, "SVITEM") == 0 )
	{
		DWORD limit = 0;
		sscanf(cheat, "%s %d", code, &limit);

		MSG_DWORD msg;
		msg.Category = MP_SURVIVAL;
		msg.Protocol = MP_SURVIVAL_ITEMUSINGCOUNT_SET;
		msg.dwObjectID = HEROID;
		msg.dwData = limit;
		NETWORK->Send(&msg, sizeof(msg));
		return TRUE;
	}
	//SW070127 타이탄
	else if( strcmp(code, "TFULL") == 0 )
	{
		MSG_DWORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_TITAN_FUEL_SPELL_MAX_SYN;
		msg.dwObjectID = HEROID;
		NETWORK->Send(&msg, sizeof(msg));
		return TRUE;
	}

	//SW070626 보부상NPC
	else if( strcmp(code, "BOBU") == 0 )
	{// 입력한 채널의 보부상 정보를 출력
		DWORD channelNum = 0;
		sscanf(cheat, "%s %d", code, &channelNum);

		//채널 숫자 유효 검색 ->agent 에서..
		MSG_DWORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_BOBUSANGINFO_REQUEST_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData = channelNum - 1;	//코드내에서는 0 부터..

		NETWORK->Send(&msg, sizeof(msg));
		return TRUE;
	}
	else if( strcmp(code, "BOBUEND") == 0 )
	{
		DWORD channelNum = 0;
		sscanf(cheat, "%s %d", code, &channelNum);

		//채널 숫자 유효 검색
		MSG_DWORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_BOBUSANG_LEAVE_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData = channelNum - 1;	//코드내에서는 0 부터..

		NETWORK->Send(&msg, sizeof(msg));
		return TRUE;
	}
	else if( strcmp(code, "BOBUREADY") == 0 )
	{
		BOBUSANGMGR->InitCheatData();

		DWORD channelNum = 0;
		sscanf(cheat, "%s %d", code, &channelNum);

		if( channelNum < 1 )
		{
			CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : Not a Avaliable Num. Check the input Num!");
			return TRUE;
		}

		BOBUSANGINFO* pInfo = BOBUSANGMGR->GetNewBobusangInfo();
		pInfo->AppearanceChannel = channelNum - 1;
		BOBUSANGMGR->SetValidity(BVD_CNL);

		BOBUSANGMGR->SetCheatReady(TRUE);

		BOBUSANGMGR->PrintInfo(pInfo);
		return TRUE;
	}
	else if( strcmp(code, "BOBUMAP") == 0 )
	{
		if(!BOBUSANGMGR->CheckCheatReady())
			return TRUE;

		DWORD mapNum = 0;
		sscanf(cheat, "%s %d", code, &mapNum);

		//!!! "bobusanginfo.bin" 유효맵번호 검사

		BOBUSANGINFO* pInfo = BOBUSANGMGR->GetNewBobusangInfo();
		pInfo->AppearanceMapNum = mapNum;
		BOBUSANGMGR->SetValidity(BVD_MAP);

		BOBUSANGMGR->PrintInfo(pInfo);

		return TRUE;
	}
	else if( strcmp(code, "BOBUPOS") == 0 )
	{
		if(!BOBUSANGMGR->CheckCheatReady())
			return TRUE;

		DWORD posIdx = 0;
		sscanf(cheat, "%s %d", code, &posIdx);

		//!!! "bobusanginfo.bin" 유효위치번호 검사
		if( posIdx < 1 || posIdx > BOBUSANG_POSNUM_MAX )
		{
			CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : Not a Avaliable Num. Check the input Num!");
			return TRUE;
		}

		BOBUSANGINFO* pInfo = BOBUSANGMGR->GetNewBobusangInfo();
		pInfo->AppearancePosIdx = posIdx - 1;
		BOBUSANGMGR->SetValidity(BVD_POS);

		BOBUSANGMGR->PrintInfo(pInfo);
		return TRUE;
	}
	else if( strcmp(code, "BOBUSELL") == 0 )
	{
		if(!BOBUSANGMGR->CheckCheatReady())
			return TRUE;

		DWORD SellIdx = 0;
		sscanf(cheat, "%s %d", code, &SellIdx);

		//!!! "bobusanginfo.bin" 유효위치번호 검사
		if( SellIdx < 1 || SellIdx > DEALITEM_BIN_TABNUM )
		{
			CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : Not a Avaliable Num. Check the input Num!");
			return TRUE;
		}
		BOBUSANGINFO* pInfo = BOBUSANGMGR->GetNewBobusangInfo();
		pInfo->SellingListIndex = SellIdx - 1;
		BOBUSANGMGR->SetValidity(BVD_SEL);

		BOBUSANGMGR->PrintInfo(pInfo);
		return TRUE;
	}
	else if( strcmp(code, "BOBUAPR") == 0 )
	{
		if(!BOBUSANGMGR->CheckCheatReady())
			return TRUE;

		DWORD y,m,d,h,n;
		sscanf(cheat, "%s %d %d %d %d %d", code, &y, &m, &d, &h, &n);

		stTIME curTime, interval, tmpTime;
		curTime.value = GetCurTime();
		interval.SetTime(0,0,0,0,5,0);
		curTime += interval;
		tmpTime.SetTime(y,m,d,h,n,0);

		if( curTime.value > tmpTime.value )
		{
			CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : Not a Avaliable Num. Check the input Num! And Need more than 5 min");
			return TRUE;
		}

		BOBUSANGINFO* pInfo = BOBUSANGMGR->GetNewBobusangInfo();
		pInfo->AppearanceTime = tmpTime.value;
		BOBUSANGMGR->SetValidity(BVD_APR);

		BOBUSANGMGR->PrintInfo(pInfo);
		return TRUE;
	}
	else if( strcmp(code, "BOBUDSAPR") == 0 )
	{
		if(!BOBUSANGMGR->CheckCheatReady())
			return TRUE;

		DWORD y,m,d,h,n;
		sscanf(cheat, "%s %d %d %d %d %d", code, &y, &m, &d, &h, &n);

		stTIME curTime, interval, tmpTime;
		curTime.value = GetCurTime();
		interval.SetTime(0,0,0,0,8,0);
		curTime += interval;
		tmpTime.SetTime(y,m,d,h,n,0);

		if( curTime.value > tmpTime.value )
		{
			CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : Not a Avaliable Num. Check the input Num! And Need more than 8 min");
			return TRUE;
		}

		BOBUSANGINFO* pInfo = BOBUSANGMGR->GetNewBobusangInfo();
		if( pInfo->AppearanceTime > tmpTime.value )	// 출현시각 보다 빠르면
		{
			CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : Not a Avaliable Num. Check the input Num! Appearance must be faster than Disappearance");
			return TRUE;
		}

		pInfo->DisappearanceTime = tmpTime.value;
		BOBUSANGMGR->SetValidity(BVD_DSAPR);

		BOBUSANGMGR->PrintInfo(pInfo);
		return TRUE;
	}
	else if( strcmp(code, "BOBUAUTO") == 0 )
	{
		if(!BOBUSANGMGR->CheckCheatReady())
			return TRUE;

		BOBUSANGINFO* pInfo = BOBUSANGMGR->GetNewBobusangInfo();
		pInfo->AppearanceChannel	= gChannelNum;
		pInfo->AppearanceMapNum		= MAP->GetMapNum();
		pInfo->AppearancePosIdx		= rand() % BOBUSANG_POSNUM_MAX;
		pInfo->SellingListIndex		= rand() % DEALITEM_BIN_TABNUM;

		stTIME curTime1, curTime2, fromTime, toTime;	// stTime 연산자 사용시 피연산자에는 모든 값이 유효해야한다.(순서)
		curTime1.value = curTime2.value = GetCurTime();
		fromTime.SetTime(0,0,0,0,1,0);
		toTime.SetTime(0,0,0,0,10,0);
		curTime1 += fromTime;
		curTime2 += toTime;

		pInfo->AppearanceTime		= curTime1.value;
		pInfo->DisappearanceTime	= curTime2.value;

		BOBUSANGMGR->SetValidity(BVD_ALL);

		BOBUSANGMGR->PrintInfo(pInfo);
		return TRUE;
	}
	else if( strcmp(code, "CLEARBOBU") == 0 )
	{
		BOBUSANGMGR->InitCheatData();
		return TRUE;
	}
	else if( strcmp(code, "BOBUGOGO") == 0 )
	{
		if(!BOBUSANGMGR->CheckCheatReady())
			return TRUE;

		int checkflg = BOBUSANGMGR->GetValidity();
		if( BVD_ALL != checkflg )
		{
			CHATMGR->AddMsg(CTC_CHEAT_2, "Warning : Not all info were set. Check your setting data");
			return TRUE;
		}

		MSG_BOBUSANG_INFO msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_BOBUSANGINFO_CHANGE_SYN;
		msg.dwObjectID = HEROID;
		msg.bobusangInfo = *(BOBUSANGMGR->GetNewBobusangInfo());

		BOBUSANGMGR->SetCheatReady(FALSE);

		NETWORK->Send(&msg, sizeof(msg));
		return TRUE;
	}
	else if( strcmp(code, "BOBUOFF") == 0 )
	{
		MSG_DWORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_BOBUSANG_ONOFF_SYN;
		msg.dwData = TRUE;
		msg.dwObjectID = HEROID;

		NETWORK->Send(&msg, sizeof(msg));
		return TRUE;
	}
	else if( strcmp(code, "BOBUON") == 0 )
	{
		MSG_DWORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_BOBUSANG_ONOFF_SYN;
		msg.dwData = FALSE;
		msg.dwObjectID = HEROID;

		NETWORK->Send(&msg, sizeof(msg));
		return TRUE;
	}
	else if(strcmp(code,"MOVE")==0)
	{
		float x,z;
		sscanf(cheat,"%s %f %f",code,&x,&z);

		VECTOR3 pos;
		pos.x = x;
		pos.y = 0;
		pos.z = z;

#ifdef _TESTCLIENT_
		MOVEMGR->SetPosition(HERO,&pos);
#else
		MOVE_POS msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_MOVE_SYN;
		msg.dwObjectID = HEROID;
		msg.dwMoverID = HEROID;
		msg.cpos.Compress(&pos);
		NETWORK->Send(&msg,sizeof(msg));
#endif
		return TRUE;
	}
	//2008. 5. 20. CBH - 맵 채널 분리 관련 치트 수정
	else if(strcmp(code,"MAP")==0)
	{
		int mapnum;
		int nChannel = 1;	//기본 1채널 셋팅
		sscanf(cheat,"%s %d %d",code,&mapnum, &nChannel);

		if(nChannel < 1)
			return FALSE;

		gChannelNum = nChannel-1;

		MSG_NAME_DWORD2 msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_CHANGEMAP_SYN;
		msg.dwObjectID	= HEROID;
		msg.dwData1		= (DWORD)mapnum;		
		msg.dwData2		= (DWORD)gChannelNum;
		msg.Name[0]		= 0;		
		NETWORK->Send( &msg, sizeof(msg) );

		return TRUE;
	}	
	else if(strcmp(code,"LIFE")==0)
	{		
		int life;
		sscanf(cheat,"%s %d",code,&life);

#ifdef _TESTCLIENT_
//		STATSMGR->Set_Life(HERO,life);
#else
		MSG_WORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_LIFE_SYN;
		msg.dwObjectID = HEROID;
		msg.wData = (WORD)life;
		NETWORK->Send(&msg,sizeof(msg));
#endif

		return TRUE;
	}	
	//2008. 6. 4. CBH - 악명치 치트
	/*else if(strcmp(code,"BADFAME")==0)
	{		
		int nValue;
		sscanf(cheat,"%s %d",code,&nValue);

		MSG_DWORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_BADFAME_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData = (DWORD)nValue;
		NETWORK->Send(&msg,sizeof(msg));

		return TRUE;
	}*/
	else if(strcmp(code,"RELOAD")==0)
	{
		MSGBASE msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_RELOADING;
		msg.dwObjectID = HEROID;
		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}
	else if(strcmp(code,"MP")==0)
	{		
		int mp;
		sscanf(cheat,"%s %d",code,&mp);

		MSG_WORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_MP_SYN;
		msg.dwObjectID = HEROID;
		msg.wData = (WORD)mp;
		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}
	else if(strcmp(code,"ITEM")==0)
	{
		int item;
		sscanf(cheat,"%s %d",code,&item);

		MSG_WORD2 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_ITEM_SYN;
		msg.dwObjectID = HEROID;
		msg.wData1 = (WORD)item;
		msg.wData2 = 1;
		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}
	else if(strcmp(code,"ITEMS")==0)
	{
		int item;
		int num;
		sscanf(cheat,"%s %d %d",code,&item, &num);

		MSG_WORD2 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_ITEM_SYN;
		msg.dwObjectID = HEROID;
		msg.wData1 = (WORD)item;
		msg.wData2 = (WORD)num;
		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}
	else if(strcmp(code, "DS") == 0)
	{
		CDebugDlg* pDbg = GAMEIN->GetDebugDlg();
		pDbg->SetActive(TRUE);
		return TRUE;
	}
	else if(strcmp(code, "DH") == 0)
	{
		CDebugDlg* pDbg = GAMEIN->GetDebugDlg();
		pDbg->SetActive(FALSE);
		return TRUE;
	}
	else if(strcmp(code, "MANA") == 0)
	{
		int mp;
		sscanf(cheat,"%s %d",code,&mp);

		MSG_WORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_MANA_SYN;
		msg.dwObjectID = HEROID;
		msg.wData = (WORD)mp;
		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;

	}
	/*
	else if(strcmp(code, "MUNPAFIELD_REQ") == 0)
	{
		DWORD munpaid;
		sscanf(cheat,"%s %d",code,&munpaid);

		MSG_DWORD msg;
		msg.Category = MP_BATTLE;
		msg.Protocol = MP_BATTLE_MUNPAFIELD_REQUEST_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData = munpaid;
		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}
	*/
	else if(strcmp(code, "SUNG") == 0)
	{
		int mugong = 0;
		int sung = 0;
		sscanf(cheat,"%s %d %d",code,&mugong, &sung);

		MSG_WORD2 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_MUGONGSUNG_SYN;
		msg.dwObjectID = HEROID;
		msg.wData1 = (WORD)mugong;
		msg.wData2 = (WORD)sung;
		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}
	else if(strcmp(code, "MONEY") == 0)
	{
		DWORD money;
		sscanf(cheat,"%s %d",code,&money);

		MSG_DWORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_MONEY_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData = money;
		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}
	else if(strcmp(code, "CLEARSTATE") == 0)
	{
		OBJECTSTATEMGR->EndObjectState(HERO, HERO->GetState());
		
		return TRUE;
	}
	else if(strcmp(code, "VOLUME") == 0)
	{
		float m,s,b;
		sscanf(cheat,"%s %f %f %f",code,&m,&s,&b);

		AUDIOMGR->SetMasterVolume(m);
		AUDIOMGR->SetSoundVolume(s);
		AUDIOMGR->SetBGMVolume(b);

		return TRUE;
	}
//murim-net test
	else if(strcmp(code, "MURIMNET") == 0)
	{
		MAINGAME->SetGameState( eGAMESTATE_MURIMNET );
		return TRUE;
	}
	else if(strcmp(code, "LEVELUP") == 0)
	{
		int val = 1;
		sscanf(cheat,"%s %d",code,&val);
		if(val <= HERO->GetLevel())
			val = HERO->GetLevel()+1;
		MSG_WORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_LEVELUP_SYN;
		msg.dwObjectID = HEROID;
		msg.wData = (WORD)val;
		NETWORK->Send(&msg,sizeof(msg));
		
		return TRUE;
	}

	else if(strcmp(code, "RMS") == 0)
	{
		int val = 0;
		sscanf(cheat,"%s %d",code,&val);
		if(val == 0)
			return TRUE;
		if((val < 12) || (val > 400))
			val = 12;
		MSG_WORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_GENGOL_SYN;
		msg.dwObjectID = HEROID;
		msg.wData = (WORD)val;
		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}
	else if(strcmp(code, "ALS") == 0)
	{
		int val = 0;
		sscanf(cheat,"%s %d",code,&val);
		if(val == 0)
			return TRUE;
		if((val < 12) || (val > 400))
			val = 12;
		MSG_WORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_MINCHUB_SYN;
		msg.dwObjectID = HEROID;
		msg.wData = (WORD)val;
		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
	}
	else if(strcmp(code, "CP") == 0)
	{
		int val = 0;
		sscanf(cheat,"%s %d",code,&val);
		if(val == 0)
			return TRUE;
		if((val < 12) || (val > 400))
			val = 12;
		MSG_WORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_CHERYUK_SYN;
		msg.dwObjectID = HEROID;
		msg.wData = (WORD)val;
		NETWORK->Send(&msg,sizeof(msg));

		return TRUE;
	}
	else if(strcmp(code, "TLA") == 0)
	{
		int val = 0;
		sscanf(cheat,"%s %d",code,&val);
		if(val == 0)
			return TRUE;
		if((val < 12) || (val > 400))
			val = 12;
		MSG_WORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEATE_SIMMEK_SYN;
		msg.dwObjectID = HEROID;
		msg.wData = (WORD)val;
		NETWORK->Send(&msg,sizeof(msg));

		return TRUE;
	}

	else if(strcmp(code, "ABIL") == 0)
	{
		DWORD abilexp;
		sscanf(cheat,"%s %d",code,&abilexp);

		MSG_DWORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_ABILITYEXP_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData = abilexp;
		NETWORK->Send(&msg,sizeof(msg));
		
		return TRUE;
	}
	else if( strcmp(code, "MOVETO") == 0 )
	{
/*
		DWORD dwID;
		sscanf(cheat,"%s %d", code, &dwID);

		MOVE_POS msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_MOVETOCHAR_SYN;
		msg.dwObjectID = HEROID;
		msg.dwMoverID = dwID;	//target
//		msg.cpos.Compress(&pos);
		NETWORK->Send(&msg,sizeof(msg));
		return TRUE;
*/
		sscanf(cheat, "%s %s", code, buf);
		//buf-A³¸?AI¸i

		if( strncmp( HERO->GetObjectName(), buf, MAX_NAME_LENGTH+1 ) == 0  )	//AU±a AU½A
			return TRUE;

		MSG_NAME msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_MOVETOCHAR_SYN;
		msg.dwObjectID	= HEROID;
		SafeStrCpy( msg.Name, buf, MAX_NAME_LENGTH + 1 );
		NETWORK->Send(&msg, sizeof(msg));
		return TRUE;
	}
	else if( strcmp(code, "WHEREIS") == 0 )
	{
		sscanf(cheat,"%s %s", code, buf);

//		if( strncmp( HERO->GetObjectName(), buf, MAX_NAME_LENGTH+1 ) == 0  )	//AU±a AU½A
//			return TRUE;
		if( (FILTERTABLE->IsInvalidCharInclude((unsigned char*)buf)) == TRUE )
		{
			return TRUE;
		}
		MSG_NAME msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_WHEREIS_SYN;
		msg.dwObjectID	= HEROID;
		SafeStrCpy( msg.Name, buf, MAX_NAME_LENGTH+1 );
		
		NETWORK->Send(&msg,sizeof(msg));

		return TRUE;
	}
	else if( strcmp( code, "BAN") == 0 )
	{
		sscanf(cheat, "%s %s", code, buf);
		//buf-A³¸?AI¸i

		if( strncmp( HERO->GetObjectName(), buf, MAX_NAME_LENGTH+1 ) == 0  )	//AU±a AU½A
			return TRUE;
		if( (FILTERTABLE->IsInvalidCharInclude((unsigned char*)buf)) == TRUE )
			return TRUE;

		MSG_NAME msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_BANCHARACTER_SYN;
		msg.dwObjectID	= HEROID;
		SafeStrCpy( msg.Name, buf, MAX_NAME_LENGTH+1 );

		NETWORK->Send(&msg,sizeof(msg));
		
		return TRUE;
	}
	else if( strcmp( code, "BLOCK") == 0 )
	{
		sscanf(cheat, "%s %s", code, buf);
		if( strncmp( HERO->GetObjectName(), buf, MAX_NAME_LENGTH+1 ) == 0  )	//AU±a AU½A
			return TRUE;

		MSG_NAME_DWORD msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_BLOCKCHARACTER_SYN;
		msg.dwObjectID	= HEROID;
		msg.dwData		= 1;	//block
		SafeStrCpy( msg.Name, buf, MAX_NAME_LENGTH+1 );

		NETWORK->Send(&msg,sizeof(msg));

		return TRUE;
	}
	else if( strcmp( code, "NONBLOCK" ) == 0 )
	{
		sscanf(cheat, "%s %s", code, buf);
		if( strncmp( HERO->GetObjectName(), buf, MAX_NAME_LENGTH+1 ) == 0  )	//AU±a AU½A
			return TRUE;

		MSG_NAME_DWORD msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_BLOCKCHARACTER_SYN;
		msg.dwObjectID	= HEROID;
		msg.dwData		= 0;	//nonblock
		SafeStrCpy( msg.Name, buf, MAX_NAME_LENGTH+1 );

		NETWORK->Send(&msg,sizeof(msg));

		return TRUE;
	}
	else if( strcmp( code, "PKMODE" ) == 0 )
	{
		int nAllow = -1;
		sscanf(cheat, "%s %s %d", code, buf, &nAllow );
		if( nAllow != 0 && nAllow != 1 )	return TRUE;
		// magi82(37) 맵 속성 데이터
		WORD wMapNum = MAP->GetMapNumForName( buf );
		if( wMapNum == 0 )	return TRUE;
		
		MSG_WORD2 msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_PKALLOW_SYN;
		msg.dwObjectID	= HEROID;
		msg.wData1		= wMapNum;
		msg.wData2		= (WORD)nAllow;

		NETWORK->Send(&msg,sizeof(msg));

		return TRUE;
	}	
	else if( strcmp( code, "KILLMONSTER" ) == 0 )
	{
		MSGBASE msg;
		msg.Category	= MP_CHEAT;
//		msg.Protocol	= MP_CHEAT_KILL_ALLMONSTER_SYN;
		msg.dwObjectID	= HEROID;
		
		NETWORK->Send(&msg,sizeof(msg));

		return TRUE;
	}
	else if(strcmp(code, "PARTYINFO") == 0)
	{
		DWORD PartyIdx;
#ifdef TAIWAN_LOCAL
		PartyIdx = HERO->GetPartyIdx();
#else
		sscanf(cheat,"%s %d",code,&PartyIdx);
#endif
		MSG_DWORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_PARTYINFO_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData = PartyIdx;
		NETWORK->Send(&msg,sizeof(msg));
		CHATMGR->AddMsg( CTC_SYSMSG, "Party Info Cheat");
		return TRUE;
	}
	else if(strcmp(code, "MUNPACLEAR") == 0)
	{
		MSGBASE msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_MUNPADATECLR_SYN;
		msg.dwObjectID = HEROID;
		NETWORK->Send(&msg,sizeof(msg));
		HERO->SetGuildEntryDate("2004.01.01");
		return TRUE;
	}
	else if( strcmp( code, "TIMESET" ) == 0 )
	{
		int nTime;
		sscanf(cheat, "%s %d", code, &nTime );

		MSG_INT msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_ADDTIME;
		msg.nData = nTime;
		NETWORK->Send( &msg, sizeof(msg) );
		return TRUE;
	}
//	else if( strcmp( code, "EVENT" ) == 0 )
//	{
//		int nAllow = -1;
//		int nCount = 30;
//		sscanf(cheat, "%s %d %d", code, &nAllow, &nCount);
//		if( nAllow == -1 ) return TRUE;
//		
//		if( nCount > 30 )
//			nCount = 30;
//
//		CHEATMGR->EventMonsterRegen(nAllow, nCount);
//	}
	else if( strcmp( code, "EVENTMAP" ) == 0 )
	{
		char name[MAX_NAME_LENGTH+1];
		int map;
		int channel;
		int team;
		sscanf( cheat,"%s %s %d %d %d", code, name, &map, &channel, &team );

		MSG_NAME_DWORD3 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_CHANGE_EVENTMAP_SYN;
		msg.dwObjectID = HEROID;
		SafeStrCpy( msg.Name, name, MAX_NAME_LENGTH+1 );
		msg.dwData1 = map;
		msg.dwData2 = channel;
		msg.dwData3 = team;
		
		NETWORK->Send( &msg, sizeof(msg) );
		return TRUE;
	}
	else if( strcmp( code, "EVENTSTART" ) == 0 )
	{
		int map;
		int channel;
		sscanf( cheat,"%s %d %d %d", code, &map, &channel );

		MSG_DWORD2 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_EVENT_START_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData1 = map;
		msg.dwData2 = channel;
		
		NETWORK->Send( &msg, sizeof(msg) );
		return TRUE;
	}
	else if( strcmp( code, "EVENTREADY" ) == 0 )
	{
		int map;
		int channel;
		sscanf( cheat,"%s %d %d %d", code, &map, &channel );

		MSG_DWORD2 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_EVENT_READY_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData1 = map;
		msg.dwData2 = channel;
		
		NETWORK->Send( &msg, sizeof(msg) );
		return TRUE;
	}
	// guildfieldwar
	else if( strcmp( code, "GUILDFIELDWAR0" ) == 0 )
	{
		GAMEIN->GetGFWarDeclareDlg()->SetActive( TRUE );

		return TRUE;
	}
	else if( strcmp( code, "GUILDFIELDWAR1" ) == 0 )
	{
		GUILDINFO temp;
		strcpy( temp.GuildName, "Test" );
		temp.MapNum = 17;
		temp.GuildLevel = 3;
		strcpy( temp.MasterName, "Tester닷" );
		GAMEIN->GetGFWarResultDlg()->ShowDeclare( &temp );

		return TRUE;
	}
	else if( strcmp( code, "GUILDFIELDWAR2" ) == 0 )
	{
		GAMEIN->GetGFWarResultDlg()->ShowResult( 0 );

		return TRUE;
	}
	else if( strcmp( code, "GUILDFIELDWAR3" ) == 0 )
	{
		GAMEIN->GetGFWarInfoDlg()->ShowSurrend();
		
		return TRUE;
	}	
	else if( strcmp( code, "GUILDFIELDWAR4" ) == 0 )
	{
		GAMEIN->GetGFWarResultDlg()->ShowResult( 1 );
		
		return TRUE;
	}
	else if( strcmp( code, "GUILDFIELDWAR5" ) == 0 )
	{
		GAMEIN->GetGFWarInfoDlg()->ShowSuggest();
		
		return TRUE;
	}
	else if( strcmp( code, "GETSHOPITEM" ) == 0 )
	{
		int item = 0;
		sscanf( cheat,"%s %d", code, &item );

		if( item < 55000 )
			return FALSE;

		MSG_WORD Msg;
		Msg.Category = MP_CHEAT;
		Msg.Protocol = MP_CHEAT_GETSHOPITEM;
		Msg.dwObjectID = HEROID;
		Msg.wData = (WORD)item;
		
		NETWORK->Send( &Msg, sizeof(Msg) );
		return TRUE;
	}
	else if( strcmp( code, "PARTYWARSUGGEST" ) == 0 )
	{
		PARTYWAR->SendPartyWarSuggest();
	}
	else if( strcmp( code, "PARTYWARLOCK" ) == 0 )
	{
		PARTYWAR->SendPartyWarLock( TRUE );
	}
	else if( strcmp( code, "PARTYWARDLG" ) == 0 )
	{
		PARTYWAR->TestPartyWarDialog();
	}
	else if( strcmp( code, "WJSWLR" ) == 0 )
	{
		int stage = 0;
		sscanf( cheat, "%s %d", code, &stage );

		MSG_BYTE Msg;
		Msg.Category = MP_CHEAT;
		Msg.Protocol = MP_CHEAT_STAGE_SYN;
		Msg.dwObjectID = HEROID;
		Msg.bData = (BYTE)stage;
		
		NETWORK->Send( &Msg, sizeof(Msg) );
		return TRUE;
	}
	else if( strcmp( code, "QHTMAHQZNPTMXM" ) == 0 )
	{
		MSGBASE Msg;
		Msg.Category = MP_CHEAT;
		Msg.Protocol = MP_CHEAT_BOSSMOB_SYN;
		Msg.dwObjectID = HEROID;
		NETWORK->Send( &Msg, sizeof(Msg) );
		CHATMGR->AddMsg( CTC_SYSMSG, "Boss Quest Cheat" );
		return TRUE;
		
	}
	else if( strcmp( code, "TOURNAMENTINFO" ) == 0 )
	{
		DWORD param = 0;
		sscanf( cheat, "%s %d", code, &param );

		MSG_DWORD msg;
		msg.Category = MP_GTOURNAMENT;
		msg.Protocol = MP_GTOURNAMENT_CHEAT;
		msg.dwObjectID = HEROID;
		msg.dwData = param;
		NETWORK->Send( &msg, sizeof(msg) );
		return TRUE;
	}
	else if( strcmp( code, "STARTEVENTGT" ) == 0 )
	{
		MSG_GT_EVENTSTART msg;
		msg.Category = MP_GTOURNAMENT;
		msg.Protocol = MP_GTOURNAMENT_EVENT_START;
		msg.dwObjectID = HEROID;
		msg.SenderID = HEROID;
		sscanf( cheat, "%s %s %s", code, msg.GuildName1, msg.GuildName2 );
		NETWORK->Send( &msg, sizeof(msg) );
		return TRUE;
	}
	else if( strcmp( code, "ENDEVENTGT" ) == 0 )
	{
		MSGBASE msg;
		msg.Category = MP_GTOURNAMENT;
		msg.Protocol = MP_GTOURNAMENT_EVENT_END;
		msg.dwObjectID = HEROID;
		NETWORK->Send( &msg, sizeof(msg) );
		return TRUE;
	}
	else if( strcmp( code, "SIEGEWAR" ) == 0 )
	{
		DWORD mapnum = 0;
		switch( MAP->GetMapNum() )
		{
		case jangan:
			mapnum = jangan_siege;
			break;
		case nakyang:
			mapnum = nakyang_siege;
			break;
		case gaebong:
			mapnum = gaebong_siege;
			break;
		case bukkyung:
			mapnum = bukkyung_siege;
			break;
		case jangan_siege:
		case nakyang_siege:
		case gaebong_siege:
		case bukkyung_siege:
			mapnum = 0;
			break;
		default:
			return FALSE;
		}


		DWORD param1 = 0;
		sscanf( cheat, "%s %d", code, &param1 );

		MSG_DWORD4 msg;
		msg.Category = MP_SIEGEWAR;
		msg.Protocol = MP_SIEGEWAR_CHEAT;
		msg.dwObjectID = HEROID;
		msg.dwData1 = param1;
		msg.dwData2 = MAP->GetMapNum();
		msg.dwData3 = mapnum;
		msg.dwData4 = HERO->GetGuildIdx();
		NETWORK->Send( &msg, sizeof(msg) );
		return TRUE;
	}
	else if( strcmp( code, "GUILDUNIONCLEAR" ) == 0 )
	{
		MSG_DWORD Msg;
		Msg.Category = MP_CHEAT;
		Msg.Protocol = MP_CHEAT_GUILDUNION_CLEAR;
		Msg.dwObjectID = HEROID;
		Msg.dwData = 0;
		NETWORK->Send( &Msg, sizeof(Msg) );
		return TRUE;
	}
	else if( strcmp( code, "WNTHRTJD" ) == 0 )
	{
		int wAttr = 0;
		sscanf( cheat, "%s %d", code, &wAttr );

		MSG_WORD Msg;
		Msg.Category = MP_CHEAT;
		Msg.Protocol = MP_CHEAT_CHANGE_MAINATTR_SYN;
		Msg.dwObjectID = HEROID;
		Msg.wData = (WORD)wAttr;
		NETWORK->Send( &Msg, sizeof(Msg) );
		return TRUE;
	}
	else if( strcmp( code, "QNTHRTJD" ) == 0 )
	{
		int wAttr = 0;
		sscanf( cheat, "%s %d", code, &wAttr );
		
		MSG_WORD Msg;
		Msg.Category = MP_CHEAT;
		Msg.Protocol = MP_CHEAT_CHANGE_SUBATTR_SYN;
		Msg.dwObjectID = HEROID;
		Msg.wData = (WORD)wAttr;
		NETWORK->Send( &Msg, sizeof(Msg) );
	}
	else if( strcmp( code, "SWPROFITDLG" ) == 0 )
	{
		GAMEIN->GetSWProfitDlg()->ShowProfitDlg( 10, 10000 );
	}
	else if( strcmp( code, "SWPROFITINFO" ) == 0 )
	{
		SWPROFIT->SendProfitInfoSyn();
	}
	else if( strcmp( code, "SWPROTECTREG" ) == 0 )
	{
		GAMEIN->GetSWProtectRegDlg()->Test();
	}
	else if( strcmp( code, "SWSTARTTIMEDLG" ) == 0 )
	{
		GAMEIN->GetSWStartTimeDlg()->Test();
	}
	else if( strcmp( code, "UVP" ) == 0 )
	{
		int count = 0;
		sscanf( cheat, "%s %d", code, &count );

		if(count < 1)
			count = 1;

		MSGBASE msg;
		msg.Category = 0;

		for(int i = 0; i < count; i++)
			NETWORK->Send( &msg, sizeof(msg) );

		return TRUE;
	}
	else if( strcmp( code, "VP" ) == 0 )
	{
		int count = 0;
		sscanf( cheat, "%s %d", code, &count );

		if(count < 1)
			count = 1;

		MSGBASE msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = 255;

		for(int i = 0; i < count; i++)
			NETWORK->Send( &msg, sizeof(msg) );

		return TRUE;
	}
	else if( strcmp( code, "SKILLOPTION" ) == 0 )
	{
		GAMEIN->GetSkillOptionChangeDlg()->SetActive(TRUE);

		return TRUE;
	}
	else if( strcmp( code, "SKILLCLEAR" ) == 0 )
	{
		GAMEIN->GetSkillOptionClearDlg()->SetActive(TRUE);

		return TRUE;
	}
	// magi82 cheat damage //////////////////////////////////////////////////////////////////////////
	else if(strcmp(code, "DAMAGE") == 0)
	{
		float fDamageRate = 100.0f;
		sscanf(cheat, "%s %f", code, &fDamageRate);
		if(fDamageRate > 1000.0f)
			return FALSE;

		DWORD wDamageRate = (DWORD)(fDamageRate * 1000000);

		MSG_DWORD msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_DAMAGE_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData = wDamageRate;
		NETWORK->Send(&msg,sizeof(msg));

		return TRUE;
	}
	//////////////////////////////////////////////////////////////////////////
	else if( strcmp(code, "GETSERVERINFO") == 0 )
	{
		DWORD toserver = 0;
		sscanf( cheat, "%s %d", code, &toserver );
		if( toserver > 2 )
			return FALSE;

		MSGBASE msg;
		msg.Category = MP_CHEAT;
		if( toserver == 1 )
			msg.Protocol = MP_CHEAT_AGENT_CONDITION;
		else if( toserver == 2 )
			msg.Protocol = MP_CHEAT_MAP_CONDITION;
		msg.dwObjectID = HEROID;
		NETWORK->Send(&msg, sizeof(msg));

		return TRUE;
	}
	else if( strcmp(code, "ITEMLIMITON") == 0 )
	{
		DWORD dwItemIdx = 0;
		sscanf( cheat,"%s %d", code, &dwItemIdx );

		MSG_DWORD3 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_ITEMLIMIT_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData1 = 1;	// on/off
		msg.dwData2 = dwItemIdx;
		msg.dwData3 = 1;	// on

		NETWORK->Send( &msg, sizeof(msg) );
		return TRUE;
	}
	else if( strcmp(code, "ITEMLIMITOFF") == 0 )
	{
		DWORD dwItemIdx = 0;
		sscanf( cheat,"%s %d", code, &dwItemIdx );

		MSG_DWORD3 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_ITEMLIMIT_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData1 = 1;	// on/off
		msg.dwData2 = dwItemIdx;
		msg.dwData3 = 0;	// off

		NETWORK->Send( &msg, sizeof(msg) );
		return TRUE;
	}
	else if( strcmp(code, "ITEMLIMITCOUNT") == 0 )
	{
		DWORD dwItemIdx = 0;
		int nMaxCount = 0;
		sscanf( cheat,"%s %d %d", code, &dwItemIdx, &nMaxCount );

		MSG_DWORD3 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_ITEMLIMIT_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData1 = 2;  // on/off
		msg.dwData2 = dwItemIdx;
		msg.dwData3 = nMaxCount;

		NETWORK->Send( &msg, sizeof(msg) );

		return TRUE;
	}
	else if( strcmp(code, "AUTONOTEDLG") == 0 )
	{
		/// test
		GAMEIN->GetAutoNoteDlg()->SetActive(TRUE);
		GAMEIN->GetAutoAnswerDlg()->SetActive(TRUE);
		return TRUE;
	}
	else if( strcmp(code, "AUTONOTEENABLE") == 0 )
	{
		int nOn = 0;
		sscanf( cheat,"%s %d", code, &nOn );

		MSG_DWORD2 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_AUTONOTE_SETTING_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData1 = 0;
		msg.dwData2 = nOn;  // on = 1/ off = 0

		NETWORK->Send( &msg, sizeof(msg) );

		return TRUE;
	}
	else if( strcmp(code, "SETAUTONOTEREPLYTIME") == 0 )
	{
		DWORD dwReplyTime = 120;
		sscanf( cheat,"%s %d", code, &dwReplyTime );

		MSG_DWORD2 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_AUTONOTE_SETTING_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData1 = 1;
		msg.dwData2 = dwReplyTime;	// 초

		NETWORK->Send( &msg, sizeof(msg) );

		return TRUE;
	}
	else if( strcmp(code, "SETAUTONOTEREPLYCHANCE") == 0 )
	{
		DWORD dwChance = 3;
		sscanf( cheat,"%s %d", code, &dwChance );

		MSG_DWORD2 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_AUTONOTE_SETTING_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData1 = 2;
		msg.dwData2 = dwChance; 

		NETWORK->Send( &msg, sizeof(msg) );

		return TRUE;
	}
	else if( strcmp(code, "SETAUTONOTEUSETIME") == 0 )
	{
		DWORD dwTime = 60;
		sscanf( cheat,"%s %d", code, &dwTime );

		MSG_DWORD2 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_AUTONOTE_SETTING_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData1 = 3;
		msg.dwData2 = dwTime;	// 분

		NETWORK->Send( &msg, sizeof(msg) );

		return TRUE;
	}
	else if( strcmp(code, "SETAUTONOTEBLOCKTIME") == 0 )
	{
		DWORD dwTime = 60;
		sscanf( cheat,"%s %d", code, &dwTime );

		MSG_DWORD2 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_AUTONOTE_SETTING_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData1 = 4;
		msg.dwData2 = dwTime;	// 분

		NETWORK->Send( &msg, sizeof(msg) );

		return TRUE;
	}
	else if( strcmp(code, "AUTONOTESYSTEM") == 0 )		// 시스템전체
	{
		int nOn = 0;
		sscanf( cheat,"%s %d", code, &nOn );

		MSG_DWORD2 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_AUTONOTE_SETTING_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData1 = 5;
		msg.dwData2 = nOn;  // on = 1/ off = 0

		NETWORK->Send( &msg, sizeof(msg) );

		return TRUE;
	}
	else if( strcmp(code, "FORTWARWAREHOUSEDLG") == 0)
	{
//		GAMEIN->GetFWWareHouseDlg()->SetActive( TRUE );

		// 요새전 임시로
		MSG_INT msg;
		msg.Category = MP_FORTWAR;
		msg.Protocol = MP_FORTWAR_WAREHOUSE_INFO_SYN;
		msg.dwObjectID = HEROID;
		msg.nData = FORTWARWAREIDX_1; // 1, 2, 3
		NETWORK->Send(&msg, sizeof(msg));
	}
	else if( strcmp(code, "FORTWARWAREHOUSEDLG1") == 0)
	{
		//		GAMEIN->GetFWWareHouseDlg()->SetActive( TRUE );

		// 요새전 임시로
		MSGBASE msg;
		msg.Category = MP_FORTWAR;
		msg.Protocol = MP_FORTWAR_SIEGEWAREHOUSE_INFO_SYN;
		msg.dwObjectID = HEROID;
		NETWORK->Send(&msg, sizeof(msg));
	}
#endif
#endif

	return FALSE;
}

#endif //_CHEATENABLE_


CCheatMsgParser::CCheatMsgParser()
{
	m_bCheatEnable = FALSE;

	m_pTargetObj = NULL;
#ifdef _TESTCLIENT_
	m_bCheatEnable = TRUE;
#endif

	m_wItemOptionKind		= 0;
	m_bReadyToOptionCheat	= FALSE;
	m_wTargetItemIdx		= 0;
	m_ItemAbsPos			= 0;
	m_OptionCount			= 0;
}

CCheatMsgParser::~CCheatMsgParser()
{

}
/*
void CCheatMsgParser::LoadItem()
{
	CMHFile file;

	if(!file.Init("./Resource/eventitem.txt", "rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR)) return;

	WORD idx = 0;
	int num;
	while(1)
	{
		if(file.IsEOF()) break;

		num = file.GetInt(); // ¼±AA ¿E¼CAC °¹¼o
		m_itemCount[idx] = num;

		for( int i=0;i<num;++i)
			strcpy(m_eventitem[idx][i].strName, file.GetString());

		++idx;
	}

	m_bEvent = TRUE;
}
*/



void CCheatMsgParser::NetworkMsgParse(BYTE Category,BYTE Protocol,MSGBASE* pMsg)
{
	ASSERT(Category == MP_CHEAT);

	switch(Protocol) 
	{

#ifdef _CHEATENABLE_
		
	case MP_CHEAT_CHANGEMAP_ACK:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			MAPTYPE mapnum = pmsg->wData;
			if(mapnum < 200)								//?
				PEACEWARMGR->Peace_WarMode(HERO, TRUE);

			MAPCHANGE->SetGameInInitKind(eGameInInitKind_MapChange);
			MAINGAME->SetGameState(eGAMESTATE_MAPCHANGE,&mapnum,4);
		}
		break;

	case MP_CHEAT_CHANGEMAP_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			
			if( pmsg->dwData == 0 )
			{
				//¸EAI ¾ø´A °I
			}
			else
			{
				//A¤³IAI Æ²¸° °I
				gChannelNum = pmsg->dwData;
			}
		}
		break;

	case MP_CHEAT_WHEREIS_ACK:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			
//			char* p = MAP->GetMapName( pmsg->wData );
//			if( p[0] == 0 )
//			{
				CHATMGR->AddMsg( CTC_TOGM, CHATMGR->GetChatMsg( 430 ) );
//			}
//			else
//			{
//				CHATMGR->AddMsg( CTC_TOGM, CHATMGR->GetChatMsg( 431 ),
//					MAP->GetMapName( pmsg->wData ) );
//			}
			//GM_TOOLAI °æ¿i¿¡ E­¸e¿¡ Aa·ACØAO´A °I Aß°¡CI½A¿A.
#ifdef _GMTOOL_
			GMTOOLMGR->DisplayWhereInfo( "SELECT", 0, 0, 0 );	//¼±AAA￠ ¶C´A ·IμuAß?¿¡ AOA≫¶§..
#endif
		}
		break;

	case MP_CHEAT_WHEREIS_MAPSERVER_ACK:
		{
			MSG_WHERE_INFO* pmsg = (MSG_WHERE_INFO*)pMsg;

			VECTOR3 vPos;
			pmsg->cpos.Decompress( &vPos );

			// magi82(37) 맵 속성 데이터
			char* strMapName = MAP->GetMapName( pmsg->wMapNum );
			if( strMapName[0] == 0 )
			{
				CHATMGR->AddMsg( CTC_TOGM, CHATMGR->GetChatMsg( 430 ) );
#ifdef _GMTOOL_
				GMTOOLMGR->DisplayWhereInfo( "SELECT", 0, 0, 0 );	//¼±AAA￠ ¶C´A ·IμuAß?¿¡ AOA≫¶§..
#endif
			}
			else
			{
				CHATMGR->AddMsg( CTC_TOGM, CHATMGR->GetChatMsg( 431 ),
					strMapName, pmsg->bChannel,
					(int)(vPos.x/100.0f), (int)(vPos.z/100.0f) );
#ifdef _GMTOOL_
				GMTOOLMGR->DisplayWhereInfo( strMapName, pmsg->bChannel,
					(int)(vPos.x/100.0f), (int)(vPos.z/100.0f) );	//¼±AAA￠ ¶C´A ·IμuAß?¿¡ AOA≫¶§..
#endif
			}
		}
		break;

	case MP_CHEAT_WHEREIS_NACK:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;

			switch( pmsg->bData )
			{
//			case 0:
//				break;
			case 1:
				CHATMGR->AddMsg( CTC_TOGM, CHATMGR->GetChatMsg( 432 ) );
				break;
			case 2:
				CHATMGR->AddMsg( CTC_TOGM, CHATMGR->GetChatMsg( 433 ) );
				break;
			default:
				CHATMGR->AddMsg( CTC_TOGM, CHATMGR->GetChatMsg( 434 ) );
				break;
			}

		}
		break;

	case MP_CHEAT_BLOCKCHARACTER_ACK:
		{
			CHATMGR->AddMsg( CTC_TOGM, CHATMGR->GetChatMsg( 435 ) );
		}
		break;

	case MP_CHEAT_BLOCKCHARACTER_NACK:
		{
			CHATMGR->AddMsg( CTC_TOGM, CHATMGR->GetChatMsg( 432 ) );
		}
		break;

	case MP_CHEAT_BANCHARACTER_ACK:
		{
			CHATMGR->AddMsg( CTC_TOGM, CHATMGR->GetChatMsg( 436 ) );
		}
		break;

	case MP_CHEAT_BANCHARACTER_NACK:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
/*
			switch( pmsg->bData )
			{
//			case 0:
//				break;
			case 1:
				CHATMGR->AddMsg( CTC_TOGM, "A¸AcCIAo ¾E´A A³¸?AIAO´I´U." );
				break;
			case 2:
				CHATMGR->AddMsg( CTC_TOGM, "A￠¼OCIAo ¾EAº A³¸?AIAO´I´U." );
				break;
			default:
				CHATMGR->AddMsg( CTC_TOGM, "CA·¹AI¾i Aß¹æ¿¡ ½CÆÐCI¿´½A´I´U." );
				break;
			}
*/
		}
		break;
	case MP_CHEAT_ITEM_ACK:
		{
			ITEMOBTAINARRAY * pmsg = (ITEMOBTAINARRAY *)pMsg;
		
			CItem* pItem = NULL;
//			CItem* pPreItem = NULL;

			for(int i = 0 ; i < pmsg->ItemNum ; ++i)
			{
				if( pItem = GAMEIN->GetInventoryDialog()->GetItemForPos( pmsg->GetItem(i)->Position ) )
				{
					pItem->SetItemBaseInfo(pmsg->GetItem(i));
//					DeleteItem(pmsg->GetItem(i)->Position, pPreItem);

				}
				else
				{
					pItem = ITEMMGR->MakeNewItem(pmsg->GetItem(i),"MP_CHEAT_ITEM_ACK");	
					BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(pItem);
					if(!rt)
					{
						ASSERT(0);
					}
				}


				// Game event 
//				GAMEEVENTMGR->AddEvent(eGameEvent_ObtainItem, (pItem->GetItemBaseInfo())->wIconIdx );
			}

			//³ª´ⓒ±aA￠AI AOA¸¸e Ao¿i´U.
			cDialog* pDlg = WINDOWMGR->GetWindowForID( DBOX_DIVIDE_INV );
			if( pDlg )
			{
				((cDivideBox*)pDlg)->ExcuteDBFunc( 0 );	//Ae¼O¹oÆ° ´ⓒ¸￡±a
			}

//			if( pmsg->ItemNum > 1 )
//				CHATMGR->AddMsg( CTC_GETITEM, CHATMGR->GetChatMsg( 124 ), pItem->GetItemInfo()->ItemName, pmsg->ItemNum );
//			else
				CHATMGR->AddMsg( CTC_GETITEM, CHATMGR->GetChatMsg( 121 ), pItem->GetItemInfo()->ItemName );
			
//			EFFECTMGR->StartHeroEffectProcess(eEffect_GetItem);

			ITEMMGR->ItemDropEffect( pItem->GetItemIdx() );
//			EFFECTMGR->StartHeroEffectProcess(eEffect_GetItem);

			
//			EFFECTMGR->StartHeroEffectProcess(eEffect_GetItem);
			QUICKMGR->RefreshQickItem();
		}
		break;
	case MP_CHEAT_AGENTCHECK_ACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, "CHEAT_AGENT_ACK!" );
		}
		break;

#endif	//_CHEATENABLE_

	case MP_CHEAT_MOVE_ACK:		//GMAI move½AANAa¾ßCN´U.
		{
			MOVE_POS* pmsg = (MOVE_POS*)pMsg;
			VECTOR3 pos;			
			pmsg->cpos.Decompress(&pos);

			BASEMOVE_INFO info;
			info.bMoving = FALSE;
			info.KyungGongIdx = 0;
			info.MoveMode = eMoveMode_Run;
			info.CurPosition = pos;
			MOVEMGR->InitMove(HERO,&info);
			//MOVEMGR->HeroMoveStop();		//movestopA≫ ¾ECN AIA?..?

//			CPet* pPet = PETMGR->GetCurSummonPet();
			if(HERO->GetPet())
			{
				MOVEMGR->InitMove(HERO->GetPet(),&info);
			}
		}
		break;
		
	case MP_CHEAT_HIDE_ACK:		//GM¼u±a
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwData1 );
			if( !pPlayer ) return;
			
			ASSERT(pPlayer->GetObjectKind() == eObjectKind_Player);
			
			if(pmsg->dwData2 == 0 )
			{
#ifdef _GMTOOL_
				if( MAINGAME->GetUserLevel() <= eUSERLEVEL_GM  && CAMERA->GetCameraMode() != eCM_EyeView )
					pPlayer->GetEngineObject()->SetAlpha( 1.0f );
				else
#endif
					pPlayer->GetEngineObject()->Show();
				
				pPlayer->GetCharacterTotalInfo()->bVisible = TRUE;				
			}
			else
			{
#ifdef _GMTOOL_
				if( MAINGAME->GetUserLevel() <= eUSERLEVEL_GM  && CAMERA->GetCameraMode() != eCM_EyeView )
					pPlayer->GetEngineObject()->SetAlpha( 0.3f );
				else
#endif
					pPlayer->GetEngineObject()->HideWithScheduling();

				
				pPlayer->GetCharacterTotalInfo()->bVisible = FALSE;
				if( pPlayer->GetID() == OBJECTMGR->GetSelectedObjectID() )
				{
					OBJECTMGR->SetSelectedObject( NULL );	//°¨Aß·A´A A³¸?AI°¡ ¼±AAμC¾i AOA¸¸e ¼±AA CØA|
				}
			}

			OBJECTMGR->ApplyShadowOption(pPlayer);		//±×¸²AU ¾ø¾O±a
			OBJECTMGR->ApplyOverInfoOption(pPlayer);	//AI¸§ ¾ø¾O±a

		}
		break;
		
	case MP_CHEAT_LIFE_ACK:
		{
		}
		break;
		
	case MP_CHEAT_MP_ACK:
		{
		}
		break;
		
#ifdef _GMTOOL_
	case MP_CHEAT_GM_LOGIN_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			if( MAINGAME->GetUserLevel() <= eUSERLEVEL_GM )
			{
				GMTOOLMGR->Login( TRUE, (int)pmsg->dwData );
			}
		}
		break;

	case MP_CHEAT_GM_LOGIN_NACK:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;

			if( MAINGAME->GetUserLevel() <= eUSERLEVEL_GM )
			{
				GMTOOLMGR->Login( FALSE, (int)pmsg->bData );
			}
		}
		break;
	case MP_CHEAT_PARTYINFO_ACK:
		{
			PARTY_INFO * pmsg = (PARTY_INFO*)pMsg;
			for(int i=0; i<MAX_PARTY_LISTNUM; ++i)
			{
				if(pmsg->Member[i].dwMemberID == 0)
					SafeStrCpy(pmsg->Member[i].Name, "!!!", MAX_NAME_LENGTH+1);
				CHATMGR->AddMsg( CTC_SYSMSG, "%s, %d, %d, %d", 
					pmsg->Member[i].Name, pmsg->Member[i].Level, pmsg->Member[i].bLogged,
					pmsg->Member[i].LifePercent);
			}
		}
		break;
		
	case MP_CHEAT_EVENTMAP_RESULT:
		{
			MSG_NAME_WORD* pmsg = (MSG_NAME_WORD*)pMsg;
			if( pMsg->dwObjectID == 0 )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 716 ), pmsg->wData, pmsg->Name );
			}
			else if( pMsg->dwObjectID == 1 )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 716 ), pmsg->wData, pmsg->Name );
			}
			else if( pMsg->dwObjectID == 2 )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 717 ), pmsg->wData );
			}

			EVENTMAP->Init();
		}
		break;
#endif

	case MP_CHEAT_EVENTNOTIFY_ON:
		{
			MSG_EVENTNOTIFY_ON* pmsg = (MSG_EVENTNOTIFY_ON*)pMsg;

			NOTIFYMGR->SetEventNotifyStr( pmsg->strTitle, pmsg->strContext );
			NOTIFYMGR->SetEventNotify( TRUE );

			if( GAMEIN->GetEventNotifyDialog() )
			{
				GAMEIN->GetEventNotifyDialog()->SetTitle( pmsg->strTitle );
				GAMEIN->GetEventNotifyDialog()->SetContext( pmsg->strContext );
				GAMEIN->GetEventNotifyDialog()->SetActive( TRUE );
				NOTIFYMGR->SetEventNotifyChanged( FALSE );
				//사운드?
			}
			else
			{
				NOTIFYMGR->SetEventNotifyChanged( TRUE );
			}

			NOTIFYMGR->ResetEventApply();
			for(int i=0; i<eEvent_Max; ++i)
			{
				if( pmsg->EventList[i] )
					NOTIFYMGR->SetEventApply( i );
			}
		}
		break;
		
	case MP_CHEAT_EVENTNOTIFY_OFF:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			NOTIFYMGR->SetEventNotify( FALSE );
			NOTIFYMGR->SetEventNotifyChanged( FALSE );

			if( GAMEIN->GetEventNotifyDialog() )
			{
				GAMEIN->GetEventNotifyDialog()->SetActive( FALSE );	
			}
		}
		break;
	case MP_CHEAT_PET_SKILLREADY_ACK:
		{
			//스킬게이지 채워주고
			//발동가능상태
			PETMGR->SetSkillRechargeAmount(PET_MAX_SKILL_CHARGE);
			PETMGR->SetSkillGuageFull(TRUE);
			PETMGR->SetReadyToSendSkillMsg(TRUE);
		}
	case MP_CHEAT_PET_FRIENDSHIP_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if(pmsg->dwData == 1)
				CHATMGR->AddMsg( CTC_SYSMSG, "There is No Summoned Pet" );
			else if(pmsg->dwData == 2)
				CHATMGR->AddMsg( CTC_SYSMSG, "Friendship Limit Error" );
		}
		break;
	case MP_CHEAT_PET_SELECTED_FRIENDSHIP_ACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			char tmp[256];
			wsprintf(tmp, "TargetPet'sFriendship has changed from %d to %d", pmsg->dwData1/100000, pmsg->dwData2/100000 );
			CHATMGR->AddMsg( CTC_SYSMSG, tmp );
		}
		break;
	case MP_CHEAT_PET_SELECTED_FRIENDSHIP_NACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			enum tempflag{Err_PFR_OverValue = 0xfffffa, Err_PFR_NonSummon = 0xfffffb};

			if(pmsg->dwData1 == Err_PFR_OverValue)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, "Friendship Limit Error" );
			}
			else if(pmsg->dwData2 == Err_PFR_NonSummon)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, "There is No Summoned Pet" );
			}
			else
			{
				ASSERT(0);
			}
		}
		break;
	//SW070626 보부상NPC
	case MP_CHEAT_BOBUSANGINFO_REQUEST_ACK:
		{
			MSG_BOBUSANG_INFO_REQUEST* pmsg = (MSG_BOBUSANG_INFO_REQUEST*)pMsg;
#ifdef _GMTOOL_ 
			BOBUSANGMGR->PrintInfo(&pmsg->bobusangInfo[0],"CUR", pmsg->appearanceState);
			BOBUSANGMGR->PrintInfo(&pmsg->bobusangInfo[1],"NEXT");
#endif
		}
		break;
	case MP_CHEAT_BOBUSANGINFO_REQUEST_NACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, "[BobusangInfo] Request failed : Channel Num is not valiable" );
		}
		break;
	case MP_CHEAT_BOBUSANG_LEAVE_ACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, "[BobusangInfo] Request Succeed" );
		}
		break;
	case MP_CHEAT_BOBUSANG_LEAVE_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			if( 99 == pmsg->dwData)
				CHATMGR->AddMsg( CTC_SYSMSG, "[LeaveBobusang] Request failed : Channel Num is not valiable" );
			else if( 999 == pmsg->dwData)
				CHATMGR->AddMsg( CTC_SYSMSG, "[LeaveBobusang] Request failed : Map Num is not valiable" );
			else if( 9999 == pmsg->dwData)
				CHATMGR->AddMsg( CTC_SYSMSG, "[LeaveBobusang] Request failed : There is no Bobusang in that channel" );
		}
		break;
	case MP_CHEAT_BOBUSANGINFO_CHANGE_ACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, "[Change BobusangInfo] Request Succeed" );
		}
		break;
	case MP_CHEAT_BOBUSANGINFO_CHANGE_NACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, "[Change BobusangInfo] Request failed : Channel Num is not valiable" );
		}
		break;
	case MP_CHEAT_BOBUSANG_ONOFF_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			if( TRUE == pmsg->dwData )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, "BOBUSANG is ACTIVED.." );
			}
			else
				CHATMGR->AddMsg( CTC_SYSMSG, "BOBUSNAG is DEACTIVED.." );
		}
		break;
	case MP_CHEAT_BOBUSANG_ONOFF_NACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, "BOBUSNAG ACTIVATION is NOT CHANGED" );
		}
		break;
#ifdef _JAPAN_LOCAL_
	case MP_CHEAT_CHANGE_MAINATTR_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if( HEROID == pmsg->dwObjectID )
			{
				HERO->SetMainCharAttr( LOWORD(pmsg->dwData) );
				HERO->SetSubCharAttr( 0 );

				CHATMGR->AddMsg( CTC_SYSMSG, "Change Character Main Attribute" );
				
				ITEMMGR->RefreshAllItem();
				GAMEIN->GetCharacterDialog()->UpdateData();
			}
		}
		break;
	case MP_CHEAT_CHANGE_SUBATTR_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if( HEROID == pmsg->dwObjectID )
			{
				HERO->SetMainCharAttr( LOWORD(pmsg->dwData) );
				HERO->SetSubCharAttr( HIWORD(pmsg->dwData) );
				CHATMGR->AddMsg( CTC_SYSMSG, "Change Character Sub Attribute" );

				ITEMMGR->RefreshAllItem();
				GAMEIN->GetCharacterDialog()->UpdateData();
			}
		}
		break;
	case MP_CHEAT_CHANGE_MAINATTR_NACK:
	case MP_CHEAT_CHANGE_SUBATTR_NACK:
		break;
#endif
	// 06. 05 HIDE NPC - 이영준
	case MP_CHEAT_NPCHIDE_ACK:
		{
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;

//#ifndef _HK_LOCAL_	//hk block
			OBJECTMGR->HideNpc(pmsg->wData1, (BOOL)pmsg->wData2);
//#endif
		}
		break;
	case MP_CHEAT_NPCHIDE_NACK:
		{
		}
		break;

	case MP_CHEAT_MAP_CONDITION:
		{
			MSG_PROSESSINFO* pmsg = (MSG_PROSESSINFO*)pMsg;

			int k = 0;
		}
		break;
/*
	case MP_CHEAT_FBOSSMONSTER_POSITION_ACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			
			CHATMGR->AddMsg( CTC_SYSMSG, "FBossMonster Position : [ %d ] [ %d ]", pmsg->dwData1/100, pmsg->dwData2/100);
		}
		break;

	case MP_CHEAT_FBOSSMONSTER_POSITION_NACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, "FeildBossMonster Not Regen." );
		}
		break;*/
	case MP_CHEAT_ITEMLIMIT_ACK:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

			if( pmsg->dwData1 == 1 )			// on/off
			{
				if( pmsg->dwData3 == 0 )
                    CHATMGR->AddMsg( CTC_SYSMSG, "아이템[%d]관련 이벤트가 Off되었습니다.", pmsg->dwData2 );
				else if( pmsg->dwData3 == 1 )
					CHATMGR->AddMsg( CTC_SYSMSG, "아이템[%d]관련 이벤트가 On되었습니다..", pmsg->dwData2 );

			}
			else if( pmsg->dwData1 == 2 )		// count
			{
				CHATMGR->AddMsg( CTC_SYSMSG, "아이템[%d]의 제한수가 %d개로 설정되었습니다.", pmsg->dwData2, pmsg->dwData3 );
			}
		}
		break;
	case MP_CHEAT_ITEMLIMIT_NACK:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			if( pmsg->dwData1 == 1 )			// on/off
			{
				CHATMGR->AddMsg( CTC_SYSMSG, "아이템[%d]의 제한설정이 불가능합니다.", pmsg->dwData2 );
			}
			else if( pmsg->dwData1 == 2 )		// count
			{
				CHATMGR->AddMsg( CTC_SYSMSG, "아이템[%d]의 제한설정이 불가능합니다.", pmsg->dwData2 );
			}
		}
		break;
	case MP_CHEAT_AUTONOTE_SETTING_ACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			switch( pmsg->dwData1 )
			{
			case 0:
				{
					if( pmsg->dwData2 == 0 )			// on/off
					{
						CHATMGR->AddMsg( CTC_SYSMSG, "살생부 사용불가능한 맵으로 설정되었습니다." );
					}
					else if( pmsg->dwData2 == 1 )		// on/off
					{
						CHATMGR->AddMsg( CTC_SYSMSG, "살생부 사용가능한 맵으로 설정되었습니다." );
					}
				}
				break;
			case 1:	CHATMGR->AddMsg( CTC_SYSMSG, "살생부 응답시간이 %d초로 설정되었습니다.", pmsg->dwData2 );	break;
			case 2:	CHATMGR->AddMsg( CTC_SYSMSG, "살생부 응답기회가 %d번으로 설정되었습니다.", pmsg->dwData2 );	break;
			case 3:	CHATMGR->AddMsg( CTC_SYSMSG, "살생부 재사용시간이 %분으로 설정되었습니다.", pmsg->dwData2 );	break;
			case 4:	CHATMGR->AddMsg( CTC_SYSMSG, "살생부 블럭시간이 %분으로 설정되었습니다.", pmsg->dwData2 );	break;
			case 5:
				{
					if( pmsg->dwData2 == 0 )			// on/off
					{
						CHATMGR->AddMsg( CTC_SYSMSG, "살생부 사용이 불가능하도록 설정되었습니다." );
					}
					else if( pmsg->dwData2 == 1 )		// on/off
					{
						CHATMGR->AddMsg( CTC_SYSMSG, "살생부 사용이 가능하도록 설정되었습니다." );
					}
				}
				break;
			}			
		}
		break;

	default:
		{
		}
		break;

	}
}
/*
void CCheatMsgParser::EventMonsterRegen(WORD type, WORD count)
{
	if( !m_bEvent ) return;

	MSG_EVENT_MONSTERREGEN msg;
	msg.Category	= MP_CHEAT;
	msg.Protocol	= MP_CHEAT_EVENT_MONSTER_REGEN;
	msg.dwObjectID	= HEROID;
//	msg.bBoss = 0;

	int i = 0;
	switch(type)
	{
	case 1:
		{
			for(i=0;i<count;++i)
			{
				msg.MonsterKind = 8;
				msg.ItemID = ExtractItem(0);
				msg.Pos = GetRegenPos();
				NETWORK->Send(&msg,sizeof(msg));
			}

			for(i=0;i<count;++i)
			{
				msg.MonsterKind = 12;
				msg.ItemID = ExtractItem(1);
				msg.Pos = GetRegenPos();
				NETWORK->Send(&msg,sizeof(msg));
			}

			for(i=0;i<count;++i)
			{
				msg.MonsterKind = 14;
				msg.ItemID = ExtractItem(2);
				msg.Pos = GetRegenPos();
				NETWORK->Send(&msg,sizeof(msg));
			}
		}
		break;
	case 2: // ¾×±I Au≫o≫c°￥·u °A´e≫e¼O
		{
			for(i=0;i<count;++i)
			{
				msg.MonsterKind = 19;
				msg.ItemID = ExtractItem(3);
				msg.Pos = GetRegenPos();
				NETWORK->Send(&msg,sizeof(msg));
			}

			for(i=0;i<count;++i)
			{
				msg.MonsterKind = 25;
				msg.ItemID = ExtractItem(4);
				msg.Pos = GetRegenPos();
				NETWORK->Send(&msg,sizeof(msg));
			}

			for(i=0;i<count;++i)
			{
				msg.MonsterKind = 24;
				msg.ItemID = ExtractItem(5);
				msg.Pos = GetRegenPos();
				NETWORK->Send(&msg,sizeof(msg));
			}
		}
		break;
	case 4: // 2±Þμ¿¿μAIAU 4±Þμ¿¿μAIAU ≫c¹≪¶oAI
		{
			for(i=0;i<count;++i)
			{
				msg.MonsterKind = 18;
				msg.ItemID = ExtractItem(6);
				msg.Pos = GetRegenPos();
				NETWORK->Send(&msg,sizeof(msg));
			}

			for(i=0;i<count;++i)
			{
				msg.MonsterKind = 16;
				msg.ItemID = ExtractItem(7);
				msg.Pos = GetRegenPos();
				NETWORK->Send(&msg,sizeof(msg));
			}

			for(i=0;i<count;++i)
			{
				msg.MonsterKind = 91;
				msg.ItemID = ExtractItem(8);
				msg.Pos = GetRegenPos();
				NETWORK->Send(&msg,sizeof(msg));
			}
		}
		break;
	case 3:
		{
			for(i=0;i<count;++i)
			{
				msg.MonsterKind = 84;
				msg.ItemID = ExtractItem(6);
				msg.Pos = GetRegenPos();
				NETWORK->Send(&msg,sizeof(msg));
			}

			for(i=0;i<count;++i)
			{
				msg.MonsterKind = 21;
				msg.ItemID = ExtractItem(7);
				msg.Pos = GetRegenPos();
				NETWORK->Send(&msg,sizeof(msg));
			}

			for(i=0;i<count;++i)
			{
				msg.MonsterKind = 99;
				msg.ItemID = ExtractItem(8);
				msg.Pos = GetRegenPos();
				NETWORK->Send(&msg,sizeof(msg));
			}
		}
		break;
	case 10: //E²¸ª
		{
			msg.MonsterKind = 1000;
			msg.ItemID = 0;
			msg.Pos = GetRegenPos();
//			msg.bBoss = 1;
			NETWORK->Send(&msg,sizeof(msg));
		}
		break;
	default:
		return;
	}
}

VECTOR3 CCheatMsgParser::GetRegenPos()
{
	VECTOR3 pos;
	int rd;
	VECTOR3 Curpos = HERO->GetCurPosition();

	rd = (rand() % 3000)-1500;
	pos.x = Curpos.x + rd;

	pos.y = Curpos.y;

	rd = (rand() % 3000)-1500;
	pos.z = Curpos.z + rd;

	return pos;
}

WORD CCheatMsgParser::ExtractItem(WORD idx)
{
	WORD rd = rand() % m_itemCount[idx];

	ITEM_INFO* pInfo;
	pInfo = ITEMMGR->FindItemInfoForName(m_eventitem[idx][rd].strName);
	if(pInfo)
		return pInfo->ItemIdx;

	return 0;
}
*/


//#endif	//_CHEATENABLE_	//KES CHEAT!!!
