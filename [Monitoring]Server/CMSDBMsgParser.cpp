#include "stdafx.h"
#include "stdio.h"
#include "DataBase.h"
#include "CMSDBMsgParser.h"
#include "MonitorNetwork.h"
#include "UserIPCheckMgr.h"


#ifdef DBID
#undef DBID
#endif
#define DBID(a)

#ifdef DBRETID
#undef DBRETID
#endif
#define DBRETID(a) a,

DBMsgFunc g_DBMsgFunc[MaxQuery] =
{
	#include "MSDBMsgID.h"
};

char txt[512];
//----------------------------------------------------------------------------------------------
// DBÄõ¸®ÇÏ´Â ÇÔ¼ö
// Programmer°¡ »ç¿ëÇÏ´Â ºÎºÐ
void InsertIpAdress(WORD wConnectIdx, WORD wTempIdx, char* strIp)
{
//	char txt[128];
	sprintf(txt, "EXEC RP_InsertIpAdress \'%s\'", strIp);	
	if(g_DB.LoginQuery(eQueryType_FreeQuery, eOperInsertIp, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void InsertOperator(WORD wConnectIdx, WORD wTempIdx, char* strId, char* strName, char* strPwd, WORD power)
{
//	char txt[128];
	sprintf(txt, "EXEC RP_InsertOperator \'%s\', \'%s\', \'%s\', %d", strId, strName, strPwd, power);	
	if(g_DB.LoginQuery(eQueryType_FreeQuery,eOperInsertOper, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void DeleteIpAdress(WORD wConnectIdx, WORD wTempIdx, DWORD ipIdx)
{
//	char txt[128];
	sprintf(txt, "EXEC RP_DeleteIp %d", ipIdx);	
	if(g_DB.LoginQuery(eQueryType_FreeQuery, eOperDeleteIp, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void DeleteOperator(WORD wConnectIdx, WORD wTempIdx, DWORD operIdx)
{
//	char txt[128];
	sprintf(txt, "EXEC RP_DeleteOperator %d", operIdx);	
	if(g_DB.LoginQuery(eQueryType_FreeQuery, eOperDeleteOper, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void TotalIpAdressInfo(WORD wConnectIdx, WORD wTempIdx, DWORD startIdx)
{
//	char txt[128];
	sprintf(txt, "EXEC RP_IpAdressInfo %d", startIdx);	
	if(g_DB.LoginQuery(eQueryType_FreeQuery, eOperTotalIpInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void TotalOperatorInfo(WORD wConnectIdx, WORD wTempIdx, DWORD startIdx, DWORD operIdx)
{
//	char txt[128];
	sprintf(txt, "EXEC RP_OperatorInfo %d, %d", startIdx, operIdx);	
	if(g_DB.LoginQuery(eQueryType_FreeQuery, eOperTotalOperInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void OperatorLoginCheck(DWORD dwConnectIdx, char* strId, char* strPwd, char* strIp)
{
//	char txt[128];
	sprintf(txt, "EXEC RP_OperatorLoginCheck \'%s\', \'%s\', \'%s\'", strId, strPwd, strIp);	
	if(g_DB.LoginQuery(eQueryType_FreeQuery, eOperLoginCheck, dwConnectIdx, txt) == FALSE)
	{
	}
}

void SearchUserInfoById(WORD wConnectIdx, WORD wTempIdx, WORD wServer, char* strId)
{
//	char txt[128];
	sprintf(txt, "EXEC RP_SearchUserInfoById %d, \'%s\'", wServer, strId);	
	if(g_DB.LoginMiddleQuery(RSearchUserInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void SearchUserInfoByIdCard(WORD wConnectIdx, WORD wTempIdx, WORD wServer, char* strIdCard)
{
	char Card1[7] = {0,};
	char Card2[8] = {0,};
	memcpy(Card1, strIdCard, 6);
	memcpy(Card2, &strIdCard[6], 7);

//	char txt[128];
	sprintf(txt, "EXEC RP_SearchUserInfoByIdCard %d, \'%s\', \'%s\'", wServer, Card1, Card2);	
	if(g_DB.LoginMiddleQuery(RSearchUserInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void SearchUserInfoByCharName(WORD wConnectIdx, WORD wTempIdx, WORD wServer, char* strCharName)
{
//	char txt[128];
	sprintf(txt, "EXEC RP_SearchUserInfoByCharName %d, \'%s\'", wServer, strCharName);	
	if(g_DB.LoginMiddleQuery(RSearchUserInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}
/*
void SearchSimpleCharacterInfo(WORD wConnectIdx, WORD wTempIdx, WORD wServer, DWORD MemberNo)
{	
//	char txt[128];
	sprintf(txt, "EXEC RP_SearchCharacterSimpleInfo %d, %d", wServer, MemberNo);	
	if(g_DB.LoginMiddleQuery(RSearchSimpleCharacterInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}
*/


void CharacterBaseInfo(WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx)
{
//	char txt[128];
	sprintf(txt, "EXEC RP_CharacterInfo %d", dwCharIdx);	
	if(g_DB.Query(eQueryType_FreeQuery, eCharacterBaseInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void UpdateCharacterBaseInfo(WORD wConnectIdx, WORD wTempIdx, TMSG_CHARACTER_UPDATEBASEINFO* Info)
{
//	char txt[128];
	sprintf(txt, "EXEC RP_UpdateCharacterInfo %d,\'%s\', \'%s\', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", 
													Info->CharIdx, Info->ObjectName, Info->NickName, Info->Stage, 
													Info->Level, Info->Fame, Info->ExpPoint, Info->wGenGol, 
													Info->wMinChub, Info->wCheRyuk, Info->wSimMek, Info->LevelUpPoint, 
													Info->Money, Info->AbilityExp);
	if(g_DB.Query(eQueryType_FreeQuery, eCharacterUpdateBaseInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}


void CharacterInvenInfo(WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx)
{
//	char txt[128];
	sprintf(txt, "EXEC RP_CharacterInvenInfo %d", dwCharIdx);	
	if(g_DB.Query(eQueryType_FreeQuery, eCharacterInvenInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void CharacterPyogukInfo(WORD wConnectIdx, WORD wTempIdx, DWORD dwUserIdx)
{
//	char txt[128];
	sprintf(txt, "EXEC RP_CharacterPyogukInfo %d", dwUserIdx);	
	if(g_DB.Query(eQueryType_FreeQuery, eCharacterPyogukInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void UpdateCharacterPyogukInfo(WORD wConnectIdx, WORD wTempIdx, DWORD UserIdx, WORD PyogukNum, MONEYTYPE Money)
{
//	char txt[128];
	sprintf(txt, "EXEC RP_UpdatePyogukInfo %d, %d, %d", UserIdx, PyogukNum, Money );	
	if(g_DB.Query(eQueryType_FreeQuery, eCharacterUpdatePyogukInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void CharacterPyogukItemInfo(WORD wConnectIdx, WORD wTempIdx, DWORD dwUserIdx, POSTYPE startPos)
{
//	char txt[128];
	sprintf(txt, "EXEC RP_CharacterPyogukItemInfo %d, %d", dwUserIdx, startPos);	
	if(g_DB.Query(eQueryType_FreeQuery, eCharacterPyogukItemInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void CharacterMugongInfo(WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx)
{
//	char txt[128];
	sprintf(txt, "EXEC RP_CharacterMugongInfo %d", dwCharIdx);	
	if(g_DB.Query(eQueryType_FreeQuery, eCharacterMugongInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void CharacterAbilityInfo(WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx)
{
//	char txt[128];
	sprintf(txt, "EXEC RP_CharacterAbilityInfo %d", dwCharIdx);	
	if(g_DB.FreeMiddleQuery(RCharacterAbilityInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void CharacterItemOptionInfo(WORD wConnectIdx, WORD wTempIdx, DWORD dwUserIdx, DWORD dwCharIdx, DWORD StartIdx)
{
//	char txt[128];
	sprintf(txt, "EXEC  RP_CharacterItemOptionInfo %d, %d, %d", dwUserIdx, dwCharIdx, StartIdx);	
	if(g_DB.Query(eQueryType_FreeQuery, eCharacterItemOptionInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void SearchMunpaIdxByName(WORD wConnectIdx, WORD wTempIdx, WORD wServer, char* strName)
{
//	char txt[128];
	sprintf(txt, "EXEC RP_SearchMunpaByName \'%s\'", strName);	
	if(g_DB.Query(eQueryType_FreeQuery, eSearchMunpaByName, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void MunpaBaseInfo(WORD wConnectIdx, WORD wTempIdx, DWORD MunpaIdx)
{
//	char txt[128];
	sprintf(txt, "EXEC RP_MunpaBaseInfo %d", MunpaIdx);	
	if(g_DB.Query(eQueryType_FreeQuery, eMunpaBaseInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void MunpaItemInfo(WORD wConnectIdx, WORD wTempIdx, DWORD MunpaIdx, POSTYPE startPos )
{
//	char txt[128];
	sprintf(txt, "EXEC RP_MunpaItemInfo %d, %d", MunpaIdx, startPos);	
	if(g_DB.Query(eQueryType_FreeQuery, eMunpaItemInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void MunpaMemberInfo(WORD wConnectIdx, WORD wTempIdx, DWORD MunpaIdx, DWORD startIdx)
{
//	char txt[128];
	sprintf(txt, "EXEC RP_MunpaMemberInfo %d, %d", MunpaIdx, startIdx);	
	if(g_DB.Query(eQueryType_FreeQuery, eMunpaMemberInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void GameLogItemMoveLog(WORD wConnectIdx, WORD wTempIdx, DWORD startIdx, WORD wServer, char* strId, BYTE bDay, char* strStart, char* strEnd)
{
//	char txt[128];
	sprintf(txt, "EXEC RP_GameLogItemMoveLog %d, \'%s\', %d, \'%s\', \'%s\'", startIdx, strId, bDay, strStart, strEnd);
	if(g_DB.Query(eQueryType_FreeQuery, eGameItemMoveLog, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void GameLogItemObtainLog(WORD wConnectIdx, WORD wTempIdx, DWORD startIdx, WORD wServer, char* strId, BYTE bDay, char* strStart, char* strEnd)
{
//	char txt[128];
	sprintf(txt, "EXEC RP_GameLogItemObtainLog %d, \'%s\', %d, \'%s\', \'%s\'", startIdx, strId, bDay, strStart, strEnd);
	if(g_DB.Query(eQueryType_FreeQuery, eGameItemObtainLog, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void GameLogBaseInfoLog(WORD wConnectIdx, WORD wTempIdx, DWORD startIdx, WORD wServer, char* strId, BYTE bDay, char* strStart, char* strEnd)
{
//	char txt[128];
	sprintf(txt, "EXEC RP_GameLogBaseInfoLog %d, \'%s\', %d, \'%s\', \'%s\'", startIdx, strId, bDay, strStart, strEnd);
	if(g_DB.Query(eQueryType_FreeQuery, eGameBaseInfoLog, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void GameLogMoneyLog(WORD wConnectIdx, WORD wTempIdx, DWORD startIdx, WORD wServer, char* strId, BYTE bDay, char* strStart, char* strEnd)
{
//	char txt[128];
	sprintf(txt, "EXEC RP_GameLogMoneyLog %d, \'%s\', %d, \'%s\', \'%s\'", startIdx, strId, bDay, strStart, strEnd);
	if(g_DB.Query(eQueryType_FreeQuery, eGameMoneyLog, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void GameLogItemFromChrName( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, WORD wServer, BYTE bDay, char* strStart, char* strEnd, char* strChrName )
{
//	char txt[128];
	sprintf( txt, "EXEC RP_GameLogItemFromChrName %d, \'%s\', %d, \'%s\', \'%s\'", dwStartIdx, strChrName, bDay, strStart, strEnd );
	if(g_DB.Query(eQueryType_FreeQuery, eGameItemFromChrName, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void GameLogItemFromItemDBIndex( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, WORD wServer, BYTE bDay, char* strStart, char* strEnd, DWORD dwItemDBIdx )
{
//	char txt[128];
	sprintf( txt, "EXEC RP_GameLogItemFromItemDBIdx %d, %d, %d, \'%s\', \'%s\'", dwStartIdx, dwItemDBIdx, bDay, strStart, strEnd );
	if(g_DB.Query(eQueryType_FreeQuery, eGameItemFromItemDBIndex, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void ItemDelete(DWORD dwItemDBIdx, DWORD dwOptionDBIdx)
{
//	char txt[128];
	sprintf(txt, "EXEC RP_ItemDelete %d, %d", dwItemDBIdx, dwOptionDBIdx);
	if(g_DB.Query(eQueryType_FreeQuery, eItemDelete, 0, txt) == FALSE)
	{
	}
}

void InvenItemInsert( WORD wConnectIdx, WORD wTempIdx, char* sCharName, DWORD dwCharIdx, ITEMBASE* pItem, ITEM_OPTION_INFO* pOptionInfo)
{
//	char txt[256];
	sprintf(txt, "EXEC RP_InvenItemInsert \'%s\', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d,  %d, %d, %d, %d, %d,  %d, %d,  %d, %d, %d, %d, %d,  %d",
		sCharName,
		dwCharIdx, 
		pItem->wIconIdx,
		pItem->Durability,
		pItem->Position,
		pOptionInfo->dwOptionIdx,
		pOptionInfo->GenGol,					
		pOptionInfo->MinChub,				
		pOptionInfo->CheRyuk,				
		pOptionInfo->SimMek,				
		pOptionInfo->Life,					
		pOptionInfo->NaeRyuk,			
		pOptionInfo->Shield,				
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_FIRE)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_WATER)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_TREE)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_IRON)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_EARTH)),
		pOptionInfo->PhyAttack,
		pOptionInfo->CriticalPercent,
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_FIRE)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_WATER)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_TREE)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_IRON)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_EARTH)),
		pOptionInfo->PhyDefense);
	if(g_DB.Query(eQueryType_FreeQuery, eInvenItemInsert, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RInvenItemInsert(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );
	DWORD count = pMessage->dwResult;

	if( pMessage->dwResult == 1 )
	{
		char name[17];
		strcpy( name, (char*)pData[0].Data[0] );
		DWORD dwCharIdx = (DWORD)atoi((char*)pData[0].Data[1]);
		ITEMBASE item;
		item.dwDBIdx = (DWORD)atoi((char*)pData[0].Data[2]);
		item.wIconIdx = (DWORD)atoi((char*)pData[0].Data[3]);
		item.Position = (WORD)atoi((char*)pData[0].Data[4]);
		item.QuickPosition = (WORD)atoi((char*)pData[0].Data[5]);
		item.Durability = (WORD)atoi((char*)pData[0].Data[6]);

		// log
		OPERID* pData = IPCHECKMGR->GetID( connectIdx );
		if( strcmp(pData->sID, "") == 0 )
			return;

		InsertLogTool( eToolLog_RM, eRMLog_ItemInsert, pData->dwIdx, pData->sID, dwCharIdx, name,
					   item.dwDBIdx, item.wIconIdx, item.Position, item.Durability );

//		InsertItemMoneyLog( 0, "", dwCharIdx, name, eLog_RMTool, 0, 0, 0, item.wIconIdx, item.dwDBIdx,
//							item.Position, item.Position, item.Durability, 0 );
	}
}

void PyogukItemInsert( WORD wConnectIdx, WORD wTempIdx, DWORD dwUserIdx, ITEMBASE* pItem, ITEM_OPTION_INFO* pOptionInfo )
{
//	char txt[256];
	sprintf(txt, "EXEC RP_PyogukItemInsert %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d,  %d, %d, %d, %d, %d,  %d, %d,  %d, %d, %d, %d, %d,  %d",
		dwUserIdx, 
		pItem->wIconIdx,
		pItem->Durability,
		pItem->Position,
		pOptionInfo->dwOptionIdx,
		pOptionInfo->GenGol,					
		pOptionInfo->MinChub,				
		pOptionInfo->CheRyuk,				
		pOptionInfo->SimMek,				
		pOptionInfo->Life,					
		pOptionInfo->NaeRyuk,			
		pOptionInfo->Shield,				
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_FIRE)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_WATER)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_TREE)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_IRON)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_EARTH)),
		pOptionInfo->PhyAttack,
		pOptionInfo->CriticalPercent,
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_FIRE)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_WATER)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_TREE)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_IRON)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_EARTH)),
		pOptionInfo->PhyDefense);
	if(g_DB.Query(eQueryType_FreeQuery, ePyogukItemInsert, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RPyogukItemInsert(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );
	DWORD count = pMessage->dwResult;

	if( pMessage->dwResult == 1 )
	{
		DWORD dwCharIdx = (DWORD)atoi((char*)pData[0].Data[0]);
		DWORD dwItemDBIdx = (DWORD)atoi((char*)pData[0].Data[1]);
		DWORD dwItemIdx = (DWORD)atoi((char*)pData[0].Data[2]);
		WORD wItemPos = (WORD)atoi((char*)pData[0].Data[3]);
		WORD wItemQPos = (WORD)atoi((char*)pData[0].Data[4]);
		DWORD dwItemDu = (DWORD)atoi((char*)pData[0].Data[5]);
		DWORD dwMunpaIdx = (DWORD)atoi((char*)pData[0].Data[6]);
		DWORD dwPyogukIdx = (DWORD)atoi((char*)pData[0].Data[7]);

		// log
		OPERID* pData = IPCHECKMGR->GetID( connectIdx );
		if( strcmp(pData->sID, "") == 0 )
			return;

		InsertLogTool( eToolLog_RM, eRMLog_ItemInsert, pData->dwIdx, pData->sID, dwPyogukIdx, "",
					   dwItemDBIdx, dwItemIdx, wItemPos, dwItemDu );

//		InsertItemMoneyLog( 0, "", dwPyogukIdx, "", eLog_RMTool, 0, 0, 0, dwItemIdx, dwItemDBIdx,
//							wItemPos, wItemPos, dwItemDu, 0 );
	}
}

void MunpaItemInsert( WORD wConnectIdx, WORD wTempIdx, DWORD dwMunpaIdx, ITEMBASE* pItem, ITEM_OPTION_INFO* pOptionInfo )
{
//	char txt[256];
	sprintf(txt, "EXEC RP_MunpaItemInsert %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d,  %d, %d, %d, %d, %d,  %d, %d,  %d, %d, %d, %d, %d,  %d",
		dwMunpaIdx, 
		pItem->wIconIdx,
		pItem->Durability,
		pItem->Position,
		pOptionInfo->dwOptionIdx,
		pOptionInfo->GenGol,					
		pOptionInfo->MinChub,				
		pOptionInfo->CheRyuk,				
		pOptionInfo->SimMek,				
		pOptionInfo->Life,					
		pOptionInfo->NaeRyuk,			
		pOptionInfo->Shield,				
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_FIRE)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_WATER)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_TREE)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_IRON)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_EARTH)),
		pOptionInfo->PhyAttack,
		pOptionInfo->CriticalPercent,
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_FIRE)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_WATER)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_TREE)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_IRON)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_EARTH)),
		pOptionInfo->PhyDefense);
	if(g_DB.Query(eQueryType_FreeQuery, eMunpaItemInsert, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RMunpaItemInsert(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );
	DWORD count = pMessage->dwResult;

	if( pMessage->dwResult == 1 )
	{
		DWORD dwCharIdx = (DWORD)atoi((char*)pData[0].Data[0]);
		DWORD dwItemDBIdx = (DWORD)atoi((char*)pData[0].Data[1]);
		DWORD dwItemIdx = (DWORD)atoi((char*)pData[0].Data[2]);
		WORD wItemPos = (WORD)atoi((char*)pData[0].Data[3]);
		WORD wItemQPos = (WORD)atoi((char*)pData[0].Data[4]);
		DWORD dwItemDu = (DWORD)atoi((char*)pData[0].Data[5]);
		DWORD dwMunpaIdx = (DWORD)atoi((char*)pData[0].Data[6]);
		DWORD dwPyogukIdx = (DWORD)atoi((char*)pData[0].Data[7]);

		// log
		OPERID* pData = IPCHECKMGR->GetID( connectIdx );
		if( strcmp(pData->sID, "") == 0 )
			return;

		InsertLogTool( eToolLog_RM, eRMLog_ItemInsert, pData->dwIdx, pData->sID, dwMunpaIdx, "",
					   dwItemDBIdx, dwItemIdx, wItemPos, dwItemDu );

//		InsertItemMoneyLog( 0, "", dwMunpaIdx, "", eLog_RMTool, 0, 0, 0, dwItemIdx, dwItemDBIdx,
//							wItemPos, wItemPos, dwItemDu, 0 );
	}
}

void ItemUpdate(DWORD dwDBIdx, ITEMBASE* pItem)
{
//	char txt[128];
	sprintf(txt, "EXEC RP_ItemUpdate %d, %d, %d, %d", dwDBIdx, pItem->wIconIdx, pItem->Durability, pItem->Position);
	if(g_DB.Query(eQueryType_FreeQuery, eItemUpdate, 0, txt) == FALSE)
	{
	}
}

void ItemOptionUpdate(DWORD dwItemDBIdx, ITEM_OPTION_INFO* pOptionInfo)
{
//	char txt[256];
	sprintf(txt, "EXEC RP_ItemOptionUpdate %d, %d, %d, %d, %d, %d, %d, %d,  %d, %d, %d, %d, %d,  %d, %d,  %d, %d, %d, %d, %d,  %d",
		dwItemDBIdx, 
		pOptionInfo->GenGol,					
		pOptionInfo->MinChub,				
		pOptionInfo->CheRyuk,				
		pOptionInfo->SimMek,				
		pOptionInfo->Life,					
		pOptionInfo->NaeRyuk,			
		pOptionInfo->Shield,				
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_FIRE)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_WATER)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_TREE)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_IRON)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_EARTH)),
		pOptionInfo->PhyAttack,
		pOptionInfo->CriticalPercent,
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_FIRE)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_WATER)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_TREE)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_IRON)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_EARTH)),
		pOptionInfo->PhyDefense);
	if(g_DB.Query(eQueryType_FreeQuery, eItemOptionUpdate, 0, txt) == FALSE)
	{
	}
}

void ItemOptionInsert(DWORD dwItemDBIdx, ITEM_OPTION_INFO* pOptionInfo)
{
//	char txt[256];
	sprintf(txt, "EXEC RP_ItemOptionInsert %d, %d, %d, %d, %d, %d, %d, %d,  %d, %d, %d, %d, %d,  %d, %d,  %d, %d, %d, %d, %d,  %d",
		dwItemDBIdx, 
		pOptionInfo->GenGol,					
		pOptionInfo->MinChub,				
		pOptionInfo->CheRyuk,				
		pOptionInfo->SimMek,				
		pOptionInfo->Life,					
		pOptionInfo->NaeRyuk,			
		pOptionInfo->Shield,				
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_FIRE)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_WATER)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_TREE)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_IRON)),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_EARTH)),
		pOptionInfo->PhyAttack,
		pOptionInfo->CriticalPercent,
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_FIRE)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_WATER)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_TREE)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_IRON)),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_EARTH)),
		pOptionInfo->PhyDefense);
	if(g_DB.Query(eQueryType_FreeQuery, eItemOptionInsert, 0, txt) == FALSE)
	{
	}
}

void ItemOptionDelete(DWORD dwItemDBIdx)
{
//	char txt[128];
	sprintf(txt, "EXEC RP_ItemOptionDelete %d", dwItemDBIdx);
	if(g_DB.Query(eQueryType_FreeQuery, eItemOptionDelete, 0, txt) == FALSE)
	{
	}
}

void MugongInsert(DWORD dwCharIdx, MUGONGBASE* pMugong)
{
//	char txt[128];
	sprintf(txt, "EXEC RP_MugongInsert %d, %d, %d, %d, %d, %d", dwCharIdx, pMugong->wIconIdx, pMugong->ExpPoint, pMugong->Sung, pMugong->Position, pMugong->bWear);
	if(g_DB.Query(eQueryType_FreeQuery, eMugongInsert, 0, txt) == FALSE)
	{
	}
}

void MugongUpdate(DWORD dwMugongDBIdx, MUGONGBASE* pMugong)
{
//	char txt[128];
	sprintf(txt, "EXEC RP_MugongUpdate %d, %d, %d, %d", dwMugongDBIdx, pMugong->wIconIdx, pMugong->ExpPoint, pMugong->Sung);
	if(g_DB.Query(eQueryType_FreeQuery, eMugongUpdate, 0, txt) == FALSE)
	{
	}
}

void MugongDelete(DWORD dwMugongDBIdx)
{
//	char txt[128];
	sprintf(txt, "EXEC RP_MugongDelete %d", dwMugongDBIdx);
	if(g_DB.Query(eQueryType_FreeQuery, eMugongDelete, 0, txt) == FALSE)
	{
	}
}

void AbilityUpdate(DWORD dwCharIdx, ABILITY_TOTALINFO* pAbility)
{
//	char txt[256];
	sprintf(txt, "RP_AbilityUpdate %d, \'%s\', \'%s\', \'%s\', \'%s\'",
			dwCharIdx, pAbility->AbilityDataArray[eAGK_Battle], pAbility->AbilityDataArray[eAGK_KyungGong],
			pAbility->AbilityDataArray[eAGK_Society], pAbility->AbilityDataArray[eAGK_Job]);

	if(g_DB.Query(eQueryType_FreeQuery, eAbilityUpdate, 0, txt) == FALSE)
	{
	}
}

void CharacterWearItemUpdate(DWORD dwCharIdx, BYTE type, WORD wItemIdx)
{
//	char txt[256];
	sprintf(txt, "RP_CharacterWearItemUpdate %d, %d, %d", dwCharIdx, type, wItemIdx);
	if(g_DB.Query(eQueryType_FreeQuery, eCharacterWearItemUpdate, 0, txt) == FALSE)
	{
	}
}

//---------------------------------------------------------------------------------------------
// DBResult Callback Function List
// Programmer°¡ »ç¿ëÇÏ´Â ºÎºÐ
// parameter : (LPQUERY pData, LPDBMESSAGE pMessage)

void RInsertIpAdress(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	WORD wEcode = atoi((char*)pData[0].Data[0]);

	TMSGBASE Msg;
	Msg.Category = MP_RMTOOL_ADMIN;
	if( wEcode == 0 )
		Msg.Protocol = MP_RMTOOL_ADMIN_INSERTIP_ACK;
	else
		Msg.Protocol = MP_RMTOOL_ADMIN_INSERTIP_NACK;

	Msg.dwTemplateIdx = tempIdx;

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

void RInsertOperator(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	WORD wEcode = atoi((char*)pData[0].Data[0]);

	TMSGBASE Msg;
	Msg.Category = MP_RMTOOL_ADMIN;
	if( wEcode == 0 )
		Msg.Protocol = MP_RMTOOL_ADMIN_INSERTOPERATOR_ACK;
	else
		Msg.Protocol = MP_RMTOOL_ADMIN_INSERTOPERATOR_NACK;

	Msg.dwTemplateIdx = tempIdx;

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

void RTotalIpAdressInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_IPTOTALINFO Msg;
	memset(&Msg, 0, sizeof(Msg));

	if( pMessage->dwResult == 0 )
	{
		// ÇöÀç ±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÌ´Ù ¶ó°í ¾Ë¸°´Ù.
		Msg.Category = MP_RMTOOL_ADMIN;
		Msg.Protocol = MP_RMTOOL_ADMIN_IPINFO;
		Msg.dwTemplateIdx = tempIdx;
		Msg.bEnd = TRUE;
		Msg.wCount = 0;

		MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
		return;
	}

	DWORD startIdx = 0;
	Msg.wCount = 0;

	for( DWORD i = 0 ; i < pMessage->dwResult ; ++i )
	{
		Msg.sIP[Msg.wCount].dwIPIdx = atoi((char*)pData[i].Data[eII_IPIdx]);
		strcpy( Msg.sIP[Msg.wCount].strIP, (char*)pData[i].Data[eII_IPAdress]);

		strcpy(Msg.sIP[Msg.wCount].strRegDate, (char*)pData[i].Data[eII_Date]);
		strcat(Msg.sIP[Msg.wCount].strRegDate, " ");
		strcat(Msg.sIP[Msg.wCount].strRegDate, (char*)pData[i].Data[eII_Time]);

		startIdx = Msg.sIP[Msg.wCount].dwIPIdx;
		Msg.wCount++;
	}

	Msg.Category = MP_RMTOOL_ADMIN;
	Msg.Protocol = MP_RMTOOL_ADMIN_IPINFO;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100)
	{
		Msg.bEnd = FALSE;
		TotalIpAdressInfo(connectIdx, tempIdx, startIdx);
	}
	else
	{
		// ÇöÀç ±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÌ´Ù ¶ó°í ¾Ë¸°´Ù.
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

void RTotalOperatorInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_OPERTOTALINFO Msg;
	memset(&Msg, 0, sizeof(Msg));

	if( pMessage->dwResult == 0 )
	{
		// ÇöÀç ±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÌ´Ù ¶ó°í ¾Ë¸°´Ù.
		Msg.Category = MP_RMTOOL_ADMIN;
		Msg.Protocol = MP_RMTOOL_ADMIN_OPERATORINFO;
		Msg.dwTemplateIdx = tempIdx;
		Msg.bEnd = TRUE;
		Msg.wCount = 0;	
		
		MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
		return;
	}

	DWORD MyIdx = atoi((char*)pData[0].Data[eOI_MyIdx]);
	DWORD startIdx = 0;
	Msg.wCount = 0;

	for( DWORD i = 0 ; i < pMessage->dwResult ; ++i )
	{
		Msg.sOper[Msg.wCount].dwOperIdx = atoi((char*)pData[i].Data[eOI_OperIdx]);
		strcpy(Msg.sOper[Msg.wCount].strOperID, (char*)pData[i].Data[eOI_OperID]);
		strcpy(Msg.sOper[Msg.wCount].strOperName, (char*)pData[i].Data[eOI_OperName]);
		strcpy(Msg.sOper[Msg.wCount].strOperPWD, (char*)pData[i].Data[eOI_OperPwd]);
		Msg.sOper[Msg.wCount].wOperPower = atoi((char*)pData[i].Data[eOI_OperPower]);

		strcpy(Msg.sOper[Msg.wCount].strRegDate, (char*)pData[i].Data[eOI_Date]);
		strcat(Msg.sOper[Msg.wCount].strRegDate, " ");
		strcat(Msg.sOper[Msg.wCount].strRegDate, (char*)pData[i].Data[eOI_Time]);

		startIdx = Msg.sOper[Msg.wCount].dwOperIdx;
		Msg.wCount++;
	}

	Msg.Category = MP_RMTOOL_ADMIN;
	Msg.Protocol = MP_RMTOOL_ADMIN_OPERATORINFO;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100)
	{
		Msg.bEnd = FALSE;
		TotalOperatorInfo(connectIdx, tempIdx, startIdx, MyIdx);
	}
	else
	{
		// ÇöÀç ±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÌ´Ù ¶ó°í ¾Ë¸°´Ù.
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

void ROperatorLoginCheck(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	DWORD connectIdx = pMessage->dwID;

	if( count == 0 ) // ÀÌ»ó ¿À·ù
	{
		TMSG_WORD msg;
		msg.Category = MP_RMTOOL_CONNECT;
		msg.Protocol = MP_RMTOOL_CONNECT_NACK;
		msg.dwTemplateIdx = 0;
		msg.wData = 0;

		MNETWORK->SendToMC(connectIdx, (char*)&msg, sizeof(msg));
		return;
	}

	WORD check = atoi((char*)pData[0].Data[eOI_ErroCode]);

	if( check != 0 ) // Á¢¼Ó ºÒ°¡
	{
		TMSG_WORD msg;
		msg.Category = MP_RMTOOL_CONNECT;
		msg.Protocol = MP_RMTOOL_CONNECT_NACK;
		msg.dwTemplateIdx = 0;
		msg.wData = check;

		MNETWORK->SendToMC(connectIdx, (char*)&msg, sizeof(msg));
		return;
	}

	TMSG_OPERATOR_LOGIN Msg;
	Msg.Category = MP_RMTOOL_CONNECT;
	Msg.Protocol = MP_RMTOOL_CONNECT_ACK;

	Msg.sOper.dwOperIdx = atoi((char*)pData[0].Data[eOI_OperIdx]);
	strcpy(Msg.sOper.strOperID, (char*)pData[0].Data[eOI_OperID]);
	strcpy(Msg.sOper.strOperName, (char*)pData[0].Data[eOI_OperName]);
	strcpy(Msg.sOper.strOperPWD, (char*)pData[0].Data[eOI_OperPwd]);
	Msg.sOper.wOperPower = atoi((char*)pData[0].Data[eOI_OperPower]);

	strcpy(Msg.sOper.strRegDate, (char*)pData[0].Data[eOI_Date]);
	strcat(Msg.sOper.strRegDate, " ");
	strcat(Msg.sOper.strRegDate, (char*)pData[0].Data[eOI_Time]);

	Msg.sIP.dwIPIdx = atoi((char*)pData[0].Data[eOI_IPIdx]);
	strcpy(Msg.sIP.strIP, (char*)pData[0].Data[eOI_IPAdress]);
	strcpy(Msg.sIP.strRegDate, (char*)pData[0].Data[eOI_IPDate]);
	strcat(Msg.sIP.strRegDate, " ");
	strcat(Msg.sIP.strRegDate, (char*)pData[0].Data[eOI_IPTime]);

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
	
	// ip regist
	IPCHECKMGR->RegistIP( Msg.sIP.strIP );
	IPCHECKMGR->RegistID( connectIdx, Msg.sOper.dwOperIdx, Msg.sOper.strOperID );
}

void RSearchUserInfo(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	if( count == 0 )
	{
		// ÀÏÄ¡ Á¤º¸ ¾øÀ½À» ¾Ë¸²..
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_USER;
		msg.Protocol = MP_RMTOOL_USERSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC(connectIdx, (char*)&msg, sizeof(msg));
	}
	else
	{
		TMSG_USERSEARCHRESULT Info;

		Info.sUser.bLoginFlag = atoi((char*)pData[0].Data[eUI_IsLogin]);
		Info.sUser.dwMemberNo = atoi((char*)pData[0].Data[eUI_User_Idx]);
		strcpy(Info.sUser.strID, (char*)pData[0].Data[eUI_Id]);
		strcpy(Info.sUser.strPass, (char*)pData[0].Data[eUI_Pwd]);
		strcpy(Info.sUser.strName, (char*)pData[0].Data[eUI_Name]);
		strcpy(Info.sUser.strIdCard, (char*)pData[0].Data[eUI_Idcard]);
		strcpy(Info.sUser.strPhon, (char*)pData[0].Data[eUI_Phone]);
		strcpy(Info.sUser.strEmail, (char*)pData[0].Data[eUI_Email]);
		strcpy(Info.sUser.strRegDateTime, (char*)pData[0].Data[eUI_Regdate]);
		strcpy(Info.sUser.strLastLoginTime, (char*)pData[0].Data[eUI_LastLogintime]);
		strcpy(Info.sUser.strLastLogoutTime, (char*)pData[0].Data[eUI_LastLogouttime]);

		Info.sUser.nTotalTime = atoi((char*)pData[0].Data[eUI_Totaltime]);
		int temp = Info.sUser.nTotalTime;
		int oo, mm, ss;
		ss = temp%60;
		mm = (temp/60)%60;
		oo = (temp/3600);
		sprintf(Info.sUser.strTotalTime, "%d½Ã°£ %dºÐ %dÃÊ", oo, mm, ss );

		Info.sUser.wUserLevel = atoi((char*)pData[0].Data[eUI_UserLevel]);
		Info.wServer = atoi((char*)pData[0].Data[eUI_Char_Server]);

		Info.Category = MP_RMTOOL_USER;
		Info.Protocol = MP_RMTOOL_USERSEARCH_ACK;
		Info.dwTemplateIdx = tempIdx;
		MNETWORK->SendToMC(connectIdx, (char*)&Info, sizeof(Info));
	}
}
/*
void RSearchSimpleCharacterInfo(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);


	TMSG_SIMPLERESULT Msg;

	Msg.Category = MP_RMTOOL_USER;
	Msg.Protocol = MP_RMTOOL_SIMPLECHARACTER_ACK;
	Msg.dwTemplateIdx = tempIdx;
	Msg.Count = count;

	for(BYTE i=0;i<count;++i)
	{
		Msg.sSimple[i].wServer = atoi((char*)pData[i].Data[0]);
		Msg.sSimple[i].dwCharNo = atoi((char*)pData[i].Data[1]);
		strcpy( Msg.sSimple[i].strName, (char*)pData[i].Data[2]);
		strcpy( Msg.sSimple[i].strCreateDate, (char*)pData[i].Data[3]);
	}

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}
*/
void RCharacterBaseInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	if(!count)
	{
		// error È£Ãâ
		TMSG_WORD Msg;
		Msg.Category = MP_RMTOOL_CHARACTER;
		Msg.Protocol = MP_RMTOOL_CHARACTERINFO_NACK;
		Msg.dwTemplateIdx = tempIdx;
		Msg.wData = 0; //
		MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
		return;
	}

	TMSG_CHARACTER_BASEINFO Msg;
	
	memset(&Msg.m_sObjinfo,0,sizeof(BASEOBJECT_INFO));
	memset(&Msg.m_sTotalinfo,0,sizeof(CHARACTER_TOTALINFO));
	memset(&Msg.m_sHeroinfo,0,sizeof(HERO_TOTALINFO));
	
	Msg.m_sObjinfo.dwObjectID = atoi((char*)pData->Data[eCS_ObjectID]);
	Msg.m_sObjinfo.dwUserID =  atoi((char*)pData->Data[eCS_UserID]);
	Msg.m_sObjinfo.BattleID = 0;
	Msg.m_sObjinfo.BattleTeam = 0;
	strcpy(Msg.m_sObjinfo.ObjectName, (char*)pData->Data[eCS_Name]);
	
	Msg.m_sTotalinfo.Gender = atoi((char*)pData->Data[eCS_Gender]);
	Msg.m_sTotalinfo.Life = atoi((char*)pData->Data[eCS_Life]);
	Msg.m_sTotalinfo.Shield = atoi((char*)pData->Data[eCS_Shield]);
//	strcpy(Msg.m_sTotalinfo.HonorName, (char*)pData->Data[eCS_Repute]);
	Msg.m_sTotalinfo.Stage = atoi((char*)pData->Data[eCS_Stage]);
	Msg.m_sTotalinfo.Level = atoi((char*)pData->Data[eCS_Grade]);
	Msg.m_sTotalinfo.CurMapNum = 0;
	Msg.m_sTotalinfo.LoginMapNum = atoi((char*)pData->Data[eCS_Map]);
	Msg.m_sTotalinfo.FaceType = atoi((char*)pData->Data[eCS_FaceType]);
	Msg.m_sTotalinfo.HairType = atoi((char*)pData->Data[eCS_HeadType]);
	Msg.m_sTotalinfo.WearedItemIdx[eWearedItem_Hat] = atoi((char*)pData->Data[eCS_Hat]);
	Msg.m_sTotalinfo.WearedItemIdx[eWearedItem_Weapon] = atoi((char*)pData->Data[eCS_Weapon]);
	Msg.m_sTotalinfo.WearedItemIdx[eWearedItem_Dress] = atoi((char*)pData->Data[eCS_Dress]);
	Msg.m_sTotalinfo.WearedItemIdx[eWearedItem_Shoes] = atoi((char*)pData->Data[eCS_Shoes]);
	Msg.m_sTotalinfo.WearedItemIdx[eWearedItem_Ring1] = atoi((char*)pData->Data[eCS_Ring1]);
	Msg.m_sTotalinfo.WearedItemIdx[eWearedItem_Ring2] = atoi((char*)pData->Data[eCS_Ring2]);
	Msg.m_sTotalinfo.WearedItemIdx[eWearedItem_Cape] = atoi((char*)pData->Data[eCS_Cape]);
	Msg.m_sTotalinfo.WearedItemIdx[eWearedItem_Necklace] = atoi((char*)pData->Data[eCS_Necklace]);
	Msg.m_sTotalinfo.WearedItemIdx[eWearedItem_Armlet] = atoi((char*)pData->Data[eCS_Armlet]);
	Msg.m_sTotalinfo.WearedItemIdx[eWearedItem_Belt] = atoi((char*)pData->Data[eCS_Belt]);
	Msg.m_sTotalinfo.MunpaID = atoi((char*)pData->Data[eCS_MunpaIDX]);
	Msg.m_sTotalinfo.PositionInMunpa = atoi((char*)pData->Data[eCS_MunpaPositon]);
	Msg.m_sTotalinfo.MapChangePoint_Index = atoi((char*)pData->Data[eCS_MapChangePoint]);
	Msg.m_sTotalinfo.LoginPoint_Index = atoi((char*)pData->Data[eCS_LoginPoint]);

	Msg.m_sHeroinfo.PartyID = atoi((char*)pData->Data[eCS_PartyID]);
	Msg.m_sHeroinfo.wGenGol = atoi((char*)pData->Data[eCS_Gengoal]);
	Msg.m_sHeroinfo.wNaeRyuk = atoi((char*)pData->Data[eCS_Energe]);
	Msg.m_sHeroinfo.wMinChub = atoi((char*)pData->Data[eCS_Dex]);
	Msg.m_sHeroinfo.wCheRyuk = atoi((char*)pData->Data[eCS_Sta]);
	Msg.m_sHeroinfo.wSimMek = atoi((char*)pData->Data[eCS_Simmak]);
	Msg.m_sHeroinfo.Fame = atoi((char*)pData->Data[eCS_Fame]);
	Msg.m_sHeroinfo.wKarma = atoi((char*)pData->Data[eCS_Karma]);
	Msg.m_sHeroinfo.ExpPoint = atoi((char*)pData->Data[eCS_Expoint]);
	Msg.m_sHeroinfo.LevelUpPoint = atoi((char*)pData->Data[eCS_GradeUpPoint]);
	Msg.m_sHeroinfo.Money = atoi((char*)pData->Data[eCS_Money]);
	strcpy(Msg.m_sHeroinfo.MunpaName, (char*)pData->Data[eCS_MunpaName]);
	Msg.m_sHeroinfo.KyungGongGrade = atoi((char*)pData->Data[eCS_QuickPace]);
	Msg.m_sHeroinfo.AbilityExp = atoi((char*)pData->Data[eCS_AbilityExp]);

	// Á¤º¸ º¸³»±â
	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_BASEINFO;
	Msg.dwTemplateIdx = tempIdx;
	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

void RCharacterInvenInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	// °¡Á¤!!
	// ¾ÆÀÌÅÛÀÇ Æ÷Áö¼ÇÀº Àý´ë °ãÄ¥¼ö ¾ø´Ù
	// ¾ÆÀÌÅÛÀÇ Ä³¸¯ÅÍ ¹®ÆÄ Ã¢°í ÀÎµ¦½ºµµ Àý´ë °ãÄ¥¼ö ¾ø´Ù. (ÀÌ°Ç ¾î¶»°Ô º¹±¸ÇÏ³Ä!!)
	TMSG_CHARACTER_INVENINFO Msg;
	memset(&Msg.m_sIteminfo, 0, sizeof(Msg.m_sIteminfo));

	POSTYPE p = 0;
	for(DWORD  i = 0; i < count; i++)
	{
		POSTYPE ItemPos = atoi((char*)pData[i].Data[eCI_Position]);
		// ±×¸®µå(ÀÎº¥Åä¸®)¾ÆÀÌÅÛ ¼ÂÆÃ
		if(ItemPos >= TP_INVENTORY_START && ItemPos < TP_INVENTORY_END)
		{
			ItemPos -= TP_INVENTORY_START;
			Msg.m_sIteminfo.Inventory[ItemPos].dwDBIdx = atoi((char*)pData[i].Data[eCI_DBIDX]);
			Msg.m_sIteminfo.Inventory[ItemPos].wIconIdx = atoi((char*)pData[i].Data[eCI_IDX]);
			Msg.m_sIteminfo.Inventory[ItemPos].Position = atoi((char*)pData[i].Data[eCI_Position]);
			Msg.m_sIteminfo.Inventory[ItemPos].Durability = atoi((char*)pData[i].Data[eCI_Durability]);
			Msg.m_sIteminfo.Inventory[ItemPos].QuickPosition = atoi((char*)pData[i].Data[eCI_QPosition]);
		}
		else if(ItemPos >= TP_WEAR_START && ItemPos < TP_WEAR_END)
		{				
			POSTYPE ItemGrid = ItemPos - TP_WEAR_START;
			Msg.m_sIteminfo.WearedItem[ItemGrid].dwDBIdx = atoi((char*)pData[i].Data[eCI_DBIDX]);
			Msg.m_sIteminfo.WearedItem[ItemGrid].wIconIdx = atoi((char*)pData[i].Data[eCI_IDX]);
			Msg.m_sIteminfo.WearedItem[ItemGrid].Position = atoi((char*)pData[i].Data[eCI_Position]);
			Msg.m_sIteminfo.WearedItem[ItemGrid].Durability = atoi((char*)pData[i].Data[eCI_Durability]);
			Msg.m_sIteminfo.WearedItem[ItemGrid].QuickPosition = atoi((char*)pData[i].Data[eCI_QPosition]);
		}
		else
		{
			ASSERT(0);
			continue;
		}
	}

	// Á¤º¸ º¸³»±â
	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_INVENINFO;
	Msg.dwTemplateIdx = tempIdx;
	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

void RCharacterPyogukInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_CHARACTER_PYOGUKINFO Msg;
	DWORD UserIdx;

	if( pMessage->dwResult == 0 )
	{
		UserIdx = 0;
		Msg.m_wPyogukNum = 0;
		Msg.m_dwMoney = 0;
	}
	else
	{
		UserIdx = atoi((char*)pData->Data[eCP_UserIdx]);
		Msg.m_wPyogukNum = atoi((char*)pData->Data[eCP_PyogukNum]);
		Msg.m_dwMoney = atoi((char*)pData->Data[eCP_PyogukMoney]);
	}

	if( Msg.m_wPyogukNum != 0)
		CharacterPyogukItemInfo( connectIdx, tempIdx, UserIdx, TP_PYOGUK_START-1 );

	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_PYOGUKINFO;
	Msg.dwTemplateIdx = tempIdx;

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

void RCharacterPyogukItemInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_ITEMINFO_LARGECOUNT Msg;
	memset(&Msg, 0, sizeof(Msg));

	if( pMessage->dwResult == 0 )
	{
		// ÇöÀç ±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÌ´Ù ¶ó°í ¾Ë¸°´Ù.
		Msg.Category = MP_RMTOOL_CHARACTER;
		Msg.Protocol = MP_RMTOOL_CHARACTER_PYOGUKITEMINFO;
		Msg.dwTemplateIdx = tempIdx;
		Msg.m_bEnd = TRUE;
		Msg.m_wCount = 0;

		MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
		return;
	}

	DWORD UserIdx = atoi((char*)pData[0].Data[ePI_UserId]);
	POSTYPE startPos = 0;
	Msg.m_wCount = 0;

	for( DWORD i = 0 ; i < pMessage->dwResult ; ++i )
	{
		POSTYPE ItemPos = atoi((char*)pData[i].Data[ePI_Position]);
		if(TP_PYOGUK_START<= ItemPos && ItemPos < TP_PYOGUK_END)
		{
			Msg.m_Item[Msg.m_wCount].dwDBIdx			= atoi((char*)pData[i].Data[ePI_DBIDX]);
			Msg.m_Item[Msg.m_wCount].wIconIdx			= atoi((char*)pData[i].Data[ePI_IDX]);
			Msg.m_Item[Msg.m_wCount].Position			= atoi((char*)pData[i].Data[ePI_Position]);
			Msg.m_Item[Msg.m_wCount].QuickPosition	= atoi((char*)pData[i].Data[ePI_QPosition]);
			Msg.m_Item[Msg.m_wCount].Durability		= atoi((char*)pData[i].Data[ePI_Durability]);
			
			startPos = Msg.m_Item[Msg.m_wCount].Position;
			Msg.m_wCount++;
		}
		else
		{
			ASSERT(0);
			continue;
		}
	}

	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_PYOGUKITEMINFO;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100)
	{
		Msg.m_bEnd = FALSE;
		CharacterPyogukItemInfo(connectIdx, tempIdx, UserIdx, startPos);
	}
	else
	{
		// ÇöÀç ±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÌ´Ù ¶ó°í ¾Ë¸°´Ù.
		Msg.m_bEnd = TRUE;
	}

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

void RCharacterMugongInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);
	DWORD count = pMessage->dwResult;
	
	TMSG_CHARACTER_MUGONGINFO Msg;
	TMSG_CHARACTER_MUGONGINFO2 Msg2;
	memset(&Msg, 0, sizeof(TMSG_CHARACTER_MUGONGINFO));
	memset(&Msg2, 0, sizeof(TMSG_CHARACTER_MUGONGINFO2));

	DWORD ec = 0;
	for(DWORD i = 0; i < count; i++)
	{
		WORD Mugongidx = (WORD)atoi((char*)pData[i].Data[eCM_IDX]);
			
		POSTYPE MugongPos = atoi((char*)pData[i].Data[eCM_Position]) - TP_MUGONG_START;

		if( Msg.m_sMugongInfo.mugong[MugongPos].dwDBIdx == 0 )
		{
			Msg.m_sMugongInfo.mugong[MugongPos].wIconIdx = Mugongidx;
			Msg.m_sMugongInfo.mugong[MugongPos].dwDBIdx = atoi((char*)pData[i].Data[eCM_DBIDX]);
			Msg.m_sMugongInfo.mugong[MugongPos].ExpPoint = atoi((char*)pData[i].Data[eCM_ExpPoint]);
			Msg.m_sMugongInfo.mugong[MugongPos].Sung = atoi((char*)pData[i].Data[eCM_Sung]);
			Msg.m_sMugongInfo.mugong[MugongPos].Position = atoi((char*)pData[i].Data[eCM_Position]);
			Msg.m_sMugongInfo.mugong[MugongPos].QuickPosition = atoi((char*)pData[i].Data[eCM_QPosition]);
			Msg.m_sMugongInfo.mugong[MugongPos].bWear = atoi((char*)pData[i].Data[eCM_Wear]);
		}
		else
		{
			if( ec < 5 )
			{
				Msg2.m_MugongBase[ec].wIconIdx = Mugongidx;
				Msg2.m_MugongBase[ec].dwDBIdx = atoi((char*)pData[i].Data[eCM_DBIDX]);
				Msg2.m_MugongBase[ec].ExpPoint = atoi((char*)pData[i].Data[eCM_ExpPoint]);
				Msg2.m_MugongBase[ec].Sung = atoi((char*)pData[i].Data[eCM_Sung]);
				Msg2.m_MugongBase[ec].Position = atoi((char*)pData[i].Data[eCM_Position]);
				Msg2.m_MugongBase[ec].QuickPosition = atoi((char*)pData[i].Data[eCM_QPosition]);
				Msg2.m_MugongBase[ec].bWear = atoi((char*)pData[i].Data[eCM_Wear]);
				
				++ec;
				Msg2.m_dwCount = ec;
			}
		}
	}

	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_MUGONGINFO;
	Msg.dwTemplateIdx = tempIdx;
	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));

	if( Msg2.m_dwCount )
	{
		Msg2.Category = MP_RMTOOL_CHARACTER;
		Msg2.Protocol = MP_RMTOOL_CHARACTER_MUGONGINFO2;
		Msg2.dwTemplateIdx = tempIdx;
		MNETWORK->SendToMC(connectIdx, (char*)&Msg2, sizeof(TMSG_CHARACTER_MUGONGINFO2));
	}
}

void RCharacterAbilityInfo(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage)
{
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_CHARACTER_ABILITYINFO Msg;
	memset(&Msg, 0, sizeof(Msg));

//	ASSERT(pMessage->dwResult == 1);

	if( pMessage->dwResult == 1 )
	{
		for(int kind = 0;kind<eAGK_Max;++kind)
			memcpy(Msg.sAbility.AbilityDataArray[kind], (char*)pData[0].Data[kind+1], MAX_ABILITY_NUM_PER_GROUP+1);
	}

	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_ABILLITYINFO;
	Msg.dwTemplateIdx = tempIdx;

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

void RCharacterItemOptionInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_ITEMOPTIONINFO	Msg;
	memset(&Msg, 0, sizeof(Msg));

	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTERINFO_ITEMOPTIONINFO;
	Msg.dwTemplateIdx = tempIdx;

	if( count == 0 )
	{
		// ÇöÀç±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÓÀ» ÀÇ¹ÌÇÑ´Ù.
		Msg.bEnd = TRUE;
		Msg.wCount = 0;

		MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
		return;
	}

	DWORD UserIdx = atoi((char*)pData[0].Data[eCIOI_UserID]);
	DWORD CharIdx = atoi((char*)pData[0].Data[eCIOI_CharID]);
	DWORD StartIdx = 0;

	for( DWORD i = 0 ; i < count ; ++i )
	{
		Msg.OptionInfo[i].dwOptionIdx			= (DWORD)atoi((char*)pData[i].Data[eCIOI_OptionID]);
		Msg.OptionInfo[i].dwItemDBIdx			= (DWORD)atoi((char*)pData[i].Data[eCIOI_ItemDBID]);
		Msg.OptionInfo[i].GenGol				= (WORD)atoi((char*)pData[i].Data[eCIOI_GenGol]);
		Msg.OptionInfo[i].MinChub				= (WORD)atoi((char*)pData[i].Data[eCIOI_MinChub]);				
		Msg.OptionInfo[i].CheRyuk				= (WORD)atoi((char*)pData[i].Data[eCIOI_CheRyuk]);				
		Msg.OptionInfo[i].SimMek				= (WORD)atoi((char*)pData[i].Data[eCIOI_SimMek]);					
		Msg.OptionInfo[i].Life					= (DWORD)atoi((char*)pData[i].Data[eCIOI_Life]);					
		Msg.OptionInfo[i].NaeRyuk				= (WORD)atoi((char*)pData[i].Data[eCIOI_NaeRyuk]);				
		Msg.OptionInfo[i].Shield				= (DWORD)atoi((char*)pData[i].Data[eCIOI_Shield]);					
		Msg.OptionInfo[i].AttrRegist.SetElement_Val(ATTR_FIRE,		atof((char*)pData[i].Data[eCIOI_FireAttrRegist]) );
		Msg.OptionInfo[i].AttrRegist.SetElement_Val(ATTR_WATER,	atof((char*)pData[i].Data[eCIOI_WaterAttrRegist]) );
		Msg.OptionInfo[i].AttrRegist.SetElement_Val(ATTR_TREE,		atof((char*)pData[i].Data[eCIOI_TreeAttrRegist]) );
		Msg.OptionInfo[i].AttrRegist.SetElement_Val(ATTR_IRON,		atof((char*)pData[i].Data[eCIOI_GoldAttrRegist]) );
		Msg.OptionInfo[i].AttrRegist.SetElement_Val(ATTR_EARTH,	atof((char*)pData[i].Data[eCIOI_EarthAttrRegist]) );
		Msg.OptionInfo[i].PhyAttack			= (WORD)atoi((char*)pData[i].Data[eCIOI_PhyAttack]);
		Msg.OptionInfo[i].CriticalPercent		= (WORD)atoi((char*)pData[i].Data[eCIOI_Critical]);
		Msg.OptionInfo[i].AttrAttack.SetElement_Val(ATTR_FIRE,		atof((char*)pData[i].Data[eCIOI_FireAttrAttack]) );
		Msg.OptionInfo[i].AttrAttack.SetElement_Val(ATTR_WATER,	atof((char*)pData[i].Data[eCIOI_WaterAttrAttack]) );
		Msg.OptionInfo[i].AttrAttack.SetElement_Val(ATTR_TREE,		atof((char*)pData[i].Data[eCIOI_TreeAttrAttack]) );
		Msg.OptionInfo[i].AttrAttack.SetElement_Val(ATTR_IRON,		atof((char*)pData[i].Data[eCIOI_GoldAttrAttack]) );
		Msg.OptionInfo[i].AttrAttack.SetElement_Val(ATTR_EARTH,	atof((char*)pData[i].Data[eCIOI_EarthAttrAttack]) );
		Msg.OptionInfo[i].PhyDefense			= (WORD)atoi((char*)pData[i].Data[eCIOI_PhyDefense]);				

		Msg.wCount++;
		StartIdx = Msg.OptionInfo[i].dwItemDBIdx;
	}

	if( count >= 100 )
	{
		Msg.bEnd = FALSE;
		CharacterItemOptionInfo( connectIdx, tempIdx, UserIdx, CharIdx, StartIdx );
	}
	else
	{
		// ÇöÀç ±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÌ´Ù ¶ó°í ¾Ë¸°´Ù.
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

void RSearchMunpaIdx(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	if( count == 0 )
	{
		// ÀÏÄ¡ Á¤º¸ ¾øÀ½À» ¾Ë¸²..
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_MUNPA;
		msg.Protocol = MP_RMTOOL_MUNPASEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC(connectIdx, (char*)&msg, sizeof(msg));
	}
	else
	{
		DWORD dwMunpaIdx = atoi((char*)pData[0].Data[0]);
		MunpaBaseInfo(connectIdx, tempIdx, dwMunpaIdx);
	}
}

void RMunpaBaseInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	if( count == 0 )
	{
		// ÀÏÄ¡ Á¤º¸ ¾øÀ½À» ¾Ë¸²..
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_MUNPA;
		msg.Protocol = MP_RMTOOL_MUNPASEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC(connectIdx, (char*)&msg, sizeof(msg));
	}
	else
	{
		TMSG_MUNPABASEINFO Info;

		Info.sBaseInfo.dwMunpaIdx = atoi((char*)pData[0].Data[eMB_MunpaId]);
		strcpy(Info.sBaseInfo.strMunpaName, (char*)pData[0].Data[eMB_MunpaName]);
		strcpy(Info.sBaseInfo.strMunjuName, (char*)pData[0].Data[eMB_MunjuName]);
		
		strcpy(Info.sBaseInfo.strRegDate, (char*)pData[0].Data[eMB_RegDate]);
		strcat(Info.sBaseInfo.strRegDate, " ");
		strcat(Info.sBaseInfo.strRegDate, (char*)pData[0].Data[eMB_RegTime]);

		Info.sBaseInfo.wMemberNum = atoi((char*)pData[0].Data[eMB_MemberNum]);
		Info.sBaseInfo.wLocation = atoi((char*)pData[0].Data[eMB_Location]);
		Info.sBaseInfo.bMunpaLevel = atoi((char*)pData[0].Data[eMB_MunpaLevel]);
		Info.sBaseInfo.dwFamous = atoi((char*)pData[0].Data[eMB_Famous]);
		Info.sBaseInfo.bKind = atoi((char*)pData[0].Data[eMB_Kind]);
		Info.sBaseInfo.dwMoney = atoi((char*)pData[0].Data[eMB_MunpaMoney]);

		Info.Category = MP_RMTOOL_MUNPA;
		Info.Protocol = MP_RMTOOL_MUNPABASEINFO;
		Info.dwTemplateIdx = tempIdx;

		MunpaItemInfo(connectIdx, tempIdx, Info.sBaseInfo.dwMunpaIdx, TP_MUNPAWAREHOUSE_START - 1 );
		MunpaMemberInfo(connectIdx, tempIdx, Info.sBaseInfo.dwMunpaIdx, 0);

		MNETWORK->SendToMC(connectIdx, (char*)&Info, sizeof(Info));
	}
}

void RMunpaItemInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_ITEMINFO_LARGECOUNT Msg;
	memset(&Msg, 0, sizeof(Msg));

	if( pMessage->dwResult == 0 )
	{
		// ÇöÀç ±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÌ´Ù ¶ó°í ¾Ë¸°´Ù.
		Msg.Category = MP_RMTOOL_MUNPA;
		Msg.Protocol = MP_RMTOOL_MUNPAITEMINFO;
		Msg.dwTemplateIdx = tempIdx;
		Msg.m_bEnd = TRUE;
		Msg.m_wCount = 0;

		MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
		return;
	}

	DWORD MunpaIdx = atoi((char*)pData[0].Data[ePI_UserId]);
	POSTYPE startPos = 0;
	Msg.m_wCount = 0;

	for( DWORD i = 0 ; i < pMessage->dwResult ; ++i )
	{
		POSTYPE ItemPos = atoi((char*)pData[i].Data[ePI_Position]);
		if(TP_MUNPAWAREHOUSE_START<= ItemPos && ItemPos < TP_MUNPAWAREHOUSE_END)
		{
			Msg.m_Item[Msg.m_wCount].dwDBIdx			= atoi((char*)pData[i].Data[ePI_DBIDX]);
			Msg.m_Item[Msg.m_wCount].wIconIdx			= atoi((char*)pData[i].Data[ePI_IDX]);
			Msg.m_Item[Msg.m_wCount].Position			= atoi((char*)pData[i].Data[ePI_Position]);
			Msg.m_Item[Msg.m_wCount].QuickPosition	= atoi((char*)pData[i].Data[ePI_QPosition]);
			Msg.m_Item[Msg.m_wCount].Durability		= atoi((char*)pData[i].Data[ePI_Durability]);
			
			startPos = Msg.m_Item[Msg.m_wCount].Position;
			Msg.m_wCount++;
		}
		else
		{
			ASSERT(0);
			continue;
		}
	}

	Msg.Category = MP_RMTOOL_MUNPA;
	Msg.Protocol = MP_RMTOOL_MUNPAITEMINFO;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100)
	{
		Msg.m_bEnd = FALSE;
		MunpaItemInfo(connectIdx, tempIdx, MunpaIdx, startPos);
	}
	else
	{
		// ÇöÀç ±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÌ´Ù ¶ó°í ¾Ë¸°´Ù.
		Msg.m_bEnd = TRUE;
	}

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

void RMunpaMemberInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_MUNPA_MEMBERINFO Msg;
	memset(&Msg, 0, sizeof(Msg));

	if( pMessage->dwResult == 0 )
	{
		// ÇöÀç ±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÌ´Ù ¶ó°í ¾Ë¸°´Ù.
		Msg.Category = MP_RMTOOL_MUNPA;
		Msg.Protocol = MP_RMTOOL_MUNPAMEMBERINFO;
		Msg.dwTemplateIdx = tempIdx;
		Msg.bEnd = TRUE;
		Msg.wCount = 0;

		MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
		return;
	}

	DWORD MunpaIdx = atoi((char*)pData[0].Data[eMM_MunpaId]);
	DWORD startIdx = 0;
	Msg.wCount = 0;

	for( DWORD i = 0 ; i < pMessage->dwResult ; ++i )
	{
		Msg.sMember[Msg.wCount].dwCharno		= atoi((char*)pData[i].Data[eMM_MemberIdx]);
		strcpy(Msg.sMember[Msg.wCount].strName, (char*)pData[i].Data[eMM_MemberName]);
		Msg.sMember[Msg.wCount].level		= atoi((char*)pData[i].Data[eMM_MemberLevel]);
		Msg.sMember[Msg.wCount].bJikChek		= atoi((char*)pData[i].Data[eMM_MemberJikChek]);

		strcpy(Msg.sMember[Msg.wCount].strRegDate, (char*)pData[i].Data[eMM_MemberDate]);
		strcat(Msg.sMember[Msg.wCount].strRegDate, " ");
		strcat(Msg.sMember[Msg.wCount].strRegDate, (char*)pData[i].Data[eMM_MemberTime]);
			
		startIdx = Msg.sMember[Msg.wCount].dwCharno;
		Msg.wCount++;
	}

	Msg.Category = MP_RMTOOL_MUNPA;
	Msg.Protocol = MP_RMTOOL_MUNPAMEMBERINFO;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100)
	{
		Msg.bEnd = FALSE;
		MunpaMemberInfo(connectIdx, tempIdx, MunpaIdx, startIdx);
	}
	else
	{
		// ÇöÀç ±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÌ´Ù ¶ó°í ¾Ë¸°´Ù.
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

void RGameLogItemMoveLog(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_GAMELOGITEM1 Msg;
	memset(&Msg, 0, sizeof(Msg));

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC(connectIdx, (char*)&msg, sizeof(msg));
		return;
	}

	strcpy(Msg.strCharName, (char*)pData[0].Data[eGI_CharName]);
	Msg.bDayOnly = atoi((char*)pData[0].Data[eGI_Dayonly]);
	strcpy(Msg.strStart, (char*)pData[0].Data[eGI_StartDate]);
	strcpy(Msg.strEnd, (char*)pData[0].Data[eGI_EndDate]);
	DWORD startIdx = 0;
	Msg.wCount = 0;

	for( DWORD i = 0 ; i < pMessage->dwResult ; ++i )
	{
		Msg.sLog[Msg.wCount].dwLogidx = atoi((char*)pData[i].Data[eGI_LogIdx]);
		Msg.sLog[Msg.wCount].bLogtype = atoi((char*)pData[i].Data[eGI_LogType]);
		Msg.sLog[Msg.wCount].dwCharIdx = atoi((char*)pData[i].Data[eGI_CharIdx]);
		Msg.sLog[Msg.wCount].dwTargetIdx = atoi((char*)pData[i].Data[eGI_TargetIdx]); 
		strcpy(Msg.sLog[Msg.wCount].strTargetName, (char*)pData[i].Data[eGI_TargetName]);
		Msg.sLog[Msg.wCount].dwItemIdx = atoi((char*)pData[i].Data[eGI_ItemIdx]);
		Msg.sLog[Msg.wCount].dwItemDBIdx = atoi((char*)pData[i].Data[eGI_ItemDBIdx]);
		strcpy(Msg.sLog[Msg.wCount].strLogdate, (char*)pData[i].Data[eGI_Logdate]);
		strcat(Msg.sLog[Msg.wCount].strLogdate, " ");
		strcat(Msg.sLog[Msg.wCount].strLogdate, (char*)pData[i].Data[eGI_Logtime]);
		Msg.sLog[Msg.wCount].dwMoney = atoi((char*)pData[i].Data[eGI_Money]);

		startIdx = Msg.sLog[Msg.wCount].dwLogidx;
		Msg.wCount++;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
//	Msg.Protocol = MP_RMTOOL_GAMELOG_ITEMMOVELOG;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100)
	{
		Msg.bEnd = FALSE;
		GameLogItemMoveLog( connectIdx, tempIdx, startIdx, 0, Msg.strCharName, Msg.bDayOnly, Msg.strStart, Msg.strEnd);
	}
	else
	{
		// ÇöÀç ±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÌ´Ù ¶ó°í ¾Ë¸°´Ù.
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

void RGameLogItemObtainLog(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_GAMELOGITEM1 Msg;
	memset(&Msg, 0, sizeof(Msg));

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC(connectIdx, (char*)&msg, sizeof(msg));
		return;
	}

	strcpy(Msg.strCharName, (char*)pData[0].Data[eGI_CharName]);
	Msg.bDayOnly = atoi((char*)pData[0].Data[eGI_Dayonly]);
	strcpy(Msg.strStart, (char*)pData[0].Data[eGI_StartDate]);
	strcpy(Msg.strEnd, (char*)pData[0].Data[eGI_EndDate]);
	DWORD startIdx = 0;
	Msg.wCount = 0;

	for( DWORD i = 0 ; i < pMessage->dwResult ; ++i )
	{
		Msg.sLog[Msg.wCount].dwLogidx = atoi((char*)pData[i].Data[eGI_LogIdx]);
		Msg.sLog[Msg.wCount].bLogtype = atoi((char*)pData[i].Data[eGI_LogType]);
		Msg.sLog[Msg.wCount].dwCharIdx = atoi((char*)pData[i].Data[eGI_CharIdx]);
		Msg.sLog[Msg.wCount].dwTargetIdx = atoi((char*)pData[i].Data[eGI_TargetIdx]); 
		strcpy(Msg.sLog[Msg.wCount].strTargetName, (char*)pData[i].Data[eGI_TargetName]);
		Msg.sLog[Msg.wCount].dwItemIdx = atoi((char*)pData[i].Data[eGI_ItemIdx]);
		Msg.sLog[Msg.wCount].dwItemDBIdx = atoi((char*)pData[i].Data[eGI_ItemDBIdx]);
		strcpy(Msg.sLog[Msg.wCount].strLogdate, (char*)pData[i].Data[eGI_Logdate]);
		strcat(Msg.sLog[Msg.wCount].strLogdate, " ");
		strcat(Msg.sLog[Msg.wCount].strLogdate, (char*)pData[i].Data[eGI_Logtime]);
		Msg.sLog[Msg.wCount].dwMoney = atoi((char*)pData[i].Data[eGI_Money]);

		startIdx = Msg.sLog[Msg.wCount].dwLogidx;
		Msg.wCount++;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
//	Msg.Protocol = MP_RMTOOL_GAMELOG_ITEMOBTAINLOG;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100)
	{
		Msg.bEnd = FALSE;
		GameLogItemMoveLog( connectIdx, tempIdx, startIdx, 0, Msg.strCharName, Msg.bDayOnly, Msg.strStart, Msg.strEnd);
	}
	else
	{
		// ÇöÀç ±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÌ´Ù ¶ó°í ¾Ë¸°´Ù.
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

void RGameLogBaseInfoLog(LPQUERY pData, LPDBMESSAGE pMessage)
{
}

void RGameLogMoneyLog(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_GAMELOGMONEY Msg;
	memset(&Msg, 0, sizeof(Msg));

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC(connectIdx, (char*)&msg, sizeof(msg));
		return;
	}

	strcpy(Msg.strCharName, (char*)pData[0].Data[eGLM_CharName]);
	Msg.bDayOnly = atoi((char*)pData[0].Data[eGLM_Dayonly]);
	strcpy(Msg.strStart, (char*)pData[0].Data[eGLM_StartDate]);
	strcpy(Msg.strEnd, (char*)pData[0].Data[eGLM_EndDate]);
	DWORD startIdx = 0;
	Msg.wCount = 0;

	for( DWORD i = 0 ; i < pMessage->dwResult ; ++i )
	{
		Msg.sLog[Msg.wCount].dwLogIdx = atoi((char*)pData[i].Data[eGLM_LogIdx]);
		Msg.sLog[Msg.wCount].LogType = atoi((char*)pData[i].Data[eGLM_LogType]);
		Msg.sLog[Msg.wCount].dwCharIdx = atoi((char*)pData[i].Data[eGLM_CharIdx]);
		Msg.sLog[Msg.wCount].dwInvenMoney = atoi((char*)pData[i].Data[eGLM_InvenMoney]);
		Msg.sLog[Msg.wCount].dwPyogukMoney = atoi((char*)pData[i].Data[eGLM_PyogukMoney]);
		strcpy(Msg.sLog[Msg.wCount].strLogdate, (char*)pData[i].Data[eGLM_LogDate]);
		strcat(Msg.sLog[Msg.wCount].strLogdate, " ");
		strcat(Msg.sLog[Msg.wCount].strLogdate, (char*)pData[i].Data[eGLM_LogTime]);
		startIdx = Msg.sLog[Msg.wCount].dwLogIdx;
		Msg.wCount++;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
//	Msg.Protocol = MP_RMTOOL_GAMELOG_MONEY;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100)
	{
		Msg.bEnd = FALSE;
		GameLogMoneyLog( connectIdx, tempIdx, startIdx, 0, Msg.strCharName, Msg.bDayOnly, Msg.strStart, Msg.strEnd);
	}
	else
	{
		// ÇöÀç ±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÌ´Ù ¶ó°í ¾Ë¸°´Ù.
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));
}

void RGameLogItemFromChrName( LPQUERY pData, LPDBMESSAGE pMessage )
{
/*	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );

	TMSG_GAMELOGITEM Msg;
	memset( &Msg, 0, sizeof(Msg) );

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
//		msg.Protocol = MP_RMTOOL_GAMELOG_ITEM_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&msg, sizeof(msg) );
		return;
	}

	strcpy( Msg.strChrName, (char*)pData[0].Data[eGLI_ChrName]);
	Msg.bDayOnly = atoi((char*)pData[0].Data[eGLI_DayOnly]);
	strcpy( Msg.strStart, (char*)pData[0].Data[eGLI_StartDate] );
	strcpy( Msg.strEnd, (char*)pData[0].Data[eGLI_EndDate] );
	Msg.wCount = 0;

	DWORD startIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		Msg.sLog[i].dwLogIdx = atoi((char*)pData[i].Data[eGLI_LogIdx]);
		Msg.sLog[i].dwLogType = atoi((char*)pData[i].Data[eGLI_LogType]);
		Msg.sLog[i].dwItemIdx = atoi((char*)pData[i].Data[eGLI_ItemIdx]);
		Msg.sLog[i].dwItemDBIdx = atoi((char*)pData[i].Data[eGLI_ItemDBIdx]);
		Msg.sLog[i].dwCurChrIdx = atoi((char*)pData[i].Data[eGLI_CurChrIdx]);
		Msg.sLog[i].dwBeforeChrIdx = atoi((char*)pData[i].Data[eGLI_BeforeChrIdx]);
		Msg.sLog[i].dwMoney = atoi((char*)pData[i].Data[eGLI_Money]);
		strcpy( Msg.sLog[i].strLogDate, (char*)pData[i].Data[eGLI_LogDate] );
		strcat( Msg.sLog[i].strLogDate, " ");
		strcat( Msg.sLog[i].strLogDate, (char*)pData[i].Data[eGLI_LogTime] );
		strcpy( Msg.sLog[i].strCurChrName, (char*)pData[i].Data[eGLI_CurChrName] );
		strcpy( Msg.sLog[i].strBeforeChrName, (char*)pData[i].Data[eGLI_BeforeChrName] );
		startIdx = Msg.sLog[i].dwLogIdx;
		++Msg.wCount;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
//	Msg.Protocol = MP_RMTOOL_GAMELOG_ITEM_ACK;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100)
	{
		Msg.bEnd = FALSE;
		GameLogItemFromChrName( connectIdx, tempIdx, startIdx, 0, Msg.bDayOnly, Msg.strStart, Msg.strEnd, Msg.strChrName );
	}
	else
	{
		// ÇöÀç ±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÌ´Ù ¶ó°í ¾Ë¸°´Ù.
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(Msg) );
	*/
}

void RGameLogItemFromItemDBIndex( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );

	TMSG_GAMELOGITEM Msg;
	memset( &Msg, 0, sizeof(Msg) );

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
//		msg.Protocol = MP_RMTOOL_GAMELOG_ITEM_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&msg, sizeof(msg) );
		return;
	}

	Msg.dwItemDBIdx = atoi((char*)pData[0].Data[eGLI_ChrName]);
	Msg.bDayOnly = atoi((char*)pData[0].Data[eGLI_DayOnly]);
	strcpy( Msg.strStart, (char*)pData[0].Data[eGLI_StartDate] );
	strcpy( Msg.strEnd, (char*)pData[0].Data[eGLI_EndDate] );
	Msg.wCount = 0;

	DWORD startIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		Msg.sLog[i].dwLogIdx = atoi((char*)pData[i].Data[eGLI_LogIdx]);
		Msg.sLog[i].dwLogType = atoi((char*)pData[i].Data[eGLI_LogType]);
		Msg.sLog[i].dwItemIdx = atoi((char*)pData[i].Data[eGLI_ItemIdx]);
		Msg.sLog[i].dwItemDBIdx = atoi((char*)pData[i].Data[eGLI_ItemDBIdx]);
		Msg.sLog[i].dwCurChrIdx = atoi((char*)pData[i].Data[eGLI_CurChrIdx]);
		Msg.sLog[i].dwBeforeChrIdx = atoi((char*)pData[i].Data[eGLI_BeforeChrIdx]);
		Msg.sLog[i].dwMoney = atoi((char*)pData[i].Data[eGLI_Money]);
		strcpy( Msg.sLog[i].strLogDate, (char*)pData[i].Data[eGLI_LogDate] );
		strcat( Msg.sLog[i].strLogDate, " ");
		strcat( Msg.sLog[i].strLogDate, (char*)pData[i].Data[eGLI_LogTime] );
		strcpy( Msg.sLog[i].strCurChrName, (char*)pData[i].Data[eGLI_CurChrName] );
		strcpy( Msg.sLog[i].strBeforeChrName, (char*)pData[i].Data[eGLI_BeforeChrName] );
		startIdx = Msg.sLog[i].dwLogIdx;
		++Msg.wCount;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
//	Msg.Protocol = MP_RMTOOL_GAMELOG_ITEM_ACK;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100)
	{
		Msg.bEnd = FALSE;
		GameLogItemFromItemDBIndex( connectIdx, tempIdx, startIdx, 0, Msg.bDayOnly, Msg.strStart, Msg.strEnd, Msg.dwItemDBIdx );
	}
	else
	{
		// ÇöÀç ±îÁöÀÇ Á¤º¸°¡ ÀüºÎÀÌ´Ù ¶ó°í ¾Ë¸°´Ù.
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(Msg) );	
}

// jsd add 04.08.25 -----------------------------
// IP Check
void TotalIpInfo( DWORD startIdx )
{
//	char txt[128];
	sprintf(txt, "EXEC RP_IpAdressInfo %d", startIdx);	
	if(g_DB.LoginQuery(eQueryType_FreeQuery, eTotalIpInfo, 0, txt) == FALSE)
	{
	}
}

void RTotalIpInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	IPCHECKMGR->Release();
	DWORD count = pMessage->dwResult;

	if( count == 0 )
		return;

	DWORD startIdx = 0;

	for( DWORD i = 0; i < count; ++i )
	{
		IPCHECKMGR->RegistIP( (char*)pData[i].Data[eII_IPAdress] );
		startIdx = atoi((char*)pData[i].Data[eII_IPIdx]);
	}

// ip 100°³ ³ÑÀ¸¸é ¾ÈµÊ
//	if( count >= 100)
//		TotalIpInfo( startIdx );
}

// New Log
void ItemMoneyLogFromType( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, WORD wLogType, DWORD dwCharIdx, DWORD dwItem, int nDay, char* sSDate, char* sEDate )
{
	sprintf( txt, "EXEC TP_ItemMoneyLogFromType %d, %d, %d, %d, %d, \'%s\', \'%s\'", dwStartIdx, wLogType, dwCharIdx, dwItem, nDay, sSDate, sEDate );
	if( g_DB.LogQuery(eQueryType_FreeQuery, eItemMoneyLogFromType, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

void RItemMoneyLogFromType( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );

	TMSG_LOGITEMMONEY Msg;
	memset( &Msg, 0, sizeof(TMSG_LOGITEMMONEY) );

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&msg, sizeof(TMSGBASE) );
		return;
	}

	Msg.wCount = 0;
	Msg.wType = atoi((char*)pData[0].Data[eIML_Type]);
	Msg.dwCharIdx = atoi((char*)pData[0].Data[eIML_ChrName]);
	Msg.dwItem = atoi((char*)pData[0].Data[eIML_Item]);
	Msg.bDayOnly = atoi((char*)pData[0].Data[eIML_DayOnly]);
	strcpy( Msg.sSDate, (char*)pData[0].Data[eIML_SDate] );
	strcpy( Msg.sEDate, (char*)pData[0].Data[eIML_EDate] );

	DWORD startIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		Msg.sLog[i].dwLogIdx = atoi((char*)pData[i].Data[eIML_LogIdx]);
		Msg.sLog[i].dwLogType = atoi((char*)pData[i].Data[eIML_LogType]);
		strcpy( Msg.sLog[i].sFromChrName, (char*)pData[i].Data[eIML_FromChrName] );
		Msg.sLog[i].dwFromChrIdx = atoi((char*)pData[i].Data[eIML_FromChrIdx]);		
		strcpy( Msg.sLog[i].sToChrName, (char*)pData[i].Data[eIML_ToChrName] );
		Msg.sLog[i].dwToChrIdx = atoi((char*)pData[i].Data[eIML_ToChrIdx]);		
		Msg.sLog[i].dwFromTotalMoney = atoi((char*)pData[i].Data[eIML_FromTotalMoney]);
		Msg.sLog[i].dwToTotalMoney = atoi((char*)pData[i].Data[eIML_ToTotalMoney]);
		Msg.sLog[i].dwChangeMoney = atoi((char*)pData[i].Data[eIML_ChangeMoney]);
		Msg.sLog[i].dwItemIdx = atoi((char*)pData[i].Data[eIML_ItemIdx]);
		Msg.sLog[i].dwItemDBIdx = atoi((char*)pData[i].Data[eIML_ItemDBIdx]);
		Msg.sLog[i].dwItemDur = atoi((char*)pData[i].Data[eIML_ItemDur]);
		Msg.sLog[i].dwItemFromPosition = atoi((char*)pData[i].Data[eIML_ItemFromPosition]);
		Msg.sLog[i].dwItemToPosition = atoi((char*)pData[i].Data[eIML_ItemToPosition]);
		strcpy( Msg.sLog[i].sRegDate, (char*)pData[i].Data[eIML_RegDate] );
		strcat( Msg.sLog[i].sRegDate, " ");
		strcat( Msg.sLog[i].sRegDate, (char*)pData[i].Data[eIML_RegTime]);

		startIdx = Msg.sLog[i].dwLogIdx;
		++Msg.wCount;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
	Msg.Protocol = MP_RMTOOL_ITEMMONEYLOGFROMTYPE_ACK;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100 )
	{
		Msg.bEnd = FALSE;
		ItemMoneyLogFromType( connectIdx, tempIdx, startIdx, Msg.wType, Msg.dwCharIdx, Msg.dwItem, Msg.bDayOnly, Msg.sSDate, Msg.sEDate );
	}
	else
	{
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(Msg) );
}

void CharacterLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate )
{
	sprintf( txt, "EXEC TP_CharacterLog %d, %d, %d, \'%s\', \'%s\'", dwStartIdx, dwCharIdx, nDay, sSDate, sEDate );
	if( g_DB.LogQuery(eQueryType_FreeQuery, eCharacterLog, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

void RCharacterLog( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );

	TMSG_CHARACTERLOG Msg;
	memset( &Msg, 0, sizeof(TMSG_CHARACTERLOG) );

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&msg, sizeof(TMSGBASE) );
		return;
	}

	Msg.wCount = pMessage->dwResult;
	Msg.dwCharIdx = atoi((char*)pData[0].Data[0]);
	Msg.bDayOnly = atoi((char*)pData[0].Data[1]);
	strcpy( Msg.sSDate, (char*)pData[0].Data[2] );
	strcpy( Msg.sEDate, (char*)pData[0].Data[3] );

	DWORD startIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		Msg.sLog[i].dwLogIdx = atoi((char*)pData[i].Data[4]);
		Msg.sLog[i].dwChrIdx = atoi((char*)pData[i].Data[5]);		
		Msg.sLog[i].dwGrade = atoi((char*)pData[i].Data[6]);
		Msg.sLog[i].dwGengoal = atoi((char*)pData[i].Data[7]);
		Msg.sLog[i].dwDex = atoi((char*)pData[i].Data[8]);
		Msg.sLog[i].dwStrength = atoi((char*)pData[i].Data[9]);
		Msg.sLog[i].dwSimmak = atoi((char*)pData[i].Data[10]);
		Msg.sLog[i].dwPoint = atoi((char*)pData[i].Data[11]);
		strcpy( Msg.sLog[i].sLogDate, (char*)pData[i].Data[12] );
		strcat( Msg.sLog[i].sLogDate, " ");
		strcat( Msg.sLog[i].sLogDate, (char*)pData[i].Data[13]);

		startIdx = Msg.sLog[i].dwLogIdx;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
	Msg.Protocol = MP_RMTOOL_CHARACTERLOG_ACK;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100 )
	{
		Msg.bEnd = FALSE;
		CharacterLog( connectIdx, tempIdx, startIdx, Msg.dwCharIdx, Msg.bDayOnly, Msg.sSDate, Msg.sEDate );
	}
	else
	{
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(TMSG_CHARACTERLOG) );
}

void ExpPointLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate )
{
	sprintf( txt, "EXEC TP_ExpPointLog %d, %d, %d, \'%s\', \'%s\'", dwStartIdx, dwCharIdx, nDay, sSDate, sEDate );
	if( g_DB.LogQuery(eQueryType_FreeQuery, eExpPointLog, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

void RExpPointLog( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );

	TMSG_EXPPOINTLOG Msg;
	memset( &Msg, 0, sizeof(TMSG_EXPPOINTLOG) );

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&msg, sizeof(TMSGBASE) );
		return;
	}

	Msg.wCount = pMessage->dwResult;
	Msg.dwCharIdx = atoi((char*)pData[0].Data[0]);
	Msg.bDayOnly = atoi((char*)pData[0].Data[1]);
	strcpy( Msg.sSDate, (char*)pData[0].Data[2] );
	strcpy( Msg.sEDate, (char*)pData[0].Data[3] );

	DWORD startIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		Msg.sLog[i].dwLogIdx = atoi((char*)pData[i].Data[4]);
		Msg.sLog[i].dwChrIdx = atoi((char*)pData[i].Data[5]);		
		Msg.sLog[i].dwGrade = atoi((char*)pData[i].Data[6]);
		Msg.sLog[i].dwChangeValue = atoi((char*)pData[i].Data[7]);
		Msg.sLog[i].dwKillerKind = atoi((char*)pData[i].Data[8]);
		Msg.sLog[i].dwKillerIdx = atoi((char*)pData[i].Data[9]);
		Msg.sLog[i].dwExpPoint = atoi((char*)pData[i].Data[10]);
		Msg.sLog[i].dwAbilPoint = atoi((char*)pData[i].Data[11]);
		strcpy( Msg.sLog[i].sLogDate, (char*)pData[i].Data[12] );
		strcat( Msg.sLog[i].sLogDate, " ");
		strcat( Msg.sLog[i].sLogDate, (char*)pData[i].Data[13]);

		startIdx = Msg.sLog[i].dwLogIdx;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
	Msg.Protocol = MP_RMTOOL_EXPPOINTLOG_ACK;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100 )
	{
		Msg.bEnd = FALSE;
		ExpPointLog( connectIdx, tempIdx, startIdx, Msg.dwCharIdx, Msg.bDayOnly, Msg.sSDate, Msg.sEDate );
	}
	else
	{
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(TMSG_EXPPOINTLOG) );
}

void MugongLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate )
{
	sprintf( txt, "EXEC TP_MugongLog %d, %d, %d, \'%s\', \'%s\'", dwStartIdx, dwCharIdx, nDay, sSDate, sEDate );
	if( g_DB.LogQuery(eQueryType_FreeQuery, eMugongLog, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

void RMugongLog( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );

	TMSG_MUGONGLOG Msg;
	memset( &Msg, 0, sizeof(TMSG_MUGONGLOG) );

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&msg, sizeof(TMSGBASE) );
		return;
	}

	Msg.wCount = pMessage->dwResult;
	Msg.dwCharIdx = atoi((char*)pData[0].Data[0]);
	Msg.bDayOnly = atoi((char*)pData[0].Data[1]);
	strcpy( Msg.sSDate, (char*)pData[0].Data[2] );
	strcpy( Msg.sEDate, (char*)pData[0].Data[3] );

	DWORD startIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		Msg.sLog[i].dwLogIdx = atoi((char*)pData[i].Data[4]);
		Msg.sLog[i].dwLogType = atoi((char*)pData[i].Data[5]);		
		Msg.sLog[i].dwChrIdx = atoi((char*)pData[i].Data[6]);
		Msg.sLog[i].dwSkillIdx = atoi((char*)pData[i].Data[7]);
		Msg.sLog[i].dwSkillDBIdx = atoi((char*)pData[i].Data[8]);
		Msg.sLog[i].dwSkillLevel = atoi((char*)pData[i].Data[9]);
		Msg.sLog[i].dwSkillExp = atoi((char*)pData[i].Data[10]);
		strcpy( Msg.sLog[i].sLogDate, (char*)pData[i].Data[11] );
		strcat( Msg.sLog[i].sLogDate, " ");
		strcat( Msg.sLog[i].sLogDate, (char*)pData[i].Data[12]);

		startIdx = Msg.sLog[i].dwLogIdx;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
	Msg.Protocol = MP_RMTOOL_MUGONGLOG_ACK;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100 )
	{
		Msg.bEnd = FALSE;
		MugongLog( connectIdx, tempIdx, startIdx, Msg.dwCharIdx, Msg.bDayOnly, Msg.sSDate, Msg.sEDate );
	}
	else
	{
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(TMSG_MUGONGLOG) );

}

void MugongExpLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate )
{
	sprintf( txt, "EXEC TP_MugongExpLog %d, %d, %d, \'%s\', \'%s\'", dwStartIdx, dwCharIdx, nDay, sSDate, sEDate );
	if( g_DB.LogQuery(eQueryType_FreeQuery, eMugongExpLog, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

void RMugongExpLog( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );

	TMSG_MUGONGEXPLOG Msg;
	memset( &Msg, 0, sizeof(TMSG_MUGONGEXPLOG) );

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&msg, sizeof(TMSGBASE) );
		return;
	}

	Msg.wCount = pMessage->dwResult;
	Msg.dwCharIdx = atoi((char*)pData[0].Data[0]);
	Msg.bDayOnly = atoi((char*)pData[0].Data[1]);
	strcpy( Msg.sSDate, (char*)pData[0].Data[2] );
	strcpy( Msg.sEDate, (char*)pData[0].Data[3] );

	DWORD startIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		Msg.sLog[i].dwLogIdx = atoi((char*)pData[i].Data[4]);
		Msg.sLog[i].dwChrIdx = atoi((char*)pData[i].Data[5]);		
		Msg.sLog[i].dwChangeValue = atoi((char*)pData[i].Data[6]);
		Msg.sLog[i].dwSkillIdx = atoi((char*)pData[i].Data[7]);
		Msg.sLog[i].dwSkillDBIdx = atoi((char*)pData[i].Data[8]);
		Msg.sLog[i].dwSkillLevel = atoi((char*)pData[i].Data[9]);
		Msg.sLog[i].dwSkillExp = atoi((char*)pData[i].Data[10]);
		Msg.sLog[i].dwSkillPos = atoi((char*)pData[i].Data[11]);
		strcpy( Msg.sLog[i].sLogDate, (char*)pData[i].Data[12] );
		strcat( Msg.sLog[i].sLogDate, " ");
		strcat( Msg.sLog[i].sLogDate, (char*)pData[i].Data[13]);

		startIdx = Msg.sLog[i].dwLogIdx;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
	Msg.Protocol = MP_RMTOOL_MUGONGEXPLOG_ACK;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100 )
	{
		Msg.bEnd = FALSE;
		MugongExpLog( connectIdx, tempIdx, startIdx, Msg.dwCharIdx, Msg.bDayOnly, Msg.sSDate, Msg.sEDate );
	}
	else
	{
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(TMSG_MUGONGEXPLOG) );
}

void MoneyLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate )
{
	sprintf( txt, "EXEC TP_MoneyLog %d, %d, %d, \'%s\', \'%s\'", dwStartIdx, dwCharIdx, nDay, sSDate, sEDate );
	if( g_DB.LogQuery(eQueryType_FreeQuery, eMoneyLog, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

void RMoneyLog( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );

	TMSG_MONEYLOG Msg;
	memset( &Msg, 0, sizeof(TMSG_MONEYLOG) );

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&msg, sizeof(TMSGBASE) );
		return;
	}

	Msg.wCount = pMessage->dwResult;
	Msg.dwCharIdx = atoi((char*)pData[0].Data[0]);
	Msg.bDayOnly = atoi((char*)pData[0].Data[1]);
	strcpy( Msg.sSDate, (char*)pData[0].Data[2] );
	strcpy( Msg.sEDate, (char*)pData[0].Data[3] );

	DWORD startIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		Msg.sLog[i].dwLogIdx = atoi((char*)pData[i].Data[4]);
		Msg.sLog[i].dwChrIdx = atoi((char*)pData[i].Data[5]);		
		Msg.sLog[i].dwChangeValue = atoi((char*)pData[i].Data[6]);
		Msg.sLog[i].dwInvenMoney = atoi((char*)pData[i].Data[7]);
		Msg.sLog[i].dwTargetIdx = atoi((char*)pData[i].Data[8]);
		Msg.sLog[i].dwTargetMoney = atoi((char*)pData[i].Data[9]);
		strcpy( Msg.sLog[i].sLogDate, (char*)pData[i].Data[10] );
		strcat( Msg.sLog[i].sLogDate, " ");
		strcat( Msg.sLog[i].sLogDate, (char*)pData[i].Data[11]);

		startIdx = Msg.sLog[i].dwLogIdx;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
	Msg.Protocol = MP_RMTOOL_MONEYLOG_ACK;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100 )
	{
		Msg.bEnd = FALSE;
		MoneyLog( connectIdx, tempIdx, startIdx, Msg.dwCharIdx, Msg.bDayOnly, Msg.sSDate, Msg.sEDate );
	}
	else
	{
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(TMSG_MONEYLOG) );
}

void MoneyWrongLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate )
{
	sprintf( txt, "EXEC TP_MoneyWrongLog %d, %d, %d, \'%s\', \'%s\'", dwStartIdx, dwCharIdx, nDay, sSDate, sEDate );
	if( g_DB.LogQuery(eQueryType_FreeQuery, eMoneyWrongLog, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

void RMoneyWrongLog( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );

	TMSG_MONEYWRONGLOG Msg;
	memset( &Msg, 0, sizeof(TMSG_MONEYWRONGLOG) );

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&msg, sizeof(TMSGBASE) );
		return;
	}

	Msg.wCount = pMessage->dwResult;
	Msg.dwCharIdx = atoi((char*)pData[0].Data[0]);
	Msg.bDayOnly = atoi((char*)pData[0].Data[1]);
	strcpy( Msg.sSDate, (char*)pData[0].Data[2] );
	strcpy( Msg.sEDate, (char*)pData[0].Data[3] );

	DWORD startIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		Msg.sLog[i].dwLogIdx = atoi((char*)pData[i].Data[4]);
		Msg.sLog[i].dwChrIdx = atoi((char*)pData[i].Data[5]);		
		Msg.sLog[i].dwChangeValue = atoi((char*)pData[i].Data[6]);
		Msg.sLog[i].dwTotalMoney = atoi((char*)pData[i].Data[7]);
		Msg.sLog[i].dwPyogukMoney = atoi((char*)pData[i].Data[8]);
		Msg.sLog[i].dwTargetIdx = atoi((char*)pData[i].Data[9]);
		strcpy( Msg.sLog[i].sLogDate, (char*)pData[i].Data[10] );
		strcat( Msg.sLog[i].sLogDate, " ");
		strcat( Msg.sLog[i].sLogDate, (char*)pData[i].Data[11]);

		startIdx = Msg.sLog[i].dwLogIdx;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
	Msg.Protocol = MP_RMTOOL_MONEYWRONGLOG_ACK;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100 )
	{
		Msg.bEnd = FALSE;
		MoneyWrongLog( connectIdx, tempIdx, startIdx, Msg.dwCharIdx, Msg.bDayOnly, Msg.sSDate, Msg.sEDate );
	}
	else
	{
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(TMSG_MONEYWRONGLOG) );
}

void AbilityLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate )
{
	sprintf( txt, "EXEC TP_AbilityLog %d, %d, %d, \'%s\', \'%s\'", dwStartIdx, dwCharIdx, nDay, sSDate, sEDate );
	if( g_DB.LogQuery(eQueryType_FreeQuery, eAbilityLog, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

void RAbilityLog( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );

	TMSG_ABILITYLOG Msg;
	memset( &Msg, 0, sizeof(TMSG_ABILITYLOG) );

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&msg, sizeof(TMSGBASE) );
		return;
	}

	Msg.wCount = pMessage->dwResult;
	Msg.dwCharIdx = atoi((char*)pData[0].Data[0]);
	Msg.bDayOnly = atoi((char*)pData[0].Data[1]);
	strcpy( Msg.sSDate, (char*)pData[0].Data[2] );
	strcpy( Msg.sEDate, (char*)pData[0].Data[3] );

	DWORD startIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		Msg.sLog[i].dwLogIdx = atoi((char*)pData[i].Data[4]);
		Msg.sLog[i].dwChrIdx = atoi((char*)pData[i].Data[5]);
		Msg.sLog[i].dwAbilityIdx = atoi((char*)pData[i].Data[6]);		
		Msg.sLog[i].dwAbilityLevelTo = atoi((char*)pData[i].Data[7]);
		Msg.sLog[i].dwAbilityExpFrom = atoi((char*)pData[i].Data[8]);
		Msg.sLog[i].dwAbilityExpTo = atoi((char*)pData[i].Data[9]);
		strcpy( Msg.sLog[i].sLogDate, (char*)pData[i].Data[10] );
		strcat( Msg.sLog[i].sLogDate, " ");
		strcat( Msg.sLog[i].sLogDate, (char*)pData[i].Data[11]);

		startIdx = Msg.sLog[i].dwLogIdx;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
	Msg.Protocol = MP_RMTOOL_ABILITYLOG_ACK;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100 )
	{
		Msg.bEnd = FALSE;
		AbilityLog( connectIdx, tempIdx, startIdx, Msg.dwCharIdx, Msg.bDayOnly, Msg.sSDate, Msg.sEDate );
	}
	else
	{
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(TMSG_ABILITYLOG) );
}

void CheatLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate )
{
	sprintf( txt, "EXEC TP_CheatLog %d, %d, %d, \'%s\', \'%s\'", dwStartIdx, dwCharIdx, nDay, sSDate, sEDate );
	if( g_DB.LogQuery(eQueryType_FreeQuery, eCheatLog, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

void RCheatLog( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );

	TMSG_CHEATLOG Msg;
	memset( &Msg, 0, sizeof(TMSG_CHEATLOG) );

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&msg, sizeof(TMSGBASE) );
		return;
	}

	Msg.wCount = pMessage->dwResult;
	Msg.dwCharIdx = atoi((char*)pData[0].Data[0]);
	Msg.bDayOnly = atoi((char*)pData[0].Data[1]);
	strcpy( Msg.sSDate, (char*)pData[0].Data[2] );
	strcpy( Msg.sEDate, (char*)pData[0].Data[3] );

	DWORD startIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		Msg.sLog[i].dwLogIdx = atoi((char*)pData[i].Data[4]);
		Msg.sLog[i].dwChrIdx = atoi((char*)pData[i].Data[5]);
		Msg.sLog[i].dwLogType = atoi((char*)pData[i].Data[6]);		
		Msg.sLog[i].dwParam = atoi((char*)pData[i].Data[7]);
		Msg.sLog[i].dwItemDBIdx = atoi((char*)pData[i].Data[8]);
		Msg.sLog[i].dwItemIdx = atoi((char*)pData[i].Data[9]);
		strcpy( Msg.sLog[i].sLogDate, (char*)pData[i].Data[10] );
		strcat( Msg.sLog[i].sLogDate, " ");
		strcat( Msg.sLog[i].sLogDate, (char*)pData[i].Data[11]);

		startIdx = Msg.sLog[i].dwLogIdx;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
	Msg.Protocol = MP_RMTOOL_CHEATLOG_ACK;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100 )
	{
		Msg.bEnd = FALSE;
		AbilityLog( connectIdx, tempIdx, startIdx, Msg.dwCharIdx, Msg.bDayOnly, Msg.sSDate, Msg.sEDate );
	}
	else
	{
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(TMSG_CHEATLOG) );
}

void HackingLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate )
{
	sprintf( txt, "EXEC TP_HackingLog %d, %d, %d, \'%s\', \'%s\'", dwStartIdx, dwCharIdx, nDay, sSDate, sEDate );
	if( g_DB.LogQuery(eQueryType_FreeQuery, eHackingLog, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

void RHackingLog( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );

	TMSG_HACKINGLOG Msg;
	memset( &Msg, 0, sizeof(TMSG_HACKINGLOG) );

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&msg, sizeof(TMSGBASE) );
		return;
	}

	Msg.wCount = pMessage->dwResult;
	Msg.dwCharIdx = atoi((char*)pData[0].Data[0]);
	Msg.bDayOnly = atoi((char*)pData[0].Data[1]);
	strcpy( Msg.sSDate, (char*)pData[0].Data[2] );
	strcpy( Msg.sEDate, (char*)pData[0].Data[3] );

	DWORD startIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		Msg.sLog[i].dwLogIdx = atoi((char*)pData[i].Data[4]);
		Msg.sLog[i].dwChrIdx = atoi((char*)pData[i].Data[5]);
		Msg.sLog[i].dwCheatKind = atoi((char*)pData[i].Data[6]);
		strcpy( Msg.sLog[i].sLogDate, (char*)pData[i].Data[7] );
		strcat( Msg.sLog[i].sLogDate, " ");
		strcat( Msg.sLog[i].sLogDate, (char*)pData[i].Data[8]);

		startIdx = Msg.sLog[i].dwLogIdx;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
	Msg.Protocol = MP_RMTOOL_HACKINGLOG_ACK;
	Msg.dwTemplateIdx = tempIdx;

	if( pMessage->dwResult >= 100 )
	{
		Msg.bEnd = FALSE;
		AbilityLog( connectIdx, tempIdx, startIdx, Msg.dwCharIdx, Msg.bDayOnly, Msg.sSDate, Msg.sEDate );
	}
	else
	{
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(TMSG_HACKINGLOG) );
}

void ToolLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwLogType, char* sName, int nDay, char* sSDate, char* sEDate )
{
	sprintf( txt, "EXEC TP_ToolLog %d, %d, \'%s\', %d, \'%s\', \'%s\'", dwStartIdx, dwLogType, sName, nDay, sSDate, sEDate );
	if( g_DB.LogQuery(eQueryType_FreeQuery, eToolLog, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

void RToolLog( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD( pMessage->dwID );
	WORD connectIdx = LOWORD( pMessage->dwID );

	TMSG_TOOLLOG Msg;
	memset( &Msg, 0, sizeof(TMSG_TOOLLOG) );

	if( pMessage->dwResult == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&msg, sizeof(TMSGBASE) );
		return;
	}

	int check = atoi((char*)pData[0].Data[0]);
	if( check == -1 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_GAMELOGSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC( connectIdx, (char*)&msg, sizeof(TMSGBASE) );
		return;
	}

	Msg.Category = MP_RMTOOL_GAMELOG;
	Msg.Protocol = MP_RMTOOL_TOOLLOG_ACK;
	Msg.dwTemplateIdx = tempIdx;
	Msg.wCount = pMessage->dwResult;
	Msg.dwLogType = atoi((char*)pData[0].Data[0]);
	strcpy( Msg.sName, (char*)pData[0].Data[1]);
	Msg.bDayOnly = atoi((char*)pData[0].Data[2]);
	strcpy( Msg.sSDate, (char*)pData[0].Data[3] );
	strcpy( Msg.sEDate, (char*)pData[0].Data[4] );

	DWORD startIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		Msg.sLog[i].dwLogIdx = atoi((char*)pData[i].Data[5]);
		Msg.sLog[i].dwLogType = atoi((char*)pData[i].Data[6]);
		Msg.sLog[i].dwLogKind = atoi((char*)pData[i].Data[7]);
		Msg.sLog[i].dwOperIdx = atoi((char*)pData[i].Data[8]);
		strncpy( Msg.sLog[i].sOperName, (char*)pData[i].Data[9], 17 );
		Msg.sLog[i].dwTargetIdx = atoi((char*)pData[i].Data[10]);
		strncpy( Msg.sLog[i].sTargetName, (char*)pData[i].Data[11], 17 );
		Msg.sLog[i].dwParam1 = atoi((char*)pData[i].Data[12]);
		Msg.sLog[i].dwParam2 = atoi((char*)pData[i].Data[13]);
		Msg.sLog[i].dwParam3 = atoi((char*)pData[i].Data[14]);
		Msg.sLog[i].dwParam4 = atoi((char*)pData[i].Data[15]);
		strcpy( Msg.sLog[i].sLogDate, (char*)pData[i].Data[16] );
		strcat( Msg.sLog[i].sLogDate, " ");
		strcat( Msg.sLog[i].sLogDate, (char*)pData[i].Data[17]);

		startIdx = Msg.sLog[i].dwLogIdx;
	}	

	if( pMessage->dwResult >= 100 )
	{
		Msg.bEnd = FALSE;
		ToolLog( connectIdx, tempIdx, startIdx, Msg.dwLogType, Msg.sName, Msg.bDayOnly, Msg.sSDate, Msg.sEDate );
	}
	else
	{
		Msg.bEnd = TRUE;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(TMSG_TOOLLOG) );
}

// user info
void SearchMemberNumFromID( WORD wConnectIdx, WORD wTempIdx, char* sID )
{
	sprintf( txt, "EXEC RP_SearchMemberNumFromID \'%s\'", sID );
	if( g_DB.LoginQuery(eQueryType_FreeQuery, eSearchMemberNumFromID, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

void RSearchMemberNumFromID( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	if( count == 0 )
	{
		// No Data
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_USER;
		msg.Protocol = MP_RMTOOL_QUERY_MEMBERNUM_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC(connectIdx, (char*)&msg, sizeof(TMSGBASE));
	}
	else
	{
		TMSG_DWORD Msg;
		Msg.Category = MP_RMTOOL_USER;
		Msg.Protocol = MP_RMTOOL_QUERY_MEMBERNUM_ACK;
		Msg.dwTemplateIdx = tempIdx;
		Msg.dwData = (DWORD)atoi((char*)pData[0].Data[0]);

		MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(TMSG_DWORD));
	}
}

void SearchMemberNumFromCharName( WORD wConnectIdx, WORD wTempIdx, char* sCharName )
{
	sprintf( txt, "EXEC TP_SearchMemberNumFromCharName \'%s\'", sCharName );
	if( g_DB.Query(eQueryType_FreeQuery, eSearchMemberNumFromCharName, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

void RSearchMemberNumFromCharName( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	if( count == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_USER;
		msg.Protocol = MP_RMTOOL_QUERY_MEMBERNUM_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC(connectIdx, (char*)&msg, sizeof(TMSGBASE));
	}
	else
	{
		TMSG_DWORD Msg;
		Msg.Category = MP_RMTOOL_USER;
		Msg.Protocol = MP_RMTOOL_QUERY_MEMBERNUM_ACK;
		Msg.dwTemplateIdx = tempIdx;
		Msg.dwData = (DWORD)atoi((char*)pData[0].Data[0]);

		MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(TMSG_DWORD));
	}
}

void SearchCharIdxFromCharName( WORD wConnectIdx, WORD wTempIdx, char* sCharName )
{
	sprintf( txt, "EXEC TP_SearchCharIdxFromCharName \'%s\'", sCharName );
	if( g_DB.Query(eQueryType_FreeQuery, eSearchCharIdxFromCharName, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

void RSearchCharIdxFromCharName( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	if( count == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_GAMELOG;
		msg.Protocol = MP_RMTOOL_QUERY_CHARIDX_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC(connectIdx, (char*)&msg, sizeof(TMSGBASE));
	}
	else
	{
		TMSG_DWORD Msg;
		Msg.Category = MP_RMTOOL_GAMELOG;
		Msg.Protocol = MP_RMTOOL_QUERY_CHARIDX_ACK;
		Msg.dwTemplateIdx = tempIdx;
		Msg.dwData = (DWORD)atoi((char*)pData[0].Data[0]);	// charidx

		MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(TMSG_DWORD));
	}
}

void SearchUserInfoFromMemberNum( WORD wConnectIdx, WORD wTempIdx, DWORD dwMemberNum, WORD wServer )
{
	sprintf( txt, "EXEC RP_SearchUserInfoByMemberNo %d, %d", wServer, dwMemberNum );
	if( g_DB.LoginMiddleQuery( RSearchUserInfoFromMemberNum, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE )
	{
	}
}

void RSearchUserInfoFromMemberNum( LPMIDDLEQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	if( count == 0 )
	{
		TMSGBASE msg;
		msg.Category = MP_RMTOOL_USER;
		msg.Protocol = MP_RMTOOL_USERSEARCH_NACK;
		msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC(connectIdx, (char*)&msg, sizeof(msg));
	}
	else
	{
		TMSG_USERSEARCHRESULT Info;

		Info.sUser.bLoginFlag = atoi((char*)pData[0].Data[eUI_IsLogin]);
		Info.sUser.dwMemberNo = atoi((char*)pData[0].Data[eUI_User_Idx]);
		strcpy(Info.sUser.strID, (char*)pData[0].Data[eUI_Id]);
		strcpy(Info.sUser.strPass, (char*)pData[0].Data[eUI_Pwd]);
		strcpy(Info.sUser.strName, (char*)pData[0].Data[eUI_Name]);
		strcpy(Info.sUser.strIdCard, (char*)pData[0].Data[eUI_Idcard]);
		strcpy(Info.sUser.strPhon, (char*)pData[0].Data[eUI_Phone]);
		strcpy(Info.sUser.strEmail, (char*)pData[0].Data[eUI_Email]);
		strcpy(Info.sUser.strRegDateTime, (char*)pData[0].Data[eUI_Regdate]);
		strcpy(Info.sUser.strLastLoginTime, (char*)pData[0].Data[eUI_LastLogintime]);
		strcpy(Info.sUser.strLastLogoutTime, (char*)pData[0].Data[eUI_LastLogouttime]);

		Info.sUser.nTotalTime = atoi((char*)pData[0].Data[eUI_Totaltime]);
		int temp = Info.sUser.nTotalTime;
		int oo, mm, ss;
		ss = temp%60;
		mm = (temp/60)%60;
		oo = (temp/3600);
		sprintf(Info.sUser.strTotalTime, "%d:%d:%d", oo, mm, ss );

		Info.sUser.wUserLevel = atoi((char*)pData[0].Data[eUI_UserLevel]);
		Info.wServer = atoi((char*)pData[0].Data[eUI_Char_Server]);

		Info.Category = MP_RMTOOL_USER;
		Info.Protocol = MP_RMTOOL_USERSEARCH_ACK;
		Info.dwTemplateIdx = tempIdx;
		MNETWORK->SendToMC(connectIdx, (char*)&Info, sizeof(Info));
	}
}

void SimpleCharacterInfoFromMN( WORD wConnectIdx, WORD wTempIdx, DWORD dwMemberNum )
{
	sprintf(txt, "EXEC TP_SimpleCharacterInfoFromMN %d", dwMemberNum);	
	if(g_DB.Query(eQueryType_FreeQuery, eSimpleCharacterInfoFromMN, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RSimpleCharacterInfoFromMN( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	if( count == 0 )
	{
		TMSGBASE Msg;
		Msg.Category = MP_RMTOOL_USER;
		Msg.Protocol = MP_RMTOOL_SIMPLECHARACTER_NACK;
		Msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(TMSGBASE));
	}
	else
	{
		TMSG_SIMPLERESULT Msg;
		Msg.Category = MP_RMTOOL_USER;
		Msg.Protocol = MP_RMTOOL_SIMPLECHARACTER_ACK;
		Msg.dwTemplateIdx = tempIdx;
		Msg.Count = count;

		for( DWORD i = 0; i < count; ++i )
		{
			Msg.sSimple[i].dwUserIdx = atoi((char*)pData[i].Data[0]);
			Msg.sSimple[i].dwCharNo = atoi((char*)pData[i].Data[1]);
			strcpy( Msg.sSimple[i].strName, (char*)pData[i].Data[2]);
			strcpy( Msg.sSimple[i].strCreateDate, (char*)pData[i].Data[3]);
			strcat( Msg.sSimple[i].strCreateDate, " ");
			strcat( Msg.sSimple[i].strCreateDate, (char*)pData[i].Data[4]);
		}

		MNETWORK->SendToMC( connectIdx, (char*)&Msg, Msg.GetMsgSize() );
	}
}

void CharacterCreateInfoFromMN( WORD wConnectIdx, WORD wTempIdx, DWORD dwMemberNum )
{
	sprintf(txt, "EXEC TP_CharacterCreateInfoFromMN %d", dwMemberNum);	
	if(g_DB.Query(eQueryType_FreeQuery, eCharacterCreateInfoFromMN, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RCharacterCreateInfoFromMN( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	if( count == 0 )
	{
		TMSGBASE Msg;
		Msg.Category = MP_RMTOOL_USER;
		Msg.Protocol = MP_RMTOOL_CHARACTER_CREATEINFO_NACK;
		Msg.dwTemplateIdx = tempIdx;

		MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(TMSGBASE));
	}
	else
	{
		TMSG_CHARACTERCREATEINFO Msg;
		Msg.Category = MP_RMTOOL_USER;
		Msg.Protocol = MP_RMTOOL_CHARACTER_CREATEINFO_ACK;
		Msg.dwTemplateIdx = tempIdx;
		Msg.Count = count;

		for( BYTE i = 0; i < count; ++i )
		{
			Msg.sInfo[i].dwCharIdx = atoi((char*)pData[i].Data[0]);
			Msg.sInfo[i].dwUserIdx = atoi((char*)pData[i].Data[1]);
			strcpy( Msg.sInfo[i].sName, (char*)pData[i].Data[2] );
			Msg.sInfo[i].dwFlag = atoi((char*)pData[i].Data[3]);
			strcpy( Msg.sInfo[i].sCreateDate, (char*)pData[i].Data[4] );
			strcat( Msg.sInfo[i].sCreateDate, " ");
			strcat( Msg.sInfo[i].sCreateDate, (char*)pData[i].Data[5]);
			strcpy( Msg.sInfo[i].sCreateIP, (char*)pData[i].Data[6] );
			strcpy( Msg.sInfo[i].sDeleteDate, (char*)pData[i].Data[7] );
			strcat( Msg.sInfo[i].sDeleteDate, " ");
			strcat( Msg.sInfo[i].sDeleteDate, (char*)pData[i].Data[8]);
			strcpy( Msg.sInfo[i].sDeleteIP, (char*)pData[i].Data[9] );
			strcpy( Msg.sInfo[i].sRecoverDate, (char*)pData[i].Data[10] );
			strcat( Msg.sInfo[i].sRecoverDate, " ");
			strcat( Msg.sInfo[i].sRecoverDate, (char*)pData[i].Data[11]);
			strcpy( Msg.sInfo[i].sRecoverIP, (char*)pData[i].Data[12] );
		}

		MNETWORK->SendToMC( connectIdx, (char*)&Msg, Msg.GetMsgSize() );
	}	
}

void CheckCharacterName( WORD wConnectIdx, WORD wTempIdx, char* sName )
{
	sprintf(txt, "EXEC TP_CheckCharacterName \'%s\'", sName);	
	if(g_DB.Query(eQueryType_FreeQuery, eCheckCharacterName, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RCheckCharacterName( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD result = atoi((char*)pData->Data[0]);
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSGBASE Msg;
	Msg.Category = MP_RMTOOL_USER;
	Msg.dwTemplateIdx = tempIdx;
	
	switch( result )
	{
	case 0:
		Msg.Protocol = MP_RMTOOL_CHECKNAME_ACK;
		break;

	case 1:
	case 2:
	case 3:
		Msg.Protocol = MP_RMTOOL_CHECKNAME_NACK;
		break;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(Msg) );	
}

void CharacterRecover( WORD wConnectIdx, WORD wTempIdx, DWORD dwChrIdx, char* sName )
{
	char ip[32];
	WORD port;
	MNETWORK->GetUserAddress( wConnectIdx, ip, &port );

	sprintf(txt, "EXEC TP_CharacterRecover %d, \'%s\', \'%s\'", dwChrIdx, sName, ip);	
	if(g_DB.Query(eQueryType_FreeQuery, eCharacterRecover, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RCharacterRecover( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD result = atoi((char*)pData->Data[0]);
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_DWORD Msg;
	Msg.Category = MP_RMTOOL_USER;
	Msg.dwTemplateIdx = tempIdx;
	
	switch( result )
	{
	case 0:
		Msg.Protocol = MP_RMTOOL_CHARACTER_RECOVER_ACK;
		Msg.dwData = 0;
		break;

	case 1:	// ²Ë Âü.
	case 2:	// ÀÌ¸§ Áßº¹		
	case 3:	
	case 4:	// Name is Null
	case 5:
		Msg.Protocol = MP_RMTOOL_CHARACTER_RECOVER_NACK;
		Msg.dwData = result;
		break;
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(Msg) );
}


// quest
void CharacterMainQuestInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx )
{
	sprintf( txt, "EXEC RP_CharacterMainQuestInfo %d", dwCharIdx );	
	if(g_DB.Query(eQueryType_FreeQuery, eCharacterMainQuestInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}	
}

void RCharacterMainQuestInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_MAINQUESTINFO Msg;
	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_MAINQUESTINFO_ACK;
	Msg.dwTemplateIdx = tempIdx;
	Msg.dwCount = count;

	for( DWORD i = 0; i < count; ++i )
	{
		Msg.MainQuestInfo[i].dwCharIdx = (DWORD)atoi((char*)pData[i].Data[0]);
		Msg.MainQuestInfo[i].dwQuestIdx = (DWORD)atoi((char*)pData[i].Data[1]);
		Msg.MainQuestInfo[i].dwSubQuestDo = (DWORD)atoi((char*)pData[i].Data[2]);
		Msg.MainQuestInfo[i].dwEndParam = (DWORD)atoi((char*)pData[i].Data[3]);
		Msg.MainQuestInfo[i].dwRegDate = (DWORD)atoi((char*)pData[i].Data[4]);
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, Msg.GetMsgLength() );
}

void MainQuestDelete( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx, DWORD dwQuestIdx )
{
	sprintf( txt, "EXEC MP_MAINQUEST_Delete %d, %d", dwCharIdx, dwQuestIdx );	
	if(g_DB.Query(eQueryType_FreeQuery, eMainQuestDelete, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void CharacterSubQuestInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx, DWORD dwQuestIdx )
{
	sprintf( txt, "EXEC RP_CharacterSubQuestInfo %d, %d", dwCharIdx, dwQuestIdx );	
	if(g_DB.Query(eQueryType_FreeQuery, eCharacterSubQuestInfo, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}	
}

void RCharacterSubQuestInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_SUBQUESTINFO Msg;
	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_SUBQUESTINFO_ACK;
	Msg.dwTemplateIdx = tempIdx;
	Msg.dwCount = count;

	for( DWORD i = 0; i < count; ++i )
	{
		Msg.SubQuestInfo[i].dwCharIdx = (DWORD)atoi((char*)pData[i].Data[0]);
		Msg.SubQuestInfo[i].dwQuestIdx = (DWORD)atoi((char*)pData[i].Data[1]);
		Msg.SubQuestInfo[i].dwSubQuestIdx = (DWORD)atoi((char*)pData[i].Data[2]);
		Msg.SubQuestInfo[i].dwData = (DWORD)atoi((char*)pData[i].Data[3]);
		Msg.SubQuestInfo[i].dwRegDate = (DWORD)atoi((char*)pData[i].Data[4]);
	}

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, Msg.GetMsgLength() );
}

void InsertItemMoneyLog( DWORD FromChrIdx, char* FromChrName, DWORD ToChrIdx, char* ToChrName,
						 DWORD LogType, MONEYTYPE FromTotalMoney, MONEYTYPE ToTotalMoney, MONEYTYPE ChangeMoney,
						 DWORD ItemIdx, DWORD ItemDBIdx, POSTYPE ItemFromPos, POSTYPE ItemToPos, DURTYPE ItemDur, DWORD ExpPoint )
{
	sprintf(txt, "EXEC  %s %d, \'%s\', %d, \'%s\', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", "up_ItemMoneyLog", 
		FromChrIdx, FromChrName, ToChrIdx, ToChrName, 
		LogType, FromTotalMoney, ToTotalMoney, ChangeMoney,
		ItemIdx, ItemDBIdx, ItemFromPos, ItemToPos,
		ItemDur, ExpPoint);
	if(g_DB.LogQuery(eQueryType_FreeQuery, eLogItemMoney, 0, txt) == FALSE)
	{
	}
}

// userlevel
void ChangeUserLevel( WORD wConnectIdx, WORD wTempIdx, DWORD dwUserIdx, DWORD dwUserLevel )
{
	sprintf(txt, "EXEC TP_ChangeUserLevel %d, %d", dwUserIdx, dwUserLevel);	
	if(g_DB.LoginQuery(eQueryType_FreeQuery, eChangeUserLevel, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RChangeUserLevel( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_DWORD2 Msg;
	Msg.Category = MP_RMTOOL_USER;
	Msg.Protocol = MP_RMTOOL_CHANGE_USERLEVEL_ACK;
	Msg.dwTemplateIdx = tempIdx;
	Msg.dwData1 = (DWORD)atoi((char*)pData[0].Data[0]);		// useridx
	Msg.dwData2 = (DWORD)atoi((char*)pData[0].Data[1]);		// userlevel

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(TMSG_DWORD2) );
}

void ChangeLoginPoint( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharacterIdx, DWORD dwMapNum )
{
	sprintf(txt, "EXEC TP_ChangeLoginPoint %d, %d", dwCharacterIdx, dwMapNum);	
	if(g_DB.Query(eQueryType_FreeQuery, eChangeLoginPoint, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RChangeLoginPoint( LPQUERY pData, LPDBMESSAGE pMessage )
{
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	TMSG_DWORD2 Msg;
	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_CHANGE_LOGINPOINT_ACK;
	Msg.dwTemplateIdx = tempIdx;
	Msg.dwData1 = (DWORD)atoi((char*)pData[0].Data[0]);		// characteridx
	Msg.dwData2 = (DWORD)atoi((char*)pData[0].Data[1]);		// mapnum

	MNETWORK->SendToMC( connectIdx, (char*)&Msg, sizeof(TMSG_DWORD2) );
}

void CharacterInvenInfo2(WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx)
{
	sprintf(txt, "EXEC RP_CharacterInvenInfo %d", dwCharIdx);	
	if(g_DB.Query(eQueryType_FreeQuery, eCharacterInvenInfo2, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void RCharacterInvenInfo2(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	WORD tempIdx = HIWORD(pMessage->dwID);
	WORD connectIdx = LOWORD(pMessage->dwID);

	// °¡Á¤!!
	// ¾ÆÀÌÅÛÀÇ Æ÷Áö¼ÇÀº Àý´ë °ãÄ¥¼ö ¾ø´Ù
	// ¾ÆÀÌÅÛÀÇ Ä³¸¯ÅÍ ¹®ÆÄ Ã¢°í ÀÎµ¦½ºµµ Àý´ë °ãÄ¥¼ö ¾ø´Ù. (ÀÌ°Ç ¾î¶»°Ô º¹±¸ÇÏ³Ä!!)
	TMSG_CHARACTER_INVENINFO Msg;
	TMSG_CHARACTER_INVENINFO2 Msg2;
	memset(&Msg.m_sIteminfo, 0, sizeof(Msg.m_sIteminfo));

	POSTYPE p = 0;
	for(DWORD  i = 0; i < count; i++)
	{
		POSTYPE ItemPos = atoi((char*)pData[i].Data[eCI_Position]);
		// ±×¸®µå(ÀÎº¥Åä¸®)¾ÆÀÌÅÛ ¼ÂÆÃ
		if(ItemPos >= TP_INVENTORY_START && ItemPos < TP_INVENTORY_END)
		{
			ItemPos -= TP_INVENTORY_START;

			if( Msg.m_sIteminfo.Inventory[ItemPos].dwDBIdx == 0 )
			{
				Msg.m_sIteminfo.Inventory[ItemPos].dwDBIdx = atoi((char*)pData[i].Data[eCI_DBIDX]);
				Msg.m_sIteminfo.Inventory[ItemPos].wIconIdx = atoi((char*)pData[i].Data[eCI_IDX]);
				Msg.m_sIteminfo.Inventory[ItemPos].Position = atoi((char*)pData[i].Data[eCI_Position]);
				Msg.m_sIteminfo.Inventory[ItemPos].Durability = atoi((char*)pData[i].Data[eCI_Durability]);
				Msg.m_sIteminfo.Inventory[ItemPos].QuickPosition = atoi((char*)pData[i].Data[eCI_QPosition]);
			}
			else
			{
				ITEMBASE Item;
				Item.dwDBIdx = atoi((char*)pData[i].Data[eCI_DBIDX]);
				Item.wIconIdx = atoi((char*)pData[i].Data[eCI_IDX]);
				Item.Position = atoi((char*)pData[i].Data[eCI_Position]);
				Item.Durability = atoi((char*)pData[i].Data[eCI_Durability]);
				Item.QuickPosition = atoi((char*)pData[i].Data[eCI_QPosition]);

				Msg2.Add( &Item );
			}
		}
		else if(ItemPos >= TP_WEAR_START && ItemPos < TP_WEAR_END)
		{				
			POSTYPE ItemGrid = ItemPos - TP_WEAR_START;
			if( Msg.m_sIteminfo.WearedItem[ItemGrid].dwDBIdx == 0 )
			{				
				Msg.m_sIteminfo.WearedItem[ItemGrid].dwDBIdx = atoi((char*)pData[i].Data[eCI_DBIDX]);
				Msg.m_sIteminfo.WearedItem[ItemGrid].wIconIdx = atoi((char*)pData[i].Data[eCI_IDX]);
				Msg.m_sIteminfo.WearedItem[ItemGrid].Position = atoi((char*)pData[i].Data[eCI_Position]);
				Msg.m_sIteminfo.WearedItem[ItemGrid].Durability = atoi((char*)pData[i].Data[eCI_Durability]);
				Msg.m_sIteminfo.WearedItem[ItemGrid].QuickPosition = atoi((char*)pData[i].Data[eCI_QPosition]);
			}
			else
			{
				ITEMBASE Item;
				Item.dwDBIdx = atoi((char*)pData[i].Data[eCI_DBIDX]);
				Item.wIconIdx = atoi((char*)pData[i].Data[eCI_IDX]);
				Item.Position = atoi((char*)pData[i].Data[eCI_Position]);
				Item.Durability = atoi((char*)pData[i].Data[eCI_Durability]);
				Item.QuickPosition = atoi((char*)pData[i].Data[eCI_QPosition]);

				Msg2.Add( &Item );
			}
		}
		else
		{
			ASSERT(0);
			continue;
		}
	}

	// Á¤º¸ º¸³»±â
	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_INVENINFO;
	Msg.dwTemplateIdx = tempIdx;
	MNETWORK->SendToMC(connectIdx, (char*)&Msg, sizeof(Msg));

	if( Msg2.m_dwCount )
	{
		Msg2.Category = MP_RMTOOL_CHARACTER;
		Msg2.Protocol = MP_RMTOOL_CHARACTER_INVENINFO2;
		Msg2.dwTemplateIdx = tempIdx;
		MNETWORK->SendToMC(connectIdx, (char*)&Msg2, Msg2.GetSize());
	}
}

void ChangeItemPosition( WORD wConnectIdx, WORD wTempIdx, DWORD dwChrIdx, DWORD dwDBIdx, DWORD dwPos )
{
	sprintf(txt, "EXEC TP_ChangeItemPosition %d, %d, %d", dwChrIdx, dwDBIdx, dwPos );	
	if(g_DB.Query(eQueryType_FreeQuery, eChangeItemPosition, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void ChangeWareItemPosition( WORD wConnectIdx, WORD wTempIdx, DWORD dwUserIdx, DWORD dwDBIdx, DWORD dwPos )
{
	sprintf(txt, "EXEC TP_ChangeWareItemPosition %d, %d, %d", dwUserIdx, dwDBIdx, dwPos );	
	if(g_DB.Query(eQueryType_FreeQuery, eChangeItemPosition, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void ChangeMugongPosition( WORD wConnectIdx, WORD wTempIdx, DWORD dwChrIdx, DWORD dwDBIdx, DWORD dwPos )
{
	sprintf(txt, "EXEC TP_ChangeMugongPosition %d, %d, %d", dwChrIdx, dwDBIdx, dwPos );	
	if(g_DB.Query(eQueryType_FreeQuery, eChangeMugongPosition, MAKEDWORD(wConnectIdx, wTempIdx), txt) == FALSE)
	{
	}
}

void InsertLogTool( DWORD dwLogType, DWORD dwLogKind, DWORD dwOperIdx, char* sOperName, DWORD dwTargetIdx, char* sTargetName,
				    DWORD dwParam1, DWORD dwParam2, DWORD dwParam3, DWORD dwParam4 )
{
	sprintf(txt, "EXEC Up_ToolLog %d, %d, %d, \'%s\', %d, \'%s\', %d, %d, %d, %d", dwLogType, dwLogKind, dwOperIdx, sOperName,
		dwTargetIdx, sTargetName, dwParam1, dwParam2, dwParam3, dwParam4 );	
	if(g_DB.LogQuery(eQueryType_FreeQuery, eInsertLogTool, 0, txt) == FALSE)
	{
	}
}