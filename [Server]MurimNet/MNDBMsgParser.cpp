#include "stdafx.h"
#include "CommonDBMsgParser.h"
#include "MNDBMsgParser.h"
#include "MNPlayer.h"
#include "MurimNetSystem.h"
#include "MNPlayerManager.h"
//#include "PlayRoomManager.h"
#include "ChannelManager.h"
#include "Channel.h"


DBMsgFunc g_DBMsgFunc[MaxQuery] =
{
	RCharacterNumSendAndCharacterInfo,
};

void CharacterNumSendAndCharacterInfo(DWORD CharacterIDX)
{
	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_CHARACTER_SELECT, CharacterIDX);
	if(g_DB.Query(eQueryType_FreeQuery, eCharacterInfoQuery, 0, txt) == FALSE)
	{
	}
}

void	RCharacterNumSendAndCharacterInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
/*
	DWORD count = pMessage->dwResult;
	if(!count)
	{
		// 캐릭터 정보받기 실패오류
		ASSERTMSG(0,"DB에 케릭터 데이타가 없습니다.");
		return;
	}
	
	BASEOBJECT_INFO Objinfo;
	CHARACTER_TOTALINFO Totalinfo;
	HERO_TOTALINFO Heroinfo;
	BASEMOVE_INFO Moveinfo;
	memset(&Objinfo,0,sizeof(BASEOBJECT_INFO));
	memset(&Totalinfo,0,sizeof(CHARACTER_TOTALINFO));
	memset(&Heroinfo,0,sizeof(HERO_TOTALINFO));
	memset(&Moveinfo,0,sizeof(BASEMOVE_INFO));

	
	
	Objinfo.dwObjectID = atoi((char*)pData->Data[eCS_ObjectID]);
	Objinfo.dwUserID =  atoi((char*)pData->Data[eCS_UserID]);
	Objinfo.BattleID = 0;
	Objinfo.BattleTeam = 0;
	
	// UserIDX는 넘기지 않는다
	Heroinfo.PartyID = atoi((char*)pData->Data[eCS_PartyID]);
	
	Totalinfo.Gender = atoi((char*)pData->Data[eCS_Gender]);
	Heroinfo.wGenGol = atoi((char*)pData->Data[eCS_Gengoal]);
	Totalinfo.Life = atoi((char*)pData->Data[eCS_Life]);
	Totalinfo.Shield = atoi((char*)pData->Data[eCS_Shield]);
	Heroinfo.wNaeRyuk = atoi((char*)pData->Data[eCS_Energe]);
	Heroinfo.wMinChub = atoi((char*)pData->Data[eCS_Dex]);
	Heroinfo.wCheRyuk = atoi((char*)pData->Data[eCS_Sta]);
	Heroinfo.wSimMek = atoi((char*)pData->Data[eCS_Simmak]);
	Heroinfo.Fame = atoi((char*)pData->Data[eCS_Fame]);
	Heroinfo.wKarma = atoi((char*)pData->Data[eCS_Karma]);
	strcpy(Objinfo.ObjectName, (char*)pData->Data[eCS_Name]);
	strcpy(Totalinfo.HonorName, (char*)pData->Data[eCS_Repute]);
	Totalinfo.Stage = atoi((char*)pData->Data[eCS_Stage]);
	Totalinfo.Level = atoi((char*)pData->Data[eCS_Grade]);
	Heroinfo.ExpPoint = atoi((char*)pData->Data[eCS_Expoint]);
	Heroinfo.LevelUpPoint = atoi((char*)pData->Data[eCS_GradeUpPoint]);
	Heroinfo.Money = atoi((char*)pData->Data[eCS_Money]);
	Totalinfo.CurMapNum = 99;
	Totalinfo.LoginMapNum = atoi((char*)pData->Data[eCS_Map]);
	Totalinfo.FaceType = atoi((char*)pData->Data[eCS_FaceType]);
	Totalinfo.HairType = atoi((char*)pData->Data[eCS_HeadType]);
	Totalinfo.WearedItemIdx[eWearedItem_Hat] = atoi((char*)pData->Data[eCS_Hat]);
	Totalinfo.WearedItemIdx[eWearedItem_Weapon] = atoi((char*)pData->Data[eCS_Weapon]);
	Totalinfo.WearedItemIdx[eWearedItem_Dress] = atoi((char*)pData->Data[eCS_Dress]);
	Totalinfo.WearedItemIdx[eWearedItem_Shoes] = atoi((char*)pData->Data[eCS_Shoes]);
	Totalinfo.WearedItemIdx[eWearedItem_Ring1] = atoi((char*)pData->Data[eCS_Ring1]);
	Totalinfo.WearedItemIdx[eWearedItem_Ring2] = atoi((char*)pData->Data[eCS_Ring2]);
	Totalinfo.WearedItemIdx[eWearedItem_Cape] = atoi((char*)pData->Data[eCS_Cape]);
	Totalinfo.WearedItemIdx[eWearedItem_Necklace] = atoi((char*)pData->Data[eCS_Necklace]);
	Totalinfo.WearedItemIdx[eWearedItem_Armlet] = atoi((char*)pData->Data[eCS_Armlet]);
	Totalinfo.WearedItemIdx[eWearedItem_Belt] = atoi((char*)pData->Data[eCS_Belt]);
	Totalinfo.MunpaID = atoi((char*)pData->Data[eCS_MunpaIDX]);
	Totalinfo.PositionInMunpa = atoi((char*)pData->Data[eCS_MunpaPositon]);

	strcpy(Heroinfo.MunpaName, (char*)pData->Data[eCS_MunpaName]);
	Heroinfo.Ability[0] = atoi((char*)pData->Data[eCS_Ability1]);
	Heroinfo.Ability[1] = atoi((char*)pData->Data[eCS_Ability2]);
	Heroinfo.Ability[2] = atoi((char*)pData->Data[eCS_Ability3]);
	Heroinfo.Ability[3] = atoi((char*)pData->Data[eCS_Ability4]);
	Heroinfo.Ability[4] = atoi((char*)pData->Data[eCS_Ability5]);
	Heroinfo.Ability[5] = atoi((char*)pData->Data[eCS_Ability6]);
	Heroinfo.Ability[6] = atoi((char*)pData->Data[eCS_Ability7]);
	
	Totalinfo.MapChangePoint_Index = atoi((char*)pData->Data[eCS_MapChangePoint]);
	Totalinfo.LoginPoint_Index = atoi((char*)pData->Data[eCS_LoginPoint]);
	
	Heroinfo.KyungGongGrade = atoi((char*)pData->Data[eCS_QuickPace]);
	
	CMNPlayer* pPlayer = g_MNSystem.GetMNPlayerManager()->FindPlayer(Objinfo.dwObjectID);
	if(pPlayer == NULL)
		return;

	pPlayer->InitPlayerInfo( &Objinfo, &Totalinfo, &Heroinfo );
*/

	DWORD count = pMessage->dwResult;
	if(!count)
	{
		// 캐릭터 정보받기 실패오류
		ASSERTMSG(0,"DB에 케릭터 데이타가 없습니다.");
		return;
	}
	
	BASEOBJECT_INFO Objinfo;
	CHARACTER_TOTALINFO Totalinfo;
	HERO_TOTALINFO Heroinfo;
	BASEMOVE_INFO Moveinfo;
	memset(&Objinfo,0,sizeof(BASEOBJECT_INFO));
	memset(&Totalinfo,0,sizeof(CHARACTER_TOTALINFO));
	memset(&Heroinfo,0,sizeof(HERO_TOTALINFO));
	memset(&Moveinfo,0,sizeof(BASEMOVE_INFO));

	
	
	Objinfo.dwObjectID = atoi((char*)pData->Data[eCS_ObjectID]);
	Objinfo.dwUserID =  atoi((char*)pData->Data[eCS_UserID]);
	Objinfo.BattleID = 0;
	Objinfo.BattleTeam = 0;
	
	// UserIDX는 넘기지 않는다
	Heroinfo.PartyID = atoi((char*)pData->Data[eCS_PartyID]);
	
	Totalinfo.Gender = atoi((char*)pData->Data[eCS_Gender]);
	Heroinfo.wGenGol = atoi((char*)pData->Data[eCS_Gengoal]);
	Totalinfo.Life = atoi((char*)pData->Data[eCS_Life]);
	Totalinfo.Shield = atoi((char*)pData->Data[eCS_Shield]);
	Heroinfo.wNaeRyuk = atoi((char*)pData->Data[eCS_Energe]);
	Heroinfo.wMinChub = atoi((char*)pData->Data[eCS_Dex]);
	Heroinfo.wCheRyuk = atoi((char*)pData->Data[eCS_Sta]);
	Heroinfo.wSimMek = atoi((char*)pData->Data[eCS_Simmak]);
	Heroinfo.Fame = atoi((char*)pData->Data[eCS_Fame]);
	Heroinfo.wKarma = atoi((char*)pData->Data[eCS_Karma]);
//	strcpy(Objinfo.ObjectName, (char*)pData->Data[eCS_Name]);
	SafeStrCpy( Objinfo.ObjectName, (char*)pData->Data[eCS_Name], MAX_NAME_LENGTH + 1 );
//	strcpy(Totalinfo.HonorName, (char*)pData->Data[eCS_Repute]);
//	SafeStrCpy( Totalinfo.HonorName, (char*)pData->Data[eCS_Repute], MAX_NAME_LENGTH + 1 );
	Totalinfo.Stage = atoi((char*)pData->Data[eCS_Stage]);
	Totalinfo.Level = atoi((char*)pData->Data[eCS_Grade]);
	Heroinfo.ExpPoint = atoi((char*)pData->Data[eCS_Expoint]);
	Heroinfo.LevelUpPoint = atoi((char*)pData->Data[eCS_GradeUpPoint]);
	Heroinfo.Money = atoi((char*)pData->Data[eCS_Money]);
	Totalinfo.CurMapNum = 99;
	Totalinfo.LoginMapNum = atoi((char*)pData->Data[eCS_Map]);
	Totalinfo.FaceType = atoi((char*)pData->Data[eCS_FaceType]);
	Totalinfo.HairType = atoi((char*)pData->Data[eCS_HeadType]);
	Totalinfo.WearedItemIdx[eWearedItem_Hat] = atoi((char*)pData->Data[eCS_Hat]);
	Totalinfo.WearedItemIdx[eWearedItem_Weapon] = atoi((char*)pData->Data[eCS_Weapon]);
	Totalinfo.WearedItemIdx[eWearedItem_Dress] = atoi((char*)pData->Data[eCS_Dress]);
	Totalinfo.WearedItemIdx[eWearedItem_Shoes] = atoi((char*)pData->Data[eCS_Shoes]);
	Totalinfo.WearedItemIdx[eWearedItem_Ring1] = atoi((char*)pData->Data[eCS_Ring1]);
	Totalinfo.WearedItemIdx[eWearedItem_Ring2] = atoi((char*)pData->Data[eCS_Ring2]);
	Totalinfo.WearedItemIdx[eWearedItem_Cape] = atoi((char*)pData->Data[eCS_Cape]);
	Totalinfo.WearedItemIdx[eWearedItem_Necklace] = atoi((char*)pData->Data[eCS_Necklace]);
	Totalinfo.WearedItemIdx[eWearedItem_Armlet] = atoi((char*)pData->Data[eCS_Armlet]);
	Totalinfo.WearedItemIdx[eWearedItem_Belt] = atoi((char*)pData->Data[eCS_Belt]);
	Totalinfo.MunpaID = atoi((char*)pData->Data[eCS_MunpaIDX]);
	Totalinfo.PositionInMunpa = atoi((char*)pData->Data[eCS_MunpaPositon]);

//	strcpy(Heroinfo.MunpaName, (char*)pData->Data[eCS_MunpaName]);
	SafeStrCpy(Heroinfo.MunpaName, (char*)pData->Data[eCS_MunpaName], MAX_MUNPA_NAME+1);

	
	Totalinfo.MapChangePoint_Index = atoi((char*)pData->Data[eCS_MapChangePoint]);
	Totalinfo.LoginPoint_Index = atoi((char*)pData->Data[eCS_LoginPoint]);
	
	Heroinfo.KyungGongGrade = atoi((char*)pData->Data[eCS_QuickPace]);
	
	Heroinfo.AbilityExp = 0;//atoi((char*)pData->Data[eCS_AbilityExp]);
	
//	VECTOR3 pos,RandPos;
//	GAMERESRCMNGR->GetRegenPoint(pos,RandPos,Totalinfo.LoginPoint_Index,Totalinfo.MapChangePoint_Index);

//	CPlayer* pPlayer = g_pServerSystem->InitPlayerInfo(&Objinfo, &Totalinfo, &Heroinfo);
//	if(pPlayer == NULL)
//		return;

	CMNPlayer* pPlayer = g_MNSystem.GetMNPlayerManager()->FindPlayer(Objinfo.dwObjectID);
	if(pPlayer == NULL)
		return;

	pPlayer->InitPlayerInfo( &Objinfo, &Totalinfo, &Heroinfo );

//	pos = g_pServerSystem->GetMap()->GetTileManager()->GetNextCloserPos(pPlayer,&RandPos,&pos,70,170);
//	CCharMove::InitMove(pPlayer,&pos);
	
//	pPlayer->SetInitState(PLAYERINITSTATE_HERO_INFO,pMessage->dwID);

//---Enter Player into Default-Channel
	g_MNSystem.GetChannelManager()->GetDefaultChannel()->PlayerIn( pPlayer );

	
//확인요.
//---접속성공 알림(게임룸이나, 채널에 들어간후에 보내야함, hero setting을 위해서)
	MSGBASE msg;
	msg.Category	= MP_MURIMNET;
	msg.Protocol	= MP_MURIMNET_CONNECT_ACK;
	msg.dwObjectID	= pPlayer->GetID();

	pPlayer->SendMsg( &msg, sizeof( msg ) );
}