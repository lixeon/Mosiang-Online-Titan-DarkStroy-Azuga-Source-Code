#include "stdafx.h"
#include "CommonDBMsgParser.h"
#include "MapDBMsgParser.h"
#include "UserTable.h"
#include "Network.h"
#include "ItemManager.h"
#include "MugongManager.h"
#include "Economy.h"
#include "PartyManager.h"
#include "Party.h"
#include "Pet.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "CharMove.h"
#include "Guild.h"
#include "GuildManager.h"
#include "StreetStallManager.h"
#include "FameManager.h"
#include "CQuestBase.h"
#include "TileManager.h"
#include "AbilityManager.h"
#include "PyoGukManager.h"
#include "WantedManager.h"
#include "GuildFieldWarMgr.h"

#include "QuestManager.h"
#include "ShopItemManager.h"
#include "GuildTournamentMgr.h"
#include "GuildUnionManager.h"
#include "SiegeWarMgr.h"
#include "SiegeWarProfitMgr.h"
#include "cJackpotManager.h"
#include "QuestManager.h"
#include "GuildManager.h"	// magi82 - ¹®ÇÏ»ý°ü·Ã(070123)
#include "PackedData.h"

#include "ItemLimitManager.h"
#include "FortWarManager.h"

#include "MapItemDrop.h"	// ¸Ê ¾ÆÀÌÅÛ µå¶ø Ãß°¡ by Stiner(2008/05/29)-MapDropItem

//taiyo 
//sprintf %d¢®¢´I AeAI¡ÍiC¡§ui AO¡Ë¡ÍA ¢®¨¡A ¡§uod!!!! ¨Ïoo¢®¨ú¢®¢¯ /¨Ïo¨Ï¡þ!!
//¡Íi¢®IAIAI A¡Ë¡þAO¡§¡þ¢®¨¡¢®¢´I ¡§uod!

extern BOOL g_bPlusTime;


//////////////////////////////////////////////////////////////////
// DB¢®¢´I¡§¡þIAI ¡Ë¡þ¡§¡©¡§oAAo¡Ë¡þ| ¨Ïi©ö¡Ëi¨ö¡§u¡§¢®¡§u¢®¨Ï ¢®¨ú¡Ë¡þ¡§¡þ¡§¡ËCI¡Ë¡ÍA CO¡§uo¡Íie.
DBMsgFunc g_DBMsgFunc[MaxQuery] =
{
	NULL,
	RCharacterNumSendAndCharacterInfo,		/// CI¨Ï©ª¡§¡ÌAC A¨Ï©ª¡Ë¡þ?d¡§¡þ¡Ë¡þ¡Ë¡þ¨Ï¡Ì¡ÍiI¡Ë¡þ| ¡§uo¡§ui¡Ë?A¡Ë¡ÍU.
	RCharacterMugongInfo,		/// ¨Ïo¢®i¢®¨¡¨Ï¨£d¡§¡þ¡Ë¡þ¡Ë¡þ| ¡§uo¡§ui¡Ë?A¡Ë¡ÍU
	RCharacterItemSlotInfo,					// eCharacterItemSlotQuery
	RCharacterItemInfo,		/// ¡§u¡§¢®AIAU d¡§¡þ¡Ë¡þ¡Ë¡þ| ¡§uo¡§ui¡Ë?A¡Ë¡ÍU
	RCharacterItemOptionInfo,
	NULL,						// eCharacterItemOptionDeleteQuery
	RCharacterItemRareOptionInfo,	// eCharacterItemRareOptionQuery
	NULL,						// eCharacterItemRareOptionDeleteQuery
	RPyogukItemOptionInfo,
	RPyogukItemRareOptionInfo,
	RCharacterShopItemInfo,		// eShopItemInfoQuery
//	RCharacterPetInfo,

	RSSItemInsert,
	RItemOptionInsert,
	RItemRareInsert,		
	/* New Item Work */
	NULL,						// ItemUpdateToDB
	NULL,						// ItemCombineUpdateToDB
	NULL,						// ItemMoveUpdateToDB
	NULL,						// ItemMovePyogukUpdate
	NULL,						// ItemMoveMunpaUpdate
	NULL,						// ItemMovePetInvenUpdateToDB
	RItemInsert,				// ItemInsertToDB
	NULL,						// ItemDeleteToDB
	RShopItemInvenInfo,			// eItemShopInvenInfo,
	RUsingShopItemInfo,			// eItemShopItemUseInfo
	NULL,						// eItemShopItemUsing    RaMa - 04.11.05
	NULL,						// eItemShopItemDelete
	NULL,						// eItemShopItemUpdatetime
	NULL,						// eItemShopItemUpdateParam
	NULL,						// eItemShopItemUpdateUseParam
	NULL,						// eItemShopItemUpdateUseInfo
	NULL,						// eItemShopItemMPInfo
	NULL,						// eItemShopMPUpdate
	NULL,						// eItemShopMPDel
	NULL,						// eItemShopGetItem
	NULL,						// eItemMoveShopUpdate
	RCharacterChangeName,		// eItemShopChangeName
	NULL,						// eItemCharChange
	RShopItemRareInsertToDB,	// eItemShopRareCreate
	RIsUseAbleShopAvatarItem,	// eItemShopIsUseableAvatar
	RIsAbleShopEquipItem,		// eItemShopIsUseableEquip
	NULL,						// eItemShopUpdatCharacterIdx
	NULL,						// eItemShopUpdateAllInfo

	NULL,						// RegistLoginMapInfo
	NULL,						// UnRegistLoginMapInfo
	
	/* New Mugong Work */
	NULL,						// MugongUpdateToDB
	NULL,						// MugongMoveUpdateToDB
	RMugongInsert,				// MugongInsertToDB
	NULL,						// MugongDeleteToDB

//	NULL,				/// ¨Ïo¢®i¢®¨¡¨Ï¨£¢®ief
	RMapBaseEconomy,				/// ¢®¨úaA¡§¨£¡§oA¡§u¡§uC¢®IAo¡Ë¡þ¡Ëc
	NULL,				/// ¢®¨¡¨Ï¢®¡Ë¡þA¡Íii¢®¢´ICwI¡§oAn
	NULL,			/// A¨Ï©ª¡Ë¡þ?d¡§¡þ¡Ë¡þ ¡§u¢®%i¢®IAI¡§¢®¡Ëc
	NULL,			/// HeroInfo ¡§u¢®%i¢®IAI¡§¢®¡Ëc
	NULL,			/// TotalInfo ¡§u¢®%i¢®IAI¡§¢®¡Ëc
	NULL,			// eBadFameUpdate,
	
	RAuctionCheck,
	RAuctionPageList,
	RAuctionConfirm,
	RPartyLoad,	//¡§¢®A¡§¢®¡§u		

	///// 2008. 6. CBH - ÆÄÆ¼¸ÅÄª Ãß°¡µÈ ¿É¼Ç °ü·Ã ¼öÁ¤ ///////////////////
	//RPartyInfoByUserLogin,
	//RPartyCreate,
	NULL,
	NULL,
	///////////////////////////////////////////////////////////////////////////
	NULL,
	NULL,
	NULL,
	RPartyDelMember,	// PartyDelMember
	RPartyChangeMaster, //PartyChangeMaster
	
	NULL,
	RSaveMapChangePointReturn,
	
	/*
	RMunpaLoad, //¨Ïo¡Ëc¡§¢®A 1
	RMunpaItemLoad,
	RMunpaItemOption,
	RMunpaLoadSyn,
	RMunpaLoadSyn,
	RMunpaBreakUp,
	RMunpaBreakUpMembers,
	NULL,
	NULL,
	RMunpaChangeMemberRank, // eMunpaChangeMemberRank
	RMunpaBanMember,
	RMunpaSecede,
	RMunpaAcceptMember, 
	RMunpaDenyMember,
	RMunpaJoinMember,
	RMunpaJoinMemberCancel,
	RMunpaRegularMembersInfo,
	RMunpaJoinMembersInfo,
	RMunpaBoardInfo,
	RMunpaCreateBoard,
	NULL, 
	RMunpaBoardRankInfo, 
	RMunpaBoardRankInfoUpdate,
	NULL,
	RMunpaBoardDeleteContents,
	RMunpaMoneyPutInOut,
	*/
	

	NULL,
	RCharacterPyogukInfo,			// eCharacterPyogukInfo
	RCharacterPyogukItemInfo,		// eCharacterPyogukItemInfo	
	NULL,					// PyogukMoneyUpdateToDB

	RFriendNotifyLogouttoClient,	
//	RFameMunpaUpdate,
	NULL,
	NULL,					//eBadFameCharacterUpdate
	NULL,					//ePKCharacterUpdate

	RWantedLoad,
	RWantedInfoByUserLogIn,
	NULL,
	RWantedRegist,
	NULL,
	RWantedComplete,
	RWantedDestroyed,
	RWantedOrderedList,
	RWantedMyList,
	RWantedSearch,

	RQuestTotalInfo,		// eQuestTotalInfo                         
	NULL,					// eQuestUpdate
	RQuestMainQuestLoad,	// eQuestMainQuestLoad
	RQuestSubQuestLoad,		// eQuestSubQuestLoad
	RQuestItemLoad,			// eQuestItemLoad
	NULL,					// eQuestMainQuestInsert
	NULL,					// eQuestSubQuestInsert
	NULL,					// eQuestItemInsert
	NULL,					// eQuestMainQuestUpdate
	NULL,					// eQuestSubQuestUpdate
	NULL,					// eQuestItemUpdate
	NULL,					// eQuestMainQuestDelete
	NULL,					// eQuestSubQuestDelete
	NULL,					// eQuestSubQuestDeleteAll	
	NULL,					// eQuestItemDelete
	NULL,					// eQuestItemDeleteAll
	NULL,					// eQuestMainQuestUpdateCheckTime

	RJournalGetList,		// eJournalGetList,
	NULL,					// eJournalInsertQuest,
	NULL,					// eJournalInsertWanted,
	NULL,					// eJournalInsertLevel,
	NULL,					// eJournalSaveUpdate,
	NULL,					// eJournalDelete,
	/////////////////////////////////////////////
	// Log ¢®¨¡u¢®¢´A 
	NULL,					// eLogCharacter,
	NULL,					// eLogExp,
	NULL,					// eLogMoney,
	NULL,					// eLogMoneyWrong,
	NULL,					// eLogMugong,
	NULL,					// eLogMugongExp,
	NULL,					// eLogItemMoney,
	NULL,					// eLogItemReinforce,
	NULL,					// eLogItemRare,
	NULL,					// eLogCheat,
	NULL,					// eInsertLogTool	
	NULL,					// eLogGuild
	NULL,					// eLogPet
	NULL,					// eLogGuildPoint,
	NULL,					// eLogGMToolUse
	/////////////////////////////////////////////

	NULL,					// eCharacterInfoByTime,
	NULL,					// eMugongExpByTime,
	NULL,					// eSpeedHackCheck,

	NULL,					// eGameLogItem,

	RGuildLoadGuild,		// eGuildLoadGuild, 
	RGuildLoadMember,		// eGuildLoadMember,
	RGuildCreate,			// eGuildCreate,
	RGuildBreakUp,			// eGuildBreakUp,
	RGuildDeleteMember,		// eGuildDeleteMember,
	NULL,					// eGuildAddMember,
	RGuildSecedeMember,		// eGuildSecedeMember
	NULL,					// eGuildLevelUp
	NULL,					// eGuildChangeMemberRank
	RGuildItemLoad,			// eGuildItemLoad
	RGuildItemOption,		// eGuildItemOption
	RGuildItemRareOption,	// eGuildItemRareOption
	NULL,					// eGuildMoneyPutInOut
	NULL,					// eGuildGiveNickName
	
	RConfirmUserOut,		// eConfirmUserOut
	RSaveCharInfoBeforeLogout,

	RCharacterInfoQueryToEventMap, //eCharacterInfoQueryToEventMap
	
	RLoadGuildWarRecord,	// eLoadGuildWarRecord	
	NULL,					// eGuildWarRecordDelete
	RLoadGuildFieldWar,		// eLoadGuildFieldWar
	NULL,					// eInsertGuildFieldWar
	NULL,					// eDeleteGuildFieldWar
	NULL,					// eUpdateGuildFieldWarRecord
	
	RGuildTournamentInfoLoad,			// eGuildTournament_LoadAll,
	RGuildTournamentCancelIdxLoad,		// eGuildTournament_CancelLoad,
	NULL,								// eGuildTournament_CancelInsert
	NULL,								// eGuildTournament_Insert,
	NULL,								// eGuildTournament_Delete,
	NULL,								// eGuildTournament_UpdateGuildInfo,
	NULL,								// eGuildTournament_UpdateTournamentInfo,
	
//	NULL,	//	RJackpotLoadTotalMoney,				// eJackpot_TotalMoney_Load
	NULL,	//	RJackpotAddTotalMoney,				//	eJackpot_MapMoney_Update,
	RJackpotPrizeInfo,					//	eJackpot_PrizeInfo_Send,

	RGuildUnionLoad,					// eGuildUnionLoad
	RGuildUnionCreate,					// eGuildUnionCreate
	NULL,								// eGuildUnionDestroy	
	NULL,								// eGuildUnionAddGuild
	NULL,								// eGuildUnionRemoveGuild
	NULL,								// eGuildUnionSecedeGuild

	RSiegeWarInfoLoad,					// eSiegeWarInfoLoad
	NULL,								// eSiegeWarInfoInsert
	NULL,								// eSiegeWarInfoUpdate,
	RSiegeWarGuildInfoLoad,				// eSiegeWarGuildInfoLoad
	NULL,								// eSiegeWarGuildInsert,
	NULL,								// eSiegeWarGuildUpdate,
	NULL,								// eSiegeWarGuildDelete,

	NULL,								// eChangeCharacterAttr

	RSiegeWarProfitInfoLoad,			// eSiegeWarProfitInfoLoad
	NULL,								// eSiegeWarProfitUpdateGuild
	NULL,								// eSiegeWarProfitUpdateTaxRate
	RSiegeWarProfitUpdateProfitMoney,	// eSiegeWarProfitUpdateProfitMoney

	NULL,								// eChangeGuildLocation
	NULL,								// eChangeCharacterStageAbility

	RCharacterPetInfo,					// eCharacterPetInfoQuery
	RPetInsert,							// ePetInsert
	NULL,								// ePetUpdate
	NULL,								// ePetDelete
	
	RPetInvenItemOptionInfo,			// ePetInvenItemOptionInfoQuery
	RPetInvenItemRareOptionInfo,		// ePetInvenItemRareOptionInfoQuery
	RPetInvenItemInfo,					// ePetInvenItemInfoQuery

	NULL,								// eGuildLoadNotice,
	NULL,								// eGuildUpdateNotice,

	RQuestEventCheck,					// eQuestEventCheck
	NULL,								// eQuestEventEnd

	RPetWearItemInfo,					// ePetWearItemInfoQuery

	NULL,								//eGMEvent01

	RGuildLoadGuildPointInfo,			//eGuildPointInfoLoad
	RGuildLoadGuildPlustimeInfo,		//eGuildPlustimeInfoLoad
	RGuildAddHuntedMonsterCount,		//eGuildAddHuntedMonsterCount
	RGuildGetHuntedMonsterTotalCountWithInit,	//eGuildConvertHuntedMonsterCount
	RGuildAddGuildPoint,				//eGuildAddGuildPoint
	RGuildUseGuildPoint,				//eGuildUseGuildPoint
	RGuildPlustimeEnd,					//eGuildPlustimeEnd
	NULL,								// eGuildTournamentAddLog
	NULL,								// eSiegeWarAddLog

	RGuildItemLoadInNeed,				//eGuildItemLoadInNeed
	RGuildItemOptionInNeed,				//eGuildItemOptionInNeed
	RGuildItemRareOptionInNeed,			//eGuildItemRareOptionInNeed

	// magi82 //////////////////////////////////////////////////////////////////////////
	NULL,								//eCharacterExpFlag
	////////////////////////////////////////////////////////////////////////////////////

	//magi82 - ¹®ÇÏ»ý °¡ÀÔÆí¸®½Ã½ºÅÛ /////////////////////////////////////////
    RGuildTraineeInfo,					//eGuildTraineeInfo
	NULL,								//eGuildTraineeInsert
	NULL,								//eGuildTraineeDelete

	RGuildTraineeGuildInfo,				//eGuildTraineeGuildInfo
	NULL,								//eGuildTraineeGuildDelete
	
	NULL,								// eGuildTraineeDeletebyGuild
	RGuildAddStudent,					// eGuildAddStudent
	//////////////////////////////////////////////////////////////////////////

	RGuildStudentLvUpCtUpdate,			//eGuildStudentLvUpCount,
	RGuildStudentLvUpCtInit,			//eGuildStudentLvUpInfoInit,

	// magi82 - Titan(070209)
	RTitanWearItemInfo,					// eTitanWearItemInfo,

	//SW070127 Å¸ÀÌÅº
	RCharacterTitanInfo,				//eCharacterTitanInfoQuery,
	RCharacterTitanEquipItemEnduranceInfo,	//eCharacterTitanEquipItemEnduranceInfoQuery
	RTitanInsertToDB,					//eTitanInsert,
	NULL,								//eTitanUpdate,
	NULL,								//eTitanDelete,

	RTitanEquipItemInfoInsertToDB,		//eTitanEquipInfoInsert,
	NULL,								//eTitanEquipInfoUpdate,
	NULL,								//eTitanEquipInfoDelete,

	RTestGameQuery,							// eTestGameQuery
	RTestLogQuery,							// eTestLogQuery

	// magi82 - Titan(071015) Ã¢°í¿¡ Å¸ÀÌÅº Àåºñ °ü·Ã
	RPyogukTitanEnduranceInfo,			// ePyogukTitanEnduranceOptionQuery

	NULL,								// eQuest_EndQuest_New
	NULL,								// eQuest_DeleteQuest_New
	NULL,								// eQuest_EndSubQuest_New
	// magi82(33)
	RTitanEquipInsertExceptionToDB,		//eTitanEquipInfoInsertException
	RTitanEquipInsertExceptionPyogukToDB,		//eTitanEquipInfoInsertExceptionPyoguk
	NULL,		//eCharacterSkinInfoUpdate

	// magi82(41) - ¼¥¾ÆÀÌÅÛ Ãß°¡(½ºÅÝ ÃÊ±âÈ­ ÁÖ¹®¼­)
	NULL,		//eCharacterUpdateResetStatusPoint

	RCharacterSkinInfo,	//eCharacterSkinInfo

	RItemLimitInfoLoadAll,	// eItemLimitInfo_LoadAll,	// tamoo
	RItemLimitInfoUpdate,	// eItemLimitInfo_Update,	// tamoo

	RAutoNoteListLoad,		// eAutoNoteListLoad
	RAutoNoteListAdd,		// eAutoNoteListAdd

	// ¸Ê ¾ÆÀÌÅÛ µå¶ø Ãß°¡ by Stiner(2008/05/28)-MapItemDrop
	RMapItemDropListSelect,		// eMapItemDropListSelect
	RMapItemDropListUpdate,		// eMapItemDropListUpdate
	NULL,						// eMapItemDropListInit

	RFortWarInfoLoad,		// eFortWarInfoLoad
	NULL,					// eFortWarInfoUpdate
	RFortWarProfitMoneyUpdate,	// eFortWarProfitMoneyUpdate
	RFortWarItemLoad,		// eFortWarItemLoad
	RFortWarItemInsertToDB,	// eFortWarItemInsertToDB
	NULL,	
};

	

//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
// DB¢®¢´I Query¨Ïo¡Ëc; ¨Ï©ª?¡Ë¡þ¢®¨ú¡ËO¢®¡¿ ¡§u¨Ï¡À¡Ë¡ÍA CO¡§uo¡Íie.
//pjs
//¢®¨¡¨Ï¢®¡Ë¡þA ¢®¨¡a¢®¨¡u¢®¨¡¡Ë¢ç AO¡Ë¡ÍAAo ¢®¨¡E¢®ic 
char txt[512];	//¢®¨¡¨Ï¨£Ae ¢®ic¡Ë?e

void CheckAuction(MSGBASE* msg)
{
//	char txt[128];
	int CharacterIDX = msg->dwObjectID;
	sprintf(txt, "EXEC %s %d", STORED_AUCTION_CHECK, CharacterIDX);	
	g_DB.Query(eQueryType_FreeQuery,eAuctionCheck,CharacterIDX,txt);

}
//¡Ë¡þ¡Ëc¡§i©ö¡Ëi¨ù¡§¢®¡Ëc ¢®¨¡E¢®io ¨Ïi©ö|i¡¾¨Ïoy; CASE¨Ïo¡Ëc8¢®¢´I..
void AuctionSearch(SEARCHLIST* msg)
{
//	char txt[128];
	int searchtype;
	char name[15];

	strcpy(name,((char*)msg->name));
	searchtype = msg->search_type;
	int CharacterIDX = msg->dwObjectID;
	sprintf(txt, "EXEC %s %d \'%s\'", STORED_AUCTION_SEARCH, searchtype, name );
	g_DB.Query(eQueryType_FreeQuery,eAuctionPageList,CharacterIDX,txt);
}
//¡Ë¡þ¡Ëc¡§i©ö¡Ëi¨ù¡§¢®¡Ëc d¢®¢´A; CASE¨Ïo¡Ëc8¢®¢´I..
void AuctionSort(SORTLIST* msg)
{
//	char txt[128];
	int type;
	int page;
	type = msg->sort_type;
	page = msg->pagenum;
	int CharacterIDX = msg->dwObjectID;
	sprintf(txt, "EXEC %s %d \'%s\'", STORED_AUCTION_SEARCH, type, page );
	g_DB.Query(eQueryType_FreeQuery,eAuctionPageList,CharacterIDX,txt);
}
//¢®¨¡¨Ï¢®¡Ë¡þA ¡Íii¢®¢´I 
void AuctionRegister(REGISTERAUCTION* msg)
{
//	char txt[128];
	int  index = msg->PRO_Index;
	int  amount = msg->PRO_Amount;
	char duedate[10];
	strcpy(duedate,msg->BID_DUEDATE);
	int  price = msg->BID_HighPrice;
	int  immediate = msg->BID_ImmediatePrice;
	char name[15];
	strcpy(name,msg->Auctioneer);
	
	DWORD characterIDX = msg->dwObjectID;
	int CharacterIDX = msg->dwObjectID;
	sprintf(txt, "EXEC %s %d %d %s %d %d %s", STORED_AUCTION_REGISTER,index,amount,duedate,price,immediate,name);
	g_DB.Query(eQueryType_FreeQuery,eAuctionConfirm,CharacterIDX,txt);
}
//¢®¨¡¨Ï¢®¡Ë¡þA Au¡Ë?¡§I
void AuctionJoin(JOINAUCTION* msg)
{
//	char txt[128];
	int index = msg->REG_Index;
	int price = msg->BID_Price;
	char name[15];
	strcpy(name,msg->JOIN_Name);
	int CharacterIDX = msg->dwObjectID;
	sprintf(txt, "EXEC %s %d %d %s", STORED_AUCTION_JOIN, index, price, name);
	g_DB.Query(eQueryType_FreeQuery,eAuctionConfirm,CharacterIDX,txt);
}
//¢®¨¡¨Ï¢®¡Ë¡þA ¡Íii¢®¢´I, Au¡Ë?¡§I¡Ë¡þ| CASE¢®¢´I 
void AuctionCancel(AUCTIONCANCEL *msg)
{
//	char txt[128];
	int type = 0 ;
	int regindex = 0;
	char name[15];

	int CharacterIDX = msg->dwObjectID;
	sprintf(txt, "EXEC %s %d %d %s", STORED_AUCTION_CANCEL, type, regindex, name);
	g_DB.Query(eQueryType_FreeQuery,eAuctionConfirm,CharacterIDX,txt);
}


void CharacterNumSendAndCharacterInfo(DWORD CharacterIDX, DWORD Protocol)
{
#ifdef _JAPAN_LOCAL_
	sprintf(txt, "EXEC dbo.MP_CHARACTER_SelectByCharacterIDX_JP %d", CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eCharacterInfoQuery, Protocol, txt);
#elif defined _HK_LOCAL_
	sprintf(txt, "EXEC dbo.MP_CHARACTER_SelectByCharacterIDX_JP %d", CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eCharacterInfoQuery, Protocol, txt);
#elif defined _TL_LOCAL_
	sprintf(txt, "EXEC dbo.MP_CHARACTER_SelectByCharacterIDX_JP %d", CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eCharacterInfoQuery, Protocol, txt);
#else
	sprintf(txt, "EXEC %s %d", STORED_CHARACTER_SELECT, CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eCharacterInfoQuery, Protocol, txt);
#endif
}

void CharacterNumSendAndCharacterInfo2(DWORD CharacterIDX, DWORD Protocol)
{
#ifdef _JAPAN_LOCAL_
	sprintf(txt, "EXEC dbo.MP_CHARACTER_SelectByCharacterIDX_JP %d", CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eCharacterInfoQueryToEventMap, Protocol, txt);
#elif defined _HK_LOCAL_
	sprintf(txt, "EXEC dbo.MP_CHARACTER_SelectByCharacterIDX_JP %d", CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eCharacterInfoQueryToEventMap, Protocol, txt);
#elif defined _TL_LOCAL_
	sprintf(txt, "EXEC dbo.MP_CHARACTER_SelectByCharacterIDX_JP %d", CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eCharacterInfoQueryToEventMap, Protocol, txt);
#else
	sprintf(txt, "EXEC %s %d", STORED_CHARACTER_SELECT, CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eCharacterInfoQueryToEventMap, Protocol, txt);
#endif
}

void CharacterMugongInfo(DWORD CharacterIDX, DWORD Protocol)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_MAPCHANGE_MUGONGINFO, CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eCharacterMugongQuery, Protocol, txt);
}

void CharacterItemSlotInfo(DWORD CharacterIDX)
{
	sprintf(txt, "EXEC %s %d", STORED_CHARACTER_ITEMSLOTINFO, CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eCharacterItemSlotQuery, CharacterIDX, txt);
}

void CharacterItemInfo(DWORD CharacterIDX, DWORD Protocol)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_CHARACTER_ITEMINFO, CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eCharacterItemQuery, Protocol, txt);
}

void CharacterItemRareOptionInfo(DWORD CharacterIDX, DWORD Protocol)
{
	sprintf(txt, "EXEC %s %d",
		"dbo.MP_CHARACTER_ItemRareOptionInfo", //STORED_CHARACTER_ITEMRAREOPTIONINFO,
		CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eCharacterItemRareOptionQuery, Protocol, txt);
}

void CharacterItemOptionInfo(DWORD CharacterIDX, DWORD Protocol)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_CHARACTER_ITEMOPTIONINFO, CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eCharacterItemOptionQuery, Protocol, txt);
}

void PyogukItemRareOptionInfo(DWORD CharacterIDX, DWORD UserID, DWORD StartDBIdx)
{
	sprintf(txt, "EXEC dbo.MP_PYOGUK_ITEMRAREOPTION_INFO %d, %d", UserID, StartDBIdx);
	g_DB.Query(eQueryType_FreeQuery, ePyogukItemRareOptionQuery, CharacterIDX, txt);
}

void PyogukItemOptionInfo(DWORD CharacterIDX, DWORD UserID, DWORD StartDBIdx)
{
//	char txt[128];
	sprintf(txt, "EXEC dbo.MP_PYOGUK_ITEMOPTION_INFO %d, %d", UserID, StartDBIdx);
	g_DB.Query(eQueryType_FreeQuery, ePyogukItemOptionQuery, CharacterIDX, txt);
}

void PetInvenItemOptionInfo( DWORD CharacterIDX, DWORD UserID, WORD StartPos, WORD EndPos )
{
	sprintf(txt, "EXEC dbo.MP_PETINVEN_ITEMOPTION_INFO %d, %d, %d", UserID, StartPos, EndPos);
	g_DB.Query(eQueryType_FreeQuery, ePetInvenItemOptionInfoQuery, CharacterIDX, txt);
}

void PetInvenItemRareOptionInfo( DWORD CharacterIDX, DWORD UserID, WORD StartPos, WORD EndPos )
{
	sprintf(txt, "EXEC dbo.MP_PETINVEN_ITEMRAREOPTION_INFO %d, %d, %d", UserID, StartPos, EndPos);
	g_DB.Query(eQueryType_FreeQuery, ePetInvenItemRareOptionInfoQuery, CharacterIDX, txt);
}

void PetInvenItemInfo( DWORD CharacterIDX, WORD StartPos, WORD EndPos )
{
	sprintf(txt, "EXEC dbo.MP_PETINVEN_ITEMINFO %d, %d, %d", CharacterIDX, StartPos, EndPos);
	g_DB.Query(eQueryType_FreeQuery, ePetInvenItemInfoQuery, CharacterIDX, txt);
}

void CharacterItemOptionDelete(DWORD OptionIdx, DWORD dwItemDBIdx)
{
//	char txt[128];
	sprintf(txt, "EXEC dbo.MP_ITEM_OPTION_Delete %d, %d", OptionIdx, dwItemDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eCharacterItemOptionDeleteQuery, 0, txt);
}

void CharacterItemRareOptionDelete(DWORD RareIdx, DWORD dwItemDBIdx)
{
	sprintf(txt, "EXEC dbo.MP_ITEM_RARE_OPTION_Delete %d, %d", RareIdx, dwItemDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eCharacterItemRareOptionDeleteQuery, 0, txt);
}

//
void ShopItemInvenInfo( DWORD CharacterIdx)
{
	sprintf(txt, "EXEC %s %d", STORED_SHOPINVEN_INFO, CharacterIdx);
	g_DB.Query(eQueryType_FreeQuery, eItemShopInvenInfo, CharacterIdx, txt);
}

void PetWearItemInfo( DWORD CharacterIdx, POSTYPE FromPos, POSTYPE ToPos )
{
	sprintf(txt, "EXEC dbo.MP_PET_WEARITEMINFO %d, %d, %d", CharacterIdx, FromPos, ToPos);
	g_DB.Query(eQueryType_FreeQuery, ePetWearItemInfoQuery, CharacterIdx, txt);
}

// RaMa 04.11.04
void CharacterShopItemInfo(DWORD CharacterIDX, DWORD StartDBIdx)
{
	sprintf(txt, "EXEC %s %d, %d", STORED_SHOPITEM_INFO, CharacterIDX, StartDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eShopItemInfoQuery, CharacterIDX, txt);
}

////////////////////////////////////////////////////////////////////////////////
// RaMa 04.11.05
void UsingShopItemInfo( DWORD CharacterIdx )
{
	sprintf(txt, "EXEC %s %d", STORED_SHOPITEM_USEINFO, CharacterIdx);
	g_DB.Query(eQueryType_FreeQuery, eItemShopItemUseInfo, CharacterIdx, txt);
}

void SavedMovePointInfo( DWORD CharacterIdx )
{	
	g_DB.FreeMiddleQuery( RSavedMovePointInfo, CharacterIdx, 
		"EXEC %s %d", STORED_SHOPITEM_MPINFO, CharacterIdx );
}

void SavedMovePointInsert( DWORD CharacterIdx, char* Name, DWORD MapIdx, DWORD Position )
{	
	g_DB.FreeMiddleQuery( RSavedMovePointInsert, CharacterIdx, 
		"EXEC %s %d, \'%s\', %d, %d", STORED_SHOPITEM_MPINSERT, CharacterIdx, Name, MapIdx, Position );
}

void SavedMovePointUpdate( DWORD CharacterIdx, DWORD DBIdx, char* Name )
{
	sprintf(txt, "EXEC %s %d, %d, \'%s\'", STORED_SHOPITEM_MPUPDATE, CharacterIdx, DBIdx, Name);
	g_DB.Query(eQueryType_FreeQuery, eItemShopMPUpdate, 0, txt);
}

void SavedMovePointDelete( DWORD CharacterIdx, DWORD DBIdx )
{
	sprintf(txt, "EXEC %s %d, %d", STORED_SHOPITEM_MPDEL, CharacterIdx, DBIdx);
	g_DB.Query(eQueryType_FreeQuery, eItemShopMPDel, 0, txt);
}

void UsingShopItemUpdateToDB( DWORD UserIdx, DWORD CharacterIdx, DWORD dwItemIdx, DWORD dwDBIdx, DWORD Param, DWORD StartTime, DWORD RemainTime )
{
#ifdef _JAPAN_LOCAL_
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d, %d", STORED_SHOPITEM_USING_JP, UserIdx, CharacterIdx, dwItemIdx, dwDBIdx, Param, StartTime, RemainTime);
#elif defined _HK_LOCAL_
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d, %d", STORED_SHOPITEM_USING_JP, UserIdx, CharacterIdx, dwItemIdx, dwDBIdx, Param, StartTime, RemainTime);
#elif defined _TL_LOCAL_
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d, %d", STORED_SHOPITEM_USING, UserIdx, CharacterIdx, dwItemIdx, dwDBIdx, Param, StartTime, RemainTime);
#else
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d", STORED_SHOPITEM_USING, CharacterIdx, dwItemIdx, dwDBIdx, Param, StartTime, RemainTime);
#endif
	g_DB.Query(eQueryType_FreeQuery, eItemShopItemUsing, 0, txt);
}
void ShopItemDeleteToDB( DWORD CharacterIdx, DWORD dwDBIdx )
{
	sprintf(txt, "EXEC %s %d, %d", STORED_SHOPITEM_DELETE, CharacterIdx, dwDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eItemShopItemDelete, 0, txt);
}

void ShopItemUpdatetimeToDB( DWORD CharacterIdx, DWORD dwItemIdx, DWORD RemainTime )
{
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_SHOPITEM_UPDATETIME, CharacterIdx, dwItemIdx, RemainTime);
	g_DB.Query(eQueryType_FreeQuery, eItemShopItemUpdatetime, 0, txt);
}

void ShopItemParamUpdateToDB( DWORD CharacterIdx, DWORD dwItemDBIdx, DWORD Param )
{
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_SHOPITEM_UPDATEPARAM, CharacterIdx, dwItemDBIdx, Param);
	g_DB.Query(eQueryType_FreeQuery, eItemShopItemUpdateParam, 0, txt);
}

void ShopItemUseParamUpdateToDB( DWORD UserIdx, DWORD CharacterIdx, DWORD dwItemIdx, DWORD Param )
{
#ifdef _JAPAN_LOCAL_
	sprintf(txt, "EXEC %s %d, %d, %d, %d", STORED_SHOPITEM_UPDATEUSEPARAM_JP, UserIdx, CharacterIdx, dwItemIdx, Param);
#elif defined _HK_LOCAL_
	sprintf(txt, "EXEC %s %d, %d, %d, %d", STORED_SHOPITEM_UPDATEUSEPARAM_JP, UserIdx, CharacterIdx, dwItemIdx, Param);
#elif defined _TL_LOCAL_
	sprintf(txt, "EXEC %s %d, %d, %d, %d", STORED_SHOPITEM_UPDATEUSEPARAM, UserIdx, CharacterIdx, dwItemIdx, Param);
#else
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_SHOPITEM_UPDATEUSEPARAM, CharacterIdx, dwItemIdx, Param);
#endif
	g_DB.Query(eQueryType_FreeQuery, eItemShopItemUpdateUseParam, 0, txt);
}

void ShopItemUpdateUseInfoToDB( DWORD CharacterIdx, DWORD dwDBIdx, DWORD Param, DWORD RemainTime )
{
	sprintf(txt, "EXEC %s %d, %d, %d, %d", STORED_SHOPITEM_UPDATEUSEINFO, CharacterIdx, dwDBIdx, Param, RemainTime);
	g_DB.Query(eQueryType_FreeQuery, eItemShopItemUpdateUseInfo, 0, txt);
}

void ShopItemGetItem( DWORD CharacterIdx, WORD wItemIdx, DWORD Dur )
{
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_SHOPITEM_GETITEM, CharacterIdx, wItemIdx, Dur);
	g_DB.Query(eQueryType_FreeQuery, eItemShopGetItem, 0, txt);
}

void ItemMoveShopUpdateToDB(DWORD CharacterIDX, DWORD UserIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos)
{
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d", STORED_ITEM_MOVEUPDATE_SHOP, CharacterIDX, UserIDX, dwfromDBIdx, frompos, dwtoDBIdx, topos);
	g_DB.Query(eQueryType_FreeQuery, eItemMoveShopUpdate, 0, txt);
}

void ShopItemUpdateCharacterIdx( DWORD CharacterIdx, DWORD dwDBIdx )
{
	sprintf(txt, "EXEC %s %d, %d", "MP_SHOPITEM_UpdateCharacterIdx", CharacterIdx, dwDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eItemShopUpdatCharacterIdx, 0, txt);	
}

//

void CharacterAbilityInfo(DWORD CharacterIDX, DWORD Protocol)
{
	g_DB.FreeMiddleQuery(RCharacterAbilityInfo, Protocol, 
		"MP_CHARACTER_SelectAbility %d",CharacterIDX);
}

void CharacterPetInfo( DWORD CharacterIDX, DWORD UserID )
{
	sprintf(txt, "EXEC %s %d", "dbo.MP_CHARACTER_PetInfo", UserID);
	g_DB.Query(eQueryType_FreeQuery, eCharacterPetInfoQuery, CharacterIDX, txt);
}

//SW070127 Å¸ÀÌÅº
void CharacterTitanInfo( DWORD CharacterIDX, DWORD UserID )
{
	sprintf(txt, "EXEC dbo.MP_CHARACTER_TitanInfo %d", UserID);
	g_DB.Query(eQueryType_FreeQuery, eCharacterTitanInfoQuery, CharacterIDX, txt);
}

void RCharacterTitanInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = NULL;
	pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(NULL == pPlayer)	return;

	TITAN_TOTALINFO TitanInfo;

	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		TitanInfo.TitanMasterUserID		= atoi((char*)pData[i].Data[0]);
		TitanInfo.TitanDBIdx			= atoi((char*)pData[i].Data[1]);
		TitanInfo.TitanCallItemDBIdx	= atoi((char*)pData[i].Data[2]);
		TitanInfo.TitanKind				= atoi((char*)pData[i].Data[3]);
		TitanInfo.TitanGrade			= atoi((char*)pData[i].Data[4]);
		TitanInfo.Fuel					= atoi((char*)pData[i].Data[5]);
		TitanInfo.Spell					= atoi((char*)pData[i].Data[6]);
		TitanInfo.RecallTime			= atoi((char*)pData[i].Data[7]);
		TitanInfo.RegisterCharacterIdx	= atoi((char*)pData[i].Data[8]);
		TitanInfo.bRiding				= atoi((char*)pData[i].Data[9]);
		TitanInfo.Scale					= atoi((char*)pData[i].Data[10]);
		TitanInfo.MaintainTime			= atoi((char*)pData[i].Data[11]);	// magi82(23)

		pPlayer->GetTitanManager()->AddTitanTotalInfo(&TitanInfo);
		if( TitanInfo.RecallTime )
			pPlayer->GetTitanManager()->SetRecallCheckTime( TitanInfo.RecallTime );

		if( TitanInfo.RegisterCharacterIdx == pMessage->dwID )
			pPlayer->GetTitanManager()->SetRegistTitanCallItemDBIdx(TitanInfo.TitanCallItemDBIdx);
	}

	CharacterTitanEquipItemEnduranceInfo(pMessage->dwID);

	//pPlayer->SetInitState(PLAYERINITSTATE_TITAN_INFO, MP_USERCONN_GAMEIN_SYN);
}

void CharacterTitanEquipItemEnduranceInfo( DWORD CharacterIDX )
{
	sprintf(txt, "EXEC dbo.MP_CHARACTER_TitanEquipItemEnduranceInfo %d", CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eCharacterTitanEquipItemEnduranceInfoQuery, CharacterIDX, txt);
}

void RCharacterTitanEquipItemEnduranceInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = NULL;
	pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(NULL == pPlayer)	return;

	TITAN_ENDURANCE_ITEMINFO EnduranceInfo;

	EnduranceInfo.CharacterID = pMessage->dwID;

	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		EnduranceInfo.ItemDBIdx = atoi((char*)pData[i].Data[0]);
		EnduranceInfo.ItemIdx	= atoi((char*)pData[i].Data[1]);
		EnduranceInfo.Endurance = atoi((char*)pData[i].Data[2]);
		EnduranceInfo.UserIdx = atoi((char*)pData[i].Data[3]);

		pPlayer->GetTitanManager()->AddTitanEquipItemEnduranceInfo(&EnduranceInfo);
	}

	pPlayer->SetInitState(PLAYERINITSTATE_TITAN_INFO, MP_USERCONN_GAMEIN_SYN);
}
/*
void CharacterGMControlInfo(DWORD CharacterIDX, DWORD Protocol)
{
	g_DB.FreeMiddleQuery(RCharacterGMControlInfo, Protocol, 
		"MP_GM_Control_GetList %d",CharacterIDX);
}

		//TDN
void CharacterGMControlInsert(DWORD GMChrIdx,DWORD CharacterIDX, DWORD UserIdx, int kind, )
{
	g_DB.FreeMiddleQuery(RCharacterGMControlInfo, GMChrIdx, 
		"MP_GM_Control_Insert %d",GMChrIdx,CharacterIDX,UserIdx,kind,);
}
void CharacterGMControlDelete(DWORD GMChrIdx,DWORD ControlIdx)
{
	g_DB.FreeMiddleQuery(NULL, GMChrIdx, 
		"MP_GM_Control_Delete %d",);
}
*/


void AbilityLevelupLog(CPlayer* pPlayer,WORD AbilityIdx,LEVELTYPE AbilityLevel,EXPTYPE FromExp,EXPTYPE ToExp)
{
	sprintf(txt,
"INSERT TB_AbilityLog \
(character_idx,ability_idx,ability_level_to,ability_exp_from,ability_exp_to,logdate) \
values (%d, %d, %d, %u, %u, getdate())", 
pPlayer->GetID(), AbilityIdx, AbilityLevel, FromExp, ToExp);

	g_DB.LogQuery(eQueryType_FreeQuery, 0, 0, txt);
}

void SSItemInsert(DWORD CharacterIdx, WORD wItemIdx, DURTYPE Durability, POSTYPE bPosition, DWORD FromChrIdx, WORD bSeal)
{
	sprintf(txt, "EXEC %s %d, %d, %d, %d", STORED_ITEM_INSERT_INVEN, CharacterIdx, wItemIdx, Durability, bPosition, bSeal);
	g_DB.Query(eQueryType_FreeQuery, eSSItemInsert, FromChrIdx, txt);
}

void ItemUpdateToDB(DWORD CharacterIdx, DWORD dwDBIdx, WORD wItemIdx, DURTYPE Durability, POSTYPE bPosition, WORD qPosition, DWORD RareIdx)
{
	if( RareIdx < 0 )
	{
		char temp[128];
		sprintf(temp, "RareIdxError!(<0) ItemDBIdx : %d, PlayerID : %d", dwDBIdx, CharacterIdx );
		ASSERTMSG(0, temp);

		RareIdx = 0;
	}

	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d, %d", STORED_ITEM_UPDATE, 
					CharacterIdx, dwDBIdx, wItemIdx, Durability, bPosition, qPosition, RareIdx );
	g_DB.Query(eQueryType_FreeQuery, eItemUpdate, 0, txt);
}

/*
void ItemPyogukMunpaUpdateToDB(DWORD UserIDX, DWORD MunpaIDX, DWORD dwDBIdx, WORD wItemIdx, DURTYPE Durability, POSTYPE bPosition, WORD qPosition)
{
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d, %d", STORED_ITEM_PYOGUKMUNPA_UPDATE, 
					UserIDX, MunpaIDX, dwDBIdx, wItemIdx, Durability, bPosition, qPosition );
	g_DB.Query(eQueryType_FreeQuery, eItemPyogukMunpaUpdate, 0, txt);
}
*/
/* bDel:0 fromItem Update, bDel:1 fromItem Delete */
void ItemCombineUpdateToDB(DWORD dwfromDBIdx, DURTYPE fromDur, DWORD dwToDBIdx, DURTYPE toDur)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d, %d", STORED_ITEM_COMBINEUPDATE, dwfromDBIdx, fromDur, dwToDBIdx, toDur);
	g_DB.Query(eQueryType_FreeQuery, eItemCombineUpdate, 0, txt);
}

void ItemMoveUpdateToDB(DWORD CharacterIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d", STORED_ITEM_MOVEUPDATE_INVEN, dwfromDBIdx, frompos, dwtoDBIdx, topos, CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eItemMoveUpdate, 0, txt);
}

void ItemMovePyogukUpdateToDB(DWORD CharacterIDX, DWORD UserIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d", STORED_ITEM_MOVEUPDATE_PYOGUK, CharacterIDX, UserIDX, dwfromDBIdx, frompos, dwtoDBIdx, topos);
	g_DB.Query(eQueryType_FreeQuery, eItemMovePyogukUpdate, 0, txt);
}

void ItemMoveGuildUpdateToDB(DWORD CharacterIDX, DWORD MunpaIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d", STORED_ITEM_MOVEUPDATE_GUILD, CharacterIDX, MunpaIDX, dwfromDBIdx, frompos, dwtoDBIdx, topos);
	g_DB.Query(eQueryType_FreeQuery, eItemMoveGuildUpdate, 0, txt);
}

void ItemMovePetInvenUpdateToDB( DWORD CharacterIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos )
{
	sprintf(txt, "EXEC dbo.MP_ITEM_MoveUpdatePetInven %d, %d, %d, %d, %d", CharacterIDX, dwfromDBIdx, frompos, dwtoDBIdx, topos);
	g_DB.Query(eQueryType_FreeQuery, eItemMovePetInvenUpdate, 0, txt);
}

/* dwKey¢®¨¡¡§¡Ì: DB¡Ë?¡Ë¢ç ¢®io¢®¢´I ¨Ï©ªO; ItemAI ARRAYAC ¨Ï©ª¡Ë¢ç ItemAIAo¡Ë?I ArrayIdx¡Ë¡þ| ¨Ï©ªO¡Ë¡ÍA¡Ë¡ÍU.
   CI¨Ï©ª¡§¡ÌAC ARRAY¢®¨¡¡Ë¢ç DB¡Ë?¡Ë¢ç ¡Ë¡þ¨Ï¡Ì¡ÍiI ¢®¨ú¡Ë¡þ¡§u¡§¡þ ¡ÍiC¡§uu;¡ËO¢®¡¿¡Ë¡þ¡Ë¡þ ¡Ë¡ÍU= AU¡§u¢®A; ¡§uoCaCI¢®¨úa'C¡§¨£¡§u¢®¨Ï CE¡Ë?a
   0AI¡Ë¡þe ~¢®¢´a ¨Ï©ª¡§¡Ì¡Ë¡þOAo¡Ë¡ÍA ¢®¨¡e¡§uO A¨Ï¨£Ca (¢®¨¡¨Ïo¡§uo¡Ë¡þ| C¢®I¡§oACI¢®¨úa¡Íi¡Íi CN¡Ë¡ÍU.)
   LOWORD():EndValue, HIWORD(): ArrayIdx
*/
void ItemInsertToDB(DWORD CharacterIdx, WORD wItemIdx, DURTYPE Durability, POSTYPE bPosition, DWORD dwKey, WORD bSeal)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d ", STORED_ITEM_INSERT_INVEN, CharacterIdx, wItemIdx, Durability, bPosition, bSeal);
	g_DB.Query(eQueryType_FreeQuery, eItemInsert, dwKey, txt);
}


void ItemOptionInsertToDB(DWORD CharacterIdx, WORD wItemIdx, POSTYPE Pos, ITEM_OPTION_INFO * pOptionInfo)
{
//	char txt[256];
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d, %d, %d, %d, %d,  %d, %d, %d, %d, %d,  %d, %d,  %d, %d, %d, %d, %d,  %d", STORED_ITEM_OPTION_INSERT,
		CharacterIdx, 
		wItemIdx,
		pOptionInfo->dwItemDBIdx,
		pOptionInfo->GenGol,					
		pOptionInfo->MinChub,				
		pOptionInfo->CheRyuk,				
		pOptionInfo->SimMek,				
		pOptionInfo->Life,					
		pOptionInfo->NaeRyuk,			
		pOptionInfo->Shield,				
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_FIRE)*100+0.00001),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_WATER)*100+0.00001),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_TREE)*100+0.00001),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_IRON)*100+0.00001),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_EARTH)*100+0.00001),
		pOptionInfo->PhyAttack,
		pOptionInfo->CriticalPercent,
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_FIRE)*100+0.00001),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_WATER)*100+0.00001),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_TREE)*100+0.00001),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_IRON)*100+0.00001),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_EARTH)*100+0.00001),
		pOptionInfo->PhyDefense);
	g_DB.Query(eQueryType_FreeQuery, eItemOptionInsert, Pos, txt);
	
	/*
	g_DB.FreeQuery(RItemOptionInsert,Pos,
		"EXEC %s %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %.2f, %.2f, %.2f, %.2f, %.2f, %d, %d, %.2f, %.2f, %.2f, %.2f, %.2f, %d", STORED_ITEM_OPTION_INSERT,
		CharacterIdx, 
		wItemIdx,
		pOptionInfo->dwItemDBIdx,
		pOptionInfo->GenGol,					
		pOptionInfo->MinChub,				
		pOptionInfo->CheRyuk,				
		pOptionInfo->SimMek,				
		pOptionInfo->Life,					
		pOptionInfo->NaeRyuk,			
		pOptionInfo->Shield,				
		pOptionInfo->AttrRegist.GetElement_Val(ATTR_FIRE),
		pOptionInfo->AttrRegist.GetElement_Val(ATTR_WATER),
		pOptionInfo->AttrRegist.GetElement_Val(ATTR_TREE),
		pOptionInfo->AttrRegist.GetElement_Val(ATTR_IRON),
		pOptionInfo->AttrRegist.GetElement_Val(ATTR_EARTH),
		pOptionInfo->PhyAttack,
		pOptionInfo->CriticalPercent,
		pOptionInfo->AttrAttack.GetElement_Val(ATTR_FIRE),
		pOptionInfo->AttrAttack.GetElement_Val(ATTR_WATER),
		pOptionInfo->AttrAttack.GetElement_Val(ATTR_TREE),
		pOptionInfo->AttrAttack.GetElement_Val(ATTR_IRON),
		pOptionInfo->AttrAttack.GetElement_Val(ATTR_EARTH),
		pOptionInfo->PhyDefense);
		*/
}
void ItemRareInsertToDB(DWORD CharacterIdx, WORD wItemIdx, POSTYPE bPosition, DWORD dwKey, ITEM_RARE_OPTION_INFO* pRareOptionInfo )
{
	char txt[1024];

	sprintf(txt, "EXEC %s %d, %d, %d,  %d, %d, %d, %d, %d, %d, %d,  %d, %d, %d, %d, %d,  %d,  %d, %d, %d, %d, %d,  %d", 
		"dbo.MP_ITEM_RARE_OPTION_Insert",//STORED_ITEM_RARE_OPTION_INSERT,
		CharacterIdx, 
		wItemIdx,
//		Durability,
		bPosition,
//		bSeal,

//		pRareOptionInfo->dwItemDBIdx,
		pRareOptionInfo->GenGol,					
		pRareOptionInfo->MinChub,				
		pRareOptionInfo->CheRyuk,				
		pRareOptionInfo->SimMek,				
		pRareOptionInfo->Life,					
		pRareOptionInfo->NaeRyuk,			
		pRareOptionInfo->Shield,				
		(int)(pRareOptionInfo->AttrRegist.GetElement_Val(ATTR_FIRE)*100+0.00001),
		(int)(pRareOptionInfo->AttrRegist.GetElement_Val(ATTR_WATER)*100+0.00001),
		(int)(pRareOptionInfo->AttrRegist.GetElement_Val(ATTR_TREE)*100+0.00001),
		(int)(pRareOptionInfo->AttrRegist.GetElement_Val(ATTR_IRON)*100+0.00001),
		(int)(pRareOptionInfo->AttrRegist.GetElement_Val(ATTR_EARTH)*100+0.00001),
		pRareOptionInfo->PhyAttack,
//		pRareOptionInfo->CriticalPercent,
		(int)(pRareOptionInfo->AttrAttack.GetElement_Val(ATTR_FIRE)*100+0.00001),
		(int)(pRareOptionInfo->AttrAttack.GetElement_Val(ATTR_WATER)*100+0.00001),
		(int)(pRareOptionInfo->AttrAttack.GetElement_Val(ATTR_TREE)*100+0.00001),
		(int)(pRareOptionInfo->AttrAttack.GetElement_Val(ATTR_IRON)*100+0.00001),
		(int)(pRareOptionInfo->AttrAttack.GetElement_Val(ATTR_EARTH)*100+0.00001),
		pRareOptionInfo->PhyDefense);

	g_DB.Query(eQueryType_FreeQuery, eItemRareInsert, dwKey, txt);
}

void ShopItemRareInsertToDB(DWORD CharacterIdx, DWORD ItemIdx, DWORD ItemPos, DWORD ItemDBIdx, ITEM_RARE_OPTION_INFO* pRareOptionInfo )
{
	char txt[1024];

	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d,  %d, %d, %d, %d, %d,  %d,  %d, %d, %d, %d, %d,  %d", 
		"dbo.MP_SHOPITEM_RARE_OPTION_Insert",//STORED_ITEM_RARE_OPTION_INSERT,
		CharacterIdx, ItemIdx, ItemPos,	ItemDBIdx,
		//		pRareOptionInfo->dwItemDBIdx,
		pRareOptionInfo->GenGol,					
		pRareOptionInfo->MinChub,				
		pRareOptionInfo->CheRyuk,				
		pRareOptionInfo->SimMek,				
		pRareOptionInfo->Life,					
		pRareOptionInfo->NaeRyuk,			
		pRareOptionInfo->Shield,				
		(int)(pRareOptionInfo->AttrRegist.GetElement_Val(ATTR_FIRE)*100+0.00001),
		(int)(pRareOptionInfo->AttrRegist.GetElement_Val(ATTR_WATER)*100+0.00001),
		(int)(pRareOptionInfo->AttrRegist.GetElement_Val(ATTR_TREE)*100+0.00001),
		(int)(pRareOptionInfo->AttrRegist.GetElement_Val(ATTR_IRON)*100+0.00001),
		(int)(pRareOptionInfo->AttrRegist.GetElement_Val(ATTR_EARTH)*100+0.00001),
		pRareOptionInfo->PhyAttack,
		//		pRareOptionInfo->CriticalPercent,
		(int)(pRareOptionInfo->AttrAttack.GetElement_Val(ATTR_FIRE)*100+0.00001),
		(int)(pRareOptionInfo->AttrAttack.GetElement_Val(ATTR_WATER)*100+0.00001),
		(int)(pRareOptionInfo->AttrAttack.GetElement_Val(ATTR_TREE)*100+0.00001),
		(int)(pRareOptionInfo->AttrAttack.GetElement_Val(ATTR_IRON)*100+0.00001),
		(int)(pRareOptionInfo->AttrAttack.GetElement_Val(ATTR_EARTH)*100+0.00001),
		pRareOptionInfo->PhyDefense);

	g_DB.Query(eQueryType_FreeQuery, eItemShopRareCreate, 0, txt);
}


void IsUseAbleShopAvatarItem(DWORD CharacterIdx, DWORD ItemDBIdx, DWORD ItemIdx, DWORD ItemPos )
{
	sprintf(txt, "EXEC %s %d, %d, %d, %d", STORED_SHOPITEM_AVATARABLE, CharacterIdx, ItemDBIdx, ItemIdx, ItemPos );
	g_DB.Query(eQueryType_FreeQuery, eItemShopIsUseableAvatar, CharacterIdx, txt);
}

void IsAbleShopEquipItem( DWORD CharacterIdx, DWORD ItemDBIdx, DWORD FromItemIdx, DWORD FromPos, DWORD ToItemIdx, DWORD ToPos )
{
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d", STORED_SHOPITEM_EQUIPABLE, CharacterIdx, ItemDBIdx, 
		FromItemIdx, FromPos, ToItemIdx, ToPos );
	g_DB.Query(eQueryType_FreeQuery, eItemShopIsUseableEquip, CharacterIdx, txt);
}


void ShopItemAllUseInfoUpdateToDB( DWORD ItemDBIdx, DWORD CharacterIdx, WORD ItemIdx, DWORD Param, DWORD Remaintime )
{
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d", STORED_SHOPITEM_UPDAETALL, ItemDBIdx, CharacterIdx, ItemIdx, 
		Param, Remaintime );
	g_DB.Query(eQueryType_FreeQuery, eItemShopUpdateAllInfo, CharacterIdx, txt);
}


void ItemDeleteToDB(DWORD dwDBIdx)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_ITEM_DELETE, dwDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eItemDelete, 0, txt);
}

void MugongUpdateToDB(MUGONGBASE* msg, char* type)
{
//	char txt[128];
	ASSERT(msg->Position);
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d, %d, %d", STORED_MUGONG_UPDATE, 
		msg->dwDBIdx, msg->wIconIdx, msg->ExpPoint, msg->Sung, msg->Position, msg->QuickPosition, msg->bWear, msg->OptionIndex);
	g_DB.Query(eQueryType_FreeQuery, eMugongUpdate2, 0, txt);
	
	//¾÷µ¥ÀÌÆ® ÇßÀ½
	msg->bWear = FALSE;
}

void MugongMoveUpdateToDB(DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos, char* type)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d, %d", STORED_MUGONG_MOVEUPDATE, dwfromDBIdx, frompos, dwtoDBIdx, topos);
	g_DB.Query(eQueryType_FreeQuery, eMugongMoveUpdate2, 0, txt);
/*
	 frompos == 0 || topos == 0 )
	{
		char temp[256];
		FILE* fp = fopen("MGMoveUpdateToDB.txt","a+");
		sprintf(temp, "%s : [Fdbidx]%d, [Fpos]%d, [Tdbidx]%d, [Tpos]%d\n", type, dwfromDBIdx, frompos, dwtoDBIdx, topos);
		fprintf(fp,temp);
		fclose(fp);
	}
*/
}

void MugongInsertToDB(DWORD CharacterIDX, WORD MugongIDX, POSTYPE MugongPos, BYTE bWeared, BYTE bSung, WORD Option)
{
//	char txt[128];
	ASSERT(MugongPos);
	sprintf(txt, "EXEC %s %u, %d, %d, %d, %d, %d", STORED_MUGONG_INSERT, CharacterIDX, MugongIDX, MugongPos, bWeared, bSung, Option);
	g_DB.Query(eQueryType_FreeQuery, eMugongInsert, CharacterIDX, txt);
}

void MugongDeleteToDB(DWORD dwDBIdx)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_MUGONG_DELETE, dwDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eMugongDelete, 0, txt);
}

void MapBaseEconomy(BYTE MapNum)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_CHARACTER_BASEECONOMY, MapNum);
	g_DB.Query(eQueryType_FreeQuery, eBaseEconomy, 0, txt);
}

void AuctionRegist(BYTE ItemIdx, DWORD SellerID, DWORD EndDate, DWORD EndTime, DWORD StartPrice, DWORD ImmediatePrice, char Memo[256])
{
//	char txt[384];
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d, %s", STORED_CHARACTER_AUCTIONREGIST, 
		ItemIdx, SellerID, EndDate, EndTime, StartPrice, ImmediatePrice, Memo);
	g_DB.Query(eQueryType_FreeQuery, eAuctionRegist, SellerID, txt);
}

void AuctionPageQuery(DWORD CharacterIDX, BYTE PageNum)
{
}
/*
void CharacterItemGetHuntIDX(DWORD CharacterIDX, DWORD OwnerID, WORD ItemIDX, DURTYPE Durability, POSTYPE bPosition, BYTE TableIDX)
{
	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d", STORED_CHARACTER_ITEMHUNT, OwnerID, ItemIDX, bPosition, 0,Durability, TableIDX );
	g_DB.Query(eQueryType_FreeQuery, eItemHunt, CharacterIDX, txt);
	{
	}
}
void CharacterDivideNewItemIDX(DWORD CharacterIDX, DWORD OwnerID, WORD ItemIDX, DURTYPE Durability, POSTYPE bPosition, BYTE TableIDX)
{
	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d", STORED_CHARACTER_ITEMHUNT, OwnerID, ItemIDX, bPosition, 0,Durability, TableIDX );
	g_DB.Query(eQueryType_FreeQuery, eItemDivideNewItem, CharacterIDX, txt);
	{
	}
}
*/


// --------------------------------------------------------------------------------------------------
// taiyo code
// CharacterDupItemInsertAC wKey¡Ë¡ÍA RCharacterDupItemInsert¡Ë?¡Ë¢ç¡§u¢®¨Ï E¢®IAaAU¡Ë¡þ| ¢®¨ú¡Ë¡þ¡§¡þ¡§¡ËCI¢®¨úa 'CN ¢®¨ú¡Ë¡þ¡§¡þ¡§¡ËAUAI¡Ë¡ÍU
// 0¢®¨¡u 0AI¡Ë?UAC ¢®¨¡¡§¡Ì; ¢®ic¡Ë?eCI¡Ë¡ÍA BOOL¢®¨¡¡§¡ÌAC CuAA¡Ë¡þ| AeCN¡Ë¡ÍU
/*
void CharacterDupItemInsert( DWORD dwKey, DWORD CharacterIDX, WORD ItemIDX, DURTYPE Durability, POSTYPE bPosition, BYTE TableIDX )
{
	char txt[128];
	sprintf( txt, "EXEC %s %d, %d, %d, %d, %d, %d", STORED_CHARACTER_ITEMHUNT, CharacterIDX, ItemIDX, bPosition, 0, Durability, TableIDX );
	g_DB.Query(eQueryType_FreeQuery, eItemDupInsert, dwKey, txt);
	{
	}
}
*/

void RegistLoginMapInfo(DWORD CharacterIDX, char* CharacterName, BYTE MapNum, WORD MapPort)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, \'%s\', %d, %d", STORED_CHARACTER_REGISTLOGINMAPINFO, CharacterIDX, CharacterName, MapNum, MapPort);
	g_DB.Query(eQueryType_FreeQuery, eRegistLoginMapInfo, 0, txt);
}

void UnRegistLoginMapInfo(DWORD CharacterIDX)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_CHARACTER_UNREGISTLOGINMAPINFO, CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eUnRegistLoginMapInfo, 0, txt);
}

//////////////////////////////////////////////////////////////////////////
// ¡§¢®A¡§¢®¡§u ¢®¨¡u¢®¢´A DBAo¡Ë¡þ¡Ëc
void PartyLoad(DWORD PartyIDX)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d ", STORED_PARTY_REGIST_MAPSERVER, PartyIDX);
	g_DB.Query(eQueryType_FreeQuery, ePartyRegistMapServer, 0, txt);
}

void PartyBreakup(DWORD PartyIDX) 
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_PARTY_BREAK, PartyIDX);
	g_DB.Query(eQueryType_FreeQuery, ePartyBreakup, PartyIDX, txt);
}

//2008. 5. 21. CBH - ¹æÆÄ »ý¼º ¿É¼Ç Ãß°¡ °ü·Ã ÇÔ¼ö ¼öÁ¤
void PartyInfoByUserLogin(DWORD PartyIDX, DWORD CharacterIDX) 
{
	//	char txt[128];
	sprintf(txt, "%d", PartyIDX);	

	g_DB.FreeMiddleQuery( RPartyInfoByUserLogin, CharacterIDX, 
		"EXEC %s %s", STORED_PARTY_INFO_BY_USERLOGIN, txt );	
}

void PartyCreate(DWORD MasterIDX, PARTY_ADDOPTION* pAddOption) 
{
	LEVELTYPE wMinLevel = pAddOption->wMinLevel;
	LEVELTYPE wMaxLevel = pAddOption->wMaxLevel;
	BYTE bPublic = pAddOption->bPublic;
	BYTE bOption = pAddOption->bOption;
	WORD wLimitCount = pAddOption->wLimitCount;
	char* szTheme = pAddOption->szTheme;

	//	char txt[128];
	sprintf(txt, "%d, %d, %d, %d, %d, %d, '%s'", MasterIDX, bOption, 
		wMinLevel, wMaxLevel, bPublic, wLimitCount, szTheme);

	g_DB.FreeMiddleQuery( RPartyCreate, 0, "EXEC %s %s", STORED_PARTY_CREATE, txt );	
}
/*
void PartyInfoByUserLogin(DWORD PartyIDX, DWORD CharacterIDX) 
{
	//	char txt[128];
	sprintf(txt, "EXEC %s %d ", STORED_PARTY_INFO_BY_USERLOGIN, PartyIDX);
	g_DB.Query(eQueryType_FreeQuery, ePartyInfoByUserLogin, CharacterIDX, txt);
}

void PartyCreate(DWORD MasterIDX, BYTE Option) 
{
	//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d", STORED_PARTY_CREATE, MasterIDX,Option);
	g_DB.Query(eQueryType_FreeQuery, ePartyCreate, MasterIDX, txt);
}
*/

//////////////////////////////////////////////////////////////////////////
//¨Ïo¡Ëc¡§¢®A ¢®¨¡u¢®¢´A DBAo¡Ë¡þ¡Ëc
/*
void MunpaItemOption(DWORD MapNum, DWORD StartDBIdx)
{
//	char txt[128];
	sprintf(txt, "EXEC MP_Munpa_ItemOption_Info %d, %d", MapNum, StartDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eMunpaItemOption, 0, txt);
}

void MunpaLoad(DWORD MapNum, DWORD StartMunpaID) // ¡Ë¡þE¡§u¢®¨Ï¨Ïoo¢®¨¡¡Ë¢ç ANs; ¡ËO¢®¡¿ ¨Ïo¡Ëc¡§¢®A d¡§¡þ¡Ë¡þ ¨Ïi©ö¡Ëi¨ö¡§u¡§¢®¡Ë?E
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d", STORED_MUNPA_LOADMUNPALIST_FIRST, MapNum, StartMunpaID);
	g_DB.Query(eQueryType_FreeQuery, eMunpaLoad, 0, txt);
}

void MunpaItemLoad(DWORD MapNum, DWORD StartItemDBIDx)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d", STORED_MUNPA_LOADITEMLIST_FIRST, MapNum, StartItemDBIDx);
	g_DB.Query(eQueryType_FreeQuery, eMunpaItemLoad, 0, txt);
}



void MunpaLoadSyn(DWORD PlayerID, MAPTYPE MapNum, DWORD GotoPage, DWORD PageUnit, char* OrderType) //A¢®©­¡ËOoAI¡§u¨Ï¡Ì¡§¢®¡Ëc¡Ë?¡Ë¢ç¡§u¢®¨Ï ¡Ë?aA¢®iAI AO; ¡ËO¢®¡¿
{

//	char txt[384];
	sprintf(txt, "EXEC %s %d, %d, %d, \'%s\' ", STORED_MUNPA_LOADMUNPALIST_CLIENT, MapNum, GotoPage, PageUnit, OrderType);
	g_DB.Query(eQueryType_FreeQuery, eMunpaLoadSyn, PlayerID, txt);
}

void MunpaCreate(MUNPA_CREATE_INFO* pmsg, char* MasterName)
{
	g_DB.FreeLargeQuery(RMunpaCreate, pmsg->dwObjectID,	
		"EXEC %s %d, \'%s\', \'%s\', \'%s\', %d, %d", 
		STORED_MUNPA_CREATE, pmsg->dwObjectID, MasterName, pmsg->MunpaName, pmsg->MunpaIntro, GAMERESRCMNGR->GetLoadMapNum(), pmsg->Kind);
}

void MunpaBreakUp(DWORD PlayerID, DWORD MunpaID)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d", STORED_MUNPA_BREAKUP, PlayerID, MunpaID);
	g_DB.Query(eQueryType_FreeQuery, eMunpaBreakUp, MunpaID, txt);
}

void MunpaBreakUpMembers(DWORD StartPlayerID, DWORD MunpaID)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d", STORED_MUNPA_BREAKUPMEMBERS, StartPlayerID, MunpaID);
	g_DB.Query(eQueryType_FreeQuery, eMunpaBreakUpMembers, MunpaID, txt);
}

void MunapBreakupClr(DWORD MunpaID)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_MUNPA_BREAKUPCLEAR, MunpaID);
	g_DB.Query(eQueryType_FreeQuery, eMunpaBreakUpClr, 0, txt);
}

void MunpaChangeMaster(DWORD FromPlayerID, DWORD ToPlayerID)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d", STORED_MUNPA_CHANGEMASTER, FromPlayerID, ToPlayerID);
	g_DB.Query(eQueryType_FreeQuery, eMunpaChangeMaster, 0, txt);
}

void MunpaChangeMemberRank(DWORD MasterID, DWORD PlayerID, int Position, DWORD MunpaID)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d, %d", STORED_MUNPA_CHANGEMEMBER_RANK, MasterID, PlayerID, Position, MunpaID);
	g_DB.Query(eQueryType_FreeQuery, eMunpaChangeMemberRank, MasterID, txt);
}

void MunpaBanMember(DWORD MasterID, DWORD TargetID, DWORD MunpaID, BOOL bLast)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d, %d", STORED_MUNPA_BANMEMBER, MasterID, TargetID, MunpaID, bLast);
	g_DB.Query(eQueryType_FreeQuery, eMunpaBanMember, MasterID, txt);
}

void MunpaSecede(DWORD PlayerID, DWORD MunpaID)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d", STORED_MUNPA_SECEDE, PlayerID, MunpaID);
	g_DB.Query(eQueryType_FreeQuery, eMunpaSecede, PlayerID, txt);
}

void MunpaAcceptMember(DWORD MasterID, DWORD TargetID, DWORD MunpaID, BOOL bLast)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d, %d", STORED_MUNPA_ACCEPTMEMBER, MasterID, TargetID, MunpaID, bLast);
	g_DB.Query(eQueryType_FreeQuery, eMunpaAcceptMember, MasterID, txt);
}

void MunpaDenyMember(DWORD MasterID, DWORD TargetID, DWORD MunpaID, BOOL bLast)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d, %d", STORED_MUNPA_DENYMEMBER, MasterID, TargetID, MunpaID, bLast);
	g_DB.Query(eQueryType_FreeQuery, eMunpaDenyMember, MasterID, txt);
}

void MunpaJoinMember(DWORD PlayerID, DWORD TargetMunpaID)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d", STORED_MUNPA_JOINMEMBER, PlayerID, TargetMunpaID);
	g_DB.Query(eQueryType_FreeQuery, eMunpaJoinMember, PlayerID, txt);
}

void MunpaJoinMemberCancel(DWORD PlayerID, DWORD MunpaIDX)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d", STORED_MUNPA_JOINMEMBERCANCEL, PlayerID, MunpaIDX);
	g_DB.Query(eQueryType_FreeQuery, eMunpaJoinMemberCancel, PlayerID, txt);
}

void MunpaHomeInfo(DWORD PlayerID, DWORD MunpaID)
{
	g_DB.FreeMiddleQuery(RMunpaHomeInfo,PlayerID,
			"EXEC %s %d", 
			STORED_MUNPA_MUNPAHOMEINFO, MunpaID);
}

void MunpaRegularMembersInfo(DWORD PlayerID, DWORD MunpaID, DWORD GotoPage)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_MUNPA_REGULARMEMBERSINFO, MunpaID, GotoPage, MAX_MUNPA_REGULARMEMBERLIST_NUM);
	g_DB.Query(eQueryType_FreeQuery, eMunpaMembersInfo, PlayerID, txt);
}

void MunpaJoinMembersInfo(DWORD PlayerID, DWORD MunpaID, DWORD GotoPage)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_MUNPA_JOINMEMBERSINFO, MunpaID, GotoPage, MAX_MUNPA_SEMIMEMBERLIST_NUM );
	g_DB.Query(eQueryType_FreeQuery, eMunpaJoinMemberInfo, PlayerID, txt);
}

void MunpaBoardInfo(DWORD PlayerID, DWORD MunpaID)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d", STORED_MUNPA_MUNPABOARDINFO, MunpaID, PlayerID);
	g_DB.Query(eQueryType_FreeQuery, eMunpaBoardInfo, PlayerID, txt);
}
void MunpaCreateBoard(SEND_MUNPA_BOARD_RANK_INFO* msg)
{
//	char txt[384];
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d, \'%s\'", 
		STORED_MUNPA_CREATEBOARD, 
		msg->ID, msg->dwObjectID, msg->RSubjectRank, msg->RContentRank, msg->WContentRank, msg->DContentRank, msg->MunpaBoardName);
	g_DB.Query(eQueryType_FreeQuery, eMunpaCreateBoard, msg->dwObjectID, txt);
}

void MunpaDeleteBoard(DWORD MunpaID, DWORD BoardID, DWORD MasterID)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d",  STORED_MUNPA_DELETEBOARD, MunpaID, MasterID, BoardID);
	g_DB.Query(eQueryType_FreeQuery, eMunpaDeleteBoard, MasterID, txt);
}

void MunpaBoardRankInfo(DWORD MasterID, DWORD MunpaID, DWORD BoardID)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d",  STORED_MUNPA_BOARDRANK_INFO, MunpaID, MasterID, BoardID);
	g_DB.Query(eQueryType_FreeQuery, eMunpaBoardRankInfo, MasterID, txt);
}

void MunpaBoardBackContent(DWORD PlayerID, DWORD BoardIdx, DWORD StartContentID, DWORD MyMunpaRank)
{
	g_DB.FreeLargeQuery(RMunpaBoardReadContent,PlayerID,
		"EXEC %s %d, %d, %d, %d, %d",  STORED_MUNPA_BOARDREADCONTENT, 
		BoardIdx, StartContentID, 1, MyMunpaRank, PlayerID); //1 : AIAu¢®¨úU
}

void MunpaBoardFrontContent(DWORD PlayerID, DWORD BoardIdx, DWORD StartContentID, DWORD MyMunpaRank)
{
	g_DB.FreeLargeQuery(RMunpaBoardReadContent,PlayerID,
		"EXEC %s %d, %d, %d, %d, %d",  STORED_MUNPA_BOARDREADCONTENT, 
		BoardIdx, StartContentID, 0, MyMunpaRank, PlayerID); //0: ¡Ë¡ÍU=¢®¨úU
}

void MunpaBoardCurrentContent(DWORD PlayerID, DWORD BoardIdx, DWORD StartContentID, DWORD MyMunpaRank)
{	
	g_DB.FreeLargeQuery(RMunpaBoardReadContent,PlayerID,
		"EXEC %s %d, %d, %d, %d, %d",  STORED_MUNPA_BOARDREADCONTENT, 
		BoardIdx, StartContentID, 2, MyMunpaRank, PlayerID); //2: CoAc¢®¨úU
}

void MunpaBoardWrite(DWORD PlayerID, DWORD BoardIDX, char* Subject, char* Content, DWORD RootContentsID, DWORD MyMunpaRank)
{
	g_DB.FreeLargeQuery(RMunpaBoardWrite,PlayerID,
		"EXEC %s %d, \'%s\', \'%s\', %d, %d, %d", STORED_MUNPA_BOARDWRITE, 
		BoardIDX, Subject, Content, PlayerID, RootContentsID, MyMunpaRank);
}

void MunpaBoardModify(DWORD PlayerID, DWORD BoardIDX, char* Subject, char* Content, DWORD OriginalContentsID)
{
	g_DB.FreeLargeQuery(RMunpaBoardModify,PlayerID,
		"EXEC %s %d, \'%s\', \'%s\', %d, %d", STORED_MUNPA_BOARDMODIFYCONTENT, 
		BoardIDX, Subject, Content, PlayerID, OriginalContentsID);
}
void MunpaBoardDeleteContents(DWORD PlayerID, DWORD ContentIDx, DWORD MyRank)
{
//	static char txt[256];
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_MUNPA_BOARDDELETECONTENT, PlayerID, ContentIDx, MyRank);
	g_DB.Query(eQueryType_FreeQuery, eMunpaBoardDeleteContent, PlayerID, txt);
}

void MunpaSearchMunpa(DWORD PlayerID, BYTE MapNum, char* Key, DWORD GotoPage)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, \'%s\', %d", STORED_MUNPA_SEARCHMUNPA, MapNum, Key, GotoPage);
	g_DB.Query(eQueryType_FreeQuery, eMunpaSearchMunpa, PlayerID, txt);
}

void MunpaMoneyPutInOut(DWORD PlayerID, DWORD TargetMunpaID, DWORD Money, BOOL bPutIn)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d, %d", STORED_MUNPA_MONEYPUTINOUT, PlayerID, TargetMunpaID, Money, bPutIn);
	g_DB.Query(eQueryType_FreeQuery, eMunpaMoneyPutIn, PlayerID, txt);
}

void MunpaModifyIntro(DWORD CharacterIDX, DWORD MunpaID, char* pIntroMsg)
{
	g_DB.FreeMiddleQuery(RMunpaHomeInfo, CharacterIDX,
		"EXEC %s %d, %d, \'%s\'", 
		STORED_MUNPA_MODIFYINTRO, CharacterIDX, MunpaID, pIntroMsg);
}*/


void GuildLoadGuild(DWORD StartGuildIdx)
{
	sprintf(txt, "EXEC %s %u", STORED_GUILD_LOADGUILD, StartGuildIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildLoadGuild, 0, txt);
}

// 06. 03. ¹®ÆÄ°øÁö - ÀÌ¿µÁØ
void GuildLoadNotice(DWORD StartDBIdx)
{
//	sprintf(txt, "EXEC %s %u", STORED_GUILD_LOADNOTICE, StartDBIdx);
//	g_DB.Query(eQueryType_FreeQuery, , 0, txt);

	g_DB.FreeMiddleQuery( RGuildLoadNotice, eGuildLoadNotice,
		"EXEC %s %u", STORED_GUILD_LOADNOTICE, StartDBIdx );
}

void GuildUpdateNotice(DWORD GuildIdx, char* Notice)
{
	sprintf(txt, "EXEC %s %d, \'%s\'", STORED_GUILD_UPDATENOTICE, GuildIdx, Notice);
	g_DB.Query(eQueryType_FreeQuery, eGuildUpdateNotice, 0, txt);
}

void GuildLoadMember(DWORD StartCharacterIdx)
{
	sprintf(txt, "EXEC %s %u", STORED_GUILD_LOADMEMBER, StartCharacterIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildLoadMember, 0, txt);
}

void GuildLoadMark(DWORD StartMarkName)
{
	char txt[128];
	sprintf(txt, "EXEC %s %u", STORED_GUILD_LOADMARK, StartMarkName);
	g_DB.FreeLargeQuery(RGuildLoadMark, 0, txt);
}

void GuildCreate(DWORD CharacterIdx, char* CharacterName, LEVELTYPE CharacterLvl, char* GuildName, char* Intro)
{
	sprintf(txt, "EXEC %s %u, \'%s\', %d, \'%s\', \'%s\', %d, %d", STORED_GUILD_CREATE, CharacterIdx, CharacterName, CharacterLvl, GuildName, Intro, GAMERESRCMNGR->GetLoadMapNum(), GUILD_1LEVEL);
	g_DB.Query(eQueryType_FreeQuery, eGuildCreate, CharacterIdx, txt);
}

void GuildBreakUp(DWORD GuildIdx, DWORD MasterIdx)
{
	sprintf(txt, "EXEC %s %d", STORED_GUILD_BREAKUP, GuildIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildBreakUp, MasterIdx, txt);
}

void GuildDeleteMember(DWORD GuildIdx, DWORD MemberIDX)
{
	sprintf(txt, "EXEC %s %d, %d", STORED_GUILD_DELETEMEMBER, GuildIdx, MemberIDX);
	g_DB.Query(eQueryType_FreeQuery, eGuildDeleteMember, GuildIdx, txt);
}

void GuildSecedeMember(DWORD GuildIdx, DWORD PlayerIdx)
{
	sprintf(txt, "EXEC %s %d, %d", STORED_GUILD_DELETEMEMBER, GuildIdx, PlayerIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildSecedeMember, GuildIdx, txt);
}

void GuildAddMember(DWORD GuildIdx, DWORD MemberIDX, char* GuildName, BYTE Rank)
{
	sprintf(txt, "EXEC %s %u, %u, \'%s\', %d", STORED_GUILD_ADDMEMBER, GuildIdx, MemberIDX, GuildName, Rank);
	g_DB.Query(eQueryType_FreeQuery, eGuildAddMember, 0, txt);
}

void GuildMarkRegist(DWORD GuildIdx, char* ImgData, DWORD CharacterIdx)
{
	char buf[1024];
	sprintf( buf, "EXEC %s %d, 0x", STORED_GUILD_MARKREGIST,GuildIdx);

	int curpos = strlen(buf);
	for(int n=0;n<GUILDMARK_BUFSIZE;++n)
	{
		sprintf(&buf[curpos],"%02x",(BYTE)ImgData[n]);
		curpos += 2;
	}

	g_DB.FreeLargeQuery(RGuildMarkRegist, CharacterIdx,	
		buf);
}

void GuildLevelUp(DWORD GuildIdx, BYTE Level)
{
	sprintf(txt, "EXEC %s %u, %d", STORED_GUILD_LEVELUP, GuildIdx, Level);
	g_DB.Query(eQueryType_FreeQuery, eGuildLevelUp, 0, txt);
}

void GuildChangeMemberRank(DWORD GuildIdx, DWORD MemberIdx, BYTE Rank)
{
	sprintf(txt, "EXEC %s %u, %u, %d", STORED_GUILD_CHANGRANK, GuildIdx, MemberIdx, Rank);
	g_DB.Query(eQueryType_FreeQuery, eGuildLevelUp, 0, txt);
}

void GuildItemLoad(DWORD MapNum, DWORD StartItemDBIDx)
{
	sprintf(txt, "EXEC %s %d, %u", STORED_GUILD_LOADITEM, MapNum, StartItemDBIDx);
	g_DB.Query(eQueryType_FreeQuery, eGuildItemLoad, 0, txt);
}

void GuildItemOption(DWORD MapNum, DWORD StartDBIdx)
{
	sprintf(txt, "EXEC %s %d, %u", STORED_GUILD_ITEMOPTION, MapNum, StartDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildItemOption, 0, txt);
}

//SW050920 Rare
void GuildItemRareOption(DWORD MapNum, DWORD StartDBIdx)
{
	sprintf(txt, "EXEC %s %d, %u", "dbo.MP_GUILD_ItemRareOption_Info", MapNum, StartDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildItemRareOption, 0, txt);
}

void GuildItemLoadInNeed( DWORD MapNum, DWORD GuildIdx )
{
	sprintf(txt, "EXEC dbo.MP_GUILD_LOADITEM_INNEED %d, %d", MapNum, GuildIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildItemLoadInNeed, GuildIdx, txt);
}

//Á¤ÆÀÀå´Ô Áö½Ã "Ã³¸®ÇÔ¼ö ºÙ¿©³õÀ»°Í~"
void RGuildItemLoadInNeed( LPQUERY pData, LPDBMESSAGE pMessage )
{
	ITEMBASE guilditem;
	DWORD GuildIdx = pMessage->dwID;

	if( 0 == pMessage->dwResult )	//¹®ÆÄÃ¹ »ý¼ºÀÌ°Å³ª ºóÃ¢°íÀÏ °æ¿ì.
	{
		GUILDMGR->SetGuildItemInfoInited(GuildIdx, TRUE);

		GUILDMGR->SendGuildItemInfoToAllWarehouseMember(GuildIdx);

		return;
	}

	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		GuildIdx = atoi((char*)pData[i].Data[eMu_IMunpaID]);

		guilditem.Durability = atoi((char*)pData[i].Data[eMu_IDurability]);
		guilditem.wIconIdx = atoi((char*)pData[i].Data[eMu_IIDX]);
		guilditem.dwDBIdx = atoi((char*)pData[i].Data[eMu_IDBIDX]);
		guilditem.Position = atoi((char*)pData[i].Data[eMu_IPosition]);
		guilditem.RareIdx = atoi((char*)pData[i].Data[eMu_IRareIdx]);
		guilditem.QuickPosition = 0;
		guilditem.ItemParam = 0;	// magi82(28)

		GUILDMGR->RegistGuildItem(GuildIdx, &guilditem);
	}
	if(pMessage->dwResult < MAX_QUERY_RESULT)
	{
		GuildItemOptionInNeed(GAMERESRCMNGR->GetLoadMapNum(), GuildIdx, 0);
		//SW050920 Rare
		//GuildItemRareOptionInNeed(GAMERESRCMNGR->GetLoadMapNum(), GuildIdx, 0);
	}	
	else
	{
		// SLOT_GUILDWAREHOUSE_NUM < MAX_QUERY_RESULT ÀÌ°÷¿¡ µé¾î¿Ã ÀÏ ¾ø´Ù.
		GuildItemLoad(GAMERESRCMNGR->GetLoadMapNum(), guilditem.dwDBIdx);
	}
}

void GuildItemOptionInNeed( DWORD MapNum, DWORD GuildIdx, DWORD StartDBIdx )
{
	sprintf(txt, "EXEC dbo.MP_GUILD_LOADITEMOPTION_INNEED %d, %d, %d", MapNum, GuildIdx, StartDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildItemOptionInNeed, 0, txt);
}

void RGuildItemOptionInNeed( LPQUERY pData, LPDBMESSAGE pMessage )
{
	ITEM_OPTION_INFO OptionInfo;
	DWORD GuildIdx;

	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		GuildIdx = atoi((char*)pData[i].Data[eGIOI_GuildIdx]);

		OptionInfo.dwOptionIdx				= (DWORD)atoi((char*)pData[i].Data[eGIOI_OptionID]);
		OptionInfo.dwItemDBIdx				= (DWORD)atoi((char*)pData[i].Data[eGIOI_ItemDBID]);
		OptionInfo.GenGol					= (WORD)atoi((char*)pData[i].Data[eGIOI_GenGol]);
		OptionInfo.MinChub					= (WORD)atoi((char*)pData[i].Data[eGIOI_MinChub]);				
		OptionInfo.CheRyuk					= (WORD)atoi((char*)pData[i].Data[eGIOI_CheRyuk]);				
		OptionInfo.SimMek					= (WORD)atoi((char*)pData[i].Data[eGIOI_SimMek]);					
		OptionInfo.Life						= (DWORD)atoi((char*)pData[i].Data[eGIOI_Life]);					
		OptionInfo.NaeRyuk					= (WORD)atoi((char*)pData[i].Data[eGIOI_NaeRyuk]);				
		OptionInfo.Shield					= (DWORD)atoi((char*)pData[i].Data[eGIOI_Shield]);					
		OptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[eGIOI_FireAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[eGIOI_WaterAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[eGIOI_TreeAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[eGIOI_GoldAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[eGIOI_EarthAttrRegist])/100.f );
		OptionInfo.PhyAttack				= (WORD)atoi((char*)pData[i].Data[eGIOI_PhyAttack]);
		OptionInfo.CriticalPercent			= (WORD)atoi((char*)pData[i].Data[eGIOI_Critical]);
		OptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[eGIOI_FireAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[eGIOI_WaterAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[eGIOI_TreeAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[eGIOI_GoldAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[eGIOI_EarthAttrAttack])/100.f );
		OptionInfo.PhyDefense				= (WORD)atoi((char*)pData[i].Data[eGIOI_PhyDefense]);				
		
		GUILDMGR->AddGuildItemOption(&OptionInfo);
	}

	if(pMessage->dwResult >= MAX_QUERY_RESULT)
	{
		GuildItemOptionInNeed(GAMERESRCMNGR->GetLoadMapNum(), GuildIdx, OptionInfo.dwOptionIdx);
	}
	else
	{
		GuildItemRareOptionInNeed(GAMERESRCMNGR->GetLoadMapNum(), GuildIdx, 0);
	}
}

void GuildItemRareOptionInNeed( DWORD MapNum, DWORD GuildIdx, DWORD StartDBIdx )
{
	sprintf(txt, "EXEC dbo.MP_GUILD_LOADITEMRAREOPTION_INNEED %d, %d, %d", MapNum, GuildIdx, StartDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildItemRareOptionInNeed, 0, txt);
}

void RGuildItemRareOptionInNeed( LPQUERY pData, LPDBMESSAGE pMessage )
{
	ITEM_RARE_OPTION_INFO RareOptionInfo;
	DWORD GuildIdx;

	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		GuildIdx = atoi((char*)pData[i].Data[eGIROI_GuildIdx]);

		RareOptionInfo.dwRareOptionIdx				= (DWORD)atoi((char*)pData[i].Data[eGIROI_RareOptionID]);
		RareOptionInfo.dwItemDBIdx				= (DWORD)atoi((char*)pData[i].Data[eGIROI_ItemDBID]);
		RareOptionInfo.GenGol					= (WORD)atoi((char*)pData[i].Data[eGIROI_GenGol]);
		RareOptionInfo.MinChub					= (WORD)atoi((char*)pData[i].Data[eGIROI_MinChub]);				
		RareOptionInfo.CheRyuk					= (WORD)atoi((char*)pData[i].Data[eGIROI_CheRyuk]);				
		RareOptionInfo.SimMek					= (WORD)atoi((char*)pData[i].Data[eGIROI_SimMek]);					
		RareOptionInfo.Life						= (DWORD)atoi((char*)pData[i].Data[eGIROI_Life]);					
		RareOptionInfo.NaeRyuk					= (WORD)atoi((char*)pData[i].Data[eGIROI_NaeRyuk]);				
		RareOptionInfo.Shield					= (DWORD)atoi((char*)pData[i].Data[eGIROI_Shield]);					
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[eGIROI_FireAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[eGIROI_WaterAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[eGIROI_TreeAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[eGIROI_GoldAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[eGIROI_EarthAttrRegist])/100.f );
		RareOptionInfo.PhyAttack				= (WORD)atoi((char*)pData[i].Data[eGIROI_PhyAttack]);
//		RareOptionInfo.CriticalPercent			= (WORD)atoi((char*)pData[i].Data[eGIROI_Critical]);
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[eGIROI_FireAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[eGIROI_WaterAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[eGIROI_TreeAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[eGIROI_GoldAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[eGIROI_EarthAttrAttack])/100.f );
		RareOptionInfo.PhyDefense				= (WORD)atoi((char*)pData[i].Data[eGIROI_PhyDefense]);
		
		GUILDMGR->AddGuildItemRareOption(&RareOptionInfo);
	}

	if(pMessage->dwResult >= MAX_QUERY_RESULT)
	{
		GuildItemRareOptionInNeed(GAMERESRCMNGR->GetLoadMapNum(), GuildIdx, RareOptionInfo.dwRareOptionIdx);
	}
	else
	{
		GUILDMGR->SetGuildItemInfoInited(GuildIdx, TRUE);

		GUILDMGR->SendGuildItemInfoToAllWarehouseMember(GuildIdx);
	}
}

//SW060719 ¹®ÆÄÆ÷ÀÎÆ®
void GuildLoadGuildPointInfo( DWORD startGuildDBIdx )
{
	sprintf(txt, "EXEC dbo.MP_GUILD_LOAD_GUILDPOINTINFO %d", startGuildDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildPointInfoLoad, 0, txt);
}

void RGuildLoadGuildPointInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	//if( !pMessage->dwResult )	return;

	GUILDPOINT_INFO GuildPointInfo;
	
	DWORD GuildDBIdx = 0;
	DWORD GuildIdx = 0;
	//if(0 == pMessage->dwResult)	return;

	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		GuildDBIdx = atoi((char*)pData[i].Data[eGPII_GuildDBIdx]);
		GuildIdx = atoi((char*)pData[i].Data[eGPII_GuildIdx]);
		GuildPointInfo.GuildPoint = atoi((char*)pData[i].Data[eGPII_GuildPoint]);
		//GuildPointInfo.GuildHuntedMonsterCount = 0;
		GuildPointInfo.GuildHuntedMonsterTotalCount = atoi((char*)pData[i].Data[eGPII_GuildHuntedMonsterTotalCount]);
		//GuildPointInfo.DBProcessTime = 0;
		//GuildPointInfo.GuildPlusTimeflg = 0;
		//µû·Î..
		//for( int plustimeKind = 0; plustimeKind < eGPT_Kind_Max; ++plustimeKind )
		//{
		//	GuildPointInfo.GuildUsingPlusTimeInfo[plustimeKind].PlusTimeEndTime = atoi((char*)pData[i].Data[eGPII_EndTimePerGuildPlustimeKind + plustimeKind]);
		//}

		GUILDMGR->InitGuildPointInfo(GuildIdx, &GuildPointInfo);
	}

	if( pMessage->dwResult >= MAX_QUERY_RESULT )
	{
		GuildLoadGuildPointInfo(GuildDBIdx);
	}
	else
	{
		GuildLoadGuildPlustimeInfo( 0 );
	}
}

void GuildLoadGuildPlustimeInfo( DWORD startDBIdx )
{
	sprintf(txt, "EXEC dbo.MP_GUILD_LOAD_GUILDPLUSTIMEINFO %d", startDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildPlustimeInfoLoad, 0, txt);
}

void RGuildLoadGuildPlustimeInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	//if( !pMessage->dwResult )	return;
	
	DWORD GuildPlustimeDBIdx = 0;
	DWORD GuildIdx = 0;
	DWORD GuildUsePlustimeIdx = 0;
	DWORD GuildUsePlustimeEndtime = 0;

	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		GuildPlustimeDBIdx = atoi((char*)pData[i].Data[0]);
		GuildIdx = atoi((char*)pData[i].Data[1]);
		GuildUsePlustimeIdx = atoi((char*)pData[i].Data[2]);
		GuildUsePlustimeEndtime = atoi((char*)pData[i].Data[3]);

		GUILDMGR->InitGuildPlustimeInfo(GuildIdx, GuildUsePlustimeIdx, GuildUsePlustimeEndtime);
	}

	if( pMessage->dwResult >= MAX_QUERY_RESULT )
	{
		GuildLoadGuildPlustimeInfo(GuildPlustimeDBIdx);
	}
	else
	{
		//RSiegeWarProfitInfoLoad(.. ¿¡¼­ ÀÌµ¿
		ItemLimitInfoLoadAll( 0 );
		// ¿ä»õÀü
        FortWarInfoLoad();
		FORTWARMGR->TotalFortWarItemLoad();
		//

		g_pServerSystem->SetStart( TRUE );
	}
}

void GuildAddHuntedMonsterCount( DWORD GuildIdx, DWORD MonsterCount )
{
	sprintf(txt, "EXEC dbo.MP_GUILD_ADD_HUNTEDMONSTER_COUNT %d, %d", GuildIdx, MonsterCount);
	g_DB.Query(eQueryType_FreeQuery, eGuildAddHuntedMonsterCount, GuildIdx, txt);
}

void RGuildAddHuntedMonsterCount( LPQUERY pData, LPDBMESSAGE pMessage )
{
	//DB ÃÑ ÇÕ»ê °ª ¹Þ¾Æ¿Í¼­ ±æµå Á¤º¸ ¼¼ÆÃ. ¸Ê¼­¹öµé¿¡°Ô ÀüÆÄ.
	DWORD GuildIdx = pMessage->dwID;
	DWORD count = pMessage->dwResult;

	if(!count)	return;

	DWORD GuildHuntedMonsterTotalCount = atoi((char*)pData->Data[eGHMC_TotalCount]);
	//DWORD DBProcessTime = atoi((char*)pData->Data[eGHMC_DB_Date]);

	GUILDMGR->SendGuildHuntedMonsterTotalCountToMapServerExceptOneself(GuildIdx, GuildHuntedMonsterTotalCount);
}

void GuildGetHuntedMonsterTotalCountWithInit( DWORD GuildIdx )
{
	sprintf(txt, "EXEC dbo.MP_GUILD_CONVERT_HUNTEDMONSTER_COUNT %d", GuildIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildGetHuntedMonsterTotalCountWithInit, GuildIdx, txt);
}

void RGuildGetHuntedMonsterTotalCountWithInit( LPQUERY pData, LPDBMESSAGE pMessage )
{
	//!!!·Î±× ³²°ÜÁÖ±â(¸Ê¹øÈ£,ÃÑÇÕ,º¯È¯µÈÆ÷ÀÎÆ®)
	//DB ÃÑ ÇÕ»ê °ª ¹Þ¾Æ¿Í Æ÷ÀÎÆ®·Î Á¤»ê
	DWORD GuildIdx = pMessage->dwID;
	DWORD count = pMessage->dwResult;

	if(!count)	return;

	DWORD GuildHuntedMonsterTotalCount = atoi((char*)pData->Data[0]);
	//DWORD GuildHuntedMonsterTotalCount = atoi((char*)pData->Data[eGCHMC_TotalCount]);
	//DWORD DBProcessTime = atoi((char*)pData->Data[eGCHMC_DB_Date]);
	if(0 == GuildHuntedMonsterTotalCount)	return;

	GUILDMGR->GuildConvertHuntedMonsterTotalCountToGuildPoint(GuildIdx, GuildHuntedMonsterTotalCount);
}

void GuildAddGuildPoint( DWORD GuildIdx, DWORD rAddPoint, DWORD rAddKind, DWORD rAdditionalData )
{
	sprintf(txt, "EXEC dbo.MP_GUILD_ADD_GUILDPOINT %d, %d, %d, %d", GuildIdx, rAddPoint, rAddKind, rAdditionalData);
	g_DB.Query(eQueryType_FreeQuery, eGuildAddGuildPoint, GuildIdx, txt);
}

void RGuildAddGuildPoint( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD GuildIdx = pMessage->dwID;
	DWORD count = pMessage->dwResult;

	if(!count)	return;

	DWORD GuildAddPoint =	atoi((char*)pData->Data[eGAGP_AddPoint]);
	DWORD GuildTotalPoint = atoi((char*)pData->Data[eGAGP_TotalPoint]);
	int AddKind =			atoi((char*)pData->Data[eGAGP_AddKind]);
	DWORD AdditionalData =	atoi((char*)pData->Data[eGAGP_AdditionalData]);

	GUILDMGR->AddGuildPointFromDBReturn(GuildIdx, GuildAddPoint, GuildTotalPoint, AddKind, AdditionalData);
}

void GuildUseGuildPoint( DWORD GuildIdx, DWORD rUsePoint, DWORD rUseKind, DWORD rKindIdx, DWORD rEndtime )
{
	sprintf(txt, "EXEC dbo.MP_GUILD_USE_GUILDPOINT %d, %d, %d, %d, %d", GuildIdx, rUsePoint, rUseKind, rKindIdx, rEndtime);
	g_DB.Query(eQueryType_FreeQuery, eGuildUseGuildPoint, GuildIdx, txt);
}

void RGuildUseGuildPoint( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD GuildIdx = pMessage->dwID;
	DWORD count = pMessage->dwResult;

	if(!count)	return;

	DWORD GuildUsePoint =	atoi((char*)pData->Data[eGUGP_UsePoint]);
	// ±æµåÆ÷ÀÎÆ®»ç¿ë½Ã ¿¡·¯Ã³¸®
	if( GuildUsePoint <= 0 )
		return;
	DWORD GuildTotalPoint = atoi((char*)pData->Data[eGUGP_TotalPoint]);
	int UseKind =			atoi((char*)pData->Data[eGUGP_UseKind]);
	DWORD KindIdx =			atoi((char*)pData->Data[eGUGP_KindIdx]);
	DWORD Endtime =			atoi((char*)pData->Data[eGUGP_Endtime]);

	GUILDMGR->UseGuildPointFromDBReturn(GuildIdx, GuildUsePoint, GuildTotalPoint, UseKind, KindIdx, Endtime);
}

void GuildPlustimeEnd( DWORD GuildIdx, DWORD rPlusTimeIdx )
{
	sprintf(txt, "EXEC dbo.MP_GUILD_PLUSTIME_END %d, %d", GuildIdx, rPlusTimeIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildPlustimeEnd, GuildIdx, txt);
}

void RGuildPlustimeEnd(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD GuildIdx = pMessage->dwID;
	DWORD count = pMessage->dwResult;

	if(!count)	return;

	DWORD PlustimeIdx =		atoi((char*)pData->Data[0]);

	GUILDMGR->EndGuildPlustimeDBReturn(GuildIdx, PlustimeIdx);
}

void LogGuildPoint( DWORD GuildIdx, DWORD TotalGuildPoint, BOOL bAdd, int eKind, DWORD GuildPoint, DWORD AddData, DWORD CharacterIdx )
{
	sprintf(txt, "EXEC dbo.Up_GuildPointLog %d, %d, %d, %d, %d,  %d, %d",
		GuildIdx,
		TotalGuildPoint,
		bAdd,
		eKind,
		GuildPoint,
		AddData,
		CharacterIdx
		);
	g_DB.LogQuery(eQueryType_FreeQuery, eLogGuildPoint, 0, txt);
}

void GuildMoneyUpdate(DWORD GuildIdx, MONEYTYPE Money)
{
	sprintf(txt, "EXEC %s %u, %u", STORED_GUILD_MONEYUPDATE, GuildIdx, Money);
	g_DB.Query(eQueryType_FreeQuery, eGuildMoneyUpdate, 0, txt);
}

void GuildGiveMemberNickName(DWORD TargetId, char * NickName)
{
	sprintf(txt, "EXEC %s %u, \'%s\'", STORED_GUILD_GIVENICKNAME, TargetId, NickName);
	g_DB.Query(eQueryType_FreeQuery, eGuildGiveNickName, 0, txt);
}

void PyogukBuyPyoguk(DWORD PlayerIDX)
{
	sprintf(txt, "EXEC %s %d", STORED_PYOGUK_BUYPYOGUK, PlayerIDX);
	g_DB.Query(eQueryType_FreeQuery, ePyogukBuyPyoguk, PlayerIDX, txt);
}

void PyogukMoneyUpdateToDB(DWORD UserIdx, MONEYTYPE PyogukMoney)
{
	sprintf(txt, "EXEC %s %u, %u", STORED_PYOGUK_MONEYUPDATE, UserIdx, PyogukMoney);
	g_DB.Query(eQueryType_FreeQuery, ePyogukMoneyUpdate, 0, txt);
}

void SaveCharInfoBeforeLogout(DWORD PlayerIdx, DWORD dwConnectionIndex, DWORD UserIdx, MONEYTYPE PyogukMoney)
{
	sprintf(txt, "EXEC %s %d, %u, %u", STORED_CHARACTER_SAVEINFOBEFORELOGOUT, dwConnectionIndex, UserIdx, PyogukMoney);
	g_DB.Query(eQueryType_FreeQuery, eSaveChrInfoBeforeOut, PlayerIdx, txt);
}


/* A¨Ï¡þ¢®¨¡¡Ë¢çEA ¢®ief
#define STORED_PYOCUK_INFO				"MP_PYOGUK_Info"
#define STORED_PYOCUK_ITEMINFO			"MP_PYOGUK_ItemInfo"
*/

/* C¢®I¢®¨ú¨Ïo d¡§¡þ¡Ë¡þ ¡§uod AU¡§u¢®A */
void CharacterPyogukInfo(DWORD UserIdx, DWORD CharacterIdx)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_PYOGUK_INFO, UserIdx);
	g_DB.Query(eQueryType_FreeQuery, eCharacterPyogukInfo, CharacterIdx, txt);
}

void CharacterPyogukItemInfo(DWORD CharacterIdx, DWORD StartDBIdx)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d", STORED_PYOGUK_ITEMINFO, CharacterIdx, StartDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eCharacterPyogukItemInfo, CharacterIdx, txt);
}

void FriendNotifyLogouttoClient(DWORD PlayerID)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_FRIEND_NOTIFYLOGOUT, PlayerID);
	g_DB.Query(eQueryType_FreeQuery, eFriendNotifyLogout, PlayerID, txt);
}

void WantedLoad(WANTEDTYPE StartWantedIDX)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_WANTED_LOAD, StartWantedIDX);
	g_DB.Query(eQueryType_FreeQuery, eWantedLoad, 0, txt);	
}

void WantedInfoByUserLogIn(DWORD CharacterIDX)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_WANTED_INFO, CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eWantedGetList, CharacterIDX, txt);
}

void WantedBuyRight(DWORD CharacterIDX, WANTEDTYPE WantedIDX, MONEYTYPE AddPrize)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_WANTED_BUYRIGHT, CharacterIDX, WantedIDX, AddPrize);
	g_DB.Query(eQueryType_FreeQuery, eWantedBuyRight, 0, txt);
}

void WantedRegist(DWORD CharacterIDX, DWORD TargetIDX, MONEYTYPE Prize)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_WANTED_REGIST, CharacterIDX, TargetIDX, Prize);
	g_DB.Query(eQueryType_FreeQuery, eWantedRegist, CharacterIDX, txt);
}

void WantedGiveUpRight(DWORD CharacterIDX, DWORD WantedIDX)
{
	sprintf(txt, "EXEC %s %d, %d", STORED_WANTED_GIVEUP, CharacterIDX, WantedIDX);
	g_DB.Query(eQueryType_FreeQuery, eWantedGiveupRight, 0, txt);
}

void WantedComplete(DWORD CharacterIDX, char* TargetName, WANTEDTYPE WantedIDX)
{
	/*A¡Ë¡Í¢®ii¢®¨úC 100¢®¨¡¨Ï©ª¢®¨¡¡Ë¢ç ¡Ë¡þ¡§¢®¡§o¡§¡þ*/
	sprintf(txt, "EXEC %s %d, \'%s\', %d", STORED_WANTED_COMPLETE, CharacterIDX, TargetName, WantedIDX);
	g_DB.Query(eQueryType_FreeQuery, eWantedComplete, WantedIDX, txt);
}

void WantedDestroyed(DWORD WantedIDX)
{
	sprintf(txt, "EXEC %s %d", STORED_WANTED_DESTROYED, WantedIDX);
	g_DB.Query(eQueryType_FreeQuery, eWantedDestroyed, WantedIDX, txt);
}

void WantedOrderedList(DWORD CharacterIDX, DWORD Page, char* OrderType, BYTE PageUnit)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, \'%s\', %d", STORED_WANTED_ORDERLIST, Page, OrderType, PageUnit);
	g_DB.Query(eQueryType_FreeQuery, eWantedOrderList, CharacterIDX, txt);
}

void WantedMyList(DWORD CharacterIDX, DWORD Page, BYTE PageUnit)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_WANTED_MYLIST, CharacterIDX, Page, PageUnit);
	g_DB.Query(eQueryType_FreeQuery, eWantedMyList, CharacterIDX, txt);
}

void WantedSearch(DWORD CharacterIDX, char* WantedName, DWORD Page, BYTE PageUnit)
{
	sprintf(txt, "EXEC %s \'%s\', %d, %d", STORED_WANTED_SEARCH, WantedName, Page, PageUnit);
	g_DB.Query(eQueryType_FreeQuery, eWantedSearch, CharacterIDX, txt);
}

void JournalGetList(DWORD CharacterIDX)
{
	sprintf(txt, "EXEC %s %d", STORED_JOURNAL_GETLIST, CharacterIDX);
	g_DB.Query(eQueryType_FreeQuery, eJournalGetList, CharacterIDX, txt);
}

void JouranlInsertQuest(DWORD CharacterIDX, BYTE Kind, DWORD QuestIDX, DWORD SubQuestIDX, int bCompleted)
{
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d", STORED_JOURNAL_INSERTQUEST, CharacterIDX, Kind, QuestIDX, SubQuestIDX, bCompleted);
	g_DB.Query(eQueryType_FreeQuery, eJournalInsertQuest, 0, txt);
}

void JouranlInsertWanted(DWORD CharacterIDX, BYTE Kind, DWORD Param, char* ParamName)
{
	sprintf(txt, "EXEC %s %d, %d, %d, \'%s\'", STORED_JOURNAL_INSERTWANTED, CharacterIDX, Kind, Param, ParamName);
	g_DB.Query(eQueryType_FreeQuery, eJournalInsertWanted, 0, txt);
}

void JournalInsertLevel(DWORD CharacterIDX, BYTE Kind, DWORD Level)
{
	sprintf(txt, "EXEC %s %d, %d, %d", STROED_JOURNAL_INSERTLEVEL, CharacterIDX, Kind, Level);
	g_DB.Query(eQueryType_FreeQuery, eJournalInsertLevel, 0, txt);
}
void JournalSaveUpdate(DWORD CharacterIDX, DWORD JournalIndex)
{
	sprintf(txt, "EXEC %s %d, %d", STROED_JOURNAL_SAVEUPDATE, CharacterIDX, JournalIndex);
	g_DB.Query(eQueryType_FreeQuery, eJournalSaveUpdate, 0, txt);
}

void JournalDelete(DWORD CharacterIDX, DWORD JournalIndex)
{
	sprintf(txt, "EXEC %s %d, %d", STROED_JOURNAL_DELETE, CharacterIDX, JournalIndex);
	g_DB.Query(eQueryType_FreeQuery, eJournalDelete, 0, txt);
}

//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// 
// Update¨Ïo¡Ëc
//void CharacterItemInsert(DWORD CharacterIDX, WORD ItemIDX, DURTYPE Durability, POSTYPE bPosition, WORD qPosition)
/*
void CharacterItemUpdate(DWORD CharacterIDX, DWORD dwDBIdx, WORD wItemIdx, DURTYPE Durability, POSTYPE bPosition, WORD qPosition)
{
	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d", STORED_CHARACTER_ITEMUPDATE, 
		CharacterIDX, dwDBIdx, wItemIdx, bPosition, qPosition, Durability);
	g_DB.Query(eQueryType_FreeQuery, eItemUpdate, CharacterIDX, txt);
	{
	}
}


void CharacterItemInsert(DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos)
{
//	char txt[128];
//	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d", STORED_CHARACTER_ITEMUPDATE, 
//		CharacterIDX, dwDBIdx, wItemIdx, bPosition, qPosition, Durability);
//	g_DB.Query(eQueryType_FreeQuery, eItemUpdate, CharacterIDX, txt);
//	{
//	}
}
*/

/*
void CharacterMugongUpdate(MUGONGBASE* msg)
{
	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d, %d", STORED_CHARACTER_MUGONGUPDATE, 
		msg->dwDBIdx, msg->wIconIdx, msg->ExpPoint, msg->Sung, msg->Position, msg->QuickPosition, msg->bWear);
	g_DB.Query(eQueryType_FreeQuery, eMugongUpdate, 0, txt);
	{
	}
	FILE* fp = fopen("MGTEST.txt","a+");
	fprintf(fp,txt);
	fclose(fp);
}

void CharacterMugongMoveUpdate(DWORD toDBIdx, POSTYPE toPos, DWORD fromDBIdx, POSTYPE fromPos)
{
	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %d, %d", STORED_CHARACTER_MUGONGMOVEUPDATE, 
		toDBIdx, toPos, fromDBIdx, fromPos);
	g_DB.Query(eQueryType_FreeQuery, eMugongMoveUpdate, 0, txt);
	{
	}
}
*/

void CharacterHeroInfoUpdate(CPlayer* pPlayer)
{
	if( !pPlayer->GetInited() )	return;

	BASEOBJECT_INFO baseinfo;
	HERO_TOTALINFO	heroinfo;
	
	pPlayer->GetBaseObjectInfo(&baseinfo);
	pPlayer->GetHeroTotalInfo(&heroinfo);
	
//	char txt[512];
	sprintf(txt, "EXEC  %s %d, %d, %d, %d, %d, %d, %d, %d, %I64d, %d, %u, %d, %u, %d, %d, %d",
		STORED_CHARACTER_HEROINFOUPDATE,
		baseinfo.dwObjectID, 
		heroinfo.wGenGol, 
		heroinfo.wNaeRyuk, 
		heroinfo.wMinChub, 
		heroinfo.wCheRyuk, 
		heroinfo.wSimMek, 
		0,//heroinfo.Fame,	//now, fame not used... but, it's occur some bugs.
		heroinfo.wKarma, 
		heroinfo.ExpPoint, 
		heroinfo.LevelUpPoint, 
		heroinfo.Money, 
		heroinfo.KyungGongGrade,
		pPlayer->GetPlayerAbilityExpPoint(), 
		heroinfo.Playtime, 
		heroinfo.LastPKModeEndTime,
		heroinfo.MaxLevel);
	g_DB.Query(eQueryType_FreeQuery, eHeroInfoUpdate, 0, txt);
}

void CharacterTotalInfoUpdate(CPlayer* pPlayer)
{
	if( !pPlayer->GetInited() )	return;

	CHARACTER_TOTALINFO totinfo;
	pPlayer->GetCharacterTotalInfo(&totinfo);

	// 06. 06 - ÀÌ¿µÁØ Áß¿ä!!!
	//MP_CHARACTER_TotalInfoUpdate ¼öÁ¤ ÇÊ¿äÇÔ
//	char txt[512];
	sprintf(txt, "EXEC  %s %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
		STORED_CHARACTER_TOTALINFOUPDATE,
		pPlayer->GetID(), totinfo.Gender,
		totinfo.Life,
		totinfo.Shield,
		totinfo.Stage,
		totinfo.Level,
		totinfo.LoginMapNum, totinfo.FaceType, totinfo.HairType, totinfo.WearedItemIdx[eWearedItem_Hat],
		totinfo.WearedItemIdx[eWearedItem_Weapon], totinfo.WearedItemIdx[eWearedItem_Dress],
		totinfo.WearedItemIdx[eWearedItem_Shoes], totinfo.WearedItemIdx[eWearedItem_Ring1],
		totinfo.WearedItemIdx[eWearedItem_Ring2], totinfo.WearedItemIdx[eWearedItem_Cape],
		totinfo.WearedItemIdx[eWearedItem_Necklace], totinfo.WearedItemIdx[eWearedItem_Armlet],
		totinfo.WearedItemIdx[eWearedItem_Belt],
		totinfo.BadFame);
	g_DB.Query(eQueryType_FreeQuery, eTotalInfoUpdate, 0, txt);
}

void CharacterBadFameUpdate(DWORD PlayerID, int BadFame, DWORD Money)
{	
	sprintf(txt, "EXEC  %s %d, %d, %u", STORED_CHARACTER_BADFAMEUPDATE, PlayerID, BadFame, Money);
	g_DB.Query(eQueryType_FreeQuery, eTotalInfoUpdate, 0, txt);
}

void CharacterLvlExpUpdate(DWORD PlayerID, LEVELTYPE level, EXPTYPE exppoint)
{
	sprintf(txt, "update TB_Character \
set character_grade = %d, character_expoint = %I64d \
where character_idx = %d", level, exppoint, PlayerID);
	g_DB.Query(eQueryType_FreeQuery, eTotalInfoUpdate, 0, txt);
}
/*
void CharacterQuickInfoUpdate(CPlayer * pPlayer)
{
	QUICKITEM quickItem;
	for(int i = QUICK_STARTPOSITION ; i < QUICK_ENDPOSITION ; i++)
	{
		if(pPlayer->GetQuickPostion(i, &quickItem))
		{
			
			switch(quickItem.eIGKinds)
			{
			case eMUGONG_ICON:
				{
					MUGONGBASE * updMugong = pPlayer->GetMugongBase(quickItem.wAbsPostion);
					CharacterMugongUpdate(updMugong);
				}
				break;
			case eITEM_ICON:
				{
					CIconTable * pTable = pPlayer->GetIconTable(GetTableIdxPosition(quickItem.wAbsPostion));
					
					ITEMBASE * updItem = pTable->GetItemInfoAbs(quickItem.wAbsPostion);
					ASSERT(updItem->dwDBIdx > 0);
					if(updItem->dwDBIdx > 0)
						CharacterItemUpdate(pPlayer->GetID(), updItem->dwDBIdx, updItem->wIconIdx, updItem->Durability, updItem->Position, i);
					
				}
				break;
			default:
				{
					//ASSERT(0);
				}
			}
		}
	}
}
*/
void CharacterMugongInfoUpdate(CPlayer* pPlayer)
{
	MUGONGBASE* pMugong;

#ifdef _JAPAN_LOCAL_
 	for(int n=TP_MUGONG_START; n<TP_MUGONG_END; ++n)
   	{
   		pMugong = pPlayer->GetMugongBase(n);
 		if(pMugong->dwDBIdx==0 || pMugong->bWear==0)
   			continue;
   		MugongUpdateToDB(pMugong, "TOTALUPDATE");
   	}
   	
 	for(n=TP_JINBUB_START; n<TP_JINBUB_START+SLOT_JINBUB_NUM; ++n)
   	{
   		pMugong = pPlayer->GetMugongBase(n);
 		if(pMugong->dwDBIdx==0 || pMugong->bWear==0)
   			continue;
   		MugongUpdateToDB(pMugong, "TOTALUPDATE");
   	}
#elif defined _HK_LOCAL_
 	for(int n=TP_MUGONG_START; n<TP_MUGONG_END; ++n)
   	{
   		pMugong = pPlayer->GetMugongBase(n);
 		if(pMugong->dwDBIdx==0 || pMugong->bWear==0)
   			continue;
   		MugongUpdateToDB(pMugong, "TOTALUPDATE");
   	}
   	
 	for(n=TP_JINBUB_START; n<TP_JINBUB_START+SLOT_JINBUB_NUM; ++n)
   	{
   		pMugong = pPlayer->GetMugongBase(n);
 		if(pMugong->dwDBIdx==0 || pMugong->bWear==0)
   			continue;
   		MugongUpdateToDB(pMugong, "TOTALUPDATE");
   	}
#elif defined _TL_LOCAL_
 	for(int n=TP_MUGONG_START; n<TP_MUGONG_END; ++n)
   	{
   		pMugong = pPlayer->GetMugongBase(n);
 		if(pMugong->dwDBIdx==0 || pMugong->bWear==0)
   			continue;
   		MugongUpdateToDB(pMugong, "TOTALUPDATE");
   	}
   	
 	for(n=TP_JINBUB_START; n<TP_JINBUB_START+SLOT_JINBUB_NUM; ++n)
   	{
   		pMugong = pPlayer->GetMugongBase(n);
 		if(pMugong->dwDBIdx==0 || pMugong->bWear==0)
   			continue;
   		MugongUpdateToDB(pMugong, "TOTALUPDATE");
   	}

#else
	for(int n=TP_MUGONG1_START; n<TP_MUGONG2_END; ++n)
	{
		pMugong = pPlayer->GetMugongBase(n);
		if(pMugong == NULL || pMugong->dwDBIdx==0 || pMugong->bWear==0)
			continue;
		MugongUpdateToDB(pMugong, "TOTALUPDATE");
	}
	
	// magi82(2) - Titan(071022) Å¸ÀÌÅº ¹«°ø °æÇèÄ¡ DB ÀúÀåÇÏ´Â ºÎºÐ
	for(int n=TP_TITANMUGONG_START; n<TP_TITANMUGONG_END; ++n)
	{
		pMugong = pPlayer->GetMugongBase(n);
		if(pMugong == NULL || pMugong->dwDBIdx==0 || pMugong->bWear==0)
			continue;
		MugongUpdateToDB(pMugong, "TOTALUPDATE");
	}

	for(n=TP_JINBUB_START; n<TP_JINBUB_END; ++n)
	{
		pMugong = pPlayer->GetMugongBase(n);
		if(pMugong == NULL || pMugong->dwDBIdx==0 || pMugong->bWear==0)
			continue;
		MugongUpdateToDB(pMugong, "TOTALUPDATE");
	}
#endif

}

void CharacterTablePartyIdxUpdate(DWORD PlayerID) 
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d", STORED_PARTY_CHARACTERUPDATE, PlayerID);
		
	g_DB.Query(eQueryType_FreeQuery, ePartyCharacterTbUpdate, 0, txt);
}

void PartyAddMember(DWORD PartyIDX, DWORD TargetID)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d ", STORED_PARTY_ADDMEMBER, PartyIDX, TargetID);
	g_DB.Query(eQueryType_FreeQuery, ePartyAddMember, TargetID, txt);
}

void PartyDelMember(DWORD PartyIDX, DWORD TargetID)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d ", STORED_PARTY_DELMEMBER, PartyIDX, TargetID);
	g_DB.Query(eQueryType_FreeQuery, ePartyDelMember, PartyIDX, txt);
}

void PartyChangeMaster(DWORD PartyID, DWORD PlayerID, DWORD TargetPlayerID)
{
//	char txt[128];
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_PARTY_CHANGEMASTER, PartyID, PlayerID, TargetPlayerID);
	g_DB.Query(eQueryType_FreeQuery, ePartyChangeMaster, PartyID, txt);
}

void SavePointUpdate(DWORD CharacterIDX, WORD LoginPoint_Idx, WORD MapNum)
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %d",	STORED_SAVE_LOGINPOINT, CharacterIDX, LoginPoint_Idx, MapNum);
	g_DB.Query(eQueryType_FreeQuery, eSavePoint, 0, txt);
}

void SaveMapChangePointUpdate(DWORD CharacterIDX, WORD MapChangePoint_Idx)
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d",	STORED_SAVE_MAPCHAGEPOINT, CharacterIDX, MapChangePoint_Idx);
	g_DB.Query(eQueryType_FreeQuery, eSavePoint, 0, txt);
}
	 
void SaveMapChangePointReturn(DWORD CharacterIDX, WORD MapChangePoint_Idx, DWORD dwConnectionIdx, LEVELTYPE lvl, EXPTYPE ExpPoint, MONEYTYPE PyogukMoney)
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %u, %d, %u, %d, %I64d, %u",	STORED_SAVE_MAPCHAGEPOINTRET, CharacterIDX, MapChangePoint_Idx, dwConnectionIdx, lvl, ExpPoint, PyogukMoney);
	g_DB.Query(eQueryType_FreeQuery, eSaveMapChangePoint, CharacterIDX, txt);
}

/*
void MunpaBoardRankInfoUpdate(DWORD CharacterIDX, DWORD MunpaID, SEND_MUNPA_BOARD_RANK_INFO* msg)
{
//	char txt[384];
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d, %d, \'%s\'", STORED_MUNPA_BOARDINFOUPDATE, 
		CharacterIDX, MunpaID, msg->ID, msg->RSubjectRank, msg->RContentRank, 
		msg->WContentRank, msg->DContentRank, msg->MunpaBoardName);
	g_DB.Query(eQueryType_FreeQuery, eMunpaBoardRankInfoUpdate, msg->dwObjectID, txt);
}

void MunpaBoardSubjectList(DWORD PlayerID, DWORD BoardID, DWORD CurPage, DWORD PageUnit)
{
	g_DB.FreeMiddleQuery(RMunpaBoardSubjectList, PlayerID,
		"EXEC  %s %d, %d, %d, %d", 
		STORED_MUNPA_BOARDSUBJECTLIST, BoardID, CurPage, PageUnit, PlayerID);
}
*/

/*
void FameMunpaUpdate(DWORD StartMunpaIDX, DWORD MapNum)
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d",	STORED_FAME_MUNPAUPDATE, StartMunpaIDX, MapNum);
	g_DB.Query(eQueryType_FreeQuery, eFameMunpaUpdate, MapNum, txt);
}*/


void FameCharacterUpdate(DWORD PlayerIDX, FAMETYPE val)
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d",	STORED_FAME_CHARACTERUPDATE, PlayerIDX, val);
	g_DB.Query(eQueryType_FreeQuery, eFameCharacterUpdate, 0, txt);
}

void BadFameCharacterUpdate(DWORD PlayerIDX, FAMETYPE val)
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d",	STORED_BADFAME_CHARACTERUPDATE, PlayerIDX, val);
	g_DB.Query(eQueryType_FreeQuery, eBadFameCharacterUpdate, 0, txt);
}

void PKCharacterUpdate(DWORD PlayerIDX, DWORD val)
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d",	STORED_PK_CHARACTERUPDATE, PlayerIDX, val);
	g_DB.Query(eQueryType_FreeQuery, ePKCharacterUpdate, 0, txt);
}

void QuestTotalInfo(DWORD PlayerID)
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d",	"MP_QUEST_TotalInfo", PlayerID);
	g_DB.Query(eQueryType_FreeQuery, eQuestTotalInfo, PlayerID, txt);
}

void QuestUpdateToDB(DWORD PlayerID, DWORD QuestIdx, QSTATETYPE State, BYTE bEnd)
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %d, %d",	"MP_QUEST_Update", PlayerID, QuestIdx, State, bEnd);
	g_DB.Query(eQueryType_FreeQuery, eQuestUpdate, 0, txt);
}

// RaMa
void QuestMainQuestLoad(DWORD PlayerID, int QuestIdx )
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d", STORED_QUEST_MAINQUEST_LOAD, PlayerID, QuestIdx );
	g_DB.Query(eQueryType_FreeQuery, eQuestMainQuestLoad, PlayerID, txt);
}

void QuestMainQuestInsert(DWORD PlayerID, DWORD mQuestIdx, QSTATETYPE State, DWORD Time)
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %d, %d", STORED_QUEST_MAINQUEST_INSERT, PlayerID, mQuestIdx, State, Time);
	g_DB.Query(eQueryType_FreeQuery, eQuestMainQuestInsert, PlayerID, txt);
}

void QuestMainQuestDelete(DWORD PlayerID, DWORD mQuestIdx)
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d", STORED_QUEST_MAINQUEST_DELETE, PlayerID, mQuestIdx);
	g_DB.Query(eQueryType_FreeQuery, eQuestMainQuestDelete, PlayerID, txt);
}

void QuestMainQuestUpdateToDB(DWORD PlayerID, DWORD mQuestIdx, QSTATETYPE State, DWORD EndParam, DWORD Time)
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %d, %d, %d", STORED_QUEST_MAINQUEST_UPDATE, PlayerID, mQuestIdx, State, EndParam, Time);
	g_DB.Query(eQueryType_FreeQuery, eQuestMainQuestUpdate, PlayerID, txt);
}

void QuestSubQuestLoad(DWORD PlayerID, int QuestIdx )
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d", STORED_QUEST_SUBQUEST_LOAD, PlayerID, QuestIdx );
	g_DB.Query(eQueryType_FreeQuery, eQuestSubQuestLoad, PlayerID, txt);
}

void QuestItemload(DWORD PlayerID, int QuestIdx )
{
	//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d", STORED_QUEST_ITEM_LOAD, PlayerID, QuestIdx);
	g_DB.Query(eQueryType_FreeQuery, eQuestItemLoad, PlayerID, txt);
}

void QuestItemInsert(DWORD PlayerID, DWORD dwQuestIdx, DWORD ItemIdx, DWORD dwItemNum )
{
	//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %d, %d", STORED_QUEST_ITEM_INSERT, PlayerID, ItemIdx, dwItemNum, dwQuestIdx );
	g_DB.Query(eQueryType_FreeQuery, eQuestItemInsert, PlayerID, txt);
}

void QuestItemDelete(DWORD PlayerID, DWORD ItemIdx)
{
	//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d", STORED_QUEST_ITEM_DELETE, PlayerID, ItemIdx);
	g_DB.Query(eQueryType_FreeQuery, eQuestItemDelete, PlayerID, txt);
}

void QuestItemDeleteAll(DWORD PlayerID, DWORD dwQuestIdx)
{
	sprintf(txt, "EXEC  %s %d, %d", STORED_QUEST_ITEM_DELETE_ALL, PlayerID, dwQuestIdx);
	g_DB.Query(eQueryType_FreeQuery, eQuestItemDeleteAll, PlayerID, txt);
}

void QuestItemUpdate(DWORD PlayerID, DWORD dwQuestIdx, DWORD ItemIdx, DWORD ItemNum)
{
	//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %d, %d", STORED_QUEST_ITEM_UPDATE, PlayerID, ItemIdx, ItemNum, dwQuestIdx );
	g_DB.Query(eQueryType_FreeQuery, eQuestItemUpdate, PlayerID, txt);
}

void QuestSubQuestDelete(DWORD PlayerID, DWORD mQuestIdx, DWORD sQuestIdx)
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %d", STORED_QUEST_SUBQUEST_DELETE, PlayerID, mQuestIdx, sQuestIdx);
	g_DB.Query(eQueryType_FreeQuery, eQuestSubQuestDelete, PlayerID, txt);
}

void QuestSubQuestDeleteAll( DWORD PlayerID, DWORD mQuestIdx )
{
	sprintf(txt, "EXEC  %s %d, %d", STORED_QUEST_SUBQUEST_DELETE_ALL, PlayerID, mQuestIdx);
	g_DB.Query(eQueryType_FreeQuery, eQuestSubQuestDeleteAll, PlayerID, txt);
}

void QuestSubQuestInsert(DWORD PlayerID, DWORD mQuestIdx, DWORD sQuestIdx, QSTATETYPE Data, QSTATETYPE Time)
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %d, %d, %d", STORED_QUEST_SUBQUEST_INSERT, PlayerID, mQuestIdx, sQuestIdx, Data, Time);
	g_DB.Query(eQueryType_FreeQuery, eQuestSubQuestInsert, PlayerID, txt);
}

void QuestSubQuestUpdateToDB(DWORD PlayerID, DWORD mQuestIdx, DWORD sQuestIdx, QSTATETYPE Data, QSTATETYPE Time)
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %d, %d, %d", STORED_QUEST_SUBQUEST_UPDATE, PlayerID, mQuestIdx, sQuestIdx, Data, Time);
	g_DB.Query(eQueryType_FreeQuery, eQuestSubQuestUpdate, PlayerID, txt);
}

void QuestMainQuestUpdateCheckTime( DWORD dwCharIdx, DWORD dwQuestIdx, DWORD dwCheckType, DWORD dwCheckTime )
{
	sprintf(txt, "dbo.MP_MAINQUEST_UpdateCheckTime %d, %d, %d, %d", dwCharIdx, dwQuestIdx, dwCheckType, dwCheckTime);
	g_DB.Query(eQueryType_FreeQuery, eQuestMainQuestUpdateCheckTime, dwCharIdx, txt);
}

void Quest_EndQuest_New( DWORD PlayerID, DWORD mQuestIdx, QSTATETYPE State, DWORD EndParam, DWORD Time )
{
	sprintf(txt, "dbo.MP_QUEST_ENDQUEST_NEW %d, %d, %d, %d, %d", PlayerID, mQuestIdx, State, EndParam, Time);
	g_DB.Query(eQueryType_FreeQuery, eQuestMainQuestUpdateCheckTime, PlayerID, txt);
}

void Quest_DeleteQuest_New( DWORD PlayerID, DWORD mQuestIdx )
{
	sprintf(txt, "dbo.MP_QUEST_DELETEQUEST_NEW %d, %d", PlayerID, mQuestIdx);
	g_DB.Query(eQueryType_FreeQuery, eQuestMainQuestUpdateCheckTime, PlayerID, txt);
}

void Quest_EndSubQuest_New( DWORD PlayerID, DWORD mQuestIdx, DWORD sQuestIdx, QSTATETYPE Data, QSTATETYPE Time )
{
	sprintf(txt, "dbo.MP_QUEST_ENDSUBQUEST_NEW %d, %d, %d, %d, %d", PlayerID, mQuestIdx, sQuestIdx, Data, Time);
	g_DB.Query(eQueryType_FreeQuery, eQuestMainQuestUpdateCheckTime, PlayerID, txt);
}


//////////////////////////////////////////////////////////////////////////
// Log ¢®¨¡u¢®¢´A E¢®IAa CO¡§uo
/* ¢®¢´¨Ïi©ö¡Ëi¨ù¢®¡¿¡§u¢®(oA, ¡§o¡§¡þAE¡§¡þ?E¢®¨Ï¡§oA E¢®IAa */
void InsertLogCharacter( DWORD charIdx, LEVELTYPE level, HERO_TOTALINFO* pTotalInfo )
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %d, %d, %d, %d, %d",	"dbo.up_CharacterLog",
						charIdx, level,
						pTotalInfo->wGenGol, pTotalInfo->wMinChub,
						pTotalInfo->wCheRyuk, pTotalInfo->wSimMek,
						pTotalInfo->LevelUpPoint);
	g_DB.LogQuery(eQueryType_FreeQuery, eLogCharacter, 0, txt);
}

/* ¡§oA¢®¨¡¢®I¡Ë¡Íe¡§¡þ¢®¨¡ E¢®IAa */
void InsertLogExp( BYTE bType, DWORD charIdx, LEVELTYPE level, DWORD changeValue, EXPTYPE NowExp, WORD MurdererKind, DWORD MurdererIdx, DWORD CurAbilPoint)
{
	sprintf(txt, "EXEC  %s %d, %d, %d, %u, %I64d, %d, %d, %d",	"dbo.up_ExpPointLog", bType, charIdx, level, changeValue, NowExp, MurdererKind, MurdererIdx, CurAbilPoint);
	g_DB.LogQuery(eQueryType_FreeQuery, eLogExp, 0, txt);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void InsertLogMoney( BYTE type, DWORD charIdx, MONEYTYPE InvenMoney, MONEYTYPE ChangeValue, DWORD TargetID, MONEYTYPE TargetMoney )
{
//	sprintf(txt, "EXEC  %s %d, %d, %d, %d",	"up_MoneyLog", type, charIdx, InvenMoney, TargetMoney );
//	g_DB.Query(eQueryType_FreeQuery, eLogMoney, 0, txt);
	sprintf(txt, "EXEC  %s %d, %d, %u, %u, %d, %u",	"dbo.up_MoneyLog", type, charIdx, InvenMoney, ChangeValue, TargetID, TargetMoney );
	g_DB.LogQuery(eQueryType_FreeQuery, eLogMoney, 0, txt);
}

void InsertLogWorngMoney( BYTE type, DWORD charIdx, MONEYTYPE changeValue, MONEYTYPE InvenMoney, MONEYTYPE PyogukMoney, DWORD targetIdx )
{
	sprintf(txt, "EXEC  %s %d, %d, %u, %u, %u, %d",	"dbo.up_WrongMoneyLog", type, charIdx, changeValue, InvenMoney, PyogukMoney, targetIdx );
	g_DB.LogQuery(eQueryType_FreeQuery, eLogMoneyWrong, 0, txt);	
}

void LogItemMoney(DWORD FromChrIdx, char* FromChrName, DWORD ToChrIdx, char* ToChrName,
				WORD LogType, MONEYTYPE FromTotalMoney, MONEYTYPE ToTotalMoney, MONEYTYPE ChangeMoney,
				DWORD ItemIdx, DWORD ItemDBIdx, POSTYPE ItemFromPos, POSTYPE ItemToPos,
				DURTYPE ItemDur, EXPTYPE ExpPoint)
{
	sprintf(txt, "EXEC  %s %u, \'%s\', %u, \'%s\', %d, %u, %u, %u, %d, %d, %d, %d, %u, %I64d", "dbo.up_ItemMoneyLog", 
		FromChrIdx, FromChrName, ToChrIdx, ToChrName, 
		LogType, FromTotalMoney, ToTotalMoney, ChangeMoney,
		ItemIdx, ItemDBIdx, ItemFromPos, ItemToPos,
		ItemDur, ExpPoint);
	g_DB.LogQuery(eQueryType_FreeQuery, eLogItemMoney, 0, txt);
}

void LogCheat(DWORD Character_Idx, BYTE LogType, MONEYTYPE Param, DWORD ItemIdx, DWORD ItemDBIdx)
{
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d", "dbo.UP_CHEATE_Log",
		Character_Idx, LogType, Param, ItemIdx, ItemDBIdx);
	g_DB.LogQuery(eQueryType_FreeQuery, eLogItemMoney, 0, txt);
}

void LogItemOption(DWORD CharacterIdx, DWORD ItemDBIdx, ITEM_OPTION_INFO* pOptionInfo)
{
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d, %d, %d, %d, %d,  %d, %d, %d, %d, %d,  %d, %d,  %d, %d, %d, %d, %d,  %d", "dbo.UP_ItemOptionLog",
		CharacterIdx,
		pOptionInfo->dwOptionIdx,
		pOptionInfo->dwItemDBIdx,
		pOptionInfo->GenGol,					
		pOptionInfo->MinChub,				
		pOptionInfo->CheRyuk,				
		pOptionInfo->SimMek,				
		pOptionInfo->Life,					
		pOptionInfo->NaeRyuk,			
		pOptionInfo->Shield,				
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_FIRE)*100+0.00001),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_WATER)*100+0.00001),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_TREE)*100+0.00001),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_IRON)*100+0.00001),
		(int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_EARTH)*100+0.00001),
		pOptionInfo->PhyAttack,
		pOptionInfo->CriticalPercent,
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_FIRE)*100+0.00001),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_WATER)*100+0.00001),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_TREE)*100+0.00001),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_IRON)*100+0.00001),
		(int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_EARTH)*100+0.00001),
		pOptionInfo->PhyDefense);
			
	g_DB.LogQuery(eQueryType_FreeQuery, eLogItemReinforce, 0, txt);
}

void LogItemRareOption(DWORD CharacterIdx, DWORD ItemDBIdx, ITEM_RARE_OPTION_INFO* pRareOptionInfo)
{
	sprintf(txt, "EXEC dbo.UP_ItemRareOptionLog %d, %d, %d,  %d, %d, %d, %d,  %d, %d, %d,  %d, %d, %d, %d, %d,  %d,  %d, %d, %d, %d, %d,  %d",
		CharacterIdx,
		pRareOptionInfo->dwRareOptionIdx,
		pRareOptionInfo->dwItemDBIdx,
		pRareOptionInfo->GenGol,					
		pRareOptionInfo->MinChub,				
		pRareOptionInfo->CheRyuk,				
		pRareOptionInfo->SimMek,				
		pRareOptionInfo->Life,					
		pRareOptionInfo->NaeRyuk,			
		pRareOptionInfo->Shield,				
		(int)(pRareOptionInfo->AttrRegist.GetElement_Val(ATTR_FIRE)*100+0.00001),
		(int)(pRareOptionInfo->AttrRegist.GetElement_Val(ATTR_WATER)*100+0.00001),
		(int)(pRareOptionInfo->AttrRegist.GetElement_Val(ATTR_TREE)*100+0.00001),
		(int)(pRareOptionInfo->AttrRegist.GetElement_Val(ATTR_IRON)*100+0.00001),
		(int)(pRareOptionInfo->AttrRegist.GetElement_Val(ATTR_EARTH)*100+0.00001),
		pRareOptionInfo->PhyAttack,
//		pRareOptionInfo->CriticalPercent,
		(int)(pRareOptionInfo->AttrAttack.GetElement_Val(ATTR_FIRE)*100+0.00001),
		(int)(pRareOptionInfo->AttrAttack.GetElement_Val(ATTR_WATER)*100+0.00001),
		(int)(pRareOptionInfo->AttrAttack.GetElement_Val(ATTR_TREE)*100+0.00001),
		(int)(pRareOptionInfo->AttrAttack.GetElement_Val(ATTR_IRON)*100+0.00001),
		(int)(pRareOptionInfo->AttrAttack.GetElement_Val(ATTR_EARTH)*100+0.00001),
		pRareOptionInfo->PhyDefense);
			
	g_DB.LogQuery(eQueryType_FreeQuery, eLogItemRare, 0, txt);
}

void LogGuild(DWORD CharacterIdx, DWORD GuildIdx, WORD LogType, DWORD Param1, DWORD Param2)
{
	sprintf(txt, "EXEC  %s %d, %d, %d, %d, %d",	"dbo.up_GuildLog", CharacterIdx, GuildIdx, LogType, Param1, Param2);
	g_DB.LogQuery(eQueryType_FreeQuery, eLogGuild, 0, txt);
}

void LogPet(DWORD CharacterIdx, DWORD UserIdx, BYTE LogType, DWORD PetDBIdx, DWORD PetSummonItemDBIdx, DWORD PetGrade, DWORD PetStamina, DWORD PetFriendship, BYTE PetAlive)
{
	sprintf(txt, "EXEC dbo.Up_PetLog %d, %d, %d, %d, %d,  %d, %d, %d, %d",
		LogType,
		PetDBIdx,
		PetSummonItemDBIdx,
		UserIdx,
		CharacterIdx,

		PetGrade,
		PetFriendship,
		PetStamina,
		PetAlive
		);
	g_DB.LogQuery(eQueryType_FreeQuery, eLogPet, 0, txt);
}

void LogGMToolUse( DWORD CharacterIdx, DWORD GMLogtype, DWORD Logkind, DWORD Param1, DWORD Param2 )
{
	sprintf(txt, "EXEC dbo.Up_GMToolUseLog %d, %d, %d, %d, %d",
		CharacterIdx,
		GMLogtype,
		Logkind,
		Param1,
		Param2
		);
	g_DB.LogQuery( eQueryType_FreeQuery, eLogGMToolUse, 0, txt );
}

/* type : 0->Obtain, 1->Remove, 2->Change Sung */
void InsertLogMugong( WORD type, DWORD charIdx, DWORD mugongIdx, DWORD mugongDBIdx, LEVELTYPE mugongSung, EXPTYPE mugongExp )
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %d, %d, %d, %u",	"dbo.up_MugongLog", type, charIdx, mugongIdx, mugongDBIdx, mugongSung, mugongExp);
	g_DB.LogQuery(eQueryType_FreeQuery, eLogMugong, 0, txt);
}

void InsertLogMugongExp( DWORD charIdx, EXPTYPE changeValue, DWORD mugongIdx, DWORD mugongDBIdx, LEVELTYPE mugongSung, EXPTYPE mugongExp, POSTYPE mugongPos ) 
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %u, %d, %d, %d, %d, %d",	"dbo.up_MungongExpLog", 
		charIdx, changeValue, mugongIdx, mugongDBIdx, mugongSung, mugongExp, mugongPos);
	g_DB.LogQuery(eQueryType_FreeQuery, eLogMugongExp, 0, txt);
}

void UpdateCharacterInfoByTime(DWORD charIdx, EXPTYPE exp, DWORD abilityExp, MONEYTYPE money, DWORD time, DWORD LastPKEndTime )
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %I64d, %u, %u, %d, %d",	"dbo.MP_CHARACTER_UpdateInfoByTime", 
		charIdx, exp, abilityExp, money, time, LastPKEndTime );
	g_DB.Query(eQueryType_FreeQuery, eCharacterInfoByTime, 0, txt);
}

void UpdateMugongExpByTime(DWORD charIdx, DWORD DBIdx, EXPTYPE exp)
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, %d, %u",	"dbo.MP_MUGONG_UpdateExpBytime", 
		charIdx, DBIdx, exp);
	g_DB.Query(eQueryType_FreeQuery, eMugongExpByTime, 0, txt);
}

void InsertSpeedHackCheck(DWORD charIdx, char* charName, DWORD Distance, DWORD skillDelay)
{
//	char txt[128];
	sprintf(txt, "EXEC  %s %d, \'%s\', %d, %d",	"dbo.up_SpeedHackCheck", 
		charIdx, charName, Distance, skillDelay);
	g_DB.Query(eQueryType_FreeQuery, eSpeedHackCheck, 0, txt);
}

void InsertLogTool( DWORD dwLogType, DWORD dwLogKind, DWORD dwOperIdx, char* sOperName, DWORD dwTargetIdx, char* sTargetName,
				    DWORD dwParam1, DWORD dwParam2, DWORD dwParam3, DWORD dwParam4 )
{
	sprintf(txt, "EXEC dbo.Up_ToolLog %d, %d, %d, \'%s\', %d, \'%s\', %d, %d, %d, %d", dwLogType, dwLogKind, dwOperIdx, sOperName,
		dwTargetIdx, sTargetName, dwParam1, dwParam2, dwParam3, dwParam4 );	
	if(g_DB.LogQuery(eQueryType_FreeQuery, eInsertLogTool, 0, txt) == FALSE)
	{
	}
}

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////
// DB¢®¢´I¡§¡þIAI ¡Ë¡þ¡§¡©¡§oAAo¡Ë¡þ| ¨Ïi©ö¡Ëi¨ö¡§u¡§¢®¡§u¢®¨Ï A¨Ï©ª¡Ë¡þ¡ËcCI¡Ë¡ÍA CO¡§uo¡Íie.

//pjs
void  RAuctionCheck(LPQUERY pData,LPDBMESSAGE PMessage)
{
	if(atoi((char*)pData->Data[0]) == 0)
	{
				
	}
	else
	{		
		AUCTIONSUCCESS msg;

		memset(&msg,NULL,sizeof(msg));

		for(int i = 0; i < 10 ; i++)
		{
		
			strcpy(msg.resultlist[i].PRO_Name,((char*)pData[i].Data[sePRO_NAME]));
//			msg.resultlist[i].PRO_Name = ((char*)pData[i].Data[sePRO_NAME]);
			msg.resultlist[i].PRO_Amount = atoi((char*)pData[i].Data[sePRO_Amount]);
			strcpy(msg.resultlist[i].Auctioneer,((char*)pData[i].Data[seAuctioneer]));
//			msg.resultlist[i].Auctioneer = ((char*)pData[i].Data[seAuctioneer]);
			msg.resultlist[i].PRO_Index = atoi((char*)pData[i].Data[seREG_Index]);
		}
		g_Console.Log(eLogDisplay, 4, "AAA");
	}
}

//Select  PRO_Name, PRO_Amount, Auctioneer,REG_Index
//sePRO_NAME, sePRO_Amount, seAuctioneer,	seREG_Index


void RAuctionPageList(LPQUERY pData,LPDBMESSAGE PMessage)
{
	if(atoi((char*)pData->Data[0]) == 0)
	{
				
	}
	else
	{		
		BASELIST msg;

		memset(&msg,NULL,sizeof(msg));

		for(int i = 0; i < 10 ; i++)
		{
			msg.imfolist[i].REG_Index = atoi((char*)pData[i].Data[eREG_Index]);
			strcpy((char*)(msg.imfolist[i].BID_DUEDATE),((char*)pData[i].Data[eBID_DueDate]));
			strcpy(msg.imfolist[i].Auctioneer,((char*)pData[i].Data[eAuctioneer]));
			strcpy(msg.imfolist[i].BID_HighName,((char*)pData[i].Data[eHigh_Price_Name]));
			msg.imfolist[i].BID_HighPrice = atoi((char*)pData[i].Data[eBID_High_Price]);
			msg.imfolist[i].BID_ImmediatePrice = atoi((char*)pData[i].Data[eBID_Immediate_Price]);
			msg.imfolist[i].BID_LeftTime = atoi((char*)pData[i].Data[eBID_Left_Time]);
			msg.imfolist[i].JOIN_Amount = atoi((char*)pData[i].Data[eJOIN_Amount]);
			msg.imfolist[i].PRO_Amount = atoi((char*)pData[i].Data[ePRO_Amount]);
			msg.imfolist[i].PRO_Index = atoi((char*)pData[i].Data[ePRO_Index]);
			strcpy(msg.imfolist[i].PRO_Name,((char*)pData[i].Data[ePRO_NAME]));
		}
	}
}
/*
enum AuctionList
{
	ePRO_NAME, ePRO_Amount, eBID_High_Price, eBID_Immediate_Price, eBID_DueDate, eHigh_Price_Name, eJOIN_Amount, 
	eBID_Left_Time, eAuctioneer,ePRO_Index ,eBID_Starttime,eREG_Index, 
};
CEATE TABLE TB_IMSI(
	PRO_Name char(15), PRO_Amount  int, BID_HighPrice int, BID_ImmediatePrice int, BID_DueDate dateTime, BID_HighName char(15), JOIN_Amount int,
	BID_LeftTime int, Auctioneer char(15), PRO_Index int, BID_StartTime dateTime, REG_Index int)

*/
void RAuctionConfirm(LPQUERY pData, LPDBMESSAGE PMessage)
{

}

void RTestIDPWQuery(LPQUERY pData, LPDBMESSAGE pMessage)
{
	g_Console.Log(eLogDisplay,0 , "ID/PW Result : UserID -- %d", pData->Data[0]);
}

void RCharacterNumSendAndCharacterInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	if(!count)
	{
		// A¨Ï©ª¡Ë¡þ?AI d¡§¡þ¡Ë¡þ¨Ïi©ö¡Ëi¨ö¢®¨úa ¡§oC¡§¢®¡§¡Ë¡Ë?7u
		ASSERTMSG(0,"DB¡Ë?¡Ë¢ç AE¡Ë¡þ?AI ¡Íi¢®IAIA¡Ë¡þ¢®¨¡¡Ë¢ç ¡§u¨Ï¨£¡§o4I¡Ë¡ÍU.");
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
	
	// UserIDX¡Ë¡ÍA ¨Ï©ªN¢®¨úaAo ¡§uE¡Ë¡ÍA¡Ë¡ÍU
	Heroinfo.PartyID = atoi((char*)pData->Data[eCS_PartyID]);
	
	Totalinfo.Gender = atoi((char*)pData->Data[eCS_Gender]);
	Heroinfo.wGenGol = atoi((char*)pData->Data[eCS_Gengoal]);
	Totalinfo.Life = atoi((char*)pData->Data[eCS_Life]);
	Totalinfo.Shield = atoi((char*)pData->Data[eCS_Shield]);
	Totalinfo.MaxLife = Totalinfo.Life;
	Totalinfo.MaxShield = Totalinfo.Shield;
	Heroinfo.wNaeRyuk = atoi((char*)pData->Data[eCS_Energe]);
	Heroinfo.wMinChub = atoi((char*)pData->Data[eCS_Dex]);
	Heroinfo.wCheRyuk = atoi((char*)pData->Data[eCS_Sta]);
	Heroinfo.wSimMek = atoi((char*)pData->Data[eCS_Simmak]);
	Heroinfo.Fame = atoi((char*)pData->Data[eCS_Fame]);
	Heroinfo.wKarma = atoi((char*)pData->Data[eCS_Karma]);
	SafeStrCpy( Objinfo.ObjectName, (char*)pData->Data[eCS_Name], MAX_NAME_LENGTH + 1 );
	Totalinfo.Stage = atoi((char*)pData->Data[eCS_Stage]);
	Totalinfo.Level = atoi((char*)pData->Data[eCS_Grade]);
	Heroinfo.ExpPoint = (EXPTYPE)_atoi64((char*)pData->Data[eCS_Expoint]);
	Heroinfo.LevelUpPoint = atoi((char*)pData->Data[eCS_GradeUpPoint]);
	Heroinfo.Money = atoi((char*)pData->Data[eCS_Money]);
	Totalinfo.CurMapNum = GAMERESRCMNGR->GetLoadMapNum();
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
	Totalinfo.PositionInMunpa = atoi((char*)pData->Data[eCS_GuildPositon]);
	SafeStrCpy(Totalinfo.GuildName, (char*)pData->Data[eCS_GuildName], MAX_GUILD_NAME+1);
	SafeStrCpy(Totalinfo.NickName, (char*)pData->Data[eCS_NickName], MAX_GUILD_NICKNAME+1);

	Totalinfo.BadFame = atoi((char*)pData->Data[eCS_BadFame]);
	
	Totalinfo.MapChangePoint_Index = atoi((char*)pData->Data[eCS_MapChangePoint]);
	Totalinfo.LoginPoint_Index = atoi((char*)pData->Data[eCS_LoginPoint]);
	
	Heroinfo.KyungGongGrade = atoi((char*)pData->Data[eCS_QuickPace]);
	
	Heroinfo.AbilityExp = atoi((char*)pData->Data[eCS_AbilityExp]);
	
	Totalinfo.Height = (float)atof((char*)pData->Data[eCS_Height]);
	Totalinfo.Width = (float)atof((char*)pData->Data[eCS_Width]);
	Heroinfo.Playtime = atoi((char*)pData->Data[eCS_Playtime]);
	Heroinfo.LastPKModeEndTime = atoi((char*)pData->Data[eCS_LastPKEndTime]);

	Heroinfo.MaxLevel = atoi((char*)pData->Data[eCS_MaxLevel]);
	Totalinfo.MarkName	= atoi((char*)pData->Data[eCS_MarkName]);
	SafeStrCpy(Heroinfo.MunpaCanEntryDate, (char*)pData->Data[eCS_MunpaCanEntryDate], 11);

	if(Heroinfo.MaxLevel < Totalinfo.Level)
		Heroinfo.MaxLevel = Totalinfo.Level;

	// magi82 //////////////////////////////////////////////////////////////////////////

	// ±¸Á¶Ã¼¿¡ ÇÃ·¡±×°ª Áý¾î³Ö¾î¾ßÇÔ(flag : LevelUp±¸°£È­ °ü·Ã)
	Heroinfo.ExpFlag = (BYTE)(atoi((char*)pData->Data[eCS_ExpFlag]));
	////////////////////////////////////////////////////////////////////////////////////

	// magi82 Levelup ±¸°£È­ ¼öÁ¤ (±¸°£È­·Î ÀÎÇÑ ±âÁ¸ Ä³¸¯¿¡°Ô °øÂ¥ Æ÷ÀÎÆ®¸¦ ¾ÈÁÖ°Ô ¸·´Â ÀÛ¾÷) ///////

	// LevelUp ±¸°£È­ °ü·Ã ÇÃ·¡±×°¡ ÇÑ¹øµµ Àû¿ëµÇÁö ¾Ê¾ÒÀ»¶§
	if(!CheckBit(Heroinfo.ExpFlag, 4))
	{
		// Ã³À½ Á¢¼ÓÇßÀ»¶§ ·¹º§ ´Ù¿îµÈ »óÅÂ¸é ¸ðµç ÇÃ·¡±× TRUE·Î ¹Ù²ãÁØ´Ù.
		if(Totalinfo.Level < Heroinfo.MaxLevel)
		{
			Heroinfo.ExpFlag = 0x0F;
		}
		else	// ==
		{
			EXPTYPE maxexp = GAMERESRCMNGR->GetMaxExpPoint( Totalinfo.Level );
			int count = (int)((((float)Heroinfo.ExpPoint / (float)maxexp)*100) / 20);

			for( int i = 0; i < count; i++ )
			{
				Heroinfo.LevelUpPoint++;	// magi82(27)
				SetOnBit(&Heroinfo.ExpFlag, i);
			}
		}

		SetOnBit(&Heroinfo.ExpFlag, 4);	// ÇÑ¹ø Àû¿ë µÇ¾úÀ¸¹Ç·Î ´ÙÀ½ºÎÅÏ Àû¿ë ¾ÈÇÔ
		UCharacterExpFlag(Objinfo.dwObjectID, Heroinfo.ExpFlag);	// ÀÛ¾÷ÈÄ DB°»½Å
	}

	////////////////////////////////////////////////////////////////////////////////////

	CPlayer* pPlayer = g_pServerSystem->InitPlayerInfo(&Objinfo, &Totalinfo, &Heroinfo);
	if(pPlayer == NULL)
		return;

	//pPlayer->GetShopItemStats()->dwSkinItemIndex = (DWORD)(atoi((char*)pData->Data[eCS_SkinInfo]));

#ifdef _JAPAN_LOCAL_
	DWORD dwAttr = (DWORD)atoi((char*)pData->Data[eCS_NickName+1]);
	Totalinfo.nMainAttr = (int)LOWORD(dwAttr);
	Totalinfo.nSubAttr = (int)HIWORD(dwAttr);
#endif
	
	VECTOR3 pos,RandPos;
	GAMERESRCMNGR->GetRegenPoint(pos,RandPos,Totalinfo.LoginPoint_Index,Totalinfo.MapChangePoint_Index);	

	pPlayer->SetLoginTime(Heroinfo.Playtime);
	
	DWORD Position = pPlayer->GetInitPoint();
	if( Position )
	{
		WORD x, z;
		stMOVEPOINT Mp;
		Mp.value = Position;
		Mp.GetMovePoint( x, z );
		pPlayer->SetInitPoint( 0 );
		RandPos.x = x;
		RandPos.z = z;

		CCharMove::SetPosition(pPlayer, &RandPos);
	}

	CCharMove::InitMove(pPlayer,&RandPos);

	// guildunion info setting
	GUILDUNIONMGR->SetGuildUnionInfoToCharacterTotalInfo( pPlayer, Totalinfo.MunpaID );

	pPlayer->SetInitState(PLAYERINITSTATE_HERO_INFO,pMessage->dwID);
   	CharacterPetInfo(Objinfo.dwObjectID, Objinfo.dwUserID);

	//SW070127 Å¸ÀÌÅº
	CharacterTitanInfo(Objinfo.dwObjectID, Objinfo.dwUserID);

#ifdef _JAPAN_LOCAL_
	CharacterItemSlotInfo( Objinfo.dwObjectID );
#elif defined _HK_LOCAL_
	CharacterItemSlotInfo( Objinfo.dwObjectID );
#elif defined _TL_LOCAL_
	CharacterItemSlotInfo( Objinfo.dwObjectID );
#else
	CharacterItemInfo( Objinfo.dwObjectID, MP_USERCONN_GAMEIN_SYN );
#endif

	// autonotelist
	AutoNoteListLoad( Objinfo.dwObjectID );
}

void RCharacterInfoQueryToEventMap(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD dwTeam = pMessage->dwID;
	DWORD count = pMessage->dwResult;
	if( !count )	return;
	
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
	
	Heroinfo.PartyID = atoi((char*)pData->Data[eCS_PartyID]);
	Totalinfo.Gender = atoi((char*)pData->Data[eCS_Gender]);
	Heroinfo.wGenGol = atoi((char*)pData->Data[eCS_Gengoal]);
	Totalinfo.Life = atoi((char*)pData->Data[eCS_Life]);
	Totalinfo.Shield = atoi((char*)pData->Data[eCS_Shield]);
	Totalinfo.MaxLife = Totalinfo.Life;
	Totalinfo.MaxShield = Totalinfo.Shield;
	Heroinfo.wNaeRyuk = atoi((char*)pData->Data[eCS_Energe]);
	Heroinfo.wMinChub = atoi((char*)pData->Data[eCS_Dex]);
	Heroinfo.wCheRyuk = atoi((char*)pData->Data[eCS_Sta]);
	Heroinfo.wSimMek = atoi((char*)pData->Data[eCS_Simmak]);
//	Heroinfo.Fame = atoi((char*)pData->Data[eCS_Fame]);
	Heroinfo.Fame = 0; //now, fame not used... but, it's occur some bugs.
	Heroinfo.wKarma = atoi((char*)pData->Data[eCS_Karma]);
	SafeStrCpy( Objinfo.ObjectName, (char*)pData->Data[eCS_Name], MAX_NAME_LENGTH + 1 );
	Totalinfo.Stage = atoi((char*)pData->Data[eCS_Stage]);
	Totalinfo.Level = atoi((char*)pData->Data[eCS_Grade]);
	Heroinfo.ExpPoint = (EXPTYPE)_atoi64((char*)pData->Data[eCS_Expoint]);
	Heroinfo.LevelUpPoint = atoi((char*)pData->Data[eCS_GradeUpPoint]);
	Heroinfo.Money = atoi((char*)pData->Data[eCS_Money]);
	Totalinfo.CurMapNum = GAMERESRCMNGR->GetLoadMapNum();
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
	Totalinfo.PositionInMunpa = atoi((char*)pData->Data[eCS_GuildPositon]);
	SafeStrCpy(Totalinfo.GuildName, (char*)pData->Data[eCS_GuildName], MAX_GUILD_NAME+1);
	SafeStrCpy(Totalinfo.NickName, (char*)pData->Data[eCS_NickName], MAX_GUILD_NICKNAME+1);

	Totalinfo.BadFame = atoi((char*)pData->Data[eCS_BadFame]);
	
	Totalinfo.MapChangePoint_Index = atoi((char*)pData->Data[eCS_MapChangePoint]);
	Totalinfo.LoginPoint_Index = atoi((char*)pData->Data[eCS_LoginPoint]);
	
	Heroinfo.KyungGongGrade = atoi((char*)pData->Data[eCS_QuickPace]);
	
	Heroinfo.AbilityExp = atoi((char*)pData->Data[eCS_AbilityExp]);
	
	Totalinfo.Height = (float)atof((char*)pData->Data[eCS_Height]);
	Totalinfo.Width = (float)atof((char*)pData->Data[eCS_Width]);
	Heroinfo.Playtime = atoi((char*)pData->Data[eCS_Playtime]);
	Heroinfo.LastPKModeEndTime = atoi((char*)pData->Data[eCS_LastPKEndTime]);

	Heroinfo.MaxLevel = atoi((char*)pData->Data[eCS_MaxLevel]);
	Totalinfo.MarkName	= atoi((char*)pData->Data[eCS_MarkName]);
	SafeStrCpy(Heroinfo.MunpaCanEntryDate, (char*)pData->Data[eCS_MunpaCanEntryDate], 11);
		
	if(Heroinfo.MaxLevel < Totalinfo.Level)
		Heroinfo.MaxLevel = Totalinfo.Level;

#ifdef _JAPAN_LOCAL_
	DWORD dwAttr = (DWORD)atoi((char*)pData->Data[eCS_NickName+1]);
	Totalinfo.nMainAttr = (int)LOWORD(dwAttr);
	Totalinfo.nSubAttr = (int)HIWORD(dwAttr);
#endif

	VECTOR3 pos,RandPos;
	GAMERESRCMNGR->GetRegenPointFromEventMap(pos,RandPos,Totalinfo.LoginPoint_Index,Totalinfo.MapChangePoint_Index,dwTeam);

	CPlayer* pPlayer = g_pServerSystem->InitPlayerInfo(&Objinfo, &Totalinfo, &Heroinfo);
	if( pPlayer == NULL )	return;

	pPlayer->SetLoginTime(Heroinfo.Playtime);

	CCharMove::InitMove(pPlayer,&RandPos);
	
	pPlayer->SetInitState( PLAYERINITSTATE_HERO_INFO, MP_USERCONN_GAMEIN_SYN );
}

void	RCharacterMugongInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	
	DWORD count = pMessage->dwResult;
//	if(count > MAX_TOTALMUGONG_NUM)
	if(count > SLOT_MUGONGTOTAL_NUM)
	{
		// ¡Íi¢®IAIAI¢®¨¡¡Ë¢ç AE¢®¨¡u¡Íi¡§¢®¡Ë¡ÍU.
	}
	else if((count == 1) && (atoi((char*)pData->Data[0]) == 0))
	{
		// ¡Íi¢®IAIAI¢®¨¡¡Ë¢ç ¡§u¨Ï¨£¡§ui¡§u¢®¨Ï PlayerID¡Ë¡þ| ¨ÏoYE?CO ¢®¨¡¨Ï¢®¡Ë?i¢®¨¡E¢®ic
		CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(atoi((char*)pData->Data[1]));
		if(pPlayer == NULL)
			return;

		MUGONG_TOTALINFO Mugonginfo;
		memset(&Mugonginfo, 0, sizeof(MUGONG_TOTALINFO));
		pPlayer->InitMugongTotalInfo(&Mugonginfo);
		pPlayer->SetInitState(PLAYERINITSTATE_MUGONG_INFO,pMessage->dwID);
	}
	else
	{
		CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser((DWORD)atoi((char*)pData[0].Data[eCM_ObjectID]));
		if(pPlayer == NULL)
			return;

		MUGONG_TOTALINFO Mugonginfo;
		const int maxoverlap = 100;
		int overlapcount = 0;
		MUGONGBASE OverLapMugong[maxoverlap];
		memset(&OverLapMugong, 0, sizeof(MUGONGBASE)*maxoverlap);
		memset(&Mugonginfo, 0, sizeof(MUGONG_TOTALINFO));
//		QUICKITEM quickItem;

		for(DWORD i = 0; i < count; i++)
		{
			WORD Mugongidx = (WORD)atoi((char*)pData[i].Data[eCM_IDX]);
			
			POSTYPE MugongPos = ConvAbsPos2MugongPos(atoi((char*)pData[i].Data[eCM_Position]));
			MUGONGBASE* pMugongBase = &Mugonginfo.mugong[MugongPos];

			//¹«°ø Æ÷Áö¼ÇÀÌ °ãÃÆ´Ù.
			if(pMugongBase->dwDBIdx != 0 && overlapcount < maxoverlap)
			{
				// magi82 - Titan(070611) Å¸ÀÌÅº ¹«°øº¯È¯ ÁÖ¼®Ã³¸®
				// Å¸ÀÌÅº ¹«°øÀÎµ¦½º´Â Ä³¸¯ÅÍ¹«°øÀÎµ¦½º+10000 ÀÌ´Ù
				//if(Mugongidx == pMugongBase->wIconIdx+10000 )
				//{
				//	pMugongBase = &Mugonginfo.Titanmugong[MugongPos];
				//}
				//else
				{
					pMugongBase = &OverLapMugong[overlapcount];
					++overlapcount;
				}
			}

			pMugongBase->wIconIdx = Mugongidx;//atoi((char*)pData[i].Data[eCM_IDX]);
			pMugongBase->dwDBIdx = atoi((char*)pData[i].Data[eCM_DBIDX]);
			pMugongBase->ExpPoint = atoi((char*)pData[i].Data[eCM_ExpPoint]);
			pMugongBase->Sung = atoi((char*)pData[i].Data[eCM_Sung]);
			pMugongBase->Position = atoi((char*)pData[i].Data[eCM_Position]);
			pMugongBase->QuickPosition = atoi((char*)pData[i].Data[eCM_QPosition]);
			pMugongBase->bWear = FALSE;

			//////////////////////////////////////////////////////////////////////////
			// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
			// ¹«°ø º¯È¯ Ãß°¡
			pMugongBase->OptionIndex = atoi((char*)pData[i].Data[eCM_Option]);
			//////////////////////////////////////////////////////////////////////////
		}
		
		//Áßº¹µÈ ¹«°øÀÌ ÀÖÀ¸¸é ºóÄ­¿¡ ³Ö¾îÁØ´Ù.
		for(int n=0;n<overlapcount;++n)
		{
			for(int m=0;m<SLOT_MUGONG_NUM;++m)
			{
				//ºóÄ­ÀÌ¸é ³Ö¾îÁØ´Ù.
				if(Mugonginfo.mugong[m].dwDBIdx == 0)
				{
					Mugonginfo.mugong[m] = OverLapMugong[n];
				#ifdef _JAPAN_LOCAL_
					Mugonginfo.mugong[m].Position = TP_MUGONG_START + m;
				#elif defined _HK_LOCAL_
					Mugonginfo.mugong[m].Position = TP_MUGONG_START + m;
				#elif defined _TL_LOCAL_
					Mugonginfo.mugong[m].Position = TP_MUGONG_START + m;
				#else
					Mugonginfo.mugong[m].Position = TP_MUGONG1_START + m;
				#endif
					break;
				}
			}

			//¸¸¾à ºóÄ­ÀÌ ¾øÀ¸¸é ´õÀÌ»ó Ã³¸®ÇÏÁö ¾ÊÀ½
			if(m==SLOT_MUGONG_NUM)
				break;
		}

		// Player¡Ë?¡Ë¢ç ¨Ïo¢®i¢®¨¡¨Ï¨£d¡§¡þ¡Ë¡þ A¨Ï¡þ¢®¨¡¡Ë¢ç
		pPlayer->InitMugongTotalInfo(&Mugonginfo);
		pPlayer->SetInitState(PLAYERINITSTATE_MUGONG_INFO,pMessage->dwID);
	}
}


void RCharacterItemSlotInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pMessage->dwID );
	if( !pPlayer )			return;

#ifdef _JAPAN_LOCAL_
	DWORD InvenSlot = (atoi((char*)pData->Data[0]));
	DWORD PyogukSlot = (atoi((char*)pData->Data[1]));
	DWORD MugongSlot = (atoi((char*)pData->Data[2]));
	DWORD CharacterSlot = (atoi((char*)pData->Data[3]));

	pPlayer->SetExtraSlotCount( PyogukSlot, InvenSlot, MugongSlot );
	pPlayer->SetExtraCharacterSlot( CharacterSlot );
#endif

#ifdef _HK_LOCAL_
	DWORD InvenSlot 	= (atoi((char*)pData->Data[0]));
	DWORD PyogukSlot 	= (atoi((char*)pData->Data[1]));
	DWORD MugongSlot 	= (atoi((char*)pData->Data[2]));
	DWORD CharacterSlot = (atoi((char*)pData->Data[3]));

	pPlayer->SetExtraSlotCount( PyogukSlot, InvenSlot, MugongSlot );
	pPlayer->SetExtraCharacterSlot( CharacterSlot );
#endif

#ifdef _TL_LOCAL_
	DWORD InvenSlot 	= (atoi((char*)pData->Data[0]));
	DWORD PyogukSlot 	= (atoi((char*)pData->Data[1]));
	DWORD MugongSlot 	= (atoi((char*)pData->Data[2]));
	DWORD CharacterSlot = (atoi((char*)pData->Data[3]));

	pPlayer->SetExtraSlotCount( PyogukSlot, InvenSlot, MugongSlot );
	pPlayer->SetExtraCharacterSlot( CharacterSlot );
#endif

	CharacterItemInfo( pPlayer->GetID(), MP_USERCONN_GAMEIN_SYN );
}


void	RCharacterItemInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer;
	DWORD count = pMessage->dwResult;
	if((count == 1) && (atoi((char*)pData->Data[0]) == 0))
	{
		// ¡Íi¢®IAIAI¢®¨¡¡Ë¢ç ¡§u¨Ï¨£¡§ui¡§u¢®¨Ï PlayerID¡Ë¡þ| ¨ÏoYE?CO ¢®¨¡¨Ï¢®¡Ë?i¢®¨¡E¢®ic
		pPlayer = (CPlayer *)g_pUserTable->FindUser(atoi((char*)pData->Data[1]));
		if(pPlayer == NULL)
			return;

		ITEM_TOTALINFO Iteminfo;
		memset(&Iteminfo, 0, sizeof(ITEM_TOTALINFO));
		pPlayer->InitItemTotalInfo(&Iteminfo);
		ShopItemInvenInfo( pPlayer->GetID() );
	}
	else
	{
		pPlayer = (CPlayer *)g_pUserTable->FindUser((DWORD)atoi((char*)pData[0].Data[eCI_ObjectID]));
		if(pPlayer == NULL)
			return;

		ITEM_TOTALINFO Iteminfo;
		const int maxoverlap = 100;
		int overlapcount = 0;
		ITEMBASE OverLapItem[maxoverlap];
		memset(&OverLapItem, 0, sizeof(ITEMBASE)*maxoverlap);
		memset(&Iteminfo, 0, sizeof(ITEM_TOTALINFO));
		for(DWORD  i = 0; i < count; i++)
		{
			POSTYPE ItemPos = atoi((char*)pData[i].Data[eCI_Position]);
			ITEMBASE* pItemBase = NULL;
			// ¢®¨ú¢®¢¯¡Ë¡þ¡Ëc¡Íia(AI¡§¡þ¢®IAa¡Ë¡þ¡Ëc)¡§u¡§¢®AIAU ¡§uA¡§¢®A
			if(ItemPos >= TP_INVENTORY_START && ItemPos < TP_INVENTORY_END)
			{
				ItemPos -= TP_INVENTORY_START;
				pItemBase = &Iteminfo.Inventory[ItemPos];
			}
			// AaAÂ¨ÃÂ¨Â£Â¡Â§uÂ¡Â§Â¢Â®AIAU Â¡Â§uAÂ¡Â§Â¢Â®A
			else if(ItemPos >= TP_WEAR_START && ItemPos < TP_WEAR_END)
			{	
				POSTYPE ItemGrid = ItemPos - TP_WEAR_START;

				pItemBase = &Iteminfo.WearedItem[ItemGrid];

				if( pItemBase )
				{
					pItemBase->ItemParam = atoi((char*)pData[i].Data[eCI_Param]);
					if( pItemBase->ItemParam & ITEM_PARAM_SEAL )
					{
						ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pItemBase->wIconIdx );

						if( pItemInfo )
						{
							if( pItemInfo->ItemKind & eSHOP_ITEM )
							{
								pItemBase->dwDBIdx = atoi((char*)pData[i].Data[eCI_DBIDX]);
								// DB¿¡ Æ÷Áö¼ÇÀ» ¾÷µ¥ÀÌÆ®ÇØÁØ´Ù.
								ItemMoveUpdateToDB( pPlayer->GetID(), 0, 0, pItemBase->dwDBIdx, 240 );
								continue;
							}
							else
							{
								pItemBase->ItemParam &= ~ITEM_PARAM_SEAL;
							}
						}
					}
				}
			}

			if(pItemBase == NULL)
			{
				// ¡Ë?¡§I¢®¨úa¡Ë?¡Ë¢ç ¡Íie¡§ui¡Ë?8e ¡§u¡§¢®AIAU ¡§¢®¢®AAo¡§uC ¡Ë?¡Ë¢ç¢®¢´?AO
				ASSERT(0);
				continue;
			}
			
			//¾ÆÀÌÅÛ Æ÷Áö¼ÇÀÌ °ãÃÆ´Ù.
			if(pItemBase->dwDBIdx != 0 && overlapcount < maxoverlap)
			{
				pItemBase = &OverLapItem[overlapcount];
				++overlapcount;
			}

			pItemBase->dwDBIdx = atoi((char*)pData[i].Data[eCI_DBIDX]);
			pItemBase->wIconIdx = atoi((char*)pData[i].Data[eCI_IDX]);
			pItemBase->Position = atoi((char*)pData[i].Data[eCI_Position]);
			pItemBase->Durability = atoi((char*)pData[i].Data[eCI_Durability]);
			pItemBase->RareIdx = atoi((char*)pData[i].Data[eCI_RareIdx]);
			pItemBase->QuickPosition = atoi((char*)pData[i].Data[eCI_QPosition]);
			pItemBase->ItemParam = atoi((char*)pData[i].Data[eCI_Param]);

			// magi82(33) Ä³¸¯ÅÍ ÀÎº¥¿¡ ÀÖ´Â Å¸ÀÌÅº ÀåÂø¾ÆÀÌÅÛÀÇ ¿¹¿ÜÃ³¸®
			ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pItemBase->wIconIdx);
			if( pInfo && pInfo->ItemKind & eTITAN_EQUIPITEM )
			{
				TITAN_ENDURANCE_ITEMINFO* pEndurance = pPlayer->GetTitanManager()->GetTitanItemEnduranceInfo(pItemBase->dwDBIdx);
				if( !pEndurance )
				{
					pPlayer->GetTitanManager()->MakeNewEnduranceInfo(pPlayer, pItemBase, eExceptionInven);				
				}
			}
		}

		//Áßº¹µÈ ¾ÆÀÌÅÛÀÌ ÀÖÀ¸¸é ºóÄ­¿¡ ³Ö¾îÁØ´Ù.
		for(int n=0;n<overlapcount;++n)
		{
			for(int m=TP_INVENTORY_START;m<TP_INVENTORY_END;++m)
			{
				//ºóÄ­ÀÌ¸é ³Ö¾îÁØ´Ù.
				if(Iteminfo.Inventory[m].dwDBIdx == 0)
				{
					Iteminfo.Inventory[m] = OverLapItem[n];
					Iteminfo.Inventory[m].Position = m;
					break;
				}
			}

			//¸¸¾à ºóÄ­ÀÌ ¾øÀ¸¸é ´õÀÌ»ó Ã³¸®ÇÏÁö ¾ÊÀ½
			if(m==TP_INVENTORY_END)
				break;
		}
		
		// Player¡Ë?¡Ë¢ç ¡§u¡§¢®AIAU d¡§¡þ¡Ë¡þA¨Ï¡þ¢®¨¡¡Ë¢ç
		pPlayer->InitItemTotalInfo(&Iteminfo);
		ShopItemInvenInfo( pPlayer->GetID() );

		// ¨Ï©ªeaA¢®E¡Ë?¡Ë¢ç ¨Ï©ª¨Ï¡À¡§u¡§¢® AO¡Ë¡ÍA ¡§u¡§¢®AIAU; AI¡§¡þ¢®IAa¡Ë¡þ¡Ëc¢®¢´I ¡Ë?A¢®¨úa¡Ë¡ÍU???
		// ¡Ë?i¡§u¢®¨ú AO¡§u¡Ëc A¨Ï©ª¡Ë¡þ¡Ëc : taiyo 
		///ITEMMGR_OBJ->QuitStreetStall(pPlayer,FALSE);		
	}
}

void RPyogukItemRareOptionInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	// C¢®I¢®¨ú¨Ïo A¢®E¢®¨¡i
	CPlayer* pPlayer = NULL;
	pPlayer = (CPlayer *)g_pUserTable->FindUser(pMessage->dwID);
	if(pPlayer == NULL)
		return;

	ITEM_RARE_OPTION_INFO RareOptionInfo;
	for( DWORD  i = 0 ; i < pMessage->dwResult ; ++i )
	{
		RareOptionInfo.dwRareOptionIdx			= (DWORD)atoi((char*)pData[i].Data[ePROI_RareOptionID]);
		RareOptionInfo.dwItemDBIdx			= (DWORD)atoi((char*)pData[i].Data[ePROI_ItemDBID]);
		RareOptionInfo.GenGol				= (WORD)atoi((char*)pData[i].Data[ePROI_GenGol]);
		RareOptionInfo.MinChub				= (WORD)atoi((char*)pData[i].Data[ePROI_MinChub]);				
		RareOptionInfo.CheRyuk				= (WORD)atoi((char*)pData[i].Data[ePROI_CheRyuk]);				
		RareOptionInfo.SimMek				= (WORD)atoi((char*)pData[i].Data[ePROI_SimMek]);					
		RareOptionInfo.Life					= (DWORD)atoi((char*)pData[i].Data[ePROI_Life]);					
		RareOptionInfo.NaeRyuk				= (WORD)atoi((char*)pData[i].Data[ePROI_NaeRyuk]);				
		RareOptionInfo.Shield				= (DWORD)atoi((char*)pData[i].Data[ePROI_Shield]);					
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[ePROI_FireAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[ePROI_WaterAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[ePROI_TreeAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[ePROI_GoldAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[ePROI_EarthAttrRegist])/100.f );
		RareOptionInfo.PhyAttack			= (WORD)atoi((char*)pData[i].Data[ePROI_PhyAttack]);
//		RareOptionInfo.CriticalPercent		= (WORD)atoi((char*)pData[i].Data[ePROI_Critical]);
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[ePROI_FireAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[ePROI_WaterAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[ePROI_TreeAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[ePROI_GoldAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[ePROI_EarthAttrAttack])/100.f );
		RareOptionInfo.PhyDefense			= (WORD)atoi((char*)pData[i].Data[ePROI_PhyDefense]);				

		pPlayer->AddItemRareOption(&RareOptionInfo);
	}

	if(pMessage->dwResult >= MAX_PYOGUKLOAD_ITEM) 
		PyogukItemRareOptionInfo(pMessage->dwID, pPlayer->GetUserID(), RareOptionInfo.dwItemDBIdx);
	else
		PyogukTitanEnduranceInfo(pMessage->dwID, pPlayer->GetUserID(), 0);
		//CharacterPyogukItemInfo(pMessage->dwID, 0);
}

void RPyogukItemOptionInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	// C¢®I¢®¨ú¨Ïo A¢®E¢®¨¡i
	CPlayer* pPlayer = NULL;
	pPlayer = (CPlayer *)g_pUserTable->FindUser(pMessage->dwID);
	if(pPlayer == NULL)
		return;

	ITEM_OPTION_INFO OptionInfo;
	for( DWORD  i = 0 ; i < pMessage->dwResult ; ++i )
	{
		OptionInfo.dwOptionIdx			= (DWORD)atoi((char*)pData[i].Data[ePOI_OptionID]);
		OptionInfo.dwItemDBIdx			= (DWORD)atoi((char*)pData[i].Data[ePOI_ItemDBID]);
		OptionInfo.GenGol				= (WORD)atoi((char*)pData[i].Data[ePOI_GenGol]);
		OptionInfo.MinChub				= (WORD)atoi((char*)pData[i].Data[ePOI_MinChub]);				
		OptionInfo.CheRyuk				= (WORD)atoi((char*)pData[i].Data[ePOI_CheRyuk]);				
		OptionInfo.SimMek				= (WORD)atoi((char*)pData[i].Data[ePOI_SimMek]);					
		OptionInfo.Life					= (DWORD)atoi((char*)pData[i].Data[ePOI_Life]);					
		OptionInfo.NaeRyuk				= (WORD)atoi((char*)pData[i].Data[ePOI_NaeRyuk]);				
		OptionInfo.Shield				= (DWORD)atoi((char*)pData[i].Data[ePOI_Shield]);					
		OptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[ePOI_FireAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[ePOI_WaterAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[ePOI_TreeAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[ePOI_GoldAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[ePOI_EarthAttrRegist])/100.f );
		OptionInfo.PhyAttack			= (WORD)atoi((char*)pData[i].Data[ePOI_PhyAttack]);
		OptionInfo.CriticalPercent		= (WORD)atoi((char*)pData[i].Data[ePOI_Critical]);
		OptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[ePOI_FireAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[ePOI_WaterAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[ePOI_TreeAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[ePOI_GoldAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[ePOI_EarthAttrAttack])/100.f );
		OptionInfo.PhyDefense			= (WORD)atoi((char*)pData[i].Data[ePOI_PhyDefense]);				

		pPlayer->AddItemOption(&OptionInfo);
	}

	if(pMessage->dwResult >= MAX_PYOGUKLOAD_ITEM)
	{
		PyogukItemOptionInfo(pMessage->dwID, pPlayer->GetUserID(), OptionInfo.dwItemDBIdx);
	}
	else
	{
		PyogukItemRareOptionInfo(pPlayer->GetID(), pPlayer->GetUserID(), 0);
	}
	//SW051007 PyogukItemRareOptionInfo ·Î ¿Å±è.
//	else
//		CharacterPyogukItemInfo(pMessage->dwID, 0);
}


void	RCharacterItemRareOptionInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if(pMessage->dwResult == 0) return;

	CPlayer* pPlayer = NULL;
	pPlayer = (CPlayer*)g_pUserTable->FindUser(atoi((char*)pData->Data[eCIRO_ObjectID]));
	if(pPlayer == NULL)
		return;

	if(pMessage->dwResult == 1 && atoi((char*)pData->Data[eCIRO_RareOptionID]) == 0)
	{
		pPlayer->SetInitState(PLAYERINITSTATE_ITEM_RARE_OPTION_INFO, pMessage->dwID);
	}
	else
	{
		ASSERTMSG(pMessage->dwResult <= 90, "Item Rare Option > 90 Error" );

		ITEM_RARE_OPTION_INFO RareOptionInfo;
		for( DWORD i = 0; i < pMessage->dwResult; ++ i )
		{
			RareOptionInfo.dwRareOptionIdx			= (DWORD)atoi((char*)pData[i].Data[eCIRO_RareOptionID]);
			RareOptionInfo.dwItemDBIdx			= (DWORD)atoi((char*)pData[i].Data[eCIRO_ItemDBID]);
			RareOptionInfo.GenGol				= (WORD)atoi((char*)pData[i].Data[eCIRO_GenGol]);
			RareOptionInfo.MinChub				= (WORD)atoi((char*)pData[i].Data[eCIRO_MinChub]);				
			RareOptionInfo.CheRyuk				= (WORD)atoi((char*)pData[i].Data[eCIRO_CheRyuk]);				
			RareOptionInfo.SimMek				= (WORD)atoi((char*)pData[i].Data[eCIRO_SimMek]);					
			RareOptionInfo.Life					= (DWORD)atoi((char*)pData[i].Data[eCIRO_Life]);					
			RareOptionInfo.NaeRyuk				= (WORD)atoi((char*)pData[i].Data[eCIRO_NaeRyuk]);				
			RareOptionInfo.Shield				= (DWORD)atoi((char*)pData[i].Data[eCIRO_Shield]);					
			RareOptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[eCIRO_FireAttrRegist])/100.f );
			RareOptionInfo.AttrRegist.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[eCIRO_WaterAttrRegist])/100.f );
			RareOptionInfo.AttrRegist.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[eCIRO_TreeAttrRegist])/100.f );
			RareOptionInfo.AttrRegist.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[eCIRO_GoldAttrRegist])/100.f );
			RareOptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[eCIRO_EarthAttrRegist])/100.f );
			RareOptionInfo.PhyAttack			= (WORD)atoi((char*)pData[i].Data[eCIRO_PhyAttack]);
//			RareOptionInfo.CriticalPercent		= (WORD)atoi((char*)pData[i].Data[eCIOI_Critical]);
			RareOptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[eCIRO_FireAttrAttack])/100.f );
			RareOptionInfo.AttrAttack.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[eCIRO_WaterAttrAttack])/100.f );
			RareOptionInfo.AttrAttack.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[eCIRO_TreeAttrAttack])/100.f );
			RareOptionInfo.AttrAttack.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[eCIRO_GoldAttrAttack])/100.f );
			RareOptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[eCIRO_EarthAttrAttack])/100.f );
			RareOptionInfo.PhyDefense			= (WORD)atoi((char*)pData[i].Data[eCIRO_PhyDefense]);				

			pPlayer->AddItemRareOption(&RareOptionInfo);
		}
		pPlayer->SetInitState(PLAYERINITSTATE_ITEM_RARE_OPTION_INFO,pMessage->dwID);

	}
}

void	RCharacterItemOptionInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if(pMessage->dwResult == 0) return;

	CPlayer* pPlayer = NULL;
	pPlayer = (CPlayer *)g_pUserTable->FindUser(atoi((char*)pData->Data[eCIOI_ObjectID]));
	if(pPlayer == NULL)
		return;

	// AI¡§¡þ¢®IAa¡Ë¡þ¡Ëc
	if(pMessage->dwResult == 1 && atoi((char*)pData->Data[eCIOI_OptionID]) == 0)
	{
		pPlayer->SetInitState(PLAYERINITSTATE_ITEM_OPTION_INFO,pMessage->dwID);
	}
	else
	{
		ASSERTMSG(pMessage->dwResult <= 90, "Item Option > 90 Error : ¨ÏoI¡Ë?i; ¡§¡þO¢®¢´?¡§¡þ¡Ë¡þ¡§u¡§¢®¡Ë?a~!");

		ITEM_OPTION_INFO OptionInfo;
		for( DWORD  i = 0 ; i < pMessage->dwResult ; ++i )
		{
			OptionInfo.dwOptionIdx			= (DWORD)atoi((char*)pData[i].Data[eCIOI_OptionID]);
			OptionInfo.dwItemDBIdx			= (DWORD)atoi((char*)pData[i].Data[eCIOI_ItemDBID]);
			OptionInfo.GenGol				= (WORD)atoi((char*)pData[i].Data[eCIOI_GenGol]);
			OptionInfo.MinChub				= (WORD)atoi((char*)pData[i].Data[eCIOI_MinChub]);				
			OptionInfo.CheRyuk				= (WORD)atoi((char*)pData[i].Data[eCIOI_CheRyuk]);				
			OptionInfo.SimMek				= (WORD)atoi((char*)pData[i].Data[eCIOI_SimMek]);					
			OptionInfo.Life					= (DWORD)atoi((char*)pData[i].Data[eCIOI_Life]);					
			OptionInfo.NaeRyuk				= (WORD)atoi((char*)pData[i].Data[eCIOI_NaeRyuk]);				
			OptionInfo.Shield				= (DWORD)atoi((char*)pData[i].Data[eCIOI_Shield]);					
			OptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[eCIOI_FireAttrRegist])/100.f );
			OptionInfo.AttrRegist.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[eCIOI_WaterAttrRegist])/100.f );
			OptionInfo.AttrRegist.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[eCIOI_TreeAttrRegist])/100.f );
			OptionInfo.AttrRegist.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[eCIOI_GoldAttrRegist])/100.f );
			OptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[eCIOI_EarthAttrRegist])/100.f );
			OptionInfo.PhyAttack			= (WORD)atoi((char*)pData[i].Data[eCIOI_PhyAttack]);
			OptionInfo.CriticalPercent		= (WORD)atoi((char*)pData[i].Data[eCIOI_Critical]);
			OptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[eCIOI_FireAttrAttack])/100.f );
			OptionInfo.AttrAttack.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[eCIOI_WaterAttrAttack])/100.f );
			OptionInfo.AttrAttack.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[eCIOI_TreeAttrAttack])/100.f );
			OptionInfo.AttrAttack.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[eCIOI_GoldAttrAttack])/100.f );
			OptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[eCIOI_EarthAttrAttack])/100.f );
			OptionInfo.PhyDefense			= (WORD)atoi((char*)pData[i].Data[eCIOI_PhyDefense]);				

			pPlayer->AddItemOption(&OptionInfo);
		}

		pPlayer->SetInitState(PLAYERINITSTATE_ITEM_OPTION_INFO,pMessage->dwID);
	}

	/*
	if(count == 1 && atoi((char*)pData->Data[1]) == 0)
	{
		// ¡Íi¢®IAIAI¢®¨¡¡Ë¢ç ¡§u¨Ï¨£¡§ui¡§u¢®¨Ï PlayerID¡Ë¡þ| ¨ÏoYE?CO ¢®¨¡¨Ï¢®¡Ë?i¢®¨¡E¢®ic
		pPlayer = (CPlayer *)g_pUserTable->FindUser(atoi((char*)pData->Data[eCIOI_ObjectID]));
		if(pPlayer == NULL)
			return;
		pPlayer->SetInitState(PLAYERINITSTATE_ITEM_OPTION_INFO,pMessage->dwID);
	}
	else
	{
		ASSERTMSG(count <= 90, "Item Option > 90 Error : ¨ÏoI¡Ë?i; ¡§¡þO¢®¢´?¡§¡þ¡Ë¡þ¡§u¡§¢®¡Ë?a~!");

		pPlayer = (CPlayer *)g_pUserTable->FindUser((DWORD)atoi((char*)pData[0].Data[eCIOI_ObjectID]));
		if(pPlayer == NULL)
			return;

		ITEM_OPTION_INFO OptionInfo;
		for(DWORD  i = 0; i < count; ++i)
		{
			OptionInfo.dwOptionIdx			= (DWORD)atoi((char*)pData[i].Data[eCIOI_OptionID]);
			OptionInfo.dwItemDBIdx			= (DWORD)atoi((char*)pData[i].Data[eCIOI_ItemDBID]);
			OptionInfo.GenGol				= (WORD)atoi((char*)pData[i].Data[eCIOI_GenGol]);
			OptionInfo.MinChub				= (WORD)atoi((char*)pData[i].Data[eCIOI_MinChub]);				
			OptionInfo.CheRyuk				= (WORD)atoi((char*)pData[i].Data[eCIOI_CheRyuk]);				
			OptionInfo.SimMek				= (WORD)atoi((char*)pData[i].Data[eCIOI_SimMek]);					
			OptionInfo.Life					= (DWORD)atoi((char*)pData[i].Data[eCIOI_Life]);					
			OptionInfo.NaeRyuk				= (WORD)atoi((char*)pData[i].Data[eCIOI_NaeRyuk]);				
			OptionInfo.Shield				= (DWORD)atoi((char*)pData[i].Data[eCIOI_Shield]);					
			OptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE,		atof((char*)pData[i].Data[eCIOI_FireAttrRegist]) );
			OptionInfo.AttrRegist.SetElement_Val(ATTR_WATER,	atof((char*)pData[i].Data[eCIOI_WaterAttrRegist]) );
			OptionInfo.AttrRegist.SetElement_Val(ATTR_TREE,		atof((char*)pData[i].Data[eCIOI_TreeAttrRegist]) );
			OptionInfo.AttrRegist.SetElement_Val(ATTR_IRON,		atof((char*)pData[i].Data[eCIOI_GoldAttrRegist]) );
			OptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH,	atof((char*)pData[i].Data[eCIOI_EarthAttrRegist]) );
			OptionInfo.PhyAttack			= (WORD)atoi((char*)pData[i].Data[eCIOI_PhyAttack]);
			OptionInfo.CriticalPercent		= (WORD)atoi((char*)pData[i].Data[eCIOI_Critical]);
			OptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE,		atof((char*)pData[i].Data[eCIOI_FireAttrAttack]) );
			OptionInfo.AttrAttack.SetElement_Val(ATTR_WATER,	atof((char*)pData[i].Data[eCIOI_WaterAttrAttack]) );
			OptionInfo.AttrAttack.SetElement_Val(ATTR_TREE,		atof((char*)pData[i].Data[eCIOI_TreeAttrAttack]) );
			OptionInfo.AttrAttack.SetElement_Val(ATTR_IRON,		atof((char*)pData[i].Data[eCIOI_GoldAttrAttack]) );
			OptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH,	atof((char*)pData[i].Data[eCIOI_EarthAttrAttack]) );
			OptionInfo.PhyDefense			= (WORD)atoi((char*)pData[i].Data[eCIOI_PhyDefense]);				

			pPlayer->AddItemOption(&OptionInfo);
		}

		pPlayer->SetInitState(PLAYERINITSTATE_ITEM_OPTION_INFO,pMessage->dwID);
	}*/
}

void RCharacterAbilityInfo(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage)
{
	ASSERT(pMessage->dwResult == 1);
	DWORD CharacterIdx = atoi((char*)pData[0].Data[0]);
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(CharacterIdx);
	if(pPlayer==NULL)
		return;
	int datacount = 0;

	ABILITY_TOTALINFO TInfo;
	for(int kind = 0;kind<eAGK_Max;++kind)
	{
		memcpy(TInfo.AbilityDataArray[kind],(char*)pData[0].Data[kind+1],MAX_ABILITY_NUM_PER_GROUP+1);
		memcpy(TInfo.AbilityQuickPositionArray[kind],(char*)pData[0].Data[eAGK_Max+kind+1],MAX_ABILITY_NUM_PER_GROUP+1);
	}

	ABILITYMGR->InitAbility(pPlayer,&TInfo,pPlayer->GetAbilityGroup());

	pPlayer->SetInitState(PLAYERINITSTATE_ABILITY_INFO,pMessage->dwID);
}

void PetInsertToDB( DWORD CharacterIdx, DWORD UserIdx, DWORD dwItemIdx, PET_TOTALINFO* pPetTotalInfo )
{
	sprintf(txt, "EXEC %s %d,%d,%d,%d,%d, %d,%d,%d,%d,%d, %d", "dbo.MP_PET_Insert",
		CharacterIdx,
		UserIdx,
		dwItemIdx,
		//dwItemDBIdx,
		//pPetTotalInfo->PetMasterUserID,
		pPetTotalInfo->PetKind,
		pPetTotalInfo->PetGrade,
		pPetTotalInfo->PetStamina,

		pPetTotalInfo->PetFriendly,
		pPetTotalInfo->PetSummonItemDBIdx,
		pPetTotalInfo->bAlive,
		pPetTotalInfo->bSummonning,
		pPetTotalInfo->bRest
		);

	g_DB.Query(eQueryType_FreeQuery, ePetInsert, pPetTotalInfo->PetSummonItemDBIdx, txt);
}

void RPetInsert(LPQUERY pData, LPDBMESSAGE pMessage)
{
	//static DWORD dwPetObjectID = PET_ID_START;

	//WORD lastNo = LOWORD(pMessage->dwID);
	//WORD ArrayID = HIWORD(pMessage->dwID);

	//ÇÃ·¹ÀÌ¾î°¡ ÀÖ´ÂÁö
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(atoi((char*)pData->Data[ePII_MasterObjectID]));
	if( NULL == pPlayer )
		return;
	if( FALSE == pPlayer->GetInited() )
		return;

	//!!!¼ÒÈ¯¾ÆÀÌÅÛÀÌ ÀÖ´ÂÁö
	//ePTI_PetSummonItemIdx

	PET_TOTALINFO PetInfo;
	if( pMessage->dwResult )
	{
		PetInfo.PetMasterUserID = atoi((char*)pData->Data[ePII_UserIdx]);
		PetInfo.PetDBIdx	= atoi((char*)pData->Data[ePII_PetDBIdx]);
		PetInfo.PetSummonItemDBIdx = atoi((char*)pData->Data[ePII_PetSummonItemDBIdx]);
		PetInfo.PetKind = atoi((char*)pData->Data[ePII_PetKind]);
		PetInfo.PetGrade = atoi((char*)pData->Data[ePII_PetGrade]);
		PetInfo.PetStamina = atoi((char*)pData->Data[ePII_PetStamina]);
		PetInfo.PetFriendly = atoi((char*)pData->Data[ePII_PetFriendship]);
		PetInfo.bAlive = atoi((char*)pData->Data[ePII_Alive]);
		PetInfo.bSummonning = atoi((char*)pData->Data[ePII_Summonning]);
		PetInfo.bRest = atoi((char*)pData->Data[ePII_Rest]);
		//dwPetObjectID = PetInfo.PetDBIdx + PET_ID_START;
	}
	else
	{
		ASSERT(0);
	}

	pPlayer->GetPetManager()->AddPetTotalInfo(&PetInfo,eServerNClient);
	//Æê »ý¼º°ú Á¤º¸ ÃÊ±âÈ­
	/*
	CPet* pPet = g_pServerSystem->AddPet(pPlayer, dwPetObjectID, &PetInfo);
	if(pPet)
	{
		pPlayer->GetPetManager()->AddPet(pPet,PetInfo.PetSummonItemDBIdx);
	}*/
}

void PetDeleteToDB( DWORD dwPetDBIdx )
{
	sprintf(txt, "EXEC dbo.MP_PET_Delete %d", dwPetDBIdx);
	g_DB.Query(eQueryType_FreeQuery, ePetDelete, 0, txt);
}

void PetUpdateToDB( DWORD UserIdx, const PET_TOTALINFO* pPetTotalInfo )
{
	//°»½ÅµÈ Æê Á¤º¸ DB ÀúÀå.
	sprintf(txt, "EXEC dbo.MP_PET_Update %d, %d, %d, %d, %d,  %d, %d, %d",
		UserIdx,
		//dwItemDBIdx,
		//pPetTotalInfo->PetMasterID,
		pPetTotalInfo->PetDBIdx,
		//pPetTotalInfo->PetKind,
		pPetTotalInfo->PetGrade,
		pPetTotalInfo->PetStamina,
		pPetTotalInfo->PetFriendly,
		//pPetTotalInfo->PetSummonItemID,
		pPetTotalInfo->bAlive,
		pPetTotalInfo->bSummonning,
		pPetTotalInfo->bRest
		);

	g_DB.Query(eQueryType_FreeQuery, ePetUpdate, 0, txt);
}

void RCharacterPetInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	//if(pMessage->dwResult == 0) return;

	CPlayer* pPlayer = NULL;
	//pPlayer = (CPlayer *)g_pUserTable->FindUser(atoi((char*)pData->Data[ePTI_MasterObjectID]));
	pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(pPlayer == NULL)
		return;

	//!!!¼ÒÈ¯¾ÆÀÌÅÛÀÌ ÀÖ´ÂÁö	//DB·ÎºÎÅÍ ¾ÆÀÌÅÛ Á¤º¸°¡ ¸ÕÀú ´Ù µµÂøÇÑ´Ù´Â º¸ÀåÀÌ ¾ø´Ù.
	//ePTI_PetSummonItemIdx

	PET_TOTALINFO PetInfo;
	//CPet* pPet = NULL;
	DWORD dwPetObjectID = 0;

	for( DWORD  i = 0 ; i < pMessage->dwResult ; ++i )
	{
		PetInfo.PetMasterUserID			=	atoi((char*)pData[i].Data[ePTI_UserID]);
		PetInfo.PetDBIdx			=	atoi((char*)pData[i].Data[ePTI_PetDBIdx]);	
		PetInfo.PetSummonItemDBIdx	=	atoi((char*)pData[i].Data[ePTI_PetSummonItemDBIdx]);
		PetInfo.PetKind				=	atoi((char*)pData[i].Data[ePTI_PetKind]);
		PetInfo.PetGrade			=	atoi((char*)pData[i].Data[ePTI_PetGrade]);
		PetInfo.PetStamina			=	atoi((char*)pData[i].Data[ePTI_PetStamina]);
		PetInfo.PetFriendly			=	atoi((char*)pData[i].Data[ePTI_PetFriendship]);
		PetInfo.bAlive				=	atoi((char*)pData[i].Data[ePTI_Alive]);
		PetInfo.bSummonning			=	atoi((char*)pData[i].Data[ePTI_Summonning]);
		PetInfo.bRest				=	atoi((char*)pData[i].Data[ePTI_Rest]);

		pPlayer->GetPetManager()->AddPetTotalInfo(&PetInfo);

	}
	pPlayer->SetInitState(PLAYERINITSTATE_PET_INFO,MP_USERCONN_GAMEIN_SYN);

}

void RPetInvenItemOptionInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = NULL;
	pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(pPlayer == NULL)
		return;

	ITEM_OPTION_INFO OptionInfo;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		OptionInfo.dwOptionIdx			= (DWORD)atoi((char*)pData[i].Data[ePIOI_OptionID]);
		if(0 == OptionInfo.dwOptionIdx)		continue;
		OptionInfo.dwItemDBIdx			= (DWORD)atoi((char*)pData[i].Data[ePIOI_ItemDBID]);
		OptionInfo.GenGol				= (WORD)atoi((char*)pData[i].Data[ePIOI_GenGol]);
		OptionInfo.MinChub				= (WORD)atoi((char*)pData[i].Data[ePIOI_MinChub]);				
		OptionInfo.CheRyuk				= (WORD)atoi((char*)pData[i].Data[ePIOI_CheRyuk]);				
		OptionInfo.SimMek				= (WORD)atoi((char*)pData[i].Data[ePIOI_SimMek]);					
		OptionInfo.Life					= (DWORD)atoi((char*)pData[i].Data[ePIOI_Life]);					
		OptionInfo.NaeRyuk				= (WORD)atoi((char*)pData[i].Data[ePIOI_NaeRyuk]);				
		OptionInfo.Shield				= (DWORD)atoi((char*)pData[i].Data[ePIOI_Shield]);					
		OptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[ePIOI_FireAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[ePIOI_WaterAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[ePIOI_TreeAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[ePIOI_GoldAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[ePIOI_EarthAttrRegist])/100.f );
		OptionInfo.PhyAttack			= (WORD)atoi((char*)pData[i].Data[ePIOI_PhyAttack]);
		OptionInfo.CriticalPercent		= (WORD)atoi((char*)pData[i].Data[ePIOI_Critical]);
		OptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[ePIOI_FireAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[ePIOI_WaterAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[ePIOI_TreeAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[ePIOI_GoldAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[ePIOI_EarthAttrAttack])/100.f );
		OptionInfo.PhyDefense			= (WORD)atoi((char*)pData[i].Data[ePIOI_PhyDefense]);				

		pPlayer->AddItemOption(&OptionInfo);
	}

	if(pMessage->dwResult >= MAX_PETINVENLOAD_ITEM)
	{
		PetInvenItemOptionInfo(pMessage->dwID, pPlayer->GetUserID(), TP_PETINVEN_START, TP_PETINVEN_END);
	}
	else
	{
		PetInvenItemRareOptionInfo(pMessage->dwID, pPlayer->GetUserID(), TP_PETINVEN_START, TP_PETINVEN_END);
	}
}

void RPetInvenItemRareOptionInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = NULL;
	pPlayer = (CPlayer *)g_pUserTable->FindUser(pMessage->dwID);
	if(pPlayer == NULL)
		return;

	ITEM_RARE_OPTION_INFO RareOptionInfo;
	for( DWORD  i = 0 ; i < pMessage->dwResult ; ++i )
	{
		RareOptionInfo.dwRareOptionIdx			= (DWORD)atoi((char*)pData[i].Data[ePIROI_RareOptionID]);
		if(0 == RareOptionInfo.dwRareOptionIdx)	continue;
		RareOptionInfo.dwItemDBIdx			= (DWORD)atoi((char*)pData[i].Data[ePIROI_ItemDBID]);
		RareOptionInfo.GenGol				= (WORD)atoi((char*)pData[i].Data[ePIROI_GenGol]);
		RareOptionInfo.MinChub				= (WORD)atoi((char*)pData[i].Data[ePIROI_MinChub]);				
		RareOptionInfo.CheRyuk				= (WORD)atoi((char*)pData[i].Data[ePIROI_CheRyuk]);				
		RareOptionInfo.SimMek				= (WORD)atoi((char*)pData[i].Data[ePIROI_SimMek]);					
		RareOptionInfo.Life					= (DWORD)atoi((char*)pData[i].Data[ePIROI_Life]);					
		RareOptionInfo.NaeRyuk				= (WORD)atoi((char*)pData[i].Data[ePIROI_NaeRyuk]);				
		RareOptionInfo.Shield				= (DWORD)atoi((char*)pData[i].Data[ePIROI_Shield]);					
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[ePIROI_FireAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[ePIROI_WaterAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[ePIROI_TreeAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[ePIROI_GoldAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[ePIROI_EarthAttrRegist])/100.f );
		RareOptionInfo.PhyAttack			= (WORD)atoi((char*)pData[i].Data[ePIROI_PhyAttack]);
		//		RareOptionInfo.CriticalPercent		= (WORD)atoi((char*)pData[i].Data[ePIROI_Critical]);
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[ePIROI_FireAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[ePIROI_WaterAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[ePIROI_TreeAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[ePIROI_GoldAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[ePIROI_EarthAttrAttack])/100.f );
		RareOptionInfo.PhyDefense			= (WORD)atoi((char*)pData[i].Data[ePIROI_PhyDefense]);				

		pPlayer->AddItemRareOption(&RareOptionInfo);
	}

	if(pMessage->dwResult >= MAX_PETINVENLOAD_ITEM)
	{
		PetInvenItemRareOptionInfo(pMessage->dwID, pPlayer->GetUserID(), TP_PETINVEN_START, TP_PETINVEN_END);
	}
	else
	{
		PetInvenItemInfo(pMessage->dwID, TP_PETINVEN_START, TP_PETINVEN_END);
	}
}

void RPetInvenItemInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(!pPlayer) return;
	
	if(!pPlayer->GetInited())	return;

	PETINVEN_ITEM PetInvenItem;
	memset(&PetInvenItem, 0, sizeof(PETINVEN_ITEM));
	//	ePIII_ObjectID = 0, ePIII_DBIDX, ePIII_IDX, ePIII_Position, ePIII_QPosition, ePIII_Durability, ePIII_MunpaIdx, ePIII_PyogukIdx, ePIII_ShopIdx, ePIII_Param, ePIII_RareIdx,

	POSTYPE ItemPos = 0;
	if(atoi((char*)pData[0].Data[ePIII_DBIDX]) != 0)
	{
		for( DWORD i = 0; i < pMessage->dwResult; ++i )
		{
			ItemPos = atoi((char*)pData[i].Data[ePIII_Position]);
			if(TP_PETINVEN_START <= ItemPos && ItemPos < TP_PETINVEN_END)
			{
				ItemPos -= TP_PETINVEN_START;
				PetInvenItem.PetInven[ItemPos].dwDBIdx = atoi((char*)pData[i].Data[ePIII_DBIDX]);
				PetInvenItem.PetInven[ItemPos].wIconIdx = atoi((char*)pData[i].Data[ePIII_IDX]);
				PetInvenItem.PetInven[ItemPos].Position = atoi((char*)pData[i].Data[ePIII_Position]);
				//PetInvenItem.PetInven[ItemPos].QuickPosition = 0	//(±âÈ¹))ÆêÀÎº¥ Äü¼³Á¤¾ÈµÊ
				PetInvenItem.PetInven[ItemPos].Durability	= atoi((char*)pData[i].Data[ePIII_Durability]);
				PetInvenItem.PetInven[ItemPos].RareIdx	= atoi((char*)pData[i].Data[ePIII_RareIdx]);
			}
			else
			{
				ASSERT(0);
			}
		}
	}
	pPlayer->InitPetInvenItemInfo(&PetInvenItem);

	if( pMessage->dwResult >= MAX_PETINVENLOAD_ITEM )
	{
		//PetInvenItemInfo(pMessage->dwID, atoi((char*)pData[pMessage->dwResult-1].Data[ePIII_DBIdx]));
		//PetInvenItemInfo(pMessage->dwID, TP_PETINVEN_START, TP_PETINVEN_END);
		PetInvenItemInfo(pMessage->dwID, ItemPos+TP_PETINVEN_START, TP_PETINVEN_END);
	}
	else
	{
		pPlayer->SendPetInvenItemInfo();
	}
}

void RPetWearItemInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer )	return;

	ITEMBASE ItemBase[SLOT_PETWEAR_NUM];
	//ITEMBASE ItemOverlap[SLOT_PETWEAR_NUM];
	memset( ItemBase, 0, sizeof(ITEMBASE)*SLOT_PETWEAR_NUM );
	//memset( ItemOverlap, 0, sizeof(ITEMBASE)*SLOT_PETWEAR_NUM );
	//int OverlapCount=0;

	int ret = atoi((char*)pData[0].Data[0]);
	if( ret == 0 )
	{
		pPlayer->InitPetWearItemInfo( ItemBase );
		// magi82 - Titan(070209)
		//pPlayer->SetInitState(PLAYERINITSTATE_ITEM_INFO,MP_USERCONN_GAMEIN_SYN);
		TitanWearItemInfo(pPlayer->GetID(), TP_TITANWEAR_START, TP_TITANSHOPITEM_END);
		return;
	}

	if( pMessage->dwResult > SLOT_PETWEAR_NUM )
	{
		char Buf[64];
		sprintf(Buf, "PetWearItem OverNum CharacterID:%d", pPlayer->GetID());
		ASSERTMSG(0, Buf);
	}

	for(DWORD i = 0; i < pMessage->dwResult; i++)
	{
		POSTYPE ItemPos = atoi((char*)pData[i].Data[eCI_Position]);
		ITEMBASE* pItemBase = NULL;

		if(ItemPos >= TP_PETWEAR_START && ItemPos < TP_PETWEAR_END)
		{
			ItemPos -= TP_PETWEAR_START;

			pItemBase = &ItemBase[ItemPos];

			/*
			if( pItemBase->dwDBIdx )
			{
				pItemBase = &ItemOverlap[OverlapCount];
				++OverlapCount;
			}*/

			pItemBase->dwDBIdx = atoi((char*)pData[i].Data[eCI_DBIDX]);
			pItemBase->wIconIdx = atoi((char*)pData[i].Data[eCI_IDX]);
			pItemBase->Position = atoi((char*)pData[i].Data[eCI_Position]);
			pItemBase->Durability = atoi((char*)pData[i].Data[eCI_Durability]);
			pItemBase->QuickPosition = atoi((char*)pData[i].Data[eCI_QPosition]);
			pItemBase->ItemParam = atoi((char*)pData[i].Data[eCI_Param]);

			pPlayer->GetPetManager()->SetPetEquipOption(pItemBase->wIconIdx, TRUE);
		}
	}

	/*
	//Áßº¹µÈ ¾ÆÀÌÅÛÀÌ ÀÖÀ¸¸é ºóÄ­¿¡ ³Ö¾îÁØ´Ù.
	for(int n=0; n<OverlapCount; n++)
	{
		for(int m=0; m<SLOT_PETWEAR_NUM; m++)
		{
			//ºóÄ­ÀÌ¸é ³Ö¾îÁØ´Ù.
			if( ItemBase[m].dwDBIdx == 0)
			{
				ItemBase[m] = ItemOverlap[n];
				ItemBase[m].Position = m;
				break;
			}
		}

		//¸¸¾à ºóÄ­ÀÌ ¾øÀ¸¸é ´õÀÌ»ó Ã³¸®ÇÏÁö ¾ÊÀ½
		if( m==SLOT_PETWEAR_NUM )
			break;
	}*/

	pPlayer->InitPetWearItemInfo( ItemBase );
	// magi82 - Titan(070209)
	//pPlayer->SetInitState(PLAYERINITSTATE_ITEM_INFO,MP_USERCONN_GAMEIN_SYN);
	TitanWearItemInfo(pPlayer->GetID(), TP_TITANWEAR_START, TP_TITANSHOPITEM_END);
}

//SW070127 Å¸ÀÌÅº
void TitanInsertToDB(DWORD UserIdx, DWORD CharacterIdx, DWORD dwItemIdx, TITAN_TOTALINFO* pTitanTotalInfo)
{
	sprintf(txt, "EXEC dbo.MP_TITAN_Insert %d,%d,%d,%d,%d, %d,%d,%d,%d,%d, %d,%d",
		UserIdx,
		//CharacterIdx,
		dwItemIdx,
		pTitanTotalInfo->TitanKind,
		pTitanTotalInfo->TitanGrade,
		pTitanTotalInfo->Fuel,

		pTitanTotalInfo->Spell,
		pTitanTotalInfo->RecallTime,
		pTitanTotalInfo->TitanCallItemDBIdx,
		pTitanTotalInfo->RegisterCharacterIdx,
		pTitanTotalInfo->bRiding,

		pTitanTotalInfo->Scale,
		pTitanTotalInfo->MaintainTime	// magi82(23)
		);

	g_DB.Query(eQueryType_FreeQuery, eTitanInsert, CharacterIdx, txt);

}

void RTitanInsertToDB(LPQUERY pData, LPDBMESSAGE pMessage)
{
	//CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(atoi((char*)pData->Data[0]));
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);

	if( NULL == pPlayer )
		return;
	if( FALSE == pPlayer->GetInited() )
		return;

	TITAN_TOTALINFO TitanInfo;
	if( pMessage->dwResult )
	{
		TitanInfo.TitanMasterUserID = atoi((char*)pData->Data[0]);
		TitanInfo.TitanDBIdx = atoi((char*)pData->Data[1]);
		TitanInfo.TitanCallItemDBIdx = atoi((char*)pData->Data[2]);
		TitanInfo.TitanKind = atoi((char*)pData->Data[3]);
		TitanInfo.TitanGrade = atoi((char*)pData->Data[4]);
		TitanInfo.Fuel = atoi((char*)pData->Data[5]);
		TitanInfo.Spell = atoi((char*)pData->Data[6]);
		TitanInfo.RecallTime = atoi((char*)pData->Data[7]);
		TitanInfo.RegisterCharacterIdx = atoi((char*)pData->Data[8]);
		TitanInfo.bRiding = atoi((char*)pData->Data[9]);
		TitanInfo.Scale = atoi((char*)pData->Data[10]);
		TitanInfo.MaintainTime = atoi((char*)pData->Data[11]);	// magi82(23)
	}
	else
	{
		ASSERT(0);
	}

	pPlayer->GetTitanManager()->AddTitanTotalInfo(&TitanInfo, eServerNClient);

}

void TitanUpdateToDB(DWORD UserIdx, const TITAN_TOTALINFO* pTitanTotalInfo)
{
	sprintf(txt, "EXEC dbo.MP_TITAN_Update %d,%d,%d,%d,%d, %d,%d,%d,%d,%d",
		UserIdx,
		pTitanTotalInfo->TitanDBIdx,
		pTitanTotalInfo->TitanGrade,
		pTitanTotalInfo->Fuel,
		pTitanTotalInfo->Spell,

		pTitanTotalInfo->RecallTime,
		pTitanTotalInfo->RegisterCharacterIdx,
		pTitanTotalInfo->bRiding,
		pTitanTotalInfo->Scale,
		pTitanTotalInfo->MaintainTime	// magi82(23)
		);

	g_DB.Query(eQueryType_FreeQuery, eTitanUpdate, pTitanTotalInfo->TitanCallItemDBIdx, txt);

}

void TitanDeleteToDB( DWORD dwCallItemDBIdx )
{
	sprintf(txt, "EXEC dbo.MP_TITAN_Delete %d", dwCallItemDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eTitanDelete, 0, txt);
}

void TitanEquipItemInfoInsertToDB( TITAN_ENDURANCE_ITEMINFO* pInfo )
{
	sprintf(txt, "EXEC dbo.MP_TITAN_EQUIP_ENDURANCE_Insert %d,%d,%d,%d,%d",
		//pInfo->OwnerUserID,
		//pInfo->OwnerCharacterID,
		pInfo->ItemDBIdx,
		pInfo->CharacterID,
		pInfo->ItemIdx,
		pInfo->Endurance,
		pInfo->UserIdx
//		pInfo->bEquip
		);

	g_DB.Query(eQueryType_FreeQuery, eTitanEquipInfoInsert, pInfo->CharacterID, txt);

}

void RTitanEquipItemInfoInsertToDB( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( NULL == pPlayer )
		return;
	if( FALSE == pPlayer->GetInited() )
		return;

	TITAN_ENDURANCE_ITEMINFO TitanItemEnduranceInfo;
	if( pMessage->dwResult )
	{
		//TitanItemEnduranceInfo.OwnerUserID = atoi((char*)pData->Data[1]);
		//TitanItemEnduranceInfo.OwnerCharacterID = atoi((char*)pData->Data[0]);
		TitanItemEnduranceInfo.CharacterID = atoi((char*)pData->Data[0]);
		TitanItemEnduranceInfo.ItemDBIdx = atoi((char*)pData->Data[1]);
		TitanItemEnduranceInfo.ItemIdx	 = atoi((char*)pData->Data[2]);
		TitanItemEnduranceInfo.Endurance = atoi((char*)pData->Data[3]);
		TitanItemEnduranceInfo.UserIdx	= atoi((char*)pData->Data[4]);
		//TitanItemEnduranceInfo.bEquip = atoi((char*)pData->Data[5]);
	}
	else
	{
		ASSERT(0);
	}

	pPlayer->GetTitanManager()->AddTitanEquipItemEnduranceInfo(&TitanItemEnduranceInfo, eServerNClient);

}

void TitanEquipItemInfoUpdateToDB( TITAN_ENDURANCE_ITEMINFO* pInfo )
{
	sprintf(txt, "EXEC dbo.MP_TITAN_EQUIP_ENDURANCE_Update %d,%d,%d,%d",
		//pInfo->OwnerUserID,
		//pInfo->OwnerCharacterID,
		//pInfo->ItemIdx,
		pInfo->ItemDBIdx,
		pInfo->CharacterID,
		pInfo->Endurance,
		pInfo->UserIdx

		//pInfo->bEquip
		);

	g_DB.Query(eQueryType_FreeQuery, eTitanEquipInfoUpdate, 0, txt);
}

void TitanEquipItemInfoDeleteToDB(DWORD dwTitanEquipDBIdx)
{
	sprintf(txt, "EXEC dbo.MP_TITAN_EQUIP_ENDURANCE_Delete %d", dwTitanEquipDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eTitanEquipInfoDelete, 0, txt);

}

// magi82(33)
void TitanEquipInsertExceptionToDB( TITAN_ENDURANCE_ITEMINFO* pInfo )
{
	sprintf(txt, "EXEC dbo.MP_TITAN_EQUIP_ENDURANCE_Insert %d,%d,%d,%d,%d",
		//pInfo->OwnerUserID,
		//pInfo->OwnerCharacterID,
		pInfo->ItemDBIdx,
		pInfo->CharacterID,
		pInfo->ItemIdx,
		pInfo->Endurance,
		pInfo->UserIdx
		//		pInfo->bEquip
		);

	g_DB.Query(eQueryType_FreeQuery, eTitanEquipInfoInsertException, pInfo->CharacterID, txt);

}

// magi82(33)
void RTitanEquipInsertExceptionToDB( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( NULL == pPlayer )
		return;

	TITAN_ENDURANCE_ITEMINFO TitanItemEnduranceInfo;
	if( pMessage->dwResult )
	{
		//TitanItemEnduranceInfo.OwnerUserID = atoi((char*)pData->Data[1]);
		//TitanItemEnduranceInfo.OwnerCharacterID = atoi((char*)pData->Data[0]);
		TitanItemEnduranceInfo.CharacterID = atoi((char*)pData->Data[0]);
		TitanItemEnduranceInfo.ItemDBIdx = atoi((char*)pData->Data[1]);
		TitanItemEnduranceInfo.ItemIdx	 = atoi((char*)pData->Data[2]);
		TitanItemEnduranceInfo.Endurance = atoi((char*)pData->Data[3]);
		TitanItemEnduranceInfo.UserIdx	= atoi((char*)pData->Data[4]);
		//TitanItemEnduranceInfo.bEquip = atoi((char*)pData->Data[5]);
	}
	else
	{
		ASSERT(0);
	}

	pPlayer->GetTitanManager()->AddTitanEquipItemEnduranceInfo(&TitanItemEnduranceInfo);

}

// magi82(33)
void TitanEquipInsertExceptionPyogukToDB(DWORD CharacterIdx, TITAN_ENDURANCE_ITEMINFO* pInfo )
{
	sprintf(txt, "EXEC dbo.MP_TITAN_EQUIP_ENDURANCE_Insert %d,%d,%d,%d,%d",
		//pInfo->OwnerUserID,
		//pInfo->OwnerCharacterID,
		pInfo->ItemDBIdx,
		pInfo->CharacterID,
		pInfo->ItemIdx,
		pInfo->Endurance,
		pInfo->UserIdx
		//		pInfo->bEquip
		);

	g_DB.Query(eQueryType_FreeQuery, eTitanEquipInfoInsertExceptionPyoguk, CharacterIdx, txt);

}

// magi82(33)
void RTitanEquipInsertExceptionPyogukToDB( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( NULL == pPlayer )
		return;

	TITAN_ENDURANCE_ITEMINFO TitanItemEnduranceInfo;
	if( pMessage->dwResult )
	{
		//TitanItemEnduranceInfo.OwnerUserID = atoi((char*)pData->Data[1]);
		//TitanItemEnduranceInfo.OwnerCharacterID = atoi((char*)pData->Data[0]);
		TitanItemEnduranceInfo.CharacterID = atoi((char*)pData->Data[0]);
		TitanItemEnduranceInfo.ItemDBIdx = atoi((char*)pData->Data[1]);
		TitanItemEnduranceInfo.ItemIdx	 = atoi((char*)pData->Data[2]);
		TitanItemEnduranceInfo.Endurance = atoi((char*)pData->Data[3]);
		TitanItemEnduranceInfo.UserIdx	= atoi((char*)pData->Data[4]);
		//TitanItemEnduranceInfo.bEquip = atoi((char*)pData->Data[5]);
	}
	else
	{
		ASSERT(0);
	}

	pPlayer->GetTitanManager()->AddTitanEquipItemEnduranceInfo(&TitanItemEnduranceInfo, eServerNClient);

}

/*
void RCharacterGMControlInfo(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage)
{
	if(pMessage->dwResult == 0)
		return;

	DWORD CharacterIdx = atoi((char*)pData[0].Data[0]);
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(CharacterIdx);
	if(pPlayer==NULL)
		return;

	for(int n=0;n<pMessage->dwResult;++n)
	{
		//TDN
	//	int kind = atoi((char*)pData[n].Data[1])

	}

	pPlayer->SetInitState(PLAYERINITSTATE_GMCONTROL_INFO,pMessage->dwID);
}

void RCharacterGMControlInsert(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage)
{
	ASSERT(pMessage->dwResult == 1);

	DWORD CharacterIdx = atoi((char*)pData[0].Data[0]);
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(CharacterIdx);
	if(pPlayer)
	{
		//TDN
//		int kind = atoi((char*)pData[n].Data[1]);

	}

	
	DWORD GMChrIdx = pMessage->dwID;
	CPlayer* pGM = (CPlayer *)g_pUserTable->FindUser(CharacterIdx);
	if(pGM==NULL)
		return;
	
		//TDN
}
*/

void RSSItemInsert(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if((atoi((char*)pData->Data[0]) == 0))
	{
		// Insert ¡§oC¡§¢®¡§¡Ë
		
	}
	else
	{
		// Insert ¡§u¡§¡þ¢®¨¡¨Ï¨£
		DWORD dwPlayerID = atoi((char*)pData->Data[eCI_ObjectID]);		// A¨Ï©ª¡Ë¡þ?¡§u¡§¢®AI¡Íi¨Ï¡Ì
		ITEMBASE info;
		info.Position = atoi((char*)pData->Data[eCI_Position]);		// Item_Position
		info.QuickPosition = atoi((char*)pData->Data[eCI_QPosition]);
		info.dwDBIdx = atoi((char*)pData->Data[eCI_DBIDX]);		// Item_DBIDX
		info.Durability = atoi((char*)pData->Data[eCI_Durability]);	// Item_Durability
		info.wIconIdx = atoi((char*)pData->Data[eCI_IDX]);			// Item_IDX
		// magi82(28)
		info.ItemParam = 0;
		info.RareIdx = 0;

		STREETSTALLMGR->CreateDupItem( dwPlayerID, &info, pMessage->dwID);
	}	
}

void	RCharacterItemGetCheatIDX(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if((atoi((char*)pData->Data[0]) == 0))
	{
		// Insert ¡§oC¡§¢®¡§¡Ë
		
	}
	else
	{
		// Insert ¡§u¡§¡þ¢®¨¡¨Ï¨£
		DWORD dwPlayerID = atoi((char*)pData->Data[eCI_ObjectID]);		// A¨Ï©ª¡Ë¡þ?¡§u¡§¢®AI¡Íi¨Ï¡Ì
		ITEMBASE info;
		info.Position = atoi((char*)pData->Data[eCI_Position]);		// Item_Position
		info.dwDBIdx = atoi((char*)pData->Data[eCI_DBIDX]);		// Item_DBIDX
		info.QuickPosition = atoi((char*)pData->Data[eCI_QPosition]);
		info.Durability = atoi((char*)pData->Data[eCI_Durability]);	// Item_Durability
		info.wIconIdx = atoi((char*)pData->Data[eCI_IDX]);			// Item_IDX
		// magi82(28)
		info.ItemParam = 0;
		info.RareIdx = 0;

		CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(dwPlayerID);
		if(pPlayer == NULL)
			return;
		
//		ITEMMGR_OBJ->CheatItemDBResult(pPlayer,&info);
	}
}
void	RCharacterItemGetDivideIDX(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if((atoi((char*)pData->Data[0]) == 0))
	{
		// Insert ¡§oC¡§¢®¡§¡Ë
	}
	else
	{
		// Insert ¡§u¡§¡þ¢®¨¡¨Ï¨£
		atoi((char*)pData->Data[eCI_ObjectID]);		// A¨Ï©ª¡Ë¡þ?¡§u¡§¢®AI¡Íi¨Ï¡Ì
		atoi((char*)pData->Data[eCI_DBIDX]);		// Item_DBIDX
		atoi((char*)pData->Data[eCI_IDX]);			// Item_IDX
		atoi((char*)pData->Data[eCI_Position]);		// Item_Position
		atoi((char*)pData->Data[eCI_QPosition]);		// Item_QuickPosition
		atoi((char*)pData->Data[eCI_Durability]);	// Item_Durability
	}
}
void RItemOptionInsert(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if( pMessage->dwResult )
	{
		// Insert ¡§u¡§¡þ¢®¨¡¨Ï¨£
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser((DWORD)atoi((char*)pData[0].Data[eCIO_ObjectID]));
		if(pPlayer == NULL)
			return;

		ITEM_OPTION_INFO OptionInfo;
		OptionInfo.dwOptionIdx				= (DWORD)atoi((char*)pData[0].Data[eCIO_OptionID]);
		OptionInfo.dwItemDBIdx				= (DWORD)atoi((char*)pData[0].Data[eCIO_ItemDBID]);
		OptionInfo.GenGol					= (WORD)atoi((char*)pData[0].Data[eCIO_GenGol]);
		OptionInfo.MinChub					= (WORD)atoi((char*)pData[0].Data[eCIO_MinChub]);				
		OptionInfo.CheRyuk					= (WORD)atoi((char*)pData[0].Data[eCIO_CheRyuk]);				
		OptionInfo.SimMek					= (WORD)atoi((char*)pData[0].Data[eCIO_SimMek]);					
		OptionInfo.Life						= (DWORD)atoi((char*)pData[0].Data[eCIO_Life]);					
		OptionInfo.NaeRyuk					= (WORD)atoi((char*)pData[0].Data[eCIO_NaeRyuk]);				
		OptionInfo.Shield					= (DWORD)atoi((char*)pData[0].Data[eCIO_Shield]);					
		OptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[0].Data[eCIO_FireAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[0].Data[eCIO_WaterAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[0].Data[eCIO_TreeAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[0].Data[eCIO_GoldAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[0].Data[eCIO_EarthAttrRegist])/100.f );
		OptionInfo.PhyAttack				= (WORD)atoi((char*)pData[0].Data[eCIO_PhyAttack]);
		OptionInfo.CriticalPercent			= (WORD)atoi((char*)pData[0].Data[eCIO_Critical]);
		OptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[0].Data[eCIO_FireAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[0].Data[eCIO_WaterAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[0].Data[eCIO_TreeAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[0].Data[eCIO_GoldAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[0].Data[eCIO_EarthAttrAttack])/100.f );
		OptionInfo.PhyDefense				= (WORD)atoi((char*)pData[0].Data[eCIO_PhyDefense]);				
		pPlayer->AddItemOption(&OptionInfo);
		ITEMMGR->ReinforceItemDBResult(pPlayer, (WORD)atoi((char*)pData[0].Data[eCIO_ItemIdx]), (WORD)pMessage->dwID, &OptionInfo);
	}
	else
	{
		ASSERT(0);
	}
}

//!!!
void RItemRareInsert(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WORD lastNo = LOWORD(pMessage->dwID);
	WORD ArrayID = HIWORD(pMessage->dwID);

	if( pMessage->dwResult )
	{
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser((DWORD)atoi((char*)pData[0].Data[eCIR_ObjectID]));
		if(pPlayer == NULL)
			return;
		if(pPlayer->GetInited() == FALSE)
			return;
		
		ITEMOBTAINARRAYINFO * pItemArrayInfo = pPlayer->GetArray(ArrayID);
		if(pItemArrayInfo == NULL)
			return;

		pItemArrayInfo->ItemArray.AddItem( atoi((char*)pData->Data[eCIR_ItemDBID]),	//¿øº» ¾ÆÀÌÅÛ DB ÀÎµ¦½º
							atoi((char*)pData->Data[eCIR_ItemIdx]),	//¿øº» ¾ÆÀÌÅÛ ¸®½ºÆ® ÀÎµ¦½º
							0,	//atoi((char*)pData->Data[eCIR_Durability]),	//°¹¼ö(°­È­ÀÎµ¦½º)
							atoi((char*)pData->Data[eCIR_Position]), //ÀÎº¥ Æ÷Áö¼Ç
							0,	//atoi((char*)pData->Data[eCIR_QPosition]),	//´ÜÃàÃ¢ Æ÷Ä¡¼Ç
							0,	//atoi((char*)pData->Data[eCIR_Param]),
							atoi((char*)pData->Data[eCIR_RareID])	//·¹¾î DB ÀÎµ¦½º
							);	
		
		ITEM_RARE_OPTION_INFO RareOptionInfo;

		RareOptionInfo.dwRareOptionIdx				= (DWORD)atoi((char*)pData[0].Data[eCIR_RareID]);	//!»ó¼ö µû·Î ¾È¸¸µé¾îµµ µÉµí
		RareOptionInfo.dwItemDBIdx				= (DWORD)atoi((char*)pData[0].Data[eCIR_ItemDBID]);
		RareOptionInfo.GenGol					= (WORD)atoi((char*)pData[0].Data[eCIR_GenGol]);
		RareOptionInfo.MinChub					= (WORD)atoi((char*)pData[0].Data[eCIR_MinChub]);				
		RareOptionInfo.CheRyuk					= (WORD)atoi((char*)pData[0].Data[eCIR_CheRyuk]);				
		RareOptionInfo.SimMek					= (WORD)atoi((char*)pData[0].Data[eCIR_SimMek]);					
		RareOptionInfo.Life						= (DWORD)atoi((char*)pData[0].Data[eCIR_Life]);					
		RareOptionInfo.NaeRyuk					= (WORD)atoi((char*)pData[0].Data[eCIR_NaeRyuk]);				
		RareOptionInfo.Shield					= (DWORD)atoi((char*)pData[0].Data[eCIR_Shield]);					
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[0].Data[eCIR_FireAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[0].Data[eCIR_WaterAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[0].Data[eCIR_TreeAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[0].Data[eCIR_GoldAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[0].Data[eCIR_EarthAttrRegist])/100.f );
		RareOptionInfo.PhyAttack				= (WORD)atoi((char*)pData[0].Data[eCIR_PhyAttack]);
//		RareOptionInfo.CriticalPercent			= (WORD)atoi((char*)pData[0].Data[eCIO_Critical]);
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[0].Data[eCIR_FireAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[0].Data[eCIR_WaterAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[0].Data[eCIR_TreeAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[0].Data[eCIR_GoldAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[0].Data[eCIR_EarthAttrAttack])/100.f );
		RareOptionInfo.PhyDefense				= (WORD)atoi((char*)pData[0].Data[eCIR_PhyDefense]);
		
		pPlayer->AddItemRareOption(&RareOptionInfo);

		if( lastNo == pItemArrayInfo->ItemArray.ItemNum )
		{
			pItemArrayInfo->CallBackEx( pPlayer, ArrayID, &RareOptionInfo );
			//ITEMMGR->ObtainItemDBResult( pPlayer, ArrayID);
		}
//		ITEMMGR->RareItemDBResult(pPlayer, (WORD)atoi((char*)pData[0].Data[eCIR_ItemIdx]), (WORD)pMessage->dwID, &RareOptionInfo);
	}
	else
	{
		ASSERT(0);
	}
}


/* New Item Return Func */
void RItemInsert(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WORD lastNo = LOWORD(pMessage->dwID);
	WORD ArrayID = HIWORD(pMessage->dwID);

	if( pMessage->dwResult )
	{
		// Insert ¡§u¡§¡þ¢®¨¡¨Ï¨£

		DWORD dwObjectID = atoi((char*)pData->Data[eCI_ObjectID]);
		DWORD dwItemIdx = atoi((char*)pData->Data[eCI_IDX]);
		DWORD dwDura = atoi((char*)pData->Data[eCI_Durability]);
		DWORD dwPos = atoi((char*)pData->Data[eCI_Position]);
		DWORD dwQPos = atoi((char*)pData->Data[eCI_QPosition]);
		DWORD dwDBIdx = atoi((char*)pData->Data[eCI_DBIDX]);
		DWORD dwParam = atoi((char*)pData->Data[eCI_Param]);

		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(atoi((char*)pData->Data[eCI_ObjectID]));
		if(pPlayer == NULL)
			return;
		if(pPlayer->GetInited() == FALSE)
			return;

		ITEMOBTAINARRAYINFO * pItemArrayInfo = pPlayer->GetArray(ArrayID);
		if(pItemArrayInfo == NULL)
			return;

		pItemArrayInfo->ItemArray.AddItem( atoi((char*)pData->Data[eCI_DBIDX]), 
							atoi((char*)pData->Data[eCI_IDX]), 
							atoi((char*)pData->Data[eCI_Durability]), 
							atoi((char*)pData->Data[eCI_Position]), 
							atoi((char*)pData->Data[eCI_QPosition]),
							atoi((char*)pData->Data[eCI_Param]),
							0	//atoi((char*)pData->Data[eCI_RareIdx]),
							);

		if( lastNo == pItemArrayInfo->ItemArray.ItemNum )
		{
			pItemArrayInfo->CallBack( pPlayer, ArrayID );
			//ITEMMGR->ObtainItemDBResult( pPlayer, ArrayID);
		}
		else
		{
			//¡§u¡§¢®¡Ë¡ÍO¡ËO¢®¡¿¡Ë¡ÍA ¡§ui¡ËO¢®i¢®¨¡O A¨Ï©ª¡Ë¡þ¡ËcCO¢®¨¡IAI¢®¨¡¡Ë¢ç?
			//A¢®©­¡ËOoAI¡§u¨Ï¡Ì¡§¢®¡Ëc AI¡§¡þ¢®IAa¡Ë¡þ¡Ëc¢®¨¡¡Ë¢ç ¡Ë¡þ¡§¨£A¨Ï¡þ¡Ë¡ÍA¡Íi¢®I.. ¡Ë¡þ¡§¡©¡§oAAo¢®¨¡¡Ë¢ç ¡§u¨Ï¨£¡§ui¡§u¢®¨Ï.
		}
	}
	else
	{
		ASSERT(0);
	}
}

/* New Mugong Return Func */
void RMugongInsert(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if((atoi((char*)pData->Data[0]) == 0))
	{
		// ¡§oC¡§¢®¡§¡Ë
		/*
		MSG_MUGONG_ADD_NACK msg;
		msg.Category = MP_MUGONG;
		msg.Protocol = MP_MUGONG_ADD_NACK;
		msg.dwObjectID = atoi((char*)pData->Data[0]);

		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(msg.dwObjectID);
		if(pPlayer == NULL)
			return;
		MUGONGMGR_OBJ->AddMugongNACK(pPlayer);
		*/
	}
	else
	{
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(atoi((char*)pData->Data[0]));
		if(pPlayer == NULL)
			return;

		MUGONGBASE sMugong;
		sMugong.dwDBIdx			= atoi((char*)pData->Data[eCM_DBIDX]);
		sMugong.wIconIdx		= atoi((char*)pData->Data[eCM_IDX]);
		sMugong.ExpPoint		= atoi((char*)pData->Data[eCM_ExpPoint]);
		sMugong.Sung			= atoi((char*)pData->Data[eCM_Sung]);
		sMugong.Position		= atoi((char*)pData->Data[eCM_Position]);
		sMugong.QuickPosition	= atoi((char*)pData->Data[eCM_QPosition]);
		sMugong.OptionIndex		= atoi((char*)pData->Data[eCM_Option]);
		sMugong.bWear			= FALSE;

		////////////////////////////////////////////////////////////////////////////////
		// 06. 06. 2Â÷ ÀüÁ÷ - ÀÌ¿µÁØ
		// ¹«°ø º¯È¯
		//sMugong.OptionIndex = 0;
		////////////////////////////////////////////////////////////////////////////////

		MUGONGMNGR->AddMugongDBResult(pPlayer, &sMugong);
	}
}

void RMapBaseEconomy(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if((atoi((char*)pData->Data[0]) == 0))
	{
		// Ai¡ËO¢®Ei¡§u¡§oC¡§¢®¡§¡Ë
	}
	else
	{
		REGIST_BASEECONOMY msg;
		msg.MapNum = atoi((char*)pData->Data[eME_MEPNUM]);
		msg.OriginNum = atoi((char*)pData->Data[eME_ORIGINNUM]);
		msg.OriginPrice = atoi((char*)pData->Data[eME_ORIGINPRICE]);
		msg.OriginAmount = atoi((char*)pData->Data[eME_ORIGINAMOUNT]);
		msg.RequireNum = atoi((char*)pData->Data[eME_REQUIRENUM]);
		msg.RequirePrice = atoi((char*)pData->Data[eME_REQUIREPRICE]);
		msg.RequireAmount = atoi((char*)pData->Data[eME_REQUIREAMOUNT]);
		msg.BuyRates = atoi((char*)pData->Data[eME_BUYRATES]);
		msg.SellRates = atoi((char*)pData->Data[eME_SELLRATES]);
		
		ECONOMY_OBJ->SetRegistEconomy(&msg);
	}
}

void RAuctionPageQuery(LPQUERY pData, LPDBMESSAGE pMessage)
{
/*	if((atoi((char*)pData->Data[0]) == 0))
	{
		// Ai¡ËO¢®Ei¡§u¡§oC¡§¢®¡§¡Ë
	}
	else
*/	{
		AUCTION_ONPAGELIST msg;
		for(int i = 0; i < 10; i++)
		{
			msg.Auction_List[i].ItemIdx = atoi((char*)pData[i].Data[eAT_ItemDBIdx]);
			msg.Auction_List[i].SellNum = atoi((char*)pData[i].Data[eAT_SellNum]);
			msg.Auction_List[i].SellerID = atoi((char*)pData[i].Data[eAT_SellerID]);
			msg.Auction_List[i].EndDate = atoi((char*)pData[i].Data[eAT_EndDate]);
			msg.Auction_List[i].EndTime = atoi((char*)pData[i].Data[eAT_EndTime]);
			msg.Auction_List[i].StartPrice = atoi((char*)pData[i].Data[eAT_StartPrice]);
			msg.Auction_List[i].ImmediatePrice = atoi((char*)pData[i].Data[eAT_ImmediatePrice]);
		}
	}
}


void RPartyLoad(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD PartyIDX = 0;
	for(DWORD i=0; i<pMessage->dwResult;++i)
	{
		PartyIDX = atoi((char*)pData[i].Data[ePA_IDX]);
		PARTYMGR->RegistParty(PartyIDX);
	}
	if(pMessage->dwResult == MAX_PARTY_LOAD_NUM)
	{
		PartyLoad(PartyIDX);
	}
}

//2008. 5. 21. CBH - ¹æÆÄ »ý¼º Ãß°¡ ¿É¼Ç °ü·Ã Ã³¸® ¼öÁ¤
void RPartyCreate(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage) 
{
	DWORD CreateErr, MasterIdx;
	
	PARTY_ADDOPTION AddOption;
	memset(&AddOption, 0, sizeof(PARTY_ADDOPTION));

	CreateErr = atoi((char*)pData->Data[ePA_ERR]);
	MasterIdx = atoi((char*)pData->Data[ePA_MASTER]);
	AddOption.bOption = atoi((char*)pData->Data[ePA_OPTION]);
	AddOption.wMinLevel = atoi((char*)pData->Data[ePA_MINLEVEL]);
	AddOption.wMaxLevel = atoi((char*)pData->Data[ePA_MAXLEVEL]);
	AddOption.bPublic = atoi((char*)pData->Data[ePA_PUBLIC]);
	AddOption.wLimitCount = atoi((char*)pData->Data[ePA_LIMITCOUNT]);
	SafeStrCpy(AddOption.szTheme, (char*)pData->Data[ePA_THEME], MAX_PARTY_NAME+1);	

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(MasterIdx);
	if(!pPlayer)
		return;

	if(CreateErr == NULL)
	{
		PARTYMGR->SendErrMsg(MasterIdx, eErr_Create_DB, MP_PARTY_CREATE_NACK);
		return;	
	}
	else
	{
		AddOption.dwPartyIndex = atoi((char*)pData->Data[ePA_PARTYID]);
	}

	PARTYMGR->CreatePartyResult(pPlayer, &AddOption);
}

void RPartyInfoByUserLogin(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage) 
{
	PARTYMEMBER member[MAX_PARTY_LISTNUM];
	BYTE opt;
	CPlayer* pPlayer;
	PARTY_ADDOPTION AddOption;
	
	memset(member, 0, sizeof(PARTYMEMBER)*MAX_PARTY_LISTNUM);
	memset(&AddOption, 0, sizeof(PARTY_ADDOPTION));

	pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(pPlayer == NULL)
		return;

	if( !pMessage->dwResult )
	{
		pPlayer->SetPartyIdx(0);
		CharacterTablePartyIdxUpdate( pMessage->dwID );
		return;
	}

	for(DWORD n=0;n<pMessage->dwResult;++n)
	{
		member[n].MemberID = atoi((char*)pData[n].Data[ePA_LIMemberId]);
		SafeStrCpy(member[n].Name,(char*)pData[n].Data[ePA_LIMemberName], MAX_NAME_LENGTH+1);
		member[n].bLogged = atoi((char*)pData[n].Data[ePA_LIMemberCurrentMap]) ? TRUE : FALSE;
		member[n].LifePercent = 0;
		member[n].NaeRyukPercent = 0;
		member[n].Level = atoi((char*)pData[n].Data[ePA_LIMemberLevel]);
	}
	opt = atoi((char*)pData[0].Data[ePA_LIOption]);	
	AddOption.wMinLevel = atoi((char*)pData[0].Data[ePA_MinLevel]);
	AddOption.wMaxLevel = atoi((char*)pData[0].Data[ePA_MaxLevel]);
	AddOption.bPublic = atoi((char*)pData[0].Data[ePA_Public]);
	AddOption.wLimitCount = atoi((char*)pData[0].Data[ePA_LimitCount]);
	SafeStrCpy(AddOption.szTheme, (char*)pData[0].Data[ePA_Theme], MAX_PARTY_NAME);	
	AddOption.bOption = opt;
	AddOption.dwPartyIndex = pPlayer->GetPartyIdx();

	PARTYMGR->UserLogInAfterDBQuery(pMessage->dwID, pPlayer->GetPartyIdx(), member, AddOption);
}
/*
void RPartyCreate(LPQUERY pData, LPDBMESSAGE pMessage) 
{
	DWORD CreateErr,PartyIdx,MasterIdx;
	BYTE Option;
	CreateErr = atoi((char*)pData->Data[ePA_ERR]);
	MasterIdx = atoi((char*)pData->Data[ePA_MASTER]);
	Option = atoi((char*)pData->Data[ePA_OPTION]);
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(MasterIdx);
	if(!pPlayer)
		return;

	if(CreateErr == NULL)
	{
		PARTYMGR->SendErrMsg(MasterIdx, eErr_Create_DB, MP_PARTY_CREATE_NACK);
		return;	
	}
	else
	{
		PartyIdx = atoi((char*)pData->Data[ePA_PARTYID]);
	}

	PARTYMGR->CreatePartyResult(pPlayer,PartyIdx,Option);
}

void RPartyInfoByUserLogin(LPQUERY pData, LPDBMESSAGE pMessage) 
{
	PARTYMEMBER member[MAX_PARTY_LISTNUM];
	BYTE opt;
	CPlayer* pPlayer;

	memset(member, 0, sizeof(PARTYMEMBER)*MAX_PARTY_LISTNUM);
	pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(pPlayer == NULL)
		return;

	if( !pMessage->dwResult )
	{
		pPlayer->SetPartyIdx(0);
		CharacterTablePartyIdxUpdate( pMessage->dwID );
		return;
	}

	for(DWORD n=0;n<pMessage->dwResult;++n)
	{
		member[n].MemberID = atoi((char*)pData[n].Data[ePA_LIMemberId]);
		SafeStrCpy(member[n].Name,(char*)pData[n].Data[ePA_LIMemberName], MAX_NAME_LENGTH+1);
		member[n].bLogged = atoi((char*)pData[n].Data[ePA_LIMemberCurrentMap]) ? TRUE : FALSE;
		member[n].LifePercent = 0;
		member[n].NaeRyukPercent = 0;
		member[n].Level = atoi((char*)pData[n].Data[ePA_LIMemberLevel]);
	}
	opt = atoi((char*)pData[0].Data[ePA_LIOption]);
	PARTYMGR->UserLogInAfterDBQuery(pMessage->dwID, pPlayer->GetPartyIdx(), member, opt);
}
*/

void RPartyChangeMaster(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CParty* pParty = PARTYMGR->GetParty(pMessage->dwID);
	if(!pParty)
	{
		ASSERT(0);
		return;
	}
	pParty->SetMasterChanging(FALSE);
		
	CPlayer* pMaster = (CPlayer*)g_pUserTable->FindUser(pParty->GetMasterID());
	if(atoi((char*)pData->Data[ePA_CMErr]) == 0) //change master ack
	{
		PARTYMGR->DoChangeMasterParty(pParty, atoi((char*)pData->Data[ePA_CMTargetID]));
		//pParty->SetMasterChanging(FALSE);
		PARTYMGR->NotifyChangesOtherMapServer(atoi((char*)pData->Data[ePA_CMTargetID]),pParty,MP_PARTY_NOTIFYCHANGEMASTER_TO_MAPSERVER, pMessage->dwID);
	}
	else //change master nack
	{
		if(!pMaster)
		{
		//	pParty->SetMasterChanging(FALSE);
			return;
		}

		MSGBASE msg;
		msg.Category = MP_PARTY;
		msg.Protocol = MP_PARTY_CHANGEMASTER_NACK;
		pMaster->SendMsg(&msg, sizeof(msg));
	}
}

void RPartyDelMember(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CParty* pParty = PARTYMGR->GetParty(pMessage->dwID);
	if(!pParty)
	{
		ASSERT(0);
		return;
	}
	if(atoi((char*)pData->Data[ePA_DMErr]) == 0) //del member ack
	{
		PARTYMGR->DelMember(atoi((char*)pData->Data[ePA_DMTargetID]), pMessage->dwID);
	}
	else //del member nack
	{
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(atoi((char*)pData->Data[ePA_DMTargetID]));
		if(!pPlayer)
			return;
		MSG_INT msg;
		msg.Category = MP_PARTY;
		msg.Protocol = MP_PARTY_DEL_NACK;
		msg.nData	 = atoi((char*)pData->Data[ePA_DMErr]);
		pPlayer->SendMsg(&msg, sizeof(msg));
	}
}
/*
void RMunpaLoad(LPQUERY pData, LPDBMESSAGE pMessage)
{
	MUNPALISTINFO_MAP munpa;
	memset(&munpa, 0, sizeof(MUNPALISTINFO_MAP));
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		munpa.MunpaID = atoi((char*)pData[i].Data[eMu_MunpaID]);
		SafeStrCpy(munpa.MunpaName, (char*)pData[i].Data[eMu_MunpaName], MAX_MUNPA_NAME + 1);
		munpa.Famous = atoi((char*)pData[i].Data[eFamous]);
		munpa.MasterID = atoi((char*)pData[i].Data[eMunpaMasterID]);
		munpa.MemberNum = atoi((char*)pData[i].Data[eMemberNum]);
		munpa.MunpaRank = atoi((char*)pData[i].Data[eMunpaRank]);
		munpa.MunpaMoney = atoi((char*)pData[i].Data[eMunpaMoney]);
		
		MUNPAMGR->RegistMunpa(&munpa);
	}

	ASSERT(pMessage->dwResult <= MAX_MUNPALOAD_LIST);
	if(pMessage->dwResult == MAX_MUNPALOAD_LIST)
	{
		// ¨Ï©ª¨Ï¡À¡§u¡§¢®AO¡Ë¡ÍA ¨Ïo¡Ëc¡§¢®A¢®¨¡¡Ë¢ç ¡Ë¡Ío AO8¡Ë¡ÍI ¡Ë¡ÍU¡§oA ¡Ë¡þ¡Ëc¡§i©ö¡Ëi¨ù¡§¢®¡Ëc¡Ë¡þ| ¨Ïi©ö¡Ëi¨ö¡§u¡§¢®¡Ë?A¡Ë¡ÍU.
		MunpaLoad(GAMERESRCMNGR->GetLoadMapNum(), munpa.MunpaID);
	}
	else
	{
		MunpaItemOption(GAMERESRCMNGR->GetLoadMapNum(), 0);
	}
}

void RMunpaItemOption(LPQUERY pData, LPDBMESSAGE pMessage)
{
	ITEM_OPTION_INFO OptionInfo;
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		OptionInfo.dwOptionIdx				= (DWORD)atoi((char*)pData[i].Data[eMOI_OptionID]);
		OptionInfo.dwItemDBIdx				= (DWORD)atoi((char*)pData[i].Data[eMOI_ItemDBID]);
		OptionInfo.GenGol					= (WORD)atoi((char*)pData[i].Data[eMOI_GenGol]);
		OptionInfo.MinChub					= (WORD)atoi((char*)pData[i].Data[eMOI_MinChub]);				
		OptionInfo.CheRyuk					= (WORD)atoi((char*)pData[i].Data[eMOI_CheRyuk]);				
		OptionInfo.SimMek					= (WORD)atoi((char*)pData[i].Data[eMOI_SimMek]);					
		OptionInfo.Life						= (DWORD)atoi((char*)pData[i].Data[eMOI_Life]);					
		OptionInfo.NaeRyuk					= (WORD)atoi((char*)pData[i].Data[eMOI_NaeRyuk]);				
		OptionInfo.Shield					= (DWORD)atoi((char*)pData[i].Data[eMOI_Shield]);					
		OptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[eMOI_FireAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[eMOI_WaterAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[eMOI_TreeAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[eMOI_GoldAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[eMOI_EarthAttrRegist])/100.f );
		OptionInfo.PhyAttack				= (WORD)atoi((char*)pData[i].Data[eMOI_PhyAttack]);
		OptionInfo.CriticalPercent			= (WORD)atoi((char*)pData[i].Data[eMOI_Critical]);
		OptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[eMOI_FireAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[eMOI_WaterAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[eMOI_TreeAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[eMOI_GoldAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[eMOI_EarthAttrAttack])/100.f );
		OptionInfo.PhyDefense				= (WORD)atoi((char*)pData[i].Data[eMOI_PhyDefense]);				
		
		MUNPAMGR->AddItemOption(&OptionInfo);
	}

	if(pMessage->dwResult >= MAX_MUNPALOAD_LIST)
	{
		MunpaItemOption(GAMERESRCMNGR->GetLoadMapNum(), OptionInfo.dwOptionIdx);
	}
	else
	{
		MunpaItemLoad(GAMERESRCMNGR->GetLoadMapNum(),0);
	}
}

void RMunpaItemLoad(LPQUERY pData, LPDBMESSAGE pMessage)
{
	ITEMBASE munpaitem;
	DWORD MunpaID;

	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		MunpaID = atoi((char*)pData[i].Data[eMu_IMunpaID]);
		
		munpaitem.Durability = atoi((char*)pData[i].Data[eMu_IDurability]);
		munpaitem.wIconIdx = atoi((char*)pData[i].Data[eMu_IIDX]);
		munpaitem.dwDBIdx = atoi((char*)pData[i].Data[eMu_IDBIDX]);
		munpaitem.Position = atoi((char*)pData[i].Data[eMu_IPosition]);
		munpaitem.QuickPosition = 0;
		MUNPAMGR->RegistMunpaItem(MunpaID, &munpaitem);
	}
	if(pMessage->dwResult < MAX_MUNPALOAD_LIST)
	{
	//	g_pServerSystem->SetStart(TRUE);
		//¨Ïo¡Ëc¡§¢®AA¢®E¢®¨¡i ¢®¨úCCN; A¡§¡Ë¡§ui¡Ë?A¡Ë¡ÍU.
		MUNPAMGR->LoadWarehouseAuthority();

		WantedLoad(0);
	}	
	else
		MunpaItemLoad(GAMERESRCMNGR->GetLoadMapNum(), munpaitem.dwDBIdx);
}

void RMunpaLoadSyn(LPQUERY pData, LPDBMESSAGE pMessage)
{	
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(!pPlayer)
		return;

	SEND_MUNPA_INFO MunpaInfo;
	memset(&MunpaInfo,0,sizeof(MunpaInfo));
	MunpaInfo.Category = MP_MUNPA;
	MunpaInfo.Protocol = MP_MUNPA_LOAD_MUNPALIST_ACK;
	MunpaInfo.dwObjectID = pMessage->dwID;
	
	MunpaInfo.TotalResultNum = atoi((char*)pData[0].Data[eMu_MLStotalnum]);
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		MunpaInfo.Munpa[i].Famous = atoi((char*)pData[i].Data[eMu_MLSmunpafamous]);
		MunpaInfo.Munpa[i].MemberNum = atoi((char*)pData[i].Data[eMu_MLSmembernum]);
		MunpaInfo.Munpa[i].MunpaID = atoi((char*)pData[i].Data[eMu_MLSmunpaid]);
		SafeStrCpy( MunpaInfo.Munpa[i].MunpaMasterName, (char*)pData[i].Data[eMu_MLSmastername], MAX_NAME_LENGTH + 1);
		SafeStrCpy( MunpaInfo.Munpa[i].MunpaName, (char*)pData[i].Data[eMu_MLSmunpaname], MAX_MUNPA_NAME + 1);
		MunpaInfo.Munpa[i].Kind = atoi((char*)pData[i].Data[eMu_MLSKind]);
	}

	pPlayer->SendMsg(&MunpaInfo,sizeof(MunpaInfo)); 
}

void RMunpaCreate(LPLARGEQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer =  (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(!pPlayer)
		return;
	if(atoi((char*)pData->Data[eMu_MCErr]) != 0)
	{
		MSG_WORD msg;
		msg.Category = MP_MUNPA;
		msg.Protocol = MP_MUNPA_CREATE_NACK;
		msg.wData = atoi((char*)pData->Data[eMu_MCErr]);
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}

	MUNPALISTINFO_MAP munpa;
	memset(&munpa,0, sizeof(munpa));
	munpa.MunpaID	=	atoi((char*)pData->Data[eMu_MCMunpaid]);
	if(!munpa.MunpaID)
	{
		ASSERTMSG(0, "¨Ïo¡Ëc¡§¢®A¡Ë¡þ| ¢®iy¡§u¡§¡þC¨Ï¡þ¡Ë¡ÍA¡Íi¢®I ¡§u¡§¢®AI¡Íi¨Ï¡Ì¡Ë¡þ| A¨Ï¡þ¡Ë¡þ¨Ï¨£ ¨Ïi©ö¡Ëi¨ö¡§u¡§¢®¡Ë?O¡§o4I¡Ë¡ÍU.");
		return;
	}
	munpa.MasterID = atoi((char*)pData->Data[eMu_MCMasterid]);
	SafeStrCpy(munpa.MunpaName, (char*)pData->Data[eMu_MCMunpaname], MAX_MUNPA_NAME + 1);
	munpa.Famous = 0; //AI¡Ë¡ÍU 0
	munpa.MemberNum = 1;
	munpa.MunpaRank = atoi((char*)pData->Data[eMu_MCMunpaRank]);

	FAMEMGR->SetPlayerFame(pPlayer, atoi((char*)pData->Data[eMu_MCMasterFame]));
	MUNPAMGR->CreateMunpaResult(&munpa);
}

void RMunpaBreakUp(LPQUERY pData, LPDBMESSAGE pMessage)
{
	int err = atoi((char*)pData->Data[eMu_MBerr]);
	DWORD MastserID = atoi((char*)pData->Data[eMu_MBmasterid]);
	CPlayer* pMaster = (CPlayer*)g_pUserTable->FindUser(MastserID);

	if( err ) // != 0 : fail
	{
		if(pMaster)
		{
			MSGBASE msg;
			msg.Category = MP_MUNPA;
			msg.Protocol = MP_MUNPA_BREAKUP_NACK;
			
			pMaster->SendMsg(&msg, sizeof(msg));
		}
		return;
	}
	
	if(pMaster)
	{
		pMaster->SetFame(atoi((char*)pData->Data[eMu_MBmasterfame]));
		FAMEMGR->SendFame(pMaster, atoi((char*)pData->Data[eMu_MBmasterfame]));
		pMaster->SetMunpaIdx(0);
		pMaster->SetMunpaMemberRank(MUNPA_NOTMEMBER);
		
		MSGBASE msg;
		msg.Category = MP_MUNPA;
		msg.Protocol = MP_MUNPA_BREAKUP_ACK;
		pMaster->SendMsg(&msg,sizeof(msg));
		
		MUNPAMGR->MarkChange(pMaster, 0, MUNPA_NOTMEMBER);
	}

	//¨Ïo¡Ëc¡§¢®A¡Ë?¨Ï¨£¡Íie ¡§uA¡§¢®AC¡§¨£¡§u¨Ï¡þAo
	MunpaBreakUpMembers(0, pMessage->dwID);

	MUNPAMGR->BreakUpMunpaResult(pMessage->dwID);
}

void RMunpaBreakUpMembers(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD MemberID=0;
	CPlayer* pMember=NULL;
	MSGBASE BaseMsg;
	MSG_DWORD2 Dword2Msg;

	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		//¢®¢´I¢®¨ú¢®¢¯ ¡§u¡§¢®¡Ë?o CO¡ËO¢®¡¿ MP_CHARACTER_TotalInfoUpdated ¡Ë?¡Ë¢ç¡§u¢®¨Ï 
		//tb_munpaA¢®¢¯AI¡§¡þi; Au¢®¨¡iC¡§¨£¡§u¢®¨Ï ¡§u¢®%i¢®IAI¡§¢®¡ËcCI¡Ë¡þe¡§u¢®¨Ï A¢®¢¯AI¡§¡þi d¡§¡þ¡Ë¡þ¡Ë¡þ| AIA¡Ë¢ç¡§oAA¨Ï¡À¡Ë¡ÍU. 
		MemberID = atoi((char*)pData[i].Data[eMu_MBmemberid]);  //¡Ë?A¡ËOoAI ¡ÍiC¡§ui AO¡Ë¡ÍA ¡Ë¡þa¨Ïoo¡Ë¡þ¡Ë¡þ ¡§¡þ¡Ë¡þ¨Ï©ª¢®iA¡§¨£¡Ë¡ÍU.
		pMember = (CPlayer*)g_pUserTable->FindUser(MemberID);
		if(pMember)
		{
			pMember->SetFame(atoi((char*)pData[i].Data[eMu_MBmemberfame]));
			FAMEMGR->SendFame(pMember, atoi((char*)pData[i].Data[eMu_MBmemberfame]));
			pMember->SetMunpaIdx(0);
			pMember->SetMunpaMemberRank(MUNPA_NOTMEMBER);
			
			BaseMsg.Category = MP_MUNPA;
			BaseMsg.Protocol = MP_MUNPA_BREAKUP_ACK;
			pMember->SendMsg(&BaseMsg,sizeof(BaseMsg));
			
			MUNPAMGR->MarkChange(pMember, 0, MUNPA_NOTMEMBER);
		}
		else
		{
			Dword2Msg.Category = MP_MUNPA;
			Dword2Msg.Protocol = MP_MUNPA_NOTIFYBREAKUP_TO_MAPSERVER;
			Dword2Msg.dwObjectID = MemberID;
			Dword2Msg.dwData1 = pMessage->dwID;
			Dword2Msg.dwData2 = atoi((char*)pData[i].Data[eMu_MBmemberfame]);
			
		//	g_Network.Send2AgentServer((char*)&msg,sizeof(msg));
			g_Network.Broadcast2AgentServer((char*)&Dword2Msg,sizeof(Dword2Msg));
		}
	}

	if(pMessage->dwResult >= MAX_MUNPALOAD_LIST)
	{
		MunpaBreakUpMembers(MemberID, pMessage->dwID);
	}
	else
	{
		//A¨Ï©ª¡Ë¡þ?AI A¢®¢¯AI¡§¡þi¡Ë?¡Ë¢ç AO¡Ë¡ÍA ¡Íi¢®IAIA¡Ë¡þ¡Ë¡þ| Ao¡Ë?i¡Ë¡ÍU.
		MunapBreakupClr(pMessage->dwID);
	}
}

void RMunpaChangeMemberRank(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pMaster = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(atoi((char*)pData->Data[eMu_MCRchangeerr]) != 0) //¡§¡þ?¢®¨¡¨Ï¢® ¡Ë¡þ¨Ï¨£C¨Ï¡þ=.
	{
		if(atoi((char*)pData->Data[eMu_MCRchangeerr]) == 5) //¢®¨¡¢®¨¡: A¢®AA¢®I
			return;
		if(pMaster)
		{
			MSG_NAME msg;
			msg.Category = MP_MUNPA;
			msg.Protocol = MP_MUNPA_CHANGERANK_NACK;
			SafeStrCpy(msg.Name, (char*)pData->Data[eMu_MCRtargetname],MAX_NAME_LENGTH+1);
			pMaster->SendMsg(&msg, sizeof(msg));
		}
		return;
	}
	else
	{
		if(pMaster)
		{
			MSG_DWORD2 send_msg;
			send_msg.Category = MP_MUNPA;
			send_msg.Protocol = MP_MUNPA_CHANGERANK_ACK; 
			send_msg.dwData1 = atoi((char*)pData->Data[eMu_MCRtargetid]);
			send_msg.dwData2 = atoi((char*)pData->Data[eMu_MCRposition]);
			
			pMaster->SendMsg(&send_msg, sizeof(send_msg));
		}
	}

	DWORD TargetID = atoi((char*)pData->Data[eMu_MCRtargetid]);
	if(TargetID == 0)  //¢®¢´I¢®¨ú¢®¢¯AI CIAo ¡§uE¡§uO8¨ÏoC¢®¢´I
		return;
	CPlayer* pTargetPlayer  = (CPlayer*)g_pUserTable->FindUser(TargetID);
	if(pTargetPlayer)
	{
		FAMEMGR->SetPlayerFame(pTargetPlayer, atoi((char*)pData->Data[eMu_MCRfame]));
		MUNPAMGR->DoChangeRank(pTargetPlayer, atoi((char*)pData->Data[eMu_MCRposition]));
	}
	else
	{
		//¡Ë¡þ¨Ï¡Ì¡Íic ¡Ë?¡Ë¢çAIAu¡§¢®¡Ëc¢®¢´I ¡§¡þ¡Ë¡þ¨Ï©ª¢®i¡§u¢®¨Ï AO8¡Ë¡þe ¡Ë?¢®©­¢®¨¡a¡ÍiE ¡Ë¡þE¡§u¢®¨Ï¨Ïoo¢®¢´I ¡§¡þ¡Ë¡þ¨Ï©ª¡§o¡Ë¡ÍU. 
		SEND_MUNPAMEMBER_RANK_CHANGE_ACK msg;
		msg.Category = MP_MUNPA;
		msg.Protocol = MP_MUNPA_NOTIFY_CHANGERANK_TO_MAPSERVER;
		msg.dwObjectID = atoi((char*)pData->Data[eMu_MCRtargetid]);
		msg.MunpaID = atoi((char*)pData->Data[eMu_MCRmunpaid]);
		msg.Rank = atoi((char*)pData->Data[eMu_MCRposition]);
		msg.Fame = atoi((char*)pData->Data[eMu_MCRfame]);
				
		g_Network.Broadcast2AgentServer((char*)&msg,sizeof(msg));
	}
}

void RMunpaAcceptMember(LPQUERY pData, LPDBMESSAGE pMessage)
{	
	CPlayer* pMaster = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	
	if(atoi((char*)pData->Data[eMu_MACerr]) != 0)
	{
		if(pMaster)
		{
			MSG_NAME nmsg;
			nmsg.Category = MP_MUNPA;
			nmsg.Protocol = MP_MUNPA_ADDMEMBER_NACK;
			SafeStrCpy(nmsg.Name, (char*)pData->Data[eMu_MACtargetname], MAX_NAME_LENGTH+1);
				
			pMaster->SendMsg(&nmsg,sizeof(nmsg)); //nack
		}
		return;
	}
	else
	{
		if(pMaster)
		{
			MSG_BYTE msg;
			msg.Category = MP_MUNPA;
			msg.Protocol = MP_MUNPA_ADDMEMBER_ACK;
			msg.bData = atoi((char*)pData->Data[eMu_MACblast]);
			
			pMaster->SendMsg(&msg,sizeof(msg)); //ack
		}
	}

	//A¡Ë¡þ¢®¨¡U CA¢®¢´¨ÏoAI¡§ui¢®¨¡¡Ë¢ç ¡Ë?A¡ËOoAI ¡ÍiC¡§uiAO8¡Ë¡þe ¡§u¡§¢®AI¡Íi¨Ï¡Ì¡Ë¡þ| ¡§u¡§¢®¡Ë¡ÍI¡Ë¡þe 0; ¡Ë¡þ¡ËcAICN¡Ë¡ÍU. 
	DWORD TargetID = atoi((char*)pData->Data[eMu_MACtargetid]);
	if(TargetID != 0) //¡Ë?A¡ËOoAIAI¡Íi¢®I
	{	
		CPlayer* pTargetPlayer = (CPlayer*)g_pUserTable->FindUser(TargetID); 
		if(!pTargetPlayer) //AI ¡Ë¡þE¡§u¢®¨Ï¨Ïoo¡Ë?¡Ë¢ç ¡§u¨Ï¨£8¡Ë¡þe ¡§¡þe¢®¢´I¡ÍiaA¨Ï©ª¡§i©ö¡Ëi¨ù¡§¢®A
		{
			MUNPAMGR->NotifyAcceptMemberOtherMapServer(TargetID, (char*)pData->Data[eMu_MACMunpaname], atoi((char*)pData->Data[eMu_MACmunpaid]));
		}	
		else 
		{	
			FAMEMGR->ChangePlayerFame(pTargetPlayer, eFame_beMember);
			MUNPAMGR->DoAcceptMember(pTargetPlayer, atoi((char*)pData->Data[eMu_MACmunpaid]), (char*)pData->Data[eMu_MACMunpaname]);
			MUNPAMGR->MarkChange(pTargetPlayer, pTargetPlayer->GetMunpaIdx(), MUNPA_MEMBER); //mark
		}
	}
}

void RMunpaBanMember(LPQUERY pData, LPDBMESSAGE pMessage)
{	
	CPlayer* pMaster = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(atoi((char*)pData->Data[eMu_MBMerr]) != 0)
	{
		if(pMaster)
		{
			MSG_NAME nmsg;
			nmsg.Category = MP_MUNPA;
			nmsg.Protocol = MP_MUNPA_BAN_NACK;
			SafeStrCpy(nmsg.Name, (char*)pData->Data[eMu_MBMtargetname], MAX_NAME_LENGTH+1);
			
			pMaster->SendMsg(&nmsg,sizeof(nmsg)); //nack
		
		}
		return;
	}
	else
	{
		if(pMaster)
		{
			MSG_NAME_DWORD msg;
			msg.Category = MP_MUNPA;
			msg.Protocol = MP_MUNPA_BAN_ACK;
			msg.dwData = atoi((char*)pData->Data[eMu_MBMblast]);
			SafeStrCpy(msg.Name, (char*)pData->Data[eMu_MBMtargetname], MAX_NAME_LENGTH+1);
			pMaster->SendMsg(&msg,sizeof(msg)); //ack
		}
	}
	
	//A¡Ë¡þ¢®¨¡U CA¢®¢´¨ÏoAI¡§ui¢®¨¡¡Ë¢ç ¡Ë?A¡ËOoAI ¡ÍiC¡§uiAO8¡Ë¡þe ¡§u¡§¢®AI¡Íi¨Ï¡Ì¡Ë¡þ| ¡§u¡§¢®¡Ë¡ÍI¡Ë¡þe 0; ¡Ë¡þ¡ËcAICN¡Ë¡ÍU. 
	DWORD TargetID = atoi((char*)pData->Data[eMu_MBMtargetid]);
	if(TargetID != 0) //¡Ë?A¡ËOoAIAI¡Ë¡þe
	{	
		CPlayer* pTargetPlayer = (CPlayer*)g_pUserTable->FindUser(TargetID); 
		if(!pTargetPlayer)
		{	
			//¡Ë¡þ¨Ï¡Ì¡Íic ¡Ë?¡Ë¢çAIAu¡§¢®¡Ëc¢®¢´I ¡§¡þ¡Ë¡þ¨Ï©ª¢®i¡§u¢®¨Ï AO8¡Ë¡þe ¡Ë?¢®©­¢®¨¡a¡ÍiE ¡Ë¡þE¡§u¢®¨Ï¨Ïoo¢®¢´I ¡§¡þ¡Ë¡þ¨Ï©ª¡§o¡Ë¡ÍU. 
			MSG_DWORD2 msg;
			msg.Category = MP_MUNPA;
			msg.Protocol = MP_MUNPA_NOTIFYBAN_TO_MAPSERVER;
			msg.dwObjectID = atoi((char*)pData->Data[eMu_MBMtargetid]);
			msg.dwData1 = atoi((char*)pData->Data[eMu_MBMmunpaid]);
			msg.dwData2 = atoi((char*)pData->Data[eMu_MBMfame]);
		
			g_Network.Broadcast2AgentServer((char*)&msg,sizeof(msg));
		}	
		else 
		{
			FAMEMGR->SetPlayerFame(pTargetPlayer, atoi((char*)pData->Data[eMu_MBMfame]));
			MUNPAMGR->DoBanMember(pTargetPlayer);
			MUNPAMGR->MarkChange(pTargetPlayer, 0, MUNPA_NOTMEMBER); //mark
		}
	}
}

void RMunpaSecede(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(!pPlayer)
		return;
	if(atoi((char*)pData->Data[0]) == 0) //success
	{
		FAMEMGR->ChangePlayerFame(pPlayer, atoi((char*)pData->Data[1]));
		pPlayer->SetMunpaIdx(0);
		pPlayer->SetMunpaMemberRank(MUNPA_NOTMEMBER);

		MSGBASE msg;
		msg.Category = MP_MUNPA;
		msg.Protocol = MP_MUNPA_SECEDE_ACK;
		
		pPlayer->SendMsg(&msg, sizeof(msg));
		
		MUNPAMGR->MarkChange(pPlayer, 0, MUNPA_NOTMEMBER); //mark
	}
	else
	{
		MSGBASE msg;
		msg.Category = MP_MUNPA;
		msg.Protocol = MP_MUNPA_SECEDE_NACK;
			
		pPlayer->SendMsg(&msg, sizeof(msg));
	}
}

void RMunpaDenyMember(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD TargetID = atoi((char*)pData->Data[eMu_MDMtargetid]);
	DWORD MunpaID  = atoi((char*)pData->Data[eMu_MDMmunpaid]);
	BOOL bLast = atoi((char*)pData->Data[eMu_MDMbLast]);

	SEND_MUNPACCEPT_INFO msg;
	msg.Category = MP_MUNPA;
	msg.Protocol = MP_MUNPA_DENYMEMBER_ACK;
	msg.MunpaID = MunpaID;
	msg.TargetID = TargetID;
	msg.bLast = bLast;

	CPlayer* pMaster = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(pMaster)
	{
		if(atoi((char*)pData->Data[eMu_MDMerr]) != 0)
		{
			MSG_NAME nmsg;
			nmsg.Category = MP_MUNPA;
			nmsg.Protocol = MP_MUNPA_DENYMEMBER_NACK;
			SafeStrCpy(nmsg.Name, (char*)pData->Data[eMu_MDMtargetname], MAX_NAME_LENGTH+1);
			
			pMaster->SendMsg(&nmsg,sizeof(nmsg)); //nack
			return;
		}
		pMaster->SendMsg(&msg,sizeof(msg)); //ack
	}
	else
	{
		if(atoi((char*)pData->Data[eMu_MDMerr]) != 0)
			return;
	}
	
	if(TargetID != 0) //¡Ë?A¡ËOoAIAI¡Ë¡þe
	{
		CPlayer* pTargetPlayer = (CPlayer*)g_pUserTable->FindUser(TargetID); 
		if(!pTargetPlayer)
		{	
			MSGBASE msg;
			msg.Category = MP_MUNPA;
			msg.Protocol = MP_MUNPA_NOTIFY_DENYMEMBER_TO_MAPSERVER;
			msg.dwObjectID = TargetID;
									
			g_Network.Broadcast2AgentServer((char*)&msg,sizeof(msg));
		}	
		else 
			MUNPAMGR->DoDenyMember(pTargetPlayer);
	}
}

void RMunpaJoinMember(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(pPlayer)
	{
		if(atoi((char*)pData->Data[0]) != 0)
		{
			MSGBASE nmsg;
			nmsg.Category = MP_MUNPA;
			nmsg.Protocol = MP_MUNPA_JOINMEMBER_NACK;
					
			pPlayer->SendMsg(&nmsg,sizeof(nmsg)); //nack
			return;
		}

		DWORD ID = atoi((char*)pData->Data[1]);
		pPlayer->SetMunpaIdx(ID);
		pPlayer->SetMunpaMemberRank(MUNPA_SEMIMEMBER);
		pPlayer->SetMunpaName((char*)pData->Data[2]);

		//¨Ïo¡Ëc¡§¢®AAI¡Íi|¡§i©ö¡Ëi¨ù¡Ë?I AI¡Ë¡þ¢®¡¿; ¡§¡þ¡Ë¡þ¨Ï©ª¡Ë?		
		SEND_MUNPA_ACCEPT_TO_MAP msg;
		msg.Category = MP_MUNPA;
		msg.Protocol = MP_MUNPA_JOINMEMBER_ACK;
		msg.MunpaID = atoi((char*)pData->Data[1]);
		SafeStrCpy(msg.MunpaName, (char*)pData->Data[2], MAX_MUNPA_NAME+1); 
		
		pPlayer->SendMsg(&msg,sizeof(msg)); //ack
	}
}

void RMunpaJoinMemberCancel(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(pPlayer)
	{
		if(atoi((char*)pData->Data[0]) != 0)
		{
			MSGBASE nmsg;
			nmsg.Category = MP_MUNPA;
			nmsg.Protocol = MP_MUNPA_JOINMEMBERCANCEL_NACK;
					
			pPlayer->SendMsg(&nmsg,sizeof(nmsg)); //nack
			return;
		}

		pPlayer->SetMunpaIdx(0);
		pPlayer->SetMunpaMemberRank(MUNPA_NOTMEMBER);
		
		MSGBASE msg;
		msg.Category = MP_MUNPA;
		msg.Protocol = MP_MUNPA_JOINMEMBERCANCEL_ACK;
		
		pPlayer->SendMsg(&msg, sizeof(msg));
	}
}

void RMunpaHomeInfo(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(!pPlayer)
		return;

	MUNPA_HOME_INFO msg;
	memset(&msg, 0, sizeof(MUNPA_HOME_INFO));
	msg.Category = MP_MUNPA;
	msg.Protocol = MP_MUNPA_MUNPAHOME_ACK;
	msg.dwObjectID = pMessage->dwID;
	ASSERT(pMessage->dwResult <= MAX_MUNPA_BOARD_NUM);
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		msg.BoardInfo[i].BoardIDX = atoi((char*)pData[i].Data[eMu_MHIBoardIDX]);
		msg.BoardInfo[i].RSubjectRank = atoi((char*)pData[i].Data[eMu_MHIRSRank]);
		msg.BoardInfo[i].RContentRank = atoi((char*)pData[i].Data[eMu_MHIRCRank]);
		msg.BoardInfo[i].WContentRank = atoi((char*)pData[i].Data[eMu_MHIWCRank]);
		msg.BoardInfo[i].DContentRank = atoi((char*)pData[i].Data[eMu_MHIDCRank]);
		SafeStrCpy(msg.BoardInfo[i].MunpaBoardName, (char*)pData[i].Data[eMu_MHIBoardName], MAX_MUNPA_BOARD_NAME+1);
	}
	msg.BoardNum = pMessage->dwResult; 
	msg.MunpaID = atoi((char*)pData[0].Data[eMu_MHIMunpaid]);
	SafeStrCpy(msg.IntroMsg,(char*)pData[0].Data[eMu_MHIIntroMsg],MAX_MUNPA_INTROMSG+1);
	msg.Kind = atoi((char*)pData[0].Data[eMu_MHIMunpaKind]);
	SafeStrCpy(msg.MunpaName, (char*)pData[0].Data[eMu_MHIMunpaName], MAX_MUNPA_NAME+1);
	SafeStrCpy(msg.CreateDay, (char*)pData[0].Data[eMu_MHICreateDate], 16);
	SafeStrCpy(msg.MasterName, (char*)pData[0].Data[eMu_MHIMasterName], MAX_NAME_LENGTH+1);
	msg.MemberNum = atoi((char*)pData[0].Data[eMu_MHIMemberNum]);
	msg.MunpaFamous = atoi((char*)pData[0].Data[eMu_MHIFamous]);
	for(i=0; i<MAX_MUNPA_BATTLEKIND; ++i)
	{
		msg.MunpaBattleRecordInfo[i].Win = 0;
		msg.MunpaBattleRecordInfo[i].Loss = 0;
		msg.MunpaBattleRecordInfo[i].Total = 0;
	}
	CMunpa* pMunpa = MUNPAMGR->GetMunpa(msg.MunpaID);
	if(pMunpa)
		msg.bIsInMap = TRUE;
	else
		msg.bIsInMap = FALSE;

	pPlayer->SendMsg(&msg, sizeof(msg));
}

void RMunpaRegularMembersInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(!pPlayer)
		return;

	SEND_MUNPA_REGULARMEMBER_LIST msg;
	msg.Category = MP_MUNPA;
	msg.Protocol = MP_MUNPA_REGULARMEMBERS_INFO_ACK;
	memset(msg.MunpaRegularMemberList,0,sizeof(MUNPA_REGULARMEMBER_INFO)*MAX_MUNPA_REGULARMEMBERLIST_NUM);
	
	msg.TotalResultNum = atoi((char*)pData[0].Data[eMu_MMTotalNum]);
	msg.MunpaID = atoi((char*)pData[0].Data[eMu_MMunpaid]);

	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		msg.MunpaRegularMemberList[i].id = atoi((char*)pData[i].Data[eMu_MMemberid]);
		SafeStrCpy(msg.MunpaRegularMemberList[i].Name , (char*)pData[i].Data[eMu_MMemberName], MAX_NAME_LENGTH+1);
		msg.MunpaRegularMemberList[i].Rank = atoi((char*)pData[i].Data[eMu_MMemberRank]);
		msg.MunpaRegularMemberList[i].Level = atoi((char*)pData[i].Data[eMu_MMemberLevel]);
		msg.MunpaRegularMemberList[i].Famous = atoi((char*)pData[i].Data[eMu_MMemberFamous]);
		SafeStrCpy(msg.MunpaRegularMemberList[i].lastvisitdate, (char*)pData[i].Data[eMu_MMember_LastVisitDay], 16);
		SafeStrCpy(msg.MunpaRegularMemberList[i].entrydate, (char*)pData[i].Data[eMu_MMember_EntryDay], 16);
		msg.MunpaRegularMemberList[i].bLogin = atoi((char*)pData[i].Data[eMu_MMember_bLogin]);
	}

	pPlayer->SendMsg(&msg,sizeof(msg)); 	
}

void RMunpaJoinMembersInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(!pPlayer)
		return;
	
	SEND_MUNPA_SEMIMEMBER_LIST msg;
	memset(&msg,0,sizeof(msg));
	msg.Category = MP_MUNPA;
	msg.Protocol = MP_MUNPA_JOINMEMBERS_INFO_ACK;
	msg.TotalResultNum = atoi((char*)pData[0].Data[eMu_MJMTotalNum]);
	msg.MunpaID = atoi((char*)pData[0].Data[eMu_MJMMunpaid]);
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
			msg.MunpaMemberList[i].id = atoi((char*)pData[i].Data[eMu_MJMCharacterID]);
			SafeStrCpy(msg.MunpaMemberList[i].Name, (char*)pData[i].Data[eMu_MJMCharacterName], MAX_NAME_LENGTH+1);
			msg.MunpaMemberList[i].Level = atoi((char*)pData[i].Data[eMu_MJMCharacterLevel]);
			msg.MunpaMemberList[i].Famous = atoi((char*)pData[i].Data[eMu_MJMCharacterFame]);
			SafeStrCpy(msg.MunpaMemberList[i].RequestJoinDay, (char*)pData[i].Data[eMu_MJMRequestJoinDay], 16);
	}
	pPlayer->SendMsg(&msg, sizeof(msg));
}


void RMunpaBoardInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(!pPlayer)
		return;
	
	SEND_MUNPA_BOARD_INFO msg;
	memset(&msg,0,sizeof(msg));
	msg.Category = MP_MUNPA;
	msg.Protocol = MP_MUNPA_BOARD_INFO_ACK;
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		msg.CurMunpaBoardNum = atoi((char*)pData[i].Data[eMu_MCurBoardNum]);
		msg.BoardIDX[i] = atoi((char*)pData[i].Data[eMu_MBoardsIdx]);
		SafeStrCpy(msg.BoardNameNum[i].BoardName, (char*)pData[i].Data[eMu_MBoardsName], MAX_MUNPA_BOARD_NAME+1);
		msg.BoardNameNum[i].WritingsNum = atoi((char*)pData[i].Data[eMu_MTotalNum]);
	}
	
	pPlayer->SendMsg(&msg, sizeof(msg));
}

void RMunpaCreateBoard(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(!pPlayer)
		return;

	if(atoi((char*)pData[0].Data[eMu_MCurBoardNum]) == 0) //ERR
	{
		MSGBASE msg;
		msg.Protocol = MP_MUNPA_CREATEBOARD_NACK;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}

	SEND_MUNPA_BOARD_INFO msg;
	memset(&msg, 0, sizeof(SEND_MUNPA_BOARD_INFO));
	msg.Category = MP_MUNPA;
	msg.Protocol = MP_MUNPA_CREATEBOARD_ACK;
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		msg.CurMunpaBoardNum = atoi((char*)pData[i].Data[eMu_MCurBoardNum]);
		msg.BoardIDX[i] = atoi((char*)pData[i].Data[eMu_MBoardsIdx]);
		SafeStrCpy(msg.BoardNameNum[i].BoardName, (char*)pData[i].Data[eMu_MBoardsName], MAX_MUNPA_BOARD_NAME+1);
		msg.BoardNameNum[i].WritingsNum = atoi((char*)pData[i].Data[eMu_MTotalNum]);
	}
	
	
	pPlayer->SendMsg(&msg, sizeof(msg));
}

void RMunpaBoardRankInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(!pPlayer)
		return;

	SEND_MUNPA_BOARD_RANK_INFO msg;
	msg.Category = MP_MUNPA;
	msg.Protocol = MP_MUNPA_BOARDRANK_INFO_ACK;
	msg.ID = atoi((char*)pData->Data[eMu_MBoardidx]);
	msg.RSubjectRank = atoi((char*)pData->Data[eMu_MRSubjectRank]);
	msg.RContentRank = atoi((char*)pData->Data[eMu_MRContentRank]);
	msg.WContentRank = atoi((char*)pData->Data[eMu_MWContentRank]);
	msg.DContentRank = atoi((char*)pData->Data[eMu_MDContentRank]);
	SafeStrCpy(msg.MunpaBoardName, (char*)pData->Data[eMu_MBoardName], MAX_MUNPA_BOARD_NAME+1);

	pPlayer->SendMsg(&msg, sizeof(msg));
}

void RMunpaBoardRankInfoUpdate(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(!pPlayer)
		return;
	
	SEND_MUNPA_BOARD_INFO msg;
	memset(&msg,0,sizeof(msg));
	msg.Category = MP_MUNPA;
	msg.Protocol = MP_MUNPA_BOARD_MODIFYRANK_INFO_ACK;
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		msg.CurMunpaBoardNum = atoi((char*)pData[i].Data[eMu_MCurBoardNum]);
		msg.BoardIDX[i] = atoi((char*)pData[i].Data[eMu_MBoardsIdx]);
		//strcpy(msg.BoardNameNum[i].BoardName, (char*)pData[i].Data[eMu_MBoardsName]);
		SafeStrCpy(msg.BoardNameNum[i].BoardName, (char*)pData[i].Data[eMu_MBoardsName], MAX_MUNPA_BOARD_NAME+1);
		msg.BoardNameNum[i].WritingsNum = atoi((char*)pData[i].Data[eMu_MTotalNum]);
	}
	
	pPlayer->SendMsg(&msg, sizeof(msg));
}

void RMunpaBoardReadContent(LPLARGEQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(!pPlayer)
		return;

	int err = atoi((char*)pData->Data[0]); 
	if(err!=0)
	{
		DWORD ErrorCode;
		// ¢®¨¡O¡§oA¨Ïo¢®¨¡AI xAcCIAo ¡§uE¢®¨¡A¨Ï©ª¡§¡Ì ¢®ief¡ÍiC¡§uu¡§o4I¡Ë¡ÍU.
		if(err==1)
			ErrorCode = ERROR_MUNPABOARDCONTENTS_NOCONTENTS;
		else if(err==2)
			ErrorCode = ERROR_MUNPABOARDCONTENTS_NOAUTHORITY;
		MSG_DWORD msg;
		msg.Category = MP_MUNPA;
		msg.Protocol = MP_MUNPA_BOARD_CONTENTS_NACK;
		msg.dwData = ErrorCode;
		pPlayer->SendMsg(&msg,sizeof(msg));
		return;
	}
	
	SEND_MUNPA_BOARD_CONTENT_INFO msg;
	memset(&msg,0,sizeof(msg));
	msg.Category = MP_MUNPA;
	msg.Protocol = MP_MUNPA_BOARD_CONTENTS_ACK;
	msg.BoardListInfo.Seq = atoi((char*)pData->Data[eMu_MBCIContentNum]);
	msg.BoardListInfo.Count = atoi((char*)pData->Data[eMu_MBCICount]);
	msg.BoardListInfo.WritingsIDX = atoi((char*)pData->Data[eMu_MBCIContentIDx]);
	SafeStrCpy(msg.BoardListInfo.RegDate, (char*)pData->Data[eMu_MBCIRegDate], 32);
	SafeStrCpy(msg.BoardListInfo.Subject, (char*)pData->Data[eMu_MBCISubject], MAX_MUNPA_BOARD_SUBJECT+1);
	SafeStrCpy(msg.BoardListInfo.WriterName, (char*)pData->Data[eMu_MBCIWriter], MAX_NAME_LENGTH+1);
	// ¢®¨¡¨Ï¨£¨Ïoe AU¡Ë¡þ¢®I¢®¨úa
//	char* pc = msg.BoardListInfo.WriterName;
//	while(*pc != 0)
//	{
//		if(*pc++ == ' ')
//			*--pc = 0;
//	}

	SafeStrCpy(msg.Content, (char*)pData->Data[eMu_MBCIContent], MAX_MUNPA_BOARD_CONTENT+1);
//	int a = strlen(msg.Content);
	msg.DCRank = atoi((char*)pData->Data[eMu_MBCDCRank]);
	msg.WCRank = atoi((char*)pData->Data[eMu_MBCWCRank]);
	

	pPlayer->SendMsg(&msg, msg.GetMsgLength());
}

void RMunpaBoardSubjectList(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(!pPlayer)
		return;

	int err = atoi((char*)pData->Data[eMu_MBLITotalPage]);
	if(err == -1)
	{
		MSGBASE ermsg;
		ermsg.Category = MP_MUNPA;
		ermsg.dwObjectID = pMessage->dwID;
		ermsg.Protocol = MP_MUNPA_BOARD_SUBJECTLIST_NACK;
		pPlayer->SendMsg(&ermsg, sizeof(ermsg));
		return;
	}

	SYSTEMTIME ti;
	GetLocalTime(&ti);
	
	char tempDate[64];
	SEND_MUNPA_BOARD_LIST_INFO msg;
	memset(&msg,0,sizeof(msg));
	msg.Category = MP_MUNPA;
	msg.Protocol = MP_MUNPA_BOARD_SUBJECTLIST_ACK;
	msg.TotalPage = atoi((char*)pData[0].Data[eMu_MBLITotalPage]);
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		msg.BoardListInfo[i].Count = atoi((char*)pData[i].Data[eMu_MBLICount]);
		SafeStrCpy(tempDate, (char*)pData[i].Data[eMu_MBLIRegDate], 64);
		if(IsSameDay(tempDate, (WORD)ti.wYear, (BYTE)ti.wMonth, (BYTE)ti.wDay))
			SafeStrCpy(tempDate, (char*)pData[i].Data[eMu_MBLIRegTime], 64);

		SafeStrCpy(msg.BoardListInfo[i].RegDate, tempDate, 32);

		msg.BoardListInfo[i].Seq = atoi((char*)pData[i].Data[eMu_MBLIContentNum]);
		strncpy(msg.BoardListInfo[i].Subject, (char*)pData[i].Data[eMu_MBLISubject], 24);
		SafeStrCpy(msg.BoardListInfo[i].WriterName, (char*)pData[i].Data[eMu_MBLIWriter],MAX_NAME_LENGTH+1);
		msg.BoardListInfo[i].WritingsIDX = atoi((char*)pData[i].Data[eMu_MBLIContentIDx]);
		msg.BoardListInfo[i].Depth = atoi((char*)pData[i].Data[eMu_MBLIDepth]);
	}
	
	pPlayer->SendMsg(&msg, sizeof(msg));
}

void RMunpaBoardModify(LPLARGEQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(!pPlayer)
		return;

	MSGBASE ermsg;
	ermsg.Category = MP_MUNPA;
	ermsg.dwObjectID = pMessage->dwID;

	int err = atoi((char*)pData->Data[0]);
	if(err == 0)
	{
		ermsg.Protocol = MP_MUNPA_BOARD_MODIFY_NACK;
	}
	else if(err == 1)
	{
		ermsg.Protocol = MP_MUNPA_BOARD_MODIFY_ACK;
	}
	ermsg.dwObjectID = pMessage->dwID;
	pPlayer->SendMsg(&ermsg, sizeof(ermsg));
}

void RMunpaBoardWrite(LPLARGEQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(!pPlayer)
		return;

	if(atoi((char*)pData->Data[0]) == 0)
	{
		WORD BOARDID = atoi((char*)pData->Data[1]);
		MunpaBoardSubjectList(pMessage->dwID, BOARDID, 1, 10);
	}
	else
	{
		MSG_DWORD msg;
		msg.Category = MP_MUNPA;
		msg.Protocol = MP_MUNPA_BOARD_WRITE_NACK;
		msg.dwData = atoi((char*)pData->Data[0]);
		pPlayer->SendMsg(&msg,sizeof(msg));
	}
}


void RMunpaBoardDeleteContents(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(!pPlayer)
		return;

	MSGBASE msg;
	msg.Category = MP_MUNPA;

	int Err = atoi((char*)pData->Data[0]);
	if(Err == 0)
	{
		msg.Protocol = MP_MUNPA_BOARD_DELETE_NACK;
	}
	else msg.Protocol = MP_MUNPA_BOARD_DELETE_ACK;
		
	msg.dwObjectID = pMessage->dwID;
	pPlayer->SendMsg(&msg, sizeof(msg));
}

void RMunpaMoneyPutInOut(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if(atoi((char*)pData->Data[eMu_MWMunpaID]) == 0)
		return;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
//	if(pPlayer)
//	{
//		pPlayer->SetMoneyNoMsgNoUpdate(atoi((char*)pData->Data[eMu_MWPlayerMoney]));
//	}
	MSG_DWORD msg;
	msg.Category = MP_MUNPA;
	
	if(atoi((char*)pData->Data[eMu_MWPutInOut]) == 1)
		msg.Protocol = MP_MUNPA_PUTIN_MONEY_ACK;
	else 
		msg.Protocol = MP_MUNPA_PUTOUT_MONEY_ACK;
//	msg.dwData1 = pMessage->dwID;
//	msg.dwData2 = atoi((char*)pData->Data[eMu_MWPlayerMoney]);
	msg.dwData = atoi((char*)pData->Data[eMu_MWMunpaMoney]);
	
	DWORD MunpaID = atoi((char*)pData->Data[eMu_MWMunpaID]);
	CMunpa* pMunpa = MUNPAMGR->GetMunpa(MunpaID);
	pMunpa->SendtoWarehousePlayerAll(&msg, sizeof(msg));

//	pMunpa->SetMoney(msg.dwData);
}*/


/*
void RCharacterPyogukItemInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pMessage->dwID);
	if(!pPlayer)
	{
		return;
	}
	
	ITEMBASE pyogukitem;
	DWORD StartItemDBIDx;
	BYTE PyogukNum;
	DWORD UserIDX;
	
	if(pMessage->dwResult < MAX_MUNPALOAD_LIST) //¡§u¡§¢®AIAU; ¡§ui¡ËO¢®i¢®¨¡O ¨Ï¡À¢®(ui¡Ë?AAo? 100¢®¨¡¨Ï©ª
	{
		PyogukNum = atoi((char*)pData[0].Data[ePyo_MaxPosition]);
		UserIDX = atoi((char*)pData[0].Data[ePyo_Userid]);
		pPlayer->InitPyogukInfo(PyogukNum, UserIDX);

		if(UserIDX == 0)
		{
			ASSERT(0);
			
			FILE* fp = fopen("PYOGUK_ERROR_LOG.txt","a+");
			fprintf(fp, "CharacterPyogukItemInfo¡Ë?¡Ë¢ç¡§u¢®¨Ï  UserIDX : %d ", atoi((char*)pData[0].Data[ePyo_Userid]));
			fprintf(fp,", CharacterIDX : %d\n", atoi((char*)pData[0].Data[ePyo_Userid+1]));
			fclose(fp);
			return;
		}
		
		for(DWORD i=0; i<pMessage->dwResult; ++i)
		{
			POSTYPE ItemPos = atoi((char*)pData[i].Data[ePyo_Position]);
			ItemPos -= PYOGUKITEM_STARTPOSITION;
			
			pyogukitem.Durability = atoi((char*)pData[i].Data[ePyo_Durability]);
			pyogukitem.wIconIdx = atoi((char*)pData[i].Data[ePyo_Idx]);
			pyogukitem.dwDBIdx = atoi((char*)pData[i].Data[ePyo_DBIdx]);
			pyogukitem.Position = atoi((char*)pData[i].Data[ePyo_Position]);
			pyogukitem.QuickPosition = 0;
			
			pPlayer->AddPyogukItem(&pyogukitem);
		}
		// ¡Íi¢®¢´
		PyogukMoneyInit(pPlayer->GetID());
	}
	else 
	{
		PyogukNum = atoi((char*)pData[0].Data[ePyo_MaxPosition]);
		UserIDX = atoi((char*)pData[0].Data[ePyo_Userid]);
		pPlayer->InitPyogukInfo(PyogukNum, UserIDX);
		if(UserIDX == 0)
		{
			ASSERT(0);
			return;
		}

		for(DWORD i=0; i<pMessage->dwResult; ++i)
		{
			POSTYPE ItemPos = atoi((char*)pData[i].Data[ePyo_Position]);
			ItemPos -= PYOGUKITEM_STARTPOSITION;
			
			pyogukitem.Durability = atoi((char*)pData[i].Data[ePyo_Durability]);
			pyogukitem.wIconIdx = atoi((char*)pData[i].Data[ePyo_Idx]);
			pyogukitem.dwDBIdx = atoi((char*)pData[i].Data[ePyo_DBIdx]);
			pyogukitem.Position = atoi((char*)pData[i].Data[ePyo_Position]);
			pyogukitem.QuickPosition = 0;
			
			StartItemDBIDx = pyogukitem.dwDBIdx;
			
			pPlayer->AddPyogukItem(&pyogukitem);
		}
		CharacterPyogukItemInfo(pMessage->dwID, MAX_MUNPALOAD_LIST);
	}

}

void RPyogukMoneyInit(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pMessage->dwID);
	if(!pPlayer)
	{
		return;
	}

	for(int i=0; i<MAX_PYOGUK_NUM; ++i)
		pPlayer->SetPyogukMoney(atoi((char*)pData->Data[0+i]),i); 
}
*/


void RCharacterPyogukInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer)
		return;

	BYTE PyogukNum;

	if( pMessage->dwResult == 0 )
	{
		PyogukNum = 0;
	}
	else
	{
		PyogukNum = atoi((char*)pData->Data[ePyo_PyogukNum]);
	}

	pPlayer->InitPyogukInfo(PyogukNum, atoi((char*)pData->Data[ePyo_Money]));

	pPlayer->SetInitState(PLAYERINITSTATE_PYOGUK_INFO,MP_USERCONN_GAMEIN_SYN);
}

void RCharacterPyogukItemInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

	if(pPlayer->GetInited() == FALSE)
		return;

	PYOGUK_ITEM PyogukItem;
	memset(&PyogukItem, 0, sizeof(PYOGUK_ITEM));

	POSTYPE ItemPos;
	BYTE i;
	if(atoi((char*)pData[0].Data[ePI_DBIdx]) != 0)
	{
		for( i = 0 ; i < pMessage->dwResult ; ++i )
		{	
			ItemPos = atoi((char*)pData[i].Data[ePI_Positon]);
			if(TP_PYOGUK_START<= ItemPos && ItemPos < TP_PYOGUK_END)
			{
				ItemPos -= TP_PYOGUK_START;
				PyogukItem.Pyoguk[ItemPos].dwDBIdx	= atoi((char*)pData[i].Data[ePI_DBIdx]);
				PyogukItem.Pyoguk[ItemPos].wIconIdx		= atoi((char*)pData[i].Data[ePI_IconIdx]);
				PyogukItem.Pyoguk[ItemPos].Position		= atoi((char*)pData[i].Data[ePI_Positon]);
				PyogukItem.Pyoguk[ItemPos].QuickPosition	= atoi((char*)pData[i].Data[ePI_QPosition]);
				PyogukItem.Pyoguk[ItemPos].Durability		= atoi((char*)pData[i].Data[ePI_Durability]);
				PyogukItem.Pyoguk[ItemPos].RareIdx		= atoi((char*)pData[i].Data[ePI_RareIdx]);

				// magi82(33)
				ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(PyogukItem.Pyoguk[ItemPos].wIconIdx);
				if( pInfo && pInfo->ItemKind & eTITAN_EQUIPITEM )
				{
					TITAN_ENDURANCE_ITEMINFO* pEndurance = pPlayer->GetTitanManager()->GetTitanItemEnduranceInfo(PyogukItem.Pyoguk[ItemPos].dwDBIdx);
					if( !pEndurance )
					{
						pPlayer->GetTitanManager()->MakeNewEnduranceInfo(pPlayer, &PyogukItem.Pyoguk[ItemPos], eExceptionPyoguk);				
					}
				}
			}
			else
			{
				//¡§u¡§¢®AIAU ¡§¢®¢®AAo¡§uC ¡Ë?¡Ë¢ç¢®¢´?
				ASSERT(0);
			}
		}
	}
	pPlayer->InitPyogukItemInfo(&PyogukItem);
	
	if( pMessage->dwResult >= MAX_PYOGUKLOAD_ITEM)
		CharacterPyogukItemInfo(pMessage->dwID, atoi((char*)pData[pMessage->dwResult-1].Data[ePI_DBIdx]));
	else //¡Ë¡ÍU A¡§¡Ë¡§ui¡Ë?8e A¢®©­¡ËOoAI¡§u¨Ï¡Ì¡§¢®¡Ëc¡Ë?¡Ë¢ç ¡§¡þ¡Ë¡þ¨Ï©ª¢®iAU. 
		PYOGUKMGR->SendPyogukItemInfo(pPlayer);
}

void RFriendNotifyLogouttoClient(LPQUERY pData, LPDBMESSAGE pMessage)
{
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		DWORD ID = atoi((char*)pData[i].Data[eFr_LToPlayerID]);
		CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(ID);
		if(!pPlayer)
		{
			MSG_NAME_DWORD cmsg;
			cmsg.Category = MP_FRIEND;
			cmsg.Protocol = MP_FRIEND_LOGOUT_NOTIFY_TO_AGENT;
			SafeStrCpy(cmsg.Name, (char*)pData[0].Data[eFr_LLogoutName], MAX_NAME_LENGTH+1);
			cmsg.dwData = pMessage->dwID;
			cmsg.dwObjectID = atoi((char*)pData[i].Data[eFr_LToPlayerID]);

			g_Network.Broadcast2AgentServer((char*)&cmsg,sizeof(cmsg));
		}
		else
		{
			MSG_NAME_DWORD msg;
			msg.Category = MP_FRIEND;
			msg.Protocol = MP_FRIEND_LOGOUT_NOTIFY_TO_CLIENT;
			SafeStrCpy(msg.Name, (char*)pData[0].Data[eFr_LLogoutName], MAX_NAME_LENGTH+1);
			msg.dwData = pMessage->dwID;			
			pPlayer->SendMsg(&msg, sizeof(msg));
		}
	}
}

void RWantedLoad(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WANTEDINFO_LOAD info;
	memset(&info, 0, sizeof(WANTEDINFO_LOAD));
	
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		info.m_WantedInfo[i].WantedIDX = (DWORD)atoi((char*)pData[i].Data[eWa_LWantedIDX]);
		info.m_WantedInfo[i].WantedChrIDX = (DWORD)atoi((char*)pData[i].Data[eWa_LPlayerID]);
		info.m_WantedInfo[i].TotalPrize = (DWORD)atoi((char*)pData[i].Data[eWa_LTotlaPrize]);
		SafeStrCpy(info.m_WantedInfo[i].WantedChrName, (char*)pData[i].Data[eWa_LPlayerName], MAX_NAME_LENGTH+1);
		info.m_WantedInfo[i].RegistChrIDX = (DWORD)atoi((char*)pData[i].Data[eWa_LRegistChridx]);
		info.m_WantedInfo[i].TotalVolunteerNum = (int)atoi((char*)pData[i].Data[eWa_LTotalVolunteerNum]);
		info.count++;
	}
	
	WANTEDMGR->LoadWantedList(&info);	
	
	if(pMessage->dwResult >= MAX_QUERY_RESULT)
		WantedLoad(info.m_WantedInfo[i-1].WantedIDX);
	else
	//	g_pServerSystem->SetStart(TRUE);
		GuildLoadGuild(0);
}

void RWantedInfoByUserLogIn(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pMessage->dwID);
	if(!pPlayer)
		return;	

	SEND_WANTEDLIST msg;
	memset(&msg, 0, sizeof(msg));
	msg.Category = MP_WANTED;
	msg.Protocol = MP_WANTED_INFO_BY_USERLOGIN;
//	msg.WantedIdx = (WORD)atoi((char*)pData[0].Data[eWa_LIMyWantedIDX]);
	msg.WantedIdx = (WANTEDTYPE)atoi((char*)pData[0].Data[eWa_LIMyWantedIDX]);
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{ //¨Ïo¢®iv¢®¨¡C 1¢®¨¡¨Ï©ª¡Ë¡ÍA ¨Ï©ªN¡§ui¡Ë?A¡Ë¡ÍU.
//		if((WORD)atoi((char*)pData[i].Data[eWa_LIWantedIDX]) != 0)
		if((WANTEDTYPE)atoi((char*)pData[i].Data[eWa_LIWantedIDX]) != 0)
		{
//			msg.List[i].WantedIDX = (WORD)atoi((char*)pData[i].Data[eWa_LIWantedIDX]);
			msg.List[i].WantedIDX = (WANTEDTYPE)atoi((char*)pData[i].Data[eWa_LIWantedIDX]);
			msg.List[i].WantedChrID = (DWORD)atoi((char*)pData[i].Data[eWa_LIWantedChrID]);
			SafeStrCpy(msg.List[i].WantedName, (char*)pData[i].Data[eWa_LIWantedName], MAX_NAME_LENGTH+1);
			SafeStrCpy(msg.List[i].RegistDate, (char*)pData[i].Data[eWa_LIRegistDate], 11);
//			pPlayer->AddWanted((WORD)atoi((char*)pData[i].Data[eWa_LIWantedIDX]));
			pPlayer->AddWanted((WANTEDTYPE)atoi((char*)pData[i].Data[eWa_LIWantedIDX]));
		}		
	}	
	pPlayer->SetWantedIdx(msg.WantedIdx);
	pPlayer->SendMsg(&msg, sizeof(msg));
}

void RWantedRegist(LPQUERY pData, LPDBMESSAGE pMessage)
{
	WANTEDINFO Info;
//	Info.WantedIDX =  (WORD)atoi((char*)pData->Data[eWa_RWanted_idx]);
	Info.WantedIDX =  (WANTEDTYPE)atoi((char*)pData->Data[eWa_RWanted_idx]);
	Info.TotalPrize =  (MONEYTYPE)atoi((char*)pData->Data[eWa_RTotalPrize]);
	Info.WantedChrIDX = (DWORD)atoi((char*)pData->Data[eWa_RWantedChr_idx]);
	SafeStrCpy(Info.WantedChrName, (char*)pData->Data[eWa_RWanted_name], MAX_NAME_LENGTH+1);
	Info.RegistChrIDX = pMessage->dwID;
	Info.TotalVolunteerNum = 0;
	WANTEDMGR->RegistKillerResult(&Info);
}

void RWantedComplete(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = NULL;

	SEND_WANTED_NOTCOMPLETE msg;
	msg.Category = MP_WANTED;
	msg.Protocol = MP_WANTED_NOTIFY_NOTCOMPLETE_TO_MAP;
	msg.WantedIDX = (WANTEDTYPE)pMessage->dwID;
	SafeStrCpy(msg.CompleterName, (char*)pData[0].Data[eWa_CCompleterName], MAX_NAME_LENGTH+1);
	msg.type = (BYTE)atoi((char*)pData[0].Data[eWa_CCompleteType]);
	
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		msg.dwObjectID = (DWORD)atoi((char*)pData[i].Data[eWa_CCharacteridx]);
		pPlayer = (CPlayer*)g_pUserTable->FindUser((DWORD)atoi((char*)pData[i].Data[eWa_CCharacteridx]));
		if(!pPlayer)
		{
			//broadcasting
			g_Network.Send2AgentServer((char*)&msg, sizeof(msg));
		}
		else
		{	
			WANTEDMGR->CanNotComplete(pPlayer, (WANTEDTYPE)pMessage->dwID, 
									(char*)pData[0].Data[eWa_CCompleterName], 
									(BYTE)atoi((char*)pData[0].Data[eWa_CCompleteType]));
		}
	}
}

void RWantedDestroyed(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = NULL;
	MSG_DWORD msg;
	msg.Category = MP_WANTED;
	msg.dwData = pMessage->dwID;

	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		msg.dwObjectID = (DWORD)atol((char*)pData[i].Data[0]);
		pPlayer = (CPlayer*)g_pUserTable->FindUser(msg.dwObjectID);
		if(pPlayer == 0)
		{
			//broadcasting
			msg.Protocol = MP_WANTED_DESTROYED_TO_MAP;
			g_Network.Send2AgentServer((char*)&msg, sizeof(msg));
		}
		else
		{	
			if( pPlayer->IsWantedOwner((WANTEDTYPE)pMessage->dwID) == TRUE )
			{
				pPlayer->DeleteWanted((WANTEDTYPE)pMessage->dwID);
				msg.Protocol = MP_WANTED_DESTROYED_TO_CLIENT;
				pPlayer->SendMsg(&msg, sizeof(msg));
			}
		}
	}
}

void RWantedOrderedList(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(!pPlayer)
		return;
	
	SEND_WANTNPCLIST msg;
	memset(&msg, 0, sizeof(msg));
	msg.Category = MP_WANTED;
	msg.Protocol = MP_WANTED_ORDERLIST_ACK;
	msg.TotalPage = (WORD)atoi((char*)pData[0].Data[eWa_OLTotal]);
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		msg.WantNpcList[i].WantedIDX = (WANTEDTYPE)atoi((char*)pData[i].Data[eWa_OLWantedIDX]);
		SafeStrCpy(msg.WantNpcList[i].WantedName, (char*)pData[i].Data[eWa_OLWantedChrName], MAX_NAME_LENGTH+1);
		msg.WantNpcList[i].Prize = (MONEYTYPE)atoi((char*)pData[i].Data[eWa_OLPrize]);
		msg.WantNpcList[i].VolunteerNum = (BYTE)atoi((char*)pData[i].Data[eWa_OLVolunteerNum]);
	}
	
	pPlayer->SendMsg(&msg, sizeof(msg));
}

void RWantedMyList(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(!pPlayer)
		return;
	
	SEND_WANTNPCLIST msg;
	memset(&msg, 0, sizeof(msg));
	msg.Category = MP_WANTED;
	msg.Protocol = MP_WANTED_MYLIST_ACK;
	msg.TotalPage = (WORD)atoi((char*)pData[0].Data[eWa_OLTotal]);
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		msg.WantNpcList[i].WantedIDX = (WANTEDTYPE)atoi((char*)pData[i].Data[eWa_OLWantedIDX]);
		SafeStrCpy(msg.WantNpcList[i].WantedName, (char*)pData[i].Data[eWa_OLWantedChrName], MAX_NAME_LENGTH+1);
		msg.WantNpcList[i].Prize = (MONEYTYPE)atoi((char*)pData[i].Data[eWa_OLPrize]);
		msg.WantNpcList[i].VolunteerNum = (BYTE)atoi((char*)pData[i].Data[eWa_OLVolunteerNum]);
	}
	
	pPlayer->SendMsg(&msg, sizeof(msg));
}

void RWantedSearch(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(!pPlayer)
		return;
	
	SEND_WANTNPCLIST msg;
	memset(&msg, 0, sizeof(msg));
	msg.Category = MP_WANTED;
	msg.Protocol = MP_WANTED_SEARCH_ACK;
	msg.TotalPage = (WORD)atoi((char*)pData[0].Data[eWa_OLTotal]);
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		msg.WantNpcList[i].WantedIDX = (WANTEDTYPE)atoi((char*)pData[i].Data[eWa_OLWantedIDX]);
		SafeStrCpy(msg.WantNpcList[i].WantedName, (char*)pData[i].Data[eWa_OLWantedChrName], MAX_NAME_LENGTH+1);
		msg.WantNpcList[i].Prize = (MONEYTYPE)atoi((char*)pData[i].Data[eWa_OLPrize]);
		msg.WantNpcList[i].VolunteerNum = (BYTE)atoi((char*)pData[i].Data[eWa_OLVolunteerNum]);
	}
	
	pPlayer->SendMsg(&msg, sizeof(msg));
}
/*
//////////////////////////////////////////////////////////////////

void RFameMunpaUpdate(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD MunpaIDX=0;
	FAMETYPE Val = 0;
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		MunpaIDX = (DWORD)atoi((char*)pData[i].Data[eFa_MunpaIDX]);
		Val = (WORD)atoi((char*)pData[i].Data[eFa_Fameval]);
		FAMEMGR->SetMunpaFame(MunpaIDX, Val);
	}
	
	if(pMessage->dwID >= MAX_MUNPALOAD_LIST)
	{
		FameMunpaUpdate(MunpaIDX, pMessage->dwID);
	}
}*/


// Quest
void RQuestTotalInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	SEND_QUEST_TOTALINFO msg;
	memset(&msg.QuestList, 0, sizeof(msg.QuestList));

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

	// client¡Ë?¡Ë¢ç ¡§¡þ¡Ë¡þ¨Ï©ª¡§u ¢®¨ú¡Ë¡þvA¡§u ¢®iy¡§u¡§¡þ..
	msg.Category = MP_QUEST;
	msg.Protocol = MP_QUEST_TOTALINFO;
	msg.dwObjectID = pMessage->dwID;

	if( pMessage->dwResult == 0 )
	{
		msg.wCount = 0;
		pPlayer->SendMsg( &msg, msg.GetSize() );
	}
	else
	{
		CQuestBase* pQuest;
		QFLAG flag;
		WORD QuestIdx;
		WORD count = 0;

		for( DWORD i = 0 ; i < pMessage->dwResult ; ++i )
		{			
			flag.value = (QSTATETYPE)atoi((char*)pData[i].Data[eQi_QuestState]);
			QuestIdx = (WORD)atoi((char*)pData[i].Data[eQi_QuestIdx]);

			if( !flag.IsSet(1) )
			{
				pQuest = new CQuestBase;
				pQuest->Init(QuestIdx, flag.value);

				pPlayer->AddQuest( pQuest );

				// A¢®©­¡ËOoAI¡§u¨Ï¡Ì¡§¢®¡Ëc ¡Ë¡þ¡§¡©¡§u¡§uAo ¡§uA¡§¢®A..
				msg.QuestList[count].QuestIdx = QuestIdx;
				msg.QuestList[count].state = flag.value;
				count++;
			}
		}

		msg.wCount = count;
		pPlayer->SendMsg( &msg, msg.GetSize() );
	}
} 

void RQuestMainQuestLoad(LPQUERY pData, LPDBMESSAGE pMessage)
{
	SEND_MAINQUEST_DATA msg;
	memset(&msg.QuestList, 0, sizeof(msg.QuestList));
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

	// client¡Ë?¡Ë¢ç ¡§¡þ¡Ë¡þ¨Ï©ª¡§u ¢®¨ú¡Ë¡þvA¡§u ¢®iy¡§u¡§¡þ..
	msg.Category = MP_QUEST;
	msg.Protocol = MP_QUEST_MAINDATA_LOAD;
	msg.dwObjectID = pMessage->dwID;				// UserID

	if( pMessage->dwResult == 0 )
	{
		msg.wCount = 0;
		pPlayer->SendMsg( &msg, msg.GetSize() );
	}
	else
	{
		DWORD state;
		WORD QuestIdx=0;
		DWORD EndParam=0;
		DWORD Time=0;
		BYTE bCheckType = 0;
		DWORD dwCheckTime = 0;
		WORD count = 0;

		for( DWORD i = 0 ; i < pMessage->dwResult ; ++i )
		{			
			QuestIdx = (WORD)atoi((char*)pData[i].Data[eQi_QuestIdx]);
			state = (QSTATETYPE)atoi((char*)pData[i].Data[eQi_QuestState]);
			EndParam = (DWORD)atoi((char*)pData[i].Data[eQi_EndParam]);
			Time = (DWORD)atoi((char*)pData[i].Data[eQi_RegistTime]);
			bCheckType = (BYTE)atoi((char*)pData[i].Data[eQi_CheckType]);
			dwCheckTime = (DWORD)atoi((char*)pData[i].Data[eQi_CheckTime]);
			
			// A¢®©­¡ËOoAI¡§u¨Ï¡Ì¡§¢®¡Ëc ¡Ë¡þ¡§¡©¡§u¡§uAo ¡§uA¡§¢®A..
			msg.QuestList[count].QuestIdx = QuestIdx;
			msg.QuestList[count].state.value = state;
			msg.QuestList[count].EndParam = EndParam;
			msg.QuestList[count].Time = Time;
			msg.QuestList[count].CheckType = bCheckType;
			msg.QuestList[count].CheckTime = dwCheckTime;
						
			if( QUESTMGR->SetMainQuestData( pPlayer, QuestIdx, state, EndParam, Time, bCheckType, dwCheckTime ) == FALSE )
			{
				msg.QuestList[count].EndParam = 1;

				QuestMainQuestUpdateToDB( pMessage->dwID, QuestIdx, state, 1, Time );
			}
			count++;
		}
		
		msg.wCount = count;
		pPlayer->SendMsg( &msg, msg.GetSize() );		

		if( count >= 100 )
		{
			QuestMainQuestLoad( pMessage->dwID, (int)QuestIdx );
			return;
		}
	}	

	// ¸ÞÀÎ Äù½ºÆ® ·ÎµåÈÄ Äù½ºÆ®¾ÆÀÌÅÛÁ¤º¸ ¾ò¾î¿Â´Ù...
	QuestItemload( pMessage->dwID );
//	QuestSubQuestLoad( pMessage->dwID );
}


void RQuestSubQuestLoad(LPQUERY pData, LPDBMESSAGE pMessage)
{
	SEND_SUBQUEST_DATA msg;
	memset(&msg.QuestList, 0, sizeof(msg.QuestList));

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

	// client¡Ë?¡Ë¢ç ¡§¡þ¡Ë¡þ¨Ï©ª¡§u ¢®¨ú¡Ë¡þvA¡§u ¢®iy¡§u¡§¡þ..
	msg.Category = MP_QUEST;
	msg.Protocol = MP_QUEST_SUBDATA_LOAD;
	msg.dwObjectID = pMessage->dwID;				// UserID

	if( pMessage->dwResult == 0 )
	{
		msg.wCount = 0;
		pPlayer->SendMsg( &msg, msg.GetSize() );
	}
	else
	{
		WORD QuestIdx;
		WORD SubQuestIdx;
		DWORD state;
		DWORD time;
		WORD count = 0;

		for( DWORD i = 0 ; i < pMessage->dwResult ; ++i )
		{			
			QuestIdx = (WORD)atoi((char*)pData[i].Data[eQsi_QuestIdx]);
			SubQuestIdx = (WORD)atoi((char*)pData[i].Data[eQsi_SubQuestIdx]);
			state = (DWORD)atoi((char*)pData[i].Data[eQsi_Data]);
			time = (DWORD)atoi((char*)pData[i].Data[eQsi_RegistTime]);
			
			// A¢®©­¡ËOoAI¡§u¨Ï¡Ì¡§¢®¡Ëc ¡Ë¡þ¡§¡©¡§u¡§uAo ¡§uA¡§¢®A..
			msg.QuestList[count].QuestIdx = QuestIdx;
			msg.QuestList[count].SubQuestIdx = SubQuestIdx;
			msg.QuestList[count].state = state;
			msg.QuestList[count].time = time;
			count++;

			QUESTMGR->SetSubQuestData( pPlayer, QuestIdx, SubQuestIdx, state, time );
		}

		msg.wCount = count;
		pPlayer->SendMsg( &msg, msg.GetSize() );

		if( count >= 100 )
		{
			QuestSubQuestLoad( pMessage->dwID, (int)QuestIdx );
			return;
		}
	}	

//	QuestItemload( pMessage->dwID );

	// ¼­ºêÄù½ºÆ® ·ÎµåÈÄ ¸ÞÀÎÄù½ºÆ® ·ÎµåÇÑ´Ù.
	QuestMainQuestLoad( pMessage->dwID );
}


void RQuestItemLoad(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

	SEND_QUESTITEM msg;
	memset(&msg.ItemList, 0, sizeof(msg.ItemList));
	// clientÂ¡Ã‹?Â¡Ã‹Â¢Ã§ Â¡Â§Â¡Ã¾Â¡Ã‹Â¡Ã¾Â¨ÃÂ©ÂªÂ¡Â§u Â¢Â®Â¨ÃºÂ¡Ã‹Â¡Ã¾vAÂ¡Â§u Â¢Â®iyÂ¡Â§uÂ¡Â§Â¡Ã¾..
	msg.Category = MP_QUEST;
	msg.Protocol = MP_QUEST_ITEM_LOAD;
	msg.dwObjectID = pMessage->dwID;				// UserID

	if( pMessage->dwResult == 0 )
	{
		msg.wCount = 0;
		pPlayer->SendMsg( &msg, msg.GetSize() );
	}
	else
	{
		DWORD ItemIdx=0;
		int Count=0;
		WORD wcount=0;
		WORD QuestIdx=0;

		for( DWORD i = 0 ; i < pMessage->dwResult ; ++i )
		{
			ItemIdx = (DWORD)atoi((char*)pData[i].Data[eQit_ItemIdx]);
			Count = (int)atoi((char*)pData[i].Data[eQit_ItemCount]);			
			QuestIdx = (WORD)atoi((char*)pData[i].Data[eQit_QuestIdx]);
			
			// A¢®©­¡ËOoAI¡§u¨Ï¡Ì¡§¢®¡Ëc ¡Ë¡þ¡§¡©¡§u¡§uAo ¡§uA¡§¢®A..
			msg.ItemList[wcount].ItemIdx = ItemIdx;
			msg.ItemList[wcount].Count = Count;
			msg.ItemList[wcount].QuestIdx = QuestIdx;
			wcount++;
			
			QUESTMGR->SetQuestItem( pPlayer, QuestIdx, ItemIdx, Count );
		}
		
		msg.wCount = wcount;
		pPlayer->SendMsg( &msg, msg.GetSize() );	

		if( wcount >= 100 )
		{
			QuestItemload( pMessage->dwID, (int)QuestIdx );
		}
	}
}

void RJournalGetList(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;
	SEND_JOURNALLIST msg;
	memset(&msg, NULL, sizeof(msg));
	msg.Category = MP_JOURNAL;
	msg.Protocol = MP_JOURNAL_GETLIST_ACK;
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		msg.m_Info[i].Index = (DWORD)atoi((char*)pData[i].Data[eJnl_Index]);
		msg.m_Info[i].Kind = (BYTE)atoi((char*)pData[i].Data[eJnl_Kind]);
		msg.m_Info[i].Param = (DWORD)atoi((char*)pData[i].Data[eJnl_Param]);
		msg.m_Info[i].Param_2 = (DWORD)atoi((char*)pData[i].Data[eJnl_Param_2]);
		msg.m_Info[i].Param_3 = (DWORD)atoi((char*)pData[i].Data[eJnl_Param_3]);
		msg.m_Info[i].bSaved = (BYTE)atoi((char*)pData[i].Data[eJnl_bSaved]);
		SafeStrCpy(msg.m_Info[i].ParamName, (char*)pData[i].Data[eJnl_ParamName], MAX_NAME_LENGTH+1);
		SafeStrCpy(msg.m_Info[i].RegDate, (char*)pData[i].Data[eJnl_RegDate], 11);
		char buf[256]={ 0, };
		strcpy(buf, (char*)pData[i].Data[eJnl_RegDate]);
		msg.wCount++;
	}
	pPlayer->SendMsg(&msg, msg.GetSize());
}

void RSaveMapChangePointReturn(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD dwConnectionIndex = (DWORD)atoi((char*)pData->Data[0]);
	DWORD Map = (DWORD)atoi((char*)pData->Data[1]);

	MSG_DWORD msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_MAP_OUT;
	msg.dwObjectID = pMessage->dwID;
	msg.dwData = Map;
	
	g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));
}


// Â¦Â®Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¯
// Â¦Â­RaMa		-> ShopItemLoading                             Â¦Â­
// Â¦Â±Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â°
void RCharacterShopItemInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

	if(pPlayer->GetInited() == FALSE)
		return;
	if(pMessage->dwResult == 0)	
		return;

	// Ã‡Ã¶?Ã§Â½ÃƒÂ°Â£ ?Ãº?Ã¥
	SHOP_ITEM	ShopItem;
	SHOP_ITEM	NewShopItem;
	memset(&ShopItem, 0, sizeof(SHOP_ITEM));
	memset(&NewShopItem, 0, sizeof(SHOP_ITEM));

	int itempos=0;
	DWORD nitemcnt=0;
	DWORD lditemcnt = 0;
	if( pMessage->dwResult<=SLOT_SHOPITEM_IMSI )
		lditemcnt = pMessage->dwResult;
	else
	{
		lditemcnt = SLOT_SHOPITEM_IMSI;
		pPlayer->GetShopItemManager()->SendMsgDwordToPlayer( MP_ITEM_SHOPITEM_SLOTOVERITEM, pMessage->dwResult );
	}

	// magi82 - ÇÊ¿ä¾ø¾î¼­ ÁÖ¼®Ã³¸®
	//stTIME curtime, endtime;
	//curtime.value = GetCurTime();

	for( DWORD i = 0 ; i < lditemcnt ; ++i )
	{
		itempos = atoi((char*)pData[i].Data[eMItm_Position]);

		if(itempos == 0)
		{
			NewShopItem.Item[nitemcnt].dwDBIdx = (DWORD)atoi((char*)pData[i].Data[eMItm_ItemDBIdx]);
			NewShopItem.Item[nitemcnt].wIconIdx = (WORD)atoi((char*)pData[i].Data[eMItm_ItemIdx]);
			NewShopItem.Item[nitemcnt].Position = 0;
			NewShopItem.Item[nitemcnt].Durability = (WORD)atoi((char*)pData[i].Data[eMItm_Durability]);
			NewShopItem.Item[nitemcnt].ItemParam = (ITEMPARAM)atoi((char*)pData[i].Data[eMItm_Param]);			
			++nitemcnt;
		}
		else
		{			
			itempos -= TP_SHOPITEM_START;

			if( ShopItem.Item[itempos].dwDBIdx != 0 )
			{
				NewShopItem.Item[nitemcnt].dwDBIdx = (DWORD)atoi((char*)pData[i].Data[eMItm_ItemDBIdx]);
				NewShopItem.Item[nitemcnt].wIconIdx = (WORD)atoi((char*)pData[i].Data[eMItm_ItemIdx]);
				NewShopItem.Item[nitemcnt].Position = 0;
				NewShopItem.Item[nitemcnt].Durability = (WORD)atoi((char*)pData[i].Data[eMItm_Durability]);
				NewShopItem.Item[nitemcnt].ItemParam = (ITEMPARAM)atoi((char*)pData[i].Data[eMItm_Param]);			
				++nitemcnt;

				continue;
			}
						
			ShopItem.Item[itempos].dwDBIdx = (DWORD)atoi((char*)pData[i].Data[eMItm_ItemDBIdx]);
			ShopItem.Item[itempos].wIconIdx = (WORD)atoi((char*)pData[i].Data[eMItm_ItemIdx]);
			ShopItem.Item[itempos].Position = (WORD)atoi((char*)pData[i].Data[eMItm_Position]);
			ShopItem.Item[itempos].Durability = (WORD)atoi((char*)pData[i].Data[eMItm_Durability]);
			ShopItem.Item[itempos].ItemParam = (ITEMPARAM)atoi((char*)pData[i].Data[eMItm_Param]);
		}


		// »ç¿ëÇÏ·Á¸é ÇÁ·Î½ÃÀú¿¡ Order by item_dbidxÃß°¡
		//StartItemDBIdx = ShopItem.Item[itempos].dwDBIdx;
	}


	// Æ÷Áö¼ÇÀÌ ¾ø´Â »õ·Î¿î ¾ÆÀÌÅÛÀÌ ÀÖÀ¸¸é.
	if( nitemcnt )
	{
		for(DWORD ni=0; ni<nitemcnt; ni++)
		{
			for(int si=0; si<SLOT_SHOPITEM_IMSI; si++)
			{
				// ÀÚ¸®°¡ ¾ø´Â°ÍÀÌ ÀÖÀ¸¸é ¼ÂÆÃÇØÁØ´Ù.
				if( ShopItem.Item[si].dwDBIdx == 0 )
				{
					ShopItem.Item[si] = NewShopItem.Item[ni];
					ShopItem.Item[si].Position = si+TP_SHOPITEM_START;

					// DB¿¡ PositionÀ» UpdateÇØÁØ´Ù.
					ItemMoveShopUpdateToDB( pPlayer->GetID(), pPlayer->GetUserID(), ShopItem.Item[si].dwDBIdx, ShopItem.Item[si].Position, 0, 0  );
					break;
				}
			}
		}
	}

	pPlayer->InitShopItemInfo( &ShopItem );
	pPlayer->GetShopItemManager()->CheckAvatarEndtime();
	pPlayer->SendShopItemInfo();
}


void RShopItemInvenInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;


	//
	ITEMBASE ItemBase[TABCELL_SHOPINVEN_NUM];
	ITEMBASE ItemOverlap[TABCELL_SHOPINVEN_NUM];
	memset( ItemBase, 0, sizeof(ITEMBASE)*TABCELL_SHOPINVEN_NUM );
	memset( ItemOverlap, 0, sizeof(ITEMBASE)*TABCELL_SHOPINVEN_NUM );
	int OverlapCount=0;


	if( pMessage->dwResult == 0 )
	{
		pPlayer->InitShopInvenInfo( ItemBase );
		//pPlayer->SetInitState(PLAYERINITSTATE_ITEM_INFO,MP_USERCONN_GAMEIN_SYN);
		PetWearItemInfo(pPlayer->GetID(), TP_PETWEAR_START, TP_PETWEAR_END);
		return;
	}

	for(DWORD  i = 0; i < pMessage->dwResult; i++)
	{
		POSTYPE ItemPos = atoi((char*)pData[i].Data[eCI_Position]);

		if(ItemPos >= TP_SHOPINVEN_START && ItemPos < TP_SHOPINVEN_END)
		{
			ItemPos -= TP_SHOPINVEN_START;

			if( ItemPos >= TABCELL_SHOPINVEN_NUM || ItemBase[ItemPos].dwDBIdx )
			{
				ItemOverlap[OverlapCount].dwDBIdx = (DWORD)atoi((char*)pData[i].Data[eCI_DBIDX]);
				ItemOverlap[OverlapCount].wIconIdx = (WORD)atoi((char*)pData[i].Data[eCI_IDX]);
				ItemOverlap[OverlapCount].Position = (POSTYPE)atoi((char*)pData[i].Data[eCI_Position]);
				ItemOverlap[OverlapCount].QuickPosition = (POSTYPE)atoi((char*)pData[i].Data[eCI_QPosition]);
				ItemOverlap[OverlapCount].Durability = (DURTYPE)atoi((char*)pData[i].Data[eCI_Durability]);
				ItemOverlap[OverlapCount].ItemParam = (ITEMPARAM)atoi((char*)pData[i].Data[eCI_Param]);
				++OverlapCount;
			}
			else
			{
				ItemBase[ItemPos].dwDBIdx = (DWORD)atoi((char*)pData[i].Data[eCI_DBIDX]);
				ItemBase[ItemPos].wIconIdx = (WORD)atoi((char*)pData[i].Data[eCI_IDX]);
				ItemBase[ItemPos].Position = (POSTYPE)atoi((char*)pData[i].Data[eCI_Position]);
				ItemBase[ItemPos].QuickPosition = (POSTYPE)atoi((char*)pData[i].Data[eCI_QPosition]);
				ItemBase[ItemPos].Durability = (DURTYPE)atoi((char*)pData[i].Data[eCI_Durability]);
				ItemBase[ItemPos].ItemParam = (ITEMPARAM)atoi((char*)pData[i].Data[eCI_Param]);
			}
		}
	}

	//Áßº¹µÈ ¾ÆÀÌÅÛÀÌ ÀÖÀ¸¸é ºóÄ­¿¡ ³Ö¾îÁØ´Ù.
	for(int n=0; n<OverlapCount; n++)
	{
		for(int m=0; m<TABCELL_SHOPINVEN_NUM; m++)
		{
			//ºóÄ­ÀÌ¸é ³Ö¾îÁØ´Ù.
			if( ItemBase[m].dwDBIdx == 0 )
			{
				ItemBase[m] = ItemOverlap[n];
				ItemBase[m].Position = m+TP_SHOPINVEN_START;
                
				// DB¿¡ Æ÷Áö¼ÇÀ» ¾÷µ¥ÀÌÆ®ÇØÁØ´Ù.
				ItemMoveUpdateToDB( pPlayer->GetID(), 0, 0, ItemBase[m].dwDBIdx, ItemBase[m].Position );
				break;
			}
		}

		//¸¸¾à ºóÄ­ÀÌ ¾øÀ¸¸é ´õÀÌ»ó Ã³¸®ÇÏÁö ¾ÊÀ½
		if( m == TABCELL_SHOPINVEN_NUM )
		{
			// ¾ÆÀÌÅÛÀÌ ´Ù Â÷ÀÖ´Ù.;;
			break;
		}
	}
	
	pPlayer->InitShopInvenInfo( ItemBase );
	//pPlayer->SetInitState(PLAYERINITSTATE_ITEM_INFO,MP_USERCONN_GAMEIN_SYN);
	PetWearItemInfo(pPlayer->GetID(), TP_PETWEAR_START, TP_PETWEAR_END);
}

// Â¦Â®Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¯
// Â¦Â­RaMa - 04.11.05  ->ShopItemUsing                          Â¦Â­
// Â¦Â±Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â¬Â¦Â°
void RUsingShopItemInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

	if( pMessage->dwResult == 0 )
	{
		pPlayer->SetInitState( PLAYERINITSTATE_SHOPITEM_USEDINFO, MP_USERCONN_GAMEIN_SYN );
		return;
	}

	SEND_SHOPITEM_USEDINFO msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_SHOPITEM_USEDINFO;


	for(DWORD i=0; i<pMessage->dwResult; i++)
	{
		msg.Item[msg.ItemCount].ItemBase.wIconIdx	= (WORD)atoi((char*)pData[i].Data[eUMtm_ItemIdx]);
		msg.Item[msg.ItemCount].ItemBase.dwDBIdx	= (DWORD)atoi((char*)pData[i].Data[eUMtm_DBIdx]);
		msg.Item[msg.ItemCount].Param				= (DWORD)atoi((char*)pData[i].Data[eUMtm_Param]);
		msg.Item[msg.ItemCount].BeginTime.value		= (DWORD)atoi((char*)pData[i].Data[eUMtm_BTime]);
		msg.Item[msg.ItemCount].Remaintime			= (DWORD)atoi((char*)pData[i].Data[eUMtm_RTime]);
		msg.Item[msg.ItemCount].ItemBase.Position	= (POSTYPE)atoi((char*)pData[i].Data[eUMtm_Position]);

		if( pPlayer->GetShopItemManager()->UsedShopItem(
			&msg.Item[msg.ItemCount].ItemBase,
			msg.Item[msg.ItemCount].Param,
			msg.Item[msg.ItemCount].BeginTime,
			msg.Item[msg.ItemCount].Remaintime) )
		{
			// ÀåºñÇÏ°í ÀÖ´Â ¾Æ¹ÙÅ¸ ¾ÆÀÌÅÛ
			if( msg.Item[msg.ItemCount].Param == eShopItemUseParam_EquipAvatar )
				pPlayer->GetShopItemManager()->PutOnAvatarItem( 
				msg.Item[msg.ItemCount].ItemBase.wIconIdx, msg.Item[msg.ItemCount].ItemBase.Position, FALSE );

			++msg.ItemCount;
		}
		else
		{
			memset( &msg.Item[msg.ItemCount], 0, sizeof(SHOPITEMBASE) );	//Å¬¶óÀÌ¾ðÆ®·Î ¸Þ¼¼Áö °¥ÇÊ¿ä°¡ ¾ø´Ù.
		}

		if( msg.ItemCount >= 100 )
		{
			g_Console.LOG( 4, "UsingShopItemInfo over 100 !!" );
			break;
		}

//		++msg.ItemCount;	//if¹® ¾ÈÀ¸·Î ¿Å±è KES
	}

	pPlayer->GetShopItemManager()->CalcPlusTime( 0, 0 );

	//
	pPlayer->SetInitState( PLAYERINITSTATE_SHOPITEM_USEDINFO, MP_USERCONN_GAMEIN_SYN );

	// 
	pPlayer->SendMsg(&msg, msg.GetSize());
}


void RSavedMovePointInfo( LPMIDDLEQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

	pPlayer->GetShopItemManager()->ReleseMovePoint();

	SEND_MOVEDATA_INFO msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_SHOPITEM_MPINFO;

	for(DWORD i=0; i<pMessage->dwResult; i++)
	{
		msg.Data[i].DBIdx = (DWORD)atoi((char*)pData[i].Data[eMPt_DBIdx]);
		strncpy(msg.Data[i].Name, (char*)pData[i].Data[eMPt_Name], MAX_SAVEDMOVE_NAME-1);
		msg.Data[i].MapNum = (WORD)atoi((char*)pData[i].Data[eMPt_MapNum]);
		msg.Data[i].Point.value = (DWORD)atoi((char*)pData[i].Data[eMPt_MapPoint]);
		++msg.Count;

		//
		pPlayer->GetShopItemManager()->AddMovePoint( &msg.Data[i] );
	}

	// ¸Ê¿¡ µé¾î¿À¸é ÇÑ¹ø º¸³»ÁØ´Ù.
	if( pPlayer->GetSavePointInit() )
	{
		msg.bInited = 1;
		pPlayer->SendMsg(&msg, msg.GetSize());
	}
	else
	{
		msg.bInited = 0;
		pPlayer->SendMsg(&msg, msg.GetSize());
		pPlayer->SetSavePointInit( TRUE );
	}
}

void RSavedMovePointInsert( LPMIDDLEQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

	if( pMessage->dwResult == 0 )	
		return;
	
	SEND_MOVEDATA_SIMPLE msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_SHOPITEM_SAVEPOINT_ADD_ACK;

	for(DWORD i=0; i<pMessage->dwResult; i++)
	{
		msg.Data.DBIdx = (DWORD)atoi((char*)pData[i].Data[eMPt_DBIdx]);
		strncpy(msg.Data.Name, (char*)pData[i].Data[eMPt_Name], MAX_SAVEDMOVE_NAME-1);
		msg.Data.MapNum = (WORD)atoi((char*)pData[i].Data[eMPt_MapNum]);
		msg.Data.Point.value = (DWORD)atoi((char*)pData[i].Data[eMPt_MapPoint]);

		//
		pPlayer->GetShopItemManager()->AddMovePoint( &msg.Data );
	}

	//
	pPlayer->SendMsg(&msg, sizeof(msg));

}


void RIsUseAbleShopAvatarItem( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

	WORD itemidx = (WORD)atoi((char*)pData->Data[0]);
	DWORD dbidx = (DWORD)atoi((char*)pData->Data[1]);
	DWORD param = (DWORD)atoi((char*)pData->Data[2]);
	DWORD begintime = (DWORD)atoi((char*)pData->Data[3]);
	DWORD remaintime = (DWORD)atoi((char*)pData->Data[4]);
	WORD itempos = (WORD)atoi((char*)pData->Data[5]);

	if( param == 9999 )
	{
		MSG_WORD2 msg;
		SetProtocol( &msg, MP_ITEM, MP_ITEM_SHOPITEM_AVATAR_USE_NACK );
		msg.wData1 = itemidx;
		msg.wData2 = itempos;
		pPlayer->SendMsg( &msg, sizeof(msg) );
		return;
	}

	//
	const ITEMBASE* pitembase = ITEMMGR->GetItemInfoAbsIn( pPlayer, itempos );
	if( !pitembase->dwDBIdx )
	{
		MSG_WORD2 msg;
		SetProtocol( &msg, MP_ITEM, MP_ITEM_SHOPITEM_AVATAR_USE_NACK );
		msg.wData1 = itemidx;
		msg.wData2 = itempos;
		pPlayer->SendMsg( &msg, sizeof(msg) );
		return;
	}

	SHOPITEMBASE shopitem;
	shopitem.ItemBase = *pitembase;
	shopitem.Param = eShopItemUseParam_EquipAvatar;
	shopitem.BeginTime.value = begintime;
	shopitem.Remaintime = remaintime;
	pPlayer->GetShopItemManager()->AddShopItemUse( &shopitem );

	if( !pPlayer->GetShopItemManager()->PutOnAvatarItem( itemidx, itempos ) )
	{
		MSG_WORD2 msg;
		SetProtocol( &msg, MP_ITEM, MP_ITEM_SHOPITEM_AVATAR_USE_NACK );
		msg.wData1 = itemidx;
		msg.wData2 = itempos;
		pPlayer->SendMsg( &msg, sizeof(msg) );
		return;
	}


	SEND_SHOPITEM_BASEINFO msg;
	SetProtocol( &msg, MP_ITEM, MP_ITEM_SHOPITEM_AVATAR_USE_ACKDB );
	msg.ShopItemIdx = itemidx;
	msg.ShopItemPos = itempos;
	msg.ShopItemBase = shopitem;
	pPlayer->SendMsg( &msg, sizeof(msg) );
}


void RIsAbleShopEquipItem( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer ) return;

	WORD itemidx = (WORD)atoi((char*)pData->Data[0]);
	DWORD dbidx = (DWORD)atoi((char*)pData->Data[1]);
	DWORD param = (DWORD)atoi((char*)pData->Data[2]);
	DWORD begintime = (DWORD)atoi((char*)pData->Data[3]);
	DWORD remaintime = (DWORD)atoi((char*)pData->Data[4]);
	WORD itempos = (WORD)atoi((char*)pData->Data[5]);
	WORD toitemidx = (WORD)atoi((char*)pData->Data[6]);
	WORD toitempos = (WORD)atoi((char*)pData->Data[7]);

	SHOPITEMBASE shopitem;

	int rt = 0;
	if( param == 9999 )
		goto _EquipMove_Failed;

	const ITEMBASE* pitembase = ITEMMGR->GetItemInfoAbsIn( pPlayer, itempos );
	if( !pitembase->dwDBIdx )
		goto _EquipMove_Failed;

	shopitem.ItemBase = *pitembase;
	shopitem.Param = param;
	shopitem.BeginTime.value = begintime;
	shopitem.Remaintime = remaintime;
	pPlayer->GetShopItemManager()->AddShopItemUse( &shopitem );

	if(EI_TRUE == (rt = ITEMMGR->MoveItem(pPlayer, itemidx, itempos, toitemidx, toitempos)))
	{
		MSG_ITEM_MOVE_ACK msg;
		SetProtocol( &msg, MP_ITEM, MP_ITEM_MOVE_ACK );
		msg.wFromItemIdx = itemidx;
		msg.FromPos = itempos;
		msg.wToItemIdx = toitemidx;
		msg.ToPos = toitempos;
		ITEMMGR->SendAckMsg(pPlayer, &msg, sizeof(msg));

		SEND_SHOPITEM_BASEINFO smsg;
		SetProtocol( &smsg, MP_ITEM, MP_ITEM_SHOPITEM_EQUIP_USEINFODB );
		smsg.ShopItemIdx = itemidx;
		smsg.ShopItemPos = toitempos;
		smsg.ShopItemBase = shopitem;
		pPlayer->SendMsg( &smsg, sizeof(smsg) );

		return;
	}

_EquipMove_Failed:
	MSG_ITEM_ERROR msg;
	SetProtocol( &msg, MP_ITEM, MP_ITEM_ERROR_NACK );
	msg.dwObjectID = pPlayer->GetID();
	msg.ECode = eItemUseErr_Move;
	ITEMMGR->SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
}



void RGuildLoadGuild(LPQUERY pData, LPDBMESSAGE pMessage)
{
	LOAD_GUILD LoadGuildInfo;
	DWORD dwTime = 0;
	memset(&LoadGuildInfo, 0, sizeof(LOAD_GUILD));
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		LoadGuildInfo.GuildInfo[i].GuildIdx = atoi((char*)pData[i].Data[eGu_LGIdx]);
		SafeStrCpy(LoadGuildInfo.GuildInfo[i].GuildName, (char*)pData[i].Data[eGu_LGName], MAX_GUILD_NAME + 1);
		LoadGuildInfo.GuildInfo[i].MapNum = atoi((char*)pData[i].Data[eGu_LGLocation]);
		LoadGuildInfo.GuildInfo[i].MasterIdx = atoi((char*)pData[i].Data[eGu_LGMasterIdx]);
		LoadGuildInfo.GuildInfo[i].GuildLevel = atoi((char*)pData[i].Data[eGu_LGLevel]);
		LoadGuildInfo.GuildInfo[i].UnionIdx = atoi((char*)pData[i].Data[eGu_LGUnionIdx]);
		SafeStrCpy(LoadGuildInfo.GuildInfo[i].MasterName, (char*)pData[i].Data[eGu_LGMasterName], MAX_NAME_LENGTH+1);
		LoadGuildInfo.GuildMoney[i] = atoi((char*)pData[i].Data[eGu_LGMoney]);
		LoadGuildInfo.GuildInfo[i].MarkName = atoi((char*)pData[i].Data[eGu_LGMarkName]);
		//SW070103 ¹®ÆÄÆ÷ÀÎÆ®°³¼±
		LoadGuildInfo.GuildInfo[i].LvUpCounter = atoi((char*)pData[i].Data[eGu_LGMarkName+2]);
		LoadGuildInfo.GuildInfo[i].bNeedMasterChecking = atoi((char*)pData[i].Data[eGu_LGMarkName+3]);

		GUILDMGR->RegistGuild(&LoadGuildInfo.GuildInfo[i], LoadGuildInfo.GuildMoney[i]);
		dwTime = (DWORD)atoi((char*)pData[i].Data[eGu_LGMarkName+1]);
		if( dwTime )
			GUILDUNIONMGR->AddEntryTime( LoadGuildInfo.GuildInfo[i].GuildIdx, dwTime );

	}

	ASSERT(pMessage->dwResult <= MAX_QUERY_RESULT);
	if(pMessage->dwResult >= MAX_QUERY_RESULT)
	{
		GuildLoadGuild(LoadGuildInfo.GuildInfo[99].GuildIdx);
	}
	else
	{
		GuildLoadNotice(0);
		GuildLoadMember(0);
		// RaMa - Guild Tournament - GuildÁ¤º¸ ÀÐ¾î¿Â ÈÄ¿¡ ÀÐ´Â´Ù.
		GuildTournamentInfoLoad();
		// RaMa - SiegeWar Ãß°¡
		if( SIEGEWARMGR->IsNeedLoadSiegeInfo() )		
			SiegeWarInfoLoad( SIEGEWARMGR->GetSiegeMapNum() );
		
		//magi82 - ¹®ÇÏ»ý °¡ÀÔÆí¸®½Ã½ºÅÛ /////////////////////////////////////////
		GuildTraineeInfo(0);

		GuildTraineeGuildInfo(0);

		GuildTraineeGuildIntroInfo(0);
		//////////////////////////////////////////////////////////////////////////
	}
}

// 06. 03. ¹®ÆÄ°øÁö - ÀÌ¿µÁØ
void RGuildLoadNotice(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage)
{
	LOAD_GUILDNOTICE GuildNotice;
	memset(&GuildNotice, 0, sizeof(LOAD_GUILDNOTICE));
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		GuildNotice.DBIdx[i] = atoi((char*)pData[i].Data[eGu_GNDBIdx]);
		GuildNotice.GuildIdx[i] = atoi((char*)pData[i].Data[eGu_GNGuildIdx]);
		strcpy(GuildNotice.GuildNotice[i], (char*)pData[i].Data[eGu_GNGuildNotice]);

		GUILDMGR->RegistGuildNotice(GuildNotice.GuildIdx[i], GuildNotice.GuildNotice[i]);
	}

//	ASSERT(pMessage->dwResult <= MAX_QUERY_RESULT);
	if(pMessage->dwResult == MAX_QUERY_RESULT)
	{
		GuildLoadNotice(GuildNotice.DBIdx[i]);
	}
}

void RGuildLoadMember(LPQUERY pData, LPDBMESSAGE pMessage)
{
	LOAD_GUILDMEMBER GuildMemberInfo;
	memset(&GuildMemberInfo, 0, sizeof(LOAD_GUILDMEMBER));
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		GuildMemberInfo.Member[i].MemberInfo.MemberIdx = atoi((char*)pData[i].Data[eGu_LMIdx]);
		SafeStrCpy(GuildMemberInfo.Member[i].MemberInfo.MemberName, (char*)pData[i].Data[eGu_LMName], MAX_NAME_LENGTH + 1);
		GuildMemberInfo.Member[i].GuildIdx = atoi((char*)pData[i].Data[eGu_LMGuildIdx]);
		GuildMemberInfo.Member[i].MemberInfo.Memberlvl = atoi((char*)pData[i].Data[eGu_LMLevel]);
		GuildMemberInfo.Member[i].MemberInfo.Rank = atoi((char*)pData[i].Data[eGu_LMRank]);
				
		GUILDMGR->LoadMembers(&GuildMemberInfo.Member[i]) ;
	}

	ASSERT(pMessage->dwResult <= MAX_QUERY_RESULT);
	if(pMessage->dwResult == MAX_QUERY_RESULT)
	{
		GuildLoadMember(GuildMemberInfo.Member[99].MemberInfo.MemberIdx);
	}
	else
	{
		GuildLoadMark(0);
	}	
}

void RGuildLoadMark(LPLARGEQUERY pData, LPDBMESSAGE pMessage)
{
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		GUILDMGR->LoadMark(atoi((char*)pData->Data[0]), (char*)pData->Data[1]);
	}
	if(pMessage->dwResult)
	{
		GuildLoadMark(atoi((char*)pData->Data[0]));
	}
	else
	{
	// server start!
	//	g_pServerSystem->SetStart(TRUE);
	
		//SW060526 ¹®ÆÄÃ¢°í ¾ÆÀÌÅÛ Á¤º¸ÀúÀå ½ÃÁ¡ º¯°æ
		//GuildItemLoad(GAMERESRCMNGR->GetLoadMapNum(), 0);	//±âÁ¸ ÇÁ·Î½ÃÁ®´Â µÐ´Ù. //GuildItemOption(.. //GuildItemRareOption(..
		LoadGuildWarRecord( 0 );
	}
}

void RGuildCreate(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	
	if((int)atoi((char*)pData->Data[eGu_CIsSuccess]) != 0 ) // 0:success
	{
		if(pPlayer == 0)
			return;
		GUILDMGR->SendNackMsg(pPlayer, MP_GUILD_CREATE_NACK, eGuildErr_Create_Name);
		return;
	}

	GUILDINFO Info;
	Info.GuildIdx = atoi((char*)pData->Data[eGu_CGuildIdx]);
	Info.MapNum = atoi((char*)pData->Data[eGu_CMapNum]);
	Info.MasterIdx = pMessage->dwID;
	Info.GuildLevel = GUILD_1LEVEL;
	Info.UnionIdx = 0;
	Info.MarkName = 0;
	SafeStrCpy(Info.GuildName, (char*)pData->Data[eGu_CName], MAX_GUILD_NAME+1);
	SafeStrCpy(Info.MasterName, (char*)pData->Data[eGu_CMasterName], MAX_NAME_LENGTH+1);
	memset(Info.GuildNotice, 0, MAX_GUILD_NOTICE+1);

	//SW070103 ¹®ÆÄÆ÷ÀÎÆ®°³¼±
	Info.LvUpCounter = 0;
	Info.bNeedMasterChecking = FALSE;

	GUILDMGR->CreateGuildResult(atoi((char*)pData->Data[eGu_CMasterlvl]), &Info);
}

void RGuildMarkRegist(LPLARGEQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(pPlayer == 0) return;
	//markidx, guildidx, imgdate
	
	GUILDMGR->RegistMarkResult(pPlayer, atoi((char*)pData->Data[1]), atoi((char*)pData->Data[0]), (char*)pData->Data[2]);
}

/*
void UpdateCharacterMoney( DWORD dwCharIdx, MONEYTYPE money, BYTE flag )
{
	sprintf(txt, "EXEC RP_UpdateCharacterMoney %d, %d, %d", dwCharIdx, money, flag );
	g_DB.Query(eQueryType_FreeQuery, eUpdateCharacterMoney, 0, txt);
}

void RUpdateCharacterMoney( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD CharacterIdx = atoi((char*)pData[0].Data[0]);
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(CharacterIdx);
	if( pPlayer == NULL ) return;
	pPlayer->RSetMoney( (MONEYTYPE)atoi((char*)pData[0].Data[1]), (BYTE)atoi((char*)pData[0].Data[2]) );
}
*/
void ConfirmUserOut( DWORD dwConnectIdx, DWORD dwUserIdx, DWORD dwCharIdx, DWORD dwKind )
{
	sprintf(txt, "EXEC RP_ConfirmUserOut %d, %d, %d", dwUserIdx, dwCharIdx, dwKind );
	g_DB.Query(eQueryType_FreeQuery, eConfirmUserOut, dwConnectIdx, txt);
}

void RConfirmUserOut( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD dwConnectIdx = pMessage->dwID;
	DWORD dwUserIdx = (DWORD)atoi((char*)pData[0].Data[0]);
	DWORD dwCharIdx = (DWORD)atoi((char*)pData[0].Data[1]);
	DWORD dwKind = (DWORD)atoi((char*)pData[0].Data[2]);

	switch( dwKind )
	{
	case 1:		// Ä³¸¯¼±ÅÃÃ¢
		{
			MSGBASE msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_BACKTOCHARSEL_ACK;
			msg.dwObjectID = dwCharIdx;

			g_Network.Send2Server(dwConnectIdx, (char*)&msg, sizeof(msg));
		}
		break;
	case 2:
		{
			MSGBASE msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_LOGINCHECK_DELETE;
			msg.dwObjectID = dwUserIdx;

			g_Network.Send2Server(dwConnectIdx, (char*)&msg, sizeof(msg));
		}
		break;
	}	
}

void RSaveCharInfoBeforeLogout( LPQUERY pData, LPDBMESSAGE pMessage )
{
	// ¸Ê¾Æ¿ôÃ³¸®
	CPlayer * pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if(pPlayer == 0)
		return;
	g_pServerSystem->RemovePlayer(pPlayer->GetID());
	//
	ConfirmUserOut( (DWORD)atoi((char*)pData->Data[0]), pPlayer->GetUserID(), pPlayer->GetID(), 1 );
}

void RGuildBreakUp( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pMaster = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	GUILDMGR->BreakUpGuildResult(pMaster, atoi((char*)pData->Data[0]), (char*)pData->Data[1]);

	DeleteGuildWarRecord( (DWORD)atoi((char*)pData->Data[0]) );
}

void RGuildDeleteMember( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if(atoi((char*)pData->Data[0]) == 0) //err
	{
		CPlayer * pMaster = (CPlayer*)g_pUserTable->FindUser( atoi((char*)pData->Data[1]) );
		if(pMaster)
			GUILDMGR->SendNackMsg(pMaster, MP_GUILD_DELETEMEMBER_NACK, eGuildErr_Err);
	}
	else
		GUILDMGR->DeleteMemberResult(pMessage->dwID, atoi((char*)pData->Data[0]), (char*)pData->Data[1]);
}

void RGuildSecedeMember( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if(atoi((char*)pData->Data[0]) == 0) //err
	{
		CPlayer * pPlayer = (CPlayer*)g_pUserTable->FindUser(atoi((char*)pData->Data[1]));
		if(pPlayer)
			GUILDMGR->SendNackMsg(pPlayer, MP_GUILD_SECEDE_NACK, eGuildErr_Err);
	}
	else
		GUILDMGR->SecedeResult(pMessage->dwID, atoi((char*)pData->Data[0]), (char*)pData->Data[1]);
}

void RGuildItemLoad( LPQUERY pData, LPDBMESSAGE pMessage )
{
	ITEMBASE guilditem;
	DWORD GuildIdx;

	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		GuildIdx = atoi((char*)pData[i].Data[eMu_IMunpaID]);
		
		guilditem.Durability = atoi((char*)pData[i].Data[eMu_IDurability]);
		guilditem.wIconIdx = atoi((char*)pData[i].Data[eMu_IIDX]);
		guilditem.dwDBIdx = atoi((char*)pData[i].Data[eMu_IDBIDX]);
		guilditem.Position = atoi((char*)pData[i].Data[eMu_IPosition]);
		guilditem.RareIdx = atoi((char*)pData[i].Data[eMu_IRareIdx]);
		guilditem.QuickPosition = 0;
		guilditem.ItemParam = 0;	// magi82(28)
		GUILDMGR->RegistGuildItem(GuildIdx, &guilditem);
	}
	if(pMessage->dwResult < MAX_QUERY_RESULT)
	{
		GuildItemOption(GAMERESRCMNGR->GetLoadMapNum(), 0);
		//SW050920 Rare
		GuildItemRareOption(GAMERESRCMNGR->GetLoadMapNum(), 0);
	}	
	else
		GuildItemLoad(GAMERESRCMNGR->GetLoadMapNum(), guilditem.dwDBIdx);
}

void RGuildItemOption(LPQUERY pData, LPDBMESSAGE pMessage)
{
	ITEM_OPTION_INFO OptionInfo;
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		OptionInfo.dwOptionIdx				= (DWORD)atoi((char*)pData[i].Data[eMOI_OptionID]);
		OptionInfo.dwItemDBIdx				= (DWORD)atoi((char*)pData[i].Data[eMOI_ItemDBID]);
		OptionInfo.GenGol					= (WORD)atoi((char*)pData[i].Data[eMOI_GenGol]);
		OptionInfo.MinChub					= (WORD)atoi((char*)pData[i].Data[eMOI_MinChub]);				
		OptionInfo.CheRyuk					= (WORD)atoi((char*)pData[i].Data[eMOI_CheRyuk]);				
		OptionInfo.SimMek					= (WORD)atoi((char*)pData[i].Data[eMOI_SimMek]);					
		OptionInfo.Life						= (DWORD)atoi((char*)pData[i].Data[eMOI_Life]);					
		OptionInfo.NaeRyuk					= (WORD)atoi((char*)pData[i].Data[eMOI_NaeRyuk]);				
		OptionInfo.Shield					= (DWORD)atoi((char*)pData[i].Data[eMOI_Shield]);					
		OptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[eMOI_FireAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[eMOI_WaterAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[eMOI_TreeAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[eMOI_GoldAttrRegist])/100.f );
		OptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[eMOI_EarthAttrRegist])/100.f );
		OptionInfo.PhyAttack				= (WORD)atoi((char*)pData[i].Data[eMOI_PhyAttack]);
		OptionInfo.CriticalPercent			= (WORD)atoi((char*)pData[i].Data[eMOI_Critical]);
		OptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[eMOI_FireAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[eMOI_WaterAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[eMOI_TreeAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[eMOI_GoldAttrAttack])/100.f );
		OptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[eMOI_EarthAttrAttack])/100.f );
		OptionInfo.PhyDefense				= (WORD)atoi((char*)pData[i].Data[eMOI_PhyDefense]);				
		
		GUILDMGR->AddGuildItemOption(&OptionInfo);
	}

	if(pMessage->dwResult >= MAX_QUERY_RESULT)
	{
		GuildItemOption(GAMERESRCMNGR->GetLoadMapNum(), OptionInfo.dwOptionIdx);
	}
	//SW051007 RGuildItemRareOption À¸·Î ¿Å±è.
//	else
//	{
////		g_pServerSystem->SetStart(TRUE);
//		
//		LoadGuildWarRecord( 0 );
//	}
}

//SW050920 Rare
void RGuildItemRareOption(LPQUERY pData, LPDBMESSAGE pMessage)
{
	ITEM_RARE_OPTION_INFO RareOptionInfo;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		RareOptionInfo.dwRareOptionIdx				= (DWORD)atoi((char*)pData[i].Data[eGORI_RareOptionID]);
		RareOptionInfo.dwItemDBIdx				= (DWORD)atoi((char*)pData[i].Data[eGORI_ItemDBID]);
		RareOptionInfo.GenGol					= (WORD)atoi((char*)pData[i].Data[eGORI_GenGol]);
		RareOptionInfo.MinChub					= (WORD)atoi((char*)pData[i].Data[eGORI_MinChub]);				
		RareOptionInfo.CheRyuk					= (WORD)atoi((char*)pData[i].Data[eGORI_CheRyuk]);				
		RareOptionInfo.SimMek					= (WORD)atoi((char*)pData[i].Data[eGORI_SimMek]);					
		RareOptionInfo.Life						= (DWORD)atoi((char*)pData[i].Data[eGORI_Life]);					
		RareOptionInfo.NaeRyuk					= (WORD)atoi((char*)pData[i].Data[eGORI_NaeRyuk]);				
		RareOptionInfo.Shield					= (DWORD)atoi((char*)pData[i].Data[eGORI_Shield]);					
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[eGORI_FireAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[eGORI_WaterAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[eGORI_TreeAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[eGORI_GoldAttrRegist])/100.f );
		RareOptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[eGORI_EarthAttrRegist])/100.f );
		RareOptionInfo.PhyAttack				= (WORD)atoi((char*)pData[i].Data[eGORI_PhyAttack]);
//		RareOptionInfo.CriticalPercent			= (WORD)atoi((char*)pData[i].Data[eGORI_Critical]);
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[i].Data[eGORI_FireAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[i].Data[eGORI_WaterAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[i].Data[eGORI_TreeAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[i].Data[eGORI_GoldAttrAttack])/100.f );
		RareOptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[i].Data[eGORI_EarthAttrAttack])/100.f );
		RareOptionInfo.PhyDefense				= (WORD)atoi((char*)pData[i].Data[eGORI_PhyDefense]);
		
		GUILDMGR->AddGuildItemRareOption(&RareOptionInfo);
	}

	if(pMessage->dwResult >= MAX_QUERY_RESULT)
	{
		GuildItemRareOption(GAMERESRCMNGR->GetLoadMapNum(), RareOptionInfo.dwRareOptionIdx);
	}
	else
	{
		LoadGuildWarRecord( 0 );
	}
}

void LoadGuildWarRecord( DWORD dwIdx )
{
	sprintf(txt, "EXEC dbo.MP_GUILDWARRECORD_Load %d", dwIdx );
	g_DB.Query(eQueryType_FreeQuery, eLoadGuildWarRecord, 0, txt);
}

void RLoadGuildWarRecord( LPQUERY pData, LPDBMESSAGE pMessage )
{
	int nStartIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		nStartIdx = atoi((char*)pData[i].Data[0]);
		DWORD dwGuildIdx = (DWORD)atoi((char*)pData[i].Data[1]);
		DWORD dwGFWVictory = (DWORD)atoi((char*)pData[i].Data[2]);
		DWORD dwGFWDraw = (DWORD)atoi((char*)pData[i].Data[3]);
		DWORD dwGFWLoose = (DWORD)atoi((char*)pData[i].Data[4]);

		GUILDFIELDWARMGR->RegistGuildFieldWarFromRecord( dwGuildIdx, dwGFWVictory, dwGFWDraw, dwGFWLoose );
	}

	if( pMessage->dwResult >= MAX_QUERY_RESULT )
	{
		LoadGuildWarRecord( nStartIdx );
	}	
	else
	{
		LoadGuildFieldWar( 0 );
	}
}

void DeleteGuildWarRecord( DWORD dwGuildIdx )
{
	sprintf(txt, "EXEC dbo.MP_GUILDWARRECORD_Delete %d", dwGuildIdx );
	g_DB.Query(eQueryType_FreeQuery, eDeleteGuildWarRecord, 0, txt);
}

void LoadGuildFieldWar( DWORD dwIdx )
{
	sprintf(txt, "EXEC dbo.MP_GUILDFIELDWAR_Load %d", dwIdx );
	g_DB.Query(eQueryType_FreeQuery, eLoadGuildFieldWar, 0, txt);
}

void RLoadGuildFieldWar( LPQUERY pData, LPDBMESSAGE pMessage )
{
	int nStartIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		nStartIdx = atoi((char*)pData[i].Data[0]);
		DWORD dwGuildIdx1 = (DWORD)atoi((char*)pData[i].Data[1]);
		DWORD dwGuildIdx2 = (DWORD)atoi((char*)pData[i].Data[2]);
		DWORD dwMoney = (DWORD)atoi((char*)pData[i].Data[3]);

		GUILDFIELDWARMGR->RegistGuildFieldWar( dwGuildIdx1, dwGuildIdx2, dwMoney );
	}

	if( pMessage->dwResult >= MAX_QUERY_RESULT )
	{
		LoadGuildFieldWar( nStartIdx );
	}	
	else
	{
//		g_pServerSystem->SetStart( TRUE );
		GuildUnionLoad( 0 );
	}
}

void InsertGuildFieldWar( DWORD dwGuildIdx1, DWORD dwGuildIdx2, DWORD dwMoney )
{
	sprintf(txt, "EXEC dbo.MP_GUILDFIELDWAR_Insert %d, %d, %u", dwGuildIdx1, dwGuildIdx2, dwMoney );
	g_DB.Query(eQueryType_FreeQuery, eInsertGuildFieldWar, 0, txt);
}

void DeleteGuildFieldWar( DWORD dwGuildIdx1, DWORD dwGuildIdx2 )
{
	sprintf(txt, "EXEC dbo.MP_GUILDFIELDWAR_Delete %d, %d", dwGuildIdx1, dwGuildIdx2 );
	g_DB.Query(eQueryType_FreeQuery, eDeleteGuildFieldWar, 0, txt);
}

void UpdateGuildFieldWarRecordToDB( DWORD dwGuildIdx, DWORD dwVitory, DWORD dwDraw, DWORD dwLoose )
{
	sprintf(txt, "EXEC dbo.MP_GUILDFIELDWAR_Record %d, %d, %d, %d", dwGuildIdx, dwVitory, dwDraw, dwLoose );
	g_DB.Query(eQueryType_FreeQuery, eUpdateGuildFieldWarRecord, 0, txt);
}


void CharacterChangeName( DWORD CharacterIdx, char* Name, DWORD DBIdx )
{
	sprintf(txt, "EXEC %s %d, \'%s\', %d", STORED_SHOPITEM_CHANGENAME, CharacterIdx, Name, DBIdx );
	g_DB.Query(eQueryType_FreeQuery, eItemShopChangeName, CharacterIdx, txt);
}


void RCharacterChangeName( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer * pPlayer = (CPlayer*)g_pUserTable->FindUser( pMessage->dwID );
	if(!pPlayer)		return;

	DWORD Result = (DWORD)atoi((char*)pData->Data[0]);
	DWORD DBIdx = 0;


	MSG_DWORD msg;
	msg.Category = MP_ITEM;

	switch( Result )
	{
	case 0:		// ¼º°ø
		{
			DBIdx = (DWORD)atoi((char*)pData->Data[1]);
			ITEMBASE* pItemBase = NULL;

			// Item Position È®ÀÎ
			for(int i=0; i<SLOT_SHOPINVEN_NUM/2; i++)
			{
				pItemBase = (ITEMBASE*)ITEMMGR->GetItemInfoAbsIn( pPlayer, i+TP_SHOPINVEN_START );
				if( !pItemBase )	continue;
				
				if( pItemBase->dwDBIdx == DBIdx )
				{
					if( EI_TRUE != ITEMMGR->DiscardItem( pPlayer, i+TP_SHOPINVEN_START, pItemBase->wIconIdx, 1 ) )
					{
						char buf[64];
						sprintf(buf, "ChangeName OK, DiscardItem Fail : %d", pMessage->dwID);
						ASSERTMSG(0, buf);
					}

					break;
				}
			}
			
			if( i >= SLOT_SHOPINVEN_NUM/2 )
			{
				char buf[64];
				sprintf(buf, "ChangeName OK, DeleteItem Fail: %d", pMessage->dwID);
				ASSERTMSG(0, buf);
			}

			DWORD time = GetCurTime();
			UsingShopItemUpdateToDB( 0, pPlayer->GetID(), pItemBase->wIconIdx, pItemBase->dwDBIdx, 0, time, 0 );
			pPlayer->GetShopItemManager()->DeleteUsingShopItem( pItemBase->wIconIdx );
			
			SHOPITEMBASE ShopItemBase;
			ShopItemBase.BeginTime.value = time;
			ShopItemBase.Param = 0;
			memcpy( &ShopItemBase.ItemBase, pItemBase, sizeof(ITEMBASE) );
			pPlayer->GetShopItemManager()->AddShopItemUse( &ShopItemBase );

			msg.Protocol = MP_ITEM_SHOPITEM_NCHANGE_ACK;
			msg.dwData = Result;


			// magi82 - ¹®ÇÏ»ý°ü·Ã(070123) /////////////////////////////////////////////

			DWORD dwGuildIdx = 0;
			BOOL bFlag = FALSE;
			PTRLISTPOS pos = GUILDMGR->GetGuildTraineeList()->GetHeadPosition();
			GUILD_TRAINEE_LIST* pTrainee;
			while(pos)
			{
				pTrainee = (GUILD_TRAINEE_LIST*)GUILDMGR->GetGuildTraineeList()->GetNext(pos);
				if( pTrainee )
				{
					if( pTrainee->dwUserIdx == pMessage->dwID)
					{
						bFlag = TRUE;
						GuildTraineeDelete(pTrainee->dwUserIdx);
						dwGuildIdx = pTrainee->dwGuildIdx;
						GUILDMGR->GetGuildTraineeList()->Remove(pTrainee);
						break;
					}
				}
			}

			if(!bFlag)
				break;

			// Brodcasting to Mapserver
			MSG_DWORD msg;
			SetProtocol( &msg, MP_GUILD, MP_GUILD_MUNHA_DELETE_BROD );
			msg.dwData = pMessage->dwID;
			PACKEDDATA_OBJ->SendToBroadCastMapServer( &msg, sizeof(msg) );

			// ¹®ÁÖÇÑÅ× º¸³»ÁØ´Ù
			CGuild* pGuild = (CGuild*)GUILDMGR->GetGuild( dwGuildIdx );
			if( !pGuild )
				return;

			MSG_NAME_DWORD Sendmsg;
			SetProtocol( &Sendmsg, MP_GUILD, MP_GUILD_MUNHA_NAMECHANGE_SYN );
			Sendmsg.dwObjectID = pGuild->GetGuildInfo()->MasterIdx;
			Sendmsg.dwData = pGuild->GetGuildInfo()->GuildIdx;
			SafeStrCpy( Sendmsg.Name, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1 );
			g_Network.Send2AgentServer((char*)&Sendmsg, sizeof(Sendmsg));

			// ºÎ¹®ÁÖ³ª Àå·ÎÇÑÅ×µµ º¸³¿
			pos = pGuild->GetMemberList()->GetHeadPosition();
			while(pos)
			{
				GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)pGuild->GetMemberList()->GetNext(pos);

				if( pInfo )
				{
					if(pInfo->Rank == GUILD_SENIOR || pInfo->Rank == GUILD_VICEMASTER)
					{
						MSG_NAME_DWORD Sendmsg;
						SetProtocol( &Sendmsg, MP_GUILD, MP_GUILD_MUNHA_NAMECHANGE_SYN );
						Sendmsg.dwObjectID = pInfo->MemberIdx;
						Sendmsg.dwData = pGuild->GetGuildInfo()->GuildIdx;
						SafeStrCpy( Sendmsg.Name, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1 );
						g_Network.Send2AgentServer((char*)&Sendmsg, sizeof(Sendmsg));
					}
				}
			}

			//////////////////////////////////////////////////////////////////////////
		}
		break;
	default:
		msg.Protocol = MP_ITEM_SHOPITEM_NCHANGE_NACK;
		msg.dwData = Result;
		break;
	}

	pPlayer->SendMsg( &msg, sizeof(msg) );	
}

void CharacterChangeInfoToDB( DWORD CharacterIdx, DWORD Gender, DWORD HairType, DWORD FaceType, float Height, float Width )
{
	sprintf(txt, "EXEC %s %d, %d, %d, %d, %f, %f", STORED_SHOPITEM_CHARCHANGE, CharacterIdx, Gender, HairType, FaceType, Height, Width );
	g_DB.Query(eQueryType_FreeQuery, eItemCharChange, 0, txt);
}


// Guild Tournament
void GuildTournamentInfoLoad()
{
	sprintf(txt, "EXEC %s", STORED_GT_LOADALL );
	g_DB.Query(eQueryType_FreeQuery, eGuildTournament_LoadAll, 0, txt);
}

void GuildTournamentCancelIdxLoad( DWORD TournamentIdx )
{
	sprintf(txt, "EXEC %s %d", STORED_GT_CANCELLOAD, TournamentIdx );
	g_DB.Query(eQueryType_FreeQuery, eGuildTournament_CancelLoad, 0, txt);
}

void GuildTournamentCancelIdxInsert( DWORD TournamentIdx, DWORD GuildIdx )
{
	sprintf(txt, "EXEC %s %d, %d", STORED_GT_CANCELINSERT, TournamentIdx, GuildIdx );
	g_DB.Query(eQueryType_FreeQuery, eGuildTournament_CancelInsert, 0, txt);
}

void GuildTournamentGuildInfoInsert( DWORD GuildIdx, char* GuildName, DWORD Position  )
{
	sprintf(txt, "EXEC %s %d, \'%s\', %d", STORED_GT_GUILDINFO_INSERT, GuildIdx, GuildName, Position );
	g_DB.Query(eQueryType_FreeQuery, eGuildTournament_Insert, 0, txt);
}

void GuildTournamentGuildInfoDelete( DWORD GuildIdx )
{
	sprintf(txt, "EXEC %s %d", STORED_GT_GUILDINFO_DELETE, GuildIdx );
	g_DB.Query(eQueryType_FreeQuery, eGuildTournament_Delete, 0, txt);
}

void GuildTournamentGuildInfoUpdate( DWORD GuildIdx, DWORD Ranking, DWORD ProcessTournament )
{
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_GT_GUILDINFO_UPDATE, GuildIdx, Ranking, ProcessTournament );
	g_DB.Query(eQueryType_FreeQuery, eGuildTournament_UpdateGuildInfo, 0, txt);
}

void GuildTournamentInfoUpdate( DWORD TournamentIdx, DWORD TournamentFight, DWORD WaitTime )
{
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_GT_TOURNAMENTINFO_UPDATE, TournamentIdx, TournamentFight, WaitTime );
	g_DB.Query(eQueryType_FreeQuery, eGuildTournament_UpdateTournamentInfo, 0, txt);
}

void RGuildTournamentInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD TournamentIdx = 0;
	DWORD CurTournament = 0;
	DWORD WaitTime = 0;
	GTDBLoadInfo GuildInfo[MAXGUILD_INTOURNAMENT];
	memset( GuildInfo, 0, sizeof(GTDBLoadInfo)*MAXGUILD_INTOURNAMENT );

	int count = 0;
	for( DWORD i=0; i<pMessage->dwResult; ++i )
	{
		// magi82 ¹®ÆÄÅä³Ê¸ÕÆ® -> 32ÆÀ ÀÌ»óÀÇ µ¥ÀÌÅÍ°¡ ¸®ÅÏµÇ´Â °æ¿ì¸¦ À§ÇÑ ¿¹¿ÜÃ³¸®(ÀÓ½Ã)
		// DB¿¡¼­ Æ÷Áö¼ÇÀÌ °ãÃÄÁö´Â ¹®ÆÄµéÀ» Ã³¸®ÇØ¾ßÇÑ´Ù.
		if( count >= MAXGUILD_INTOURNAMENT )
		{
			if(IDYES == MessageBox(NULL, "Error - RGuildTournamentInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage )", NULL, MB_YESNO|MB_TOPMOST))
				__asm int 3;
			return;
		}

		GuildInfo[count].GuildIdx = (DWORD)atoi((char*)pData[i].Data[eGT_GuildIdx]);
		if( GuildInfo[count].GuildIdx == 0 )
		{
			TournamentIdx = (DWORD)atoi((char*)pData[i].Data[eGT_Position]);
			CurTournament = (BYTE)atoi((char*)pData[i].Data[eGT_Process]);
			WaitTime = (DWORD)atoi((char*)pData[i].Data[eGT_WaitTime]);
			continue;
		}
		GuildInfo[count].Position = (BYTE)atoi((char*)pData[i].Data[eGT_Position]);
		GuildInfo[count].Ranking = (BYTE)atoi((char*)pData[i].Data[eGT_Ranking]);
		GuildInfo[count].ProcessTournament= (BYTE)atoi((char*)pData[i].Data[eGT_Process]);
		++count;
	}

	if( TournamentIdx == 0 )			return;

	GTMGR->SetTournamentInfo( TournamentIdx, CurTournament, WaitTime );
	GTMGR->SetGuildInfo( GuildInfo, pMessage->dwResult );
}

void RGuildTournamentCancelIdxLoad( LPQUERY pData, LPDBMESSAGE pMessage )
{
	for( DWORD i=0; i<pMessage->dwResult; ++i )
	{
		DWORD GuildIdx = (DWORD)atoi((char*)pData[i].Data[eGT_GuildIdx]);
		GTMGR->AddCancelGuild( GuildIdx );
	}
}


void GuildTournamentAddLog( DWORD TournamentIdx, DWORD GuildIdx, DWORD Logkind, DWORD Ranking )
{
	sprintf( txt, "EXEC dbo.Up_GT_AddLog %d, %d, %d, %d", TournamentIdx, GuildIdx, Logkind, Ranking );
	g_DB.LogQuery( eQueryType_FreeQuery, eGuildTournamentAddLog, 0, txt );
}

// jackpot
/*
void JackpotLoadTotalMoney()
{
	sprintf( txt, "EXEC dbo.MP_JACKPOT_LoadMoney" );
	g_DB.Query( eQueryType_FreeQuery, eJackpot_TotalMoney_Load, 0, txt );
}

void RJackpotLoadTotalMoney( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( pMessage->dwResult == 0 ) return;

	DWORD TotalMoney = (DWORD)atoi((char*)pData->Data[0]);
	JACKPOTMGR->SetTotalMoney( TotalMoney );
}*/

void JackpotAddTotalMoney( MONEYTYPE MapMoney )
{
	sprintf( txt, "EXEC dbo.MP_JACKPOT_AddTotalMoney %u", MapMoney );
	g_DB.Query( eQueryType_FreeQuery, eJackpot_TotalMoney_Update, 0, txt );
}
/*
void RJackpotAddTotalMoney( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( pMessage->dwResult == 0 ) return;

	DWORD TotalMoney = (DWORD)atoi((char*)pData->Data[0]);
	JACKPOTMGR->SendMsgMapUserTotalMoney( TotalMoney );
}*/

void JackpotPrizeInfo( MONEYTYPE MapMoney, DWORD PlayerID, DWORD PrizeKind, DWORD PrizePercentage, DWORD dwAbsMoney)
{
	sprintf( txt, "EXEC dbo.MP_JACKPOT_Prize %u, %d, %d, %d, %u", MapMoney, PlayerID, PrizeKind, PrizePercentage, dwAbsMoney );
	g_DB.Query( eQueryType_FreeQuery, eJackpot_PrizeInfo_Update, PlayerID, txt );
}

void RJackpotPrizeInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( pMessage->dwResult == 0 ) return;
	
	DWORD TotalMoney = (DWORD)atoi((char*)pData->Data[0]);
	DWORD PrizeMoney = (DWORD)atoi((char*)pData->Data[1]);	
	DWORD PrizeKind = (DWORD)atoi((char*)pData->Data[2]);
//	CPlayer * pPlayer = (CPlayer*)g_pUserTable->FindUser( pMessage->dwID );
//	if(!pPlayer)		return;
	DWORD PlayerID = pMessage->dwID;

	JACKPOTMGR->SetPrizeInfo( TotalMoney, PrizeMoney, PlayerID, PrizeKind ); //µÎ°¡Áö ÀÏÀ»ÇÑ´Ù. ÇØ´ç ID·Î PrizeMoneyµî·Ï°ú Àü¼­¹ö °øÁö
}

// guildunion
void GuildUnionLoad( DWORD dwStartGuildUnionIdx )
{
	sprintf( txt, "EXEC dbo.MP_GUILD_UNION_Load %d", dwStartGuildUnionIdx );
	g_DB.Query( eQueryType_FreeQuery, eGuildUnionLoad, 0, txt );
}

void RGuildUnionLoad( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD dwStartGuildIdx = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		dwStartGuildIdx = (DWORD)atoi((char*)pData[i].Data[0]);

		//SW070112 º¯°æ
/*
		GUILDUNIONMGR->LoadGuildUnion( (DWORD)atoi((char*)pData[i].Data[0])		// guildidx
			, (char*)pData[i].Data[1]					// name
			, (DWORD)atoi((char*)pData[i].Data[2])		// 0
				, (DWORD)atoi((char*)pData[i].Data[3])		// 1
				, (DWORD)atoi((char*)pData[i].Data[4])		// 2
				, (DWORD)atoi((char*)pData[i].Data[5])		// 3
				, (DWORD)atoi((char*)pData[i].Data[6])		// 4
				, (DWORD)atoi((char*)pData[i].Data[7])		// 5
				, (DWORD)atoi((char*)pData[i].Data[8])		// 6
				, (DWORD)atoi((char*)pData[i].Data[9]) );	// mark
			*/
		DWORD j = 0;
		DWORD UnionGuildIdx[MAX_GUILD_UNION_NUM] = {0,};
		while( j < MAX_GUILD_UNION_NUM )
		{
			UnionGuildIdx[j] = (DWORD)atoi((char*)pData[i].Data[j+2]);
			++j;
		}
		
		GUILDUNIONMGR->LoadGuildUnion( (DWORD)atoi((char*)pData[i].Data[0])
			, (char*)pData[i].Data[1]		//name
			, (DWORD)atoi((char*)pData[i].Data[9])
			, UnionGuildIdx
			);
	}
	if( pMessage->dwResult >= MAX_QUERY_RESULT )
	{
		GuildUnionLoad( dwStartGuildIdx );
	}	
	else
	{
		GuildUnionLoadMark( 0 );
	}	
}

void GuildUnionLoadMark( DWORD dwMarkIdx )
{
	sprintf( txt, "EXEC dbo.MP_GUILD_UNION_LoadMark %d", dwMarkIdx );
	g_DB.FreeLargeQuery( RGuildUnionLoadMark, 0, txt );
}

void RGuildUnionLoadMark( LPLARGEQUERY pData, LPDBMESSAGE pMessage )
{
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		GUILDUNIONMGR->LoadGuildUnionMark( (DWORD)atoi((char*)pData->Data[0]), (DWORD)atoi((char*)pData->Data[1]), (char*)pData->Data[2] );
	}
	if( pMessage->dwResult )
		GuildUnionLoadMark( (DWORD)atoi((char*)pData->Data[0]) );
	else
	{
#ifdef _JAPAN_LOCAL_
		g_pServerSystem->SetStart( TRUE );
//		SiegeWarProfitInfoLoad();
#else
//		g_pServerSystem->SetStart( TRUE );
		SiegeWarProfitInfoLoad();
#endif
	}
}

void GuildUnionCreate( DWORD dwCharacterIdx, DWORD dwGuildIdx, char* sUnionName )
{
	sprintf( txt, "EXEC dbo.MP_GUILD_UNION_Create %d, %s", dwGuildIdx, sUnionName );
	g_DB.Query( eQueryType_FreeQuery, eGuildUnionCreate, dwCharacterIdx, txt );
}

void RGuildUnionCreate( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( !pMessage->dwResult )	return;
	if( atoi((char*)pData->Data[0]) == 0 )
	{
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pMessage->dwID );
		if( pPlayer )
			GUILDUNIONMGR->SendNackMsg( pPlayer, MP_GUILD_UNION_CREATE_NACK, eGU_Not_ValidName );
		return;
	}
	else
		GUILDUNIONMGR->GuildUnionCreateResult( (DWORD)atoi((char*)pData->Data[0]), (char*)pData->Data[1], (DWORD)atoi((char*)pData->Data[2]) );
}

void GuildUnionDestroy( DWORD dwGuildUnionIdx, DWORD dwMarkIdx, DWORD dwMasterIdx, DWORD dwTime )
{
	sprintf( txt, "EXEC dbo.MP_GUILD_UNION_Destroy %d, %d, %d, %d", dwGuildUnionIdx, dwMarkIdx, dwMasterIdx, dwTime );
	g_DB.Query( eQueryType_FreeQuery, eGuildUnionDestroy, 0, txt );
}

void GuildUnionAddGuild( DWORD dwGuildUnionIdx, int nIndex, DWORD dwGuildIdx )
{
	sprintf( txt, "EXEC dbo.MP_GUILD_UNION_AddGuild %d, %d, %d", dwGuildUnionIdx, nIndex, dwGuildIdx );
	g_DB.Query( eQueryType_FreeQuery, eGuildUnionAddGuild, 0, txt );
}

void GuildUnionRemoveGuild( DWORD dwGuildUnionIdx, int nIndex, DWORD dwGuildIdx )
{
	sprintf( txt, "EXEC dbo.MP_GUILD_UNION_RemoveGuild %d, %d, %d", dwGuildUnionIdx, nIndex, dwGuildIdx );
	g_DB.Query( eQueryType_FreeQuery, eGuildUnionRemoveGuild, 0, txt );
}

void GuildUnionSecedeGuild( DWORD dwGuildUnionIdx, int nIndex, DWORD dwGuildIdx, DWORD dwTime )
{
	sprintf( txt, "EXEC dbo.MP_GUILD_UNION_SecedeGuild %d, %d, %d, %d", dwGuildUnionIdx, nIndex, dwGuildIdx, dwTime );
	g_DB.Query( eQueryType_FreeQuery, eGuildUnionSecedeGuild, 0, txt );
}

void GuildUnionMarkRegist( DWORD dwCharacterIdx, DWORD dwMarkIdx, DWORD dwGuildUnionIdx, char* pImgData )
{
	char buf[1024];
	sprintf( buf, "EXEC dbo.MP_GUILD_UNION_MarkRegist %d, %d, %d, 0x", dwCharacterIdx, dwMarkIdx, dwGuildUnionIdx );

	int curpos = strlen(buf);
	for( int n = 0; n < GUILDUNIONMARK_BUFSIZE; ++n )
	{
		sprintf( &buf[curpos], "%02x", (BYTE)pImgData[n] );
		curpos += 2;
	}

	g_DB.FreeLargeQuery( RGuildUnionMarkRegist, 0, buf );
}

void RGuildUnionMarkRegist( LPLARGEQUERY pData, LPDBMESSAGE pMessage )
{
	if( !pMessage->dwResult )	return;
	GUILDUNIONMGR->GuildUnionMarkRegistResult( (DWORD)atoi((char*)pData->Data[0]), (DWORD)atoi((char*)pData->Data[1]),
		(DWORD)atoi((char*)pData->Data[2]), (char*)pData->Data[3] );
}

//¦®¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¯
//¦­ SiegeWar													  ¦­
//¦±¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦°
void SiegeWarInfoLoad( DWORD MapNum )
{
	sprintf( txt, "EXEC %s %d", STORED_SIEGE_INFOLOAD, MapNum );
	g_DB.Query( eQueryType_FreeQuery, eSiegeWarInfoLoad, 0, txt );
}

void RSiegeWarInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD SiegeWarIdx = 0;
	DWORD TaxRate = 0;
	DWORD RegistTime = 0;
	DWORD SiegeWarTime = 0;
	DWORD Level = 0;
	
	SiegeWarIdx = (DWORD)atoi((char*)pData->Data[eSWI_SiegeWarIdx]);
	RegistTime = (DWORD)atoi((char*)pData->Data[eSWI_RegistTime]);
	SiegeWarTime = (DWORD)atoi((char*)pData->Data[eSWI_SiegeWarTime]);
	Level = (DWORD)atoi((char*)pData->Data[eSWI_SiegeWarLevel]);

	if( SiegeWarIdx == 0 )			return;

	SIEGEWARMGR->SetSiegeWarInfo( SiegeWarIdx, TaxRate, RegistTime, SiegeWarTime, Level );
	SiegeWarGuildInfoLoad( 0, SiegeWarIdx, SIEGEWARMGR->GetSiegeMapNum() );
}

void SiegeWarInfoInsert( DWORD SiegeWarIdx, DWORD RegistTime, DWORD SiegeWarTime, DWORD Level, DWORD MapNum )
{
	sprintf( txt, "EXEC %s %d, %d, %d, %d, %d", STORED_SIEGE_INFOINSERT, SiegeWarIdx, RegistTime, SiegeWarTime, Level, MapNum );
	g_DB.Query( eQueryType_FreeQuery, eSiegeWarInfoInsert, 0, txt );
}

void SiegeWarInfoUpdate( DWORD SiegeWarIdx, DWORD RegistTime, DWORD SiegeWarTime, DWORD Level, DWORD MapNum )
{
	sprintf( txt, "EXEC %s %d, %d, %d, %d, %d", STORED_SIEGE_INFOUPDATE, SiegeWarIdx, RegistTime, SiegeWarTime, Level, MapNum );
	g_DB.Query( eQueryType_FreeQuery, eSiegeWarInfoUpdate, 0, txt );
}

void SiegeWarGuildInfoLoad( DWORD StartDBIdx, DWORD SiegeWarIdx, DWORD MapNum )
{
	sprintf( txt, "EXEC %s %d, %d, %d", STORED_SIEGE_GUILDLOAD, StartDBIdx, SiegeWarIdx, MapNum );
	g_DB.Query( eQueryType_FreeQuery, eSiegeWarGuildInfoLoad, 0, txt );
}

void RSiegeWarGuildInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD LastDBIdx = 0;
	SIEGEWAR_GUILDDBINFO GuildInfo[100];

	for( DWORD i=0; i<pMessage->dwResult; ++i )
	{
		LastDBIdx = (DWORD)atoi((char*)pData[i].Data[eSWGI_GuildDBIdx]);
		GuildInfo[i].GuildIdx = (DWORD)atoi((char*)pData[i].Data[eSWGI_GuildIdx]);
		GuildInfo[i].Type = (BYTE)atoi((char*)pData[i].Data[eSWGI_GuildType]);
	}

	SIEGEWARMGR->SetSiegeGuildInfo( GuildInfo, pMessage->dwResult );

	if( pMessage->dwResult >= 100 )
		SiegeWarGuildInfoLoad( LastDBIdx, SIEGEWARMGR->GetSiegeWarIdx(), SIEGEWARMGR->GetSiegeMapNum() );
	else
	{
		if( g_pServerSystem->GetMapNum() == SIEGEWARMGR->GetSiegeMapNum() )
			SIEGEWARMGR->CreateSiegeBattle();
	}
}

void SiegeWarGuildInsert( DWORD SiegeWarIdx, DWORD GuildIdx, DWORD Type, DWORD MapNum )
{
	sprintf( txt, "EXEC %s %d, %d, %d, %d", STORED_SIEGE_GUILDINSERT, SiegeWarIdx, GuildIdx, Type, MapNum );
	g_DB.Query( eQueryType_FreeQuery, eSiegeWarGuildInsert, 0, txt );
}

void SiegeWarGuildUpdate( DWORD SiegeWarIdx, DWORD GuildIdx, DWORD Type, DWORD MapNum )
{
	sprintf( txt, "EXEC %s %d, %d, %d, %d", STORED_SIEGE_GUILDUPDATE, SiegeWarIdx, GuildIdx, Type, MapNum );
	g_DB.Query( eQueryType_FreeQuery, eSiegeWarGuildUpdate, 0, txt );
}

void SiegeWarGuildDelete( DWORD SiegeWarIdx, DWORD GuildIdx, DWORD MapNum )
{
	sprintf( txt, "EXEC %s %d, %d, %d", STORED_SIEGE_GUILDDELETE, SiegeWarIdx, GuildIdx, MapNum );
	g_DB.Query( eQueryType_FreeQuery, eSiegeWarGuildDelete, 0, txt );
}

void SiegeWarAddLog( DWORD SiegeWarIdx, DWORD MapNum, DWORD GuildIdx, DWORD Logkind, DWORD Value )
{
	sprintf( txt, "EXEC dbo.Up_SW_AddlOg %d, %d, %d, %d, %d", SiegeWarIdx, MapNum, GuildIdx, Logkind, Value );
	g_DB.LogQuery( eQueryType_FreeQuery, eSiegeWarAddLog, 0, txt );
}


void ChangeCharacterAttr( DWORD dwPlayerIdx, DWORD dwAttr )
{
	sprintf( txt, "EXEC dbo.MP_CHARACTER_ChangeAttr_JP %d, %d", dwPlayerIdx, dwAttr );
	g_DB.Query( eQueryType_FreeQuery, eChangeCharacterAttr, 0, txt );
}

void SiegeWarProfitInfoLoad()
{
	sprintf( txt, "EXEC dbo.MP_SIEGEWAR_PROFIT_InfoLoad" );
	g_DB.Query( eQueryType_FreeQuery, eSiegeWarProfitInfoLoad, 0, txt );
}

void RSiegeWarProfitInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage )
{
	sSWPROFIT sInfo;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		memset( &sInfo, 0, sizeof(sInfo) );
		sInfo.nSWMapNum = atoi((char*)pData[i].Data[0]);
		sInfo.dwGuildIdx = (DWORD)atoi((char*)pData[i].Data[1]);
		sInfo.nTaxRateForStore = atoi((char*)pData[i].Data[2]);
		sInfo.dwTotalMoney = (DWORD)atoi((char*)pData[i].Data[3]);
		sInfo.RegDate.value = (DWORD)atoi((char*)pData[i].Data[4]);
		SWPROFITMGR->AddProfitTable( sInfo );
	}	

	//
	//g_pServerSystem->SetStart( TRUE );

#ifdef _HK_LOCAL_	//hk block
	g_pServerSystem->SetStart( TRUE );
#elif defined _JAPAN_LOCAL_	//japan block
	g_pServerSystem->SetStart( TRUE );
	//GuildLoadGuildPointInfo(0);
#else
	//SW060719 ¹®ÆÄÆ÷ÀÎÆ®
	GuildLoadGuildPointInfo(0);
#endif
}

void SiegeWarProfitUpdateGuild( int nMapNum, DWORD dwGuildIdx )
{
	sprintf( txt, "EXEC dbo.MP_SIEGEWAR_PROFIT_UpdateGuild %d, %d", nMapNum, dwGuildIdx );
	g_DB.Query( eQueryType_FreeQuery, eSiegeWarProfitUpdateGuild, 0, txt );
}

void SiegeWarProfitUpdateTaxRate( int nMapNum, int nTaxRate, DWORD dwRegDate )
{
	sprintf( txt, "EXEC dbo.MP_SIEGEWAR_PROFIT_UpdateTaxRate %d, %d, %d", nMapNum, nTaxRate, dwRegDate );
	g_DB.Query( eQueryType_FreeQuery, eSiegeWarProfitUpdateTaxRate, 0, txt );
}

void SiegeWarProfitUpdateProfitMoney( int nMapNum, DWORD dwMoney, int nType )
{
	sprintf( txt, "EXEC dbo.MP_SIEGEWAR_PROFIT_UpdateProfitMoney %d, %u, %d", nMapNum, dwMoney, nType );
	g_DB.Query( eQueryType_FreeQuery, eSiegeWarProfitUpdateProfitMoney, 0, txt );
}

void RSiegeWarProfitUpdateProfitMoney( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( !pMessage->dwResult ) return;

	SWPROFITMGR->SetProfitMoney( atoi((char*)pData[0].Data[0]), (DWORD)atoi((char*)pData[0].Data[1]) );
}

void ChangeGuildLocation( DWORD dwGuildIdx, WORD wMapNum )
{
	sprintf( txt, "EXEC dbo.MP_GUILD_ChangeLocation %d, %d", dwGuildIdx, wMapNum );
	g_DB.Query( eQueryType_FreeQuery, eChangeGuildLocation, 0, txt );	
}

void ChangeCharacterStageAbility( DWORD dwCharacterIdx, BYTE stage, CAbilityGroup* pGroup )
{
	ABILITY_TOTALINFO TInfo;
	ABILITYMGR->GetAbilityTotalInfo( pGroup, &TInfo );

	char battle[MAX_ABILITY_NUM_PER_GROUP+1] = {0,};
	char hwa[6] = {0,};
	char geuk[6] = {0,};
	memcpy( battle, TInfo.AbilityDataArray[eAGK_Battle], MAX_ABILITY_NUM_PER_GROUP+1 );
	memcpy( hwa, &TInfo.AbilityDataArray[eAGK_Battle][17], 6 );
	memcpy( geuk, &TInfo.AbilityDataArray[eAGK_Battle][23], 6 );

	if( stage == eStage_Hwa )
	{
		memcpy( &battle[17], "AAAAAA", 6 );
		memcpy( &battle[23], hwa, 6 );
	}
	else if( stage == eStage_Geuk )
	{
		memcpy( &battle[17], geuk, 6 );
		memcpy( &battle[23], "AAAAAA", 6 );
	}
	else if( stage == eStage_Normal )
	{
		memcpy( &battle[17], "AAAAAA", 6 );
		memcpy( &battle[23], "AAAAAA", 6 );
	}

	sprintf( txt, "EXEC dbo.MP_CHARACTER_ChangeStage_Ability %d, \'%s\'", dwCharacterIdx, battle );
	g_DB.Query( eQueryType_FreeQuery, eChangeCharacterStageAbility, 0, txt );
}

void RShopItemRareInsertToDB(LPQUERY pData,  LPDBMESSAGE pMessage)
{
	DWORD characteridx = (DWORD)atoi((char*)pData[0].Data[eCIR_ObjectID]);
	DWORD pos = (DWORD)atoi((char*)pData[0].Data[eCIR_Position]);

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( characteridx );
	if( !pPlayer )			return;
	
	ITEMBASE* pItem = (ITEMBASE*)ITEMMGR->GetItemInfoAbsIn( pPlayer, (POSTYPE)pos );
	if( !pItem )			return;

	if( pItem->RareIdx )
	{
		if( FALSE == pPlayer->RemoveItemRareOption( pItem->RareIdx ) )
		{
			char szBuf[128];
			sprintf( szBuf, "ShopItemRareInsert_Error! PlayerID : %d\t ItemDBIdx : %d\n", pPlayer->GetID(),
				pItem->dwDBIdx );
			ASSERTMSG(0, szBuf);
		}
	}


	ITEM_RARE_OPTION_INFO RareOptionInfo;
	RareOptionInfo.dwRareOptionIdx				= (DWORD)atoi((char*)pData[0].Data[eCIR_RareID]);	//!»ó¼ö µû·Î ¾È¸¸µé¾îµµ µÉµí
	RareOptionInfo.dwItemDBIdx				= (DWORD)atoi((char*)pData[0].Data[eCIR_ItemDBID]);
	RareOptionInfo.GenGol					= (WORD)atoi((char*)pData[0].Data[eCIR_GenGol]);
	RareOptionInfo.MinChub					= (WORD)atoi((char*)pData[0].Data[eCIR_MinChub]);				
	RareOptionInfo.CheRyuk					= (WORD)atoi((char*)pData[0].Data[eCIR_CheRyuk]);				
	RareOptionInfo.SimMek					= (WORD)atoi((char*)pData[0].Data[eCIR_SimMek]);					
	RareOptionInfo.Life						= (DWORD)atoi((char*)pData[0].Data[eCIR_Life]);					
	RareOptionInfo.NaeRyuk					= (WORD)atoi((char*)pData[0].Data[eCIR_NaeRyuk]);				
	RareOptionInfo.Shield					= (DWORD)atoi((char*)pData[0].Data[eCIR_Shield]);					
	RareOptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[0].Data[eCIR_FireAttrRegist])/100.f );
	RareOptionInfo.AttrRegist.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[0].Data[eCIR_WaterAttrRegist])/100.f );
	RareOptionInfo.AttrRegist.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[0].Data[eCIR_TreeAttrRegist])/100.f );
	RareOptionInfo.AttrRegist.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[0].Data[eCIR_GoldAttrRegist])/100.f );
	RareOptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[0].Data[eCIR_EarthAttrRegist])/100.f );
	RareOptionInfo.PhyAttack				= (WORD)atoi((char*)pData[0].Data[eCIR_PhyAttack]);
	//		RareOptionInfo.CriticalPercent			= (WORD)atoi((char*)pData[0].Data[eCIO_Critical]);
	RareOptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE,		(float)atoi((char*)pData[0].Data[eCIR_FireAttrAttack])/100.f );
	RareOptionInfo.AttrAttack.SetElement_Val(ATTR_WATER,	(float)atoi((char*)pData[0].Data[eCIR_WaterAttrAttack])/100.f );
	RareOptionInfo.AttrAttack.SetElement_Val(ATTR_TREE,		(float)atoi((char*)pData[0].Data[eCIR_TreeAttrAttack])/100.f );
	RareOptionInfo.AttrAttack.SetElement_Val(ATTR_IRON,		(float)atoi((char*)pData[0].Data[eCIR_GoldAttrAttack])/100.f );
	RareOptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH,	(float)atoi((char*)pData[0].Data[eCIR_EarthAttrAttack])/100.f );
	RareOptionInfo.PhyDefense				= (WORD)atoi((char*)pData[0].Data[eCIR_PhyDefense]);

	pPlayer->AddItemRareOption(&RareOptionInfo);

	pItem->RareIdx = RareOptionInfo.dwRareOptionIdx;

	// Log±â·Ï
	LogItemRareOption(pPlayer->GetID(), pItem->dwDBIdx, &RareOptionInfo);

	MSG_ITEM_RAREITEM_GET msg;
	SetProtocol( &msg, MP_ITEM, MP_ITEM_SHOPITEM_RARECREATE_ACK );
	msg.dwObjectID = pPlayer->GetID();	//pPlayer->GetID();
	msg.RareItemBase = *pItem;
	msg.RareInfo = RareOptionInfo;
	pPlayer->SendMsg(&msg, sizeof(msg));
}

void QuestEventCheck( DWORD dwUserIdx, DWORD dwCharacterIdx, DWORD dwNpcIdx, DWORD dwQuestIdx, char* pTime )
{
	sprintf( txt, "EXEC dbo.MP_QUEST_EventCheck %d, %d, %d, %d, \'%s\'", dwUserIdx, dwCharacterIdx, dwNpcIdx, dwQuestIdx, pTime );
	g_DB.Query( eQueryType_FreeQuery, eQuestEventCheck, dwCharacterIdx, txt );

/*
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( dwCharacterIdx );
		if( !pPlayer )	return;
	
		MSG_DWORD2 Msg;
		Msg.Category = MP_QUEST; 
	
		DWORD dwRet = 0;
		switch( dwRet )
		{
		case 0:		// ÇÒ¼öÀÖ´Ù
			{
				Msg.Protocol = MP_QUESTEVENT_NPCTALK_ACK;
	
			}
			break;
		case 1:		// ÀÌ¹Ì Çß´Ù
			{
				Msg.Protocol = MP_QUESTEVENT_NPCTALK_NACK;
			}
			break;
		case 2:		// ÇÒ ¼ö ¾ø´Ù
			{
				Msg.Protocol = MP_QUESTEVENT_NPCTALK_NACK;
			}
			break;
		default:
			return;
		}
	
		Msg.dwData1 = dwQuestIdx;
		Msg.dwData2 = dwRet;
		pPlayer->SendMsg( &Msg, sizeof(Msg) );*/
	
}

void RQuestEventCheck( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pMessage->dwID );
	if( !pPlayer )	return;

	MSG_DWORD2 Msg;
	Msg.Category = MP_QUEST; 

	DWORD dwNpcIdx = (DWORD)atoi((char*)pData[0].Data[0]);
	DWORD dwQuestIdx = (DWORD)atoi((char*)pData[0].Data[1]);
	DWORD dwRet = (DWORD)atoi((char*)pData[0].Data[2]);
	switch( dwRet )
	{
	case 0:		// ÇÒ¼öÀÖ´Ù
		{
			Msg.Protocol = MP_QUESTEVENT_NPCTALK_ACK;
			
			// quest event
			CQuestEvent qe;
			qe.SetValue( eQuestEvent_NpcTalk, dwNpcIdx, dwQuestIdx );
			QUESTMGR->AddQuestEvent( pPlayer, &qe );
		}
		break;
	case 1:		// ÀÌ¹Ì Çß´Ù
		{
			Msg.Protocol = MP_QUESTEVENT_NPCTALK_NACK;
		}
		break;
	case 2:		// ÇÒ ¼ö ¾ø´Ù
		{
			Msg.Protocol = MP_QUESTEVENT_NPCTALK_NACK;
		}
		break;
	default:
		return;
	}

	Msg.dwData1 = dwQuestIdx;
	Msg.dwData2 = dwRet;
	pPlayer->SendMsg( &Msg, sizeof(Msg) );
}

void QuestEventEnd( DWORD dwUserIdx, DWORD dwCharacterIdx, char* pTime )
{
	sprintf( txt, "EXEC dbo.MP_QUEST_EventEnd %d, %d, \'%s\'", dwUserIdx, dwCharacterIdx, pTime );
	g_DB.Query( eQueryType_FreeQuery, eQuestEventCheck, 0, txt );
}



// event 060627 - ¿î¿µÆÀ ÀÌº¥Æ®
void GMEvent01( DWORD dwUserIdx, DWORD dwCharacterIdx, DWORD dwLevel, int nServerSetNum )
{
	sprintf( txt, "EXEC dbo.MP_GMEvent01 %d, %d, %d, %d", dwUserIdx, dwCharacterIdx, dwLevel, nServerSetNum );
	g_DB.Query( eQueryType_FreeQuery, eGMEvent01, 0, txt );
}

// magi82 //////////////////////////////////////////////////////////////////////////
void UCharacterExpFlag(DWORD CharacterIDX, BYTE ExpFlag)
{
	sprintf(txt, "EXEC %s %d, %d", STORED_CHARACTER_EXPFLAG, CharacterIDX, ExpFlag);
	g_DB.Query(eQueryType_FreeQuery, eCharacterExpFlag, CharacterIDX, txt);
}
////////////////////////////////////////////////////////////////////////////////////

//magi82 - ¹®ÇÏ»ý °¡ÀÔÆí¸®½Ã½ºÅÛ ///////////////////////////////////////////////////
void GuildTraineeInfo(DWORD StartDBIdx)
{
	sprintf(txt, "EXEC %s %u", STORED_GUILD_TRAINEE_INFO, StartDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildTraineeInfo, 0, txt);
}

void RGuildTraineeInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	GUILD_TRAINEE_LIST* temp;
	for( DWORD i = 0; i < pMessage->dwResult; i++ )
	{
		temp = new GUILD_TRAINEE_LIST;
		ZeroMemory(temp, sizeof(GUILD_TRAINEE_LIST));

		temp->dwGuildIdx = atoi( (char*)pData[i].Data[eGTII_GuildIdx] );
		temp->dwUserIdx = atoi( (char*)pData[i].Data[eGTII_UserIdx] );
		SafeStrCpy( temp->strName, (char*)pData[i].Data[eGTII_UserName], MAX_NAME_LENGTH+1 );
		temp->dwTime = atoi( (char*)pData[i].Data[eGTII_JoinTime] );

		GUILDMGR->AddGuildTraineeInfo( temp );		
	}

	ASSERT(pMessage->dwResult <= MAX_QUERY_RESULT);
	if(pMessage->dwResult >= MAX_QUERY_RESULT)
	{
		DWORD dwTemp = atoi((char*)pData[99].Data[eGTII_DBIdx]);
		GuildTraineeInfo( dwTemp );
	}
}

void GuildTraineeInsert(DWORD GuildIdx, DWORD UserIdx, char UserName[], DWORD JoinTime)
{
	sprintf(txt, "EXEC %s %u, %u, '%s', %u", STORED_GUILD_TRAINEE_INSERT, GuildIdx, UserIdx, UserName, JoinTime);
	g_DB.Query(eQueryType_FreeQuery, eGuildTraineeInsert, 0, txt);
}

void GuildTraineeDelete(DWORD UserIdx) 
{
	sprintf(txt, "EXEC %s %u", STORED_GUILD_TRAINEE_DELETE, UserIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildTraineeDelete, 0, txt);
}

void GuildTraineeGuildInfo(DWORD StartDBIdx)
{
	sprintf(txt, "EXEC %s %u", STORED_GUILD_TRAINEE_GUILDINFO, StartDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildTraineeGuildInfo, 0, txt);
}

void RGuildTraineeGuildInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	GUILD_TRAINEE_GUILDLIST* temp;
	for( DWORD i = 0; i < pMessage->dwResult; i++ )
	{
		temp = new GUILD_TRAINEE_GUILDLIST;
		ZeroMemory(temp, sizeof(GUILD_TRAINEE_GUILDLIST));

		temp->dwGuildIdx = atoi( (char*)pData[i].Data[eGTGII_GuildIdx] );
		temp->dwRank = atoi( (char*)pData[i].Data[eGTGII_GuildRank] ); 

		GUILDMGR->AddGuildTraineeGuildInfo( temp );		
	}

	ASSERT(pMessage->dwResult <= MAX_QUERY_RESULT);
	if(pMessage->dwResult >= MAX_QUERY_RESULT)
	{
		DWORD dwTemp = atoi((char*)pData[99].Data[eGTGII_DBIdx]);
		GuildTraineeGuildInfo( dwTemp );
	}
}

void GuildTraineeGuildInsert(DWORD GuildIdx)
{
	g_DB.FreeMiddleQuery( RGuildTraineeGuildInsert, 0, 
		"EXEC %s %u", STORED_GUILD_TRAINEE_GUILDINSERT, GuildIdx );
}

void RGuildTraineeGuildInsert(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage)
{
	GUILD_INTRO_INFO* temp;
	temp = new GUILD_INTRO_INFO;
	ZeroMemory(temp, sizeof(GUILD_INTRO_INFO));

	temp->dwGuildIdx = atoi( (char*)pData[0].Data[eGTGIII_DBIdx] );
	SafeStrCpy( temp->strIntro, (char*)pData[0].Data[eGTGIII_Intro], MAX_GUILD_INTRO+1 );

	GUILDMGR->AddGuildTraineeGuildIntroInfo( temp );

	// Brodcasting to Mapserver
	MSG_MUNHAINFOUPDATE msg;
	SetProtocol( &msg, MP_GUILD, MP_GUILD_MUNPA_REGIST_INTRO_BROD );
	msg.Index = temp->dwGuildIdx;
	SafeStrCpy( msg.Intro, temp->strIntro, MAX_GUILD_INTRO+1 );
	PACKEDDATA_OBJ->SendToBroadCastMapServer( &msg, sizeof(msg) );
}

void GuildTraineeGuildDelete(DWORD GuildIdx)
{
	sprintf(txt, "EXEC %s %u", STORED_GUILD_TRAINEE_GUILDDELETE, GuildIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildTraineeGuildDelete, 0, txt);
}

void GuildTraineeDeletebyGuild(DWORD GuildIdx)
{
	sprintf(txt, "EXEC %s %u", STORED_GUILD_TRAINEE_DELETEBYGUILD, GuildIdx);
	g_DB.Query(eQueryType_FreeQuery, eGuildTraineeDeletebyGuild, 0, txt);
}

void GuildAddStudent(DWORD GuildIdx, DWORD MemberIDX, char* GuildName, BYTE Rank)
{
	sprintf(txt, "EXEC %s %u, %u, \'%s\', %d", STORED_GUILD_ADD_STUDENT, GuildIdx, MemberIDX, GuildName, Rank);
	g_DB.Query(eQueryType_FreeQuery, eGuildAddStudent, 0, txt);
}

void RGuildAddStudent(LPQUERY pData, LPDBMESSAGE pMessage)
{
	int targetID = atoi((char*)pData[0].Data[eGASI_CharIdx]);
	int targetLev = atoi((char*)pData[0].Data[eGASI_Level]);
	int guildID = atoi((char*)pData[0].Data[eGASI_GuildIdx]);
	char strName[MAX_NAME_LENGTH+1] = { 0, };
	SafeStrCpy( strName, (char*)pData[0].Data[eGASI_Name], MAX_NAME_LENGTH+1 );

	CGuild* pGuild = GUILDMGR->GetGuild(guildID);
	if( !pGuild )
		return;

	GUILDMEMBERINFO member;
	member.InitInfo(targetID, strName, targetLev, GUILD_STUDENT, TRUE);
	pGuild->AddMember(&member);
	pGuild->SetLogInfo(targetID, FALSE);

	//notify
	SEND_GUILD_MEMBER_INFO notify;
	notify.Category = MP_GUILD;
	notify.Protocol = MP_GUILD_ADDMEMBER_NOTIFY_TO_MAP;
	notify.GuildIdx = guildID;
	SafeStrCpy(notify.GuildName, pGuild->GetGuildName(), MAX_GUILD_NAME+1);
	notify.MemberInfo.MemberIdx = targetID;
	notify.MemberInfo.Memberlvl = targetLev;
	SafeStrCpy(notify.MemberInfo.MemberName, strName, MAX_NAME_LENGTH+1);
	notify.MemberInfo.Rank = GUILD_STUDENT;
	g_Network.Send2AgentServer((char*)&notify, sizeof(notify));
}

void GuildTraineeGuildIntroInfo( DWORD StartDBIdx )
{	
	g_DB.FreeMiddleQuery( RGuildTraineeGuildIntroInfo, 0, 
		"EXEC %s %u", STORED_GUILD_TRAINEE_GUILDINTROINFO, StartDBIdx );
}

void RGuildTraineeGuildIntroInfo(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage)
{
	GUILD_INTRO_INFO* temp;
	for( DWORD i = 0; i < pMessage->dwResult; i++ )
	{
		temp = new GUILD_INTRO_INFO;
		ZeroMemory(temp, sizeof(GUILD_INTRO_INFO));

		temp->dwGuildIdx = atoi( (char*)pData[i].Data[eGTGIII_DBIdx] );
		SafeStrCpy( temp->strIntro, (char*)pData[i].Data[eGTGIII_Intro], MAX_GUILD_INTRO+1 );

		GUILDMGR->AddGuildTraineeGuildIntroInfo( temp );		
	}

	ASSERT(pMessage->dwResult <= MAX_QUERY_RESULT);
	if(pMessage->dwResult >= MAX_QUERY_RESULT)
	{
		DWORD dwTemp = atoi((char*)pData[99].Data[eGTGIII_DBIdx]);
		GuildTraineeGuildIntroInfo( dwTemp );
	}
}

void GuildTraineeGuildIntroUpdate(DWORD GuildIdx, char* GuildIntro)
{
	g_DB.FreeMiddleQuery( NULL, 0, 
		"EXEC %s %u, \'%s\'", STORED_GUILD_TRAINEE_GUILDINTROUPDATE, GuildIdx, GuildIntro );
}
////////////////////////////////////////////////////////////////////////////////////

void GuildStudentLvUpCtUpdate( DWORD GuildIdx, DWORD AddPoint, DWORD ToPoint )
{
	sprintf(txt, "EXEC dbo.MP_GUILD_Update_LvCounter %d, %d, %d", GuildIdx, AddPoint, ToPoint);
	g_DB.Query(eQueryType_FreeQuery, eGuildStudentLvUpCount, GuildIdx, txt);
}

void RGuildStudentLvUpCtUpdate( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD GuildIdx = pMessage->dwID;
	DWORD count = pMessage->dwResult;

	if(!count)	return;

	DWORD GuildStudentLvUpCount = atoi((char*)pData->Data[0]);
	BOOL bMasterChecking = atoi((char*)pData->Data[1]);

	GUILDMGR->SetGuildStudentLvUpCount(GuildIdx, GuildStudentLvUpCount, bMasterChecking);
}

void GuildStudentLvUpCtInit( DWORD GuildIdx, DWORD MasterIDX )
{
	sprintf(txt, "EXEC dbo.MP_GUILD_Init_LvCounter %d, %d", GuildIdx, MasterIDX);
	g_DB.Query(eQueryType_FreeQuery, eGuildStudentLvUpInfoInit, GuildIdx, txt);

}

void RGuildStudentLvUpCtInit( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD GuildIdx = pMessage->dwID;
	DWORD count = pMessage->dwResult;

	if(!count)	return;

	BOOL bInited = atoi((char*)pData->Data[0]);
	//¹®ÆÄ¿¡°Ô ÃàÇÏ±Ý Áö±Þ
	if(bInited)
		GUILDMGR->GiveGuildCerebrationMoney(GuildIdx);
	else
		ASSERT(0);
}

// magi82 - Titan(070209)
void TitanWearItemInfo( DWORD CharacterIdx, POSTYPE FromPos, POSTYPE ToPos )
{
	sprintf(txt, "EXEC %s %d, %d, %d", STORED_TITAN_WEARITEMINFO, CharacterIdx, FromPos, ToPos);
	g_DB.Query(eQueryType_FreeQuery, eTitanWearItemInfo, CharacterIdx, txt);
}

void RTitanWearItemInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer )	return;

	ITEMBASE ItemBase[SLOT_TITANWEAR_NUM];
	memset( ItemBase, 0, sizeof(ITEMBASE)*SLOT_TITANWEAR_NUM );

	ITEMBASE ShopItemBase[SLOT_TITANSHOPITEM_NUM];
	memset( ShopItemBase, 0, sizeof(ITEMBASE)*SLOT_TITANSHOPITEM_NUM );

	int ret = atoi((char*)pData[0].Data[0]);
	if( ret == 0 )
	{
		pPlayer->InitTitanWearItemInfo( ItemBase );
		pPlayer->InitTitanShopItemInfo( ShopItemBase );
		pPlayer->SetInitState(PLAYERINITSTATE_ITEM_INFO,MP_USERCONN_GAMEIN_SYN);
	}
	else
	{
		if( pMessage->dwResult > SLOT_TITANWEAR_NUM + SLOT_TITANSHOPITEM_NUM )
		{
			char Buf[64];
			sprintf(Buf, "TitanWearItem/TitanShopItem OverNum CharacterID:%d", pPlayer->GetID());
			ASSERTMSG(0, Buf);
		}

		for(DWORD i = 0; i < pMessage->dwResult; i++)
		{
			WORD wIconIdx = atoi((char*)pData[i].Data[eCI_IDX]);
			ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( wIconIdx );

			if(pInfo->ItemKind == eSHOP_ITEM_TITAN_EQUIP)
			{
				POSTYPE ItemPos = atoi((char*)pData[i].Data[eCI_Position]);
				ITEMBASE* pItemBase = NULL;

				if(ItemPos >= TP_TITANSHOPITEM_START && ItemPos < TP_TITANSHOPITEM_END)
				{
					ItemPos -= TP_TITANSHOPITEM_START;

					pItemBase = &ShopItemBase[ItemPos];

					pItemBase->dwDBIdx = atoi((char*)pData[i].Data[eCI_DBIDX]);
					pItemBase->wIconIdx = wIconIdx;
					pItemBase->Position = atoi((char*)pData[i].Data[eCI_Position]);
					pItemBase->Durability = atoi((char*)pData[i].Data[eCI_Durability]);
					pItemBase->QuickPosition = atoi((char*)pData[i].Data[eCI_QPosition]);
					pItemBase->ItemParam = atoi((char*)pData[i].Data[eCI_Param]);

					pPlayer->GetTitanManager()->SetTitanShopitemOption(pItemBase->wIconIdx, TRUE);	// magi82(26)
				}
			}
			else if(pInfo->ItemKind & eTITAN_EQUIPITEM)
			{
				POSTYPE ItemPos = atoi((char*)pData[i].Data[eCI_Position]);
				ITEMBASE* pItemBase = NULL;

				if(ItemPos >= TP_TITANWEAR_START && ItemPos < TP_TITANWEAR_END)
				{
					ItemPos -= TP_TITANWEAR_START;

					pItemBase = &ItemBase[ItemPos];

					pItemBase->dwDBIdx = atoi((char*)pData[i].Data[eCI_DBIDX]);
					pItemBase->wIconIdx = wIconIdx;
					pItemBase->Position = atoi((char*)pData[i].Data[eCI_Position]);
					pItemBase->Durability = atoi((char*)pData[i].Data[eCI_Durability]);
					pItemBase->QuickPosition = atoi((char*)pData[i].Data[eCI_QPosition]);
					pItemBase->ItemParam = atoi((char*)pData[i].Data[eCI_Param]);

					// magi82(33) Å¸ÀÌÅº ÀåÂøÀÎº¥¿¡ ÀÖ´Â Å¸ÀÌÅº ÀåÂø¾ÆÀÌÅÛÀÇ ¿¹¿ÜÃ³¸®
					ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pItemBase->wIconIdx);
					if( pInfo && pInfo->ItemKind & eTITAN_EQUIPITEM )
					{
						TITAN_ENDURANCE_ITEMINFO* pEndurance = pPlayer->GetTitanManager()->GetTitanItemEnduranceInfo(pItemBase->dwDBIdx);
						if( !pEndurance )
						{
							pPlayer->GetTitanManager()->MakeNewEnduranceInfo(pPlayer, pItemBase, eExceptionInven);				
						}
					}

					pPlayer->SetTitanWearInfo(pItemBase->Position, pItemBase, TRUE);
				}
			}
		}

		pPlayer->InitTitanWearItemInfo( ItemBase );
		pPlayer->InitTitanShopItemInfo( ShopItemBase );
		pPlayer->SetInitState(PLAYERINITSTATE_ITEM_INFO,MP_USERCONN_GAMEIN_SYN);
	}
}
//////////////////////////////////////////////////////////////////////////


void TestGameQuery( DWORD CharacterIdx )
{
	sprintf(txt, "EXEC %s %d", "dbo.MP_TEST_GAMEQUERY", CharacterIdx);
	g_DB.Query(eQueryType_FreeQuery, eTestGameQuery, CharacterIdx, txt);

}

void RTestGameQuery( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer )	return;

	g_pServerSystem->m_ProcessInfo.ProcessInfo.GameQueryReturntime = gCurTime - g_pServerSystem->m_ProcessInfo.StarTime;
	if( g_pServerSystem->m_ProcessInfo.ProcessInfo.GameQueryReturntime == 0 )
		g_pServerSystem->m_ProcessInfo.ProcessInfo.GameQueryReturntime = 1;

	if( g_pServerSystem->m_ProcessInfo.Count > 4 && g_pServerSystem->m_ProcessInfo.ProcessInfo.LogQueryReturntime )
	{
		g_pServerSystem->m_bCheckInfo = FALSE;
		pPlayer->SendMsg( &g_pServerSystem->m_ProcessInfo, sizeof(g_pServerSystem->m_ProcessInfo) );
	}
}

void TestLogQuery( DWORD CharacterIdx )
{
	sprintf(txt, "EXEC %s %d", "dbo.UP_TEST_LOGQUERY", CharacterIdx);
	g_DB.LogQuery(eQueryType_FreeQuery, eTestLogQuery, CharacterIdx, txt);

}

void RTestLogQuery( LPQUERY pData, LPDBMESSAGE pMessage )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pMessage->dwID);
	if( !pPlayer )	return;

	g_pServerSystem->m_ProcessInfo.ProcessInfo.LogQueryReturntime = gCurTime - g_pServerSystem->m_ProcessInfo.StarTime;
	if( g_pServerSystem->m_ProcessInfo.ProcessInfo.LogQueryReturntime == 0 )
		g_pServerSystem->m_ProcessInfo.ProcessInfo.LogQueryReturntime = 1;

	if( g_pServerSystem->m_ProcessInfo.Count > 4 && g_pServerSystem->m_ProcessInfo.ProcessInfo.GameQueryReturntime )
	{
		g_pServerSystem->m_bCheckInfo = FALSE;
		pPlayer->SendMsg( &g_pServerSystem->m_ProcessInfo, sizeof(g_pServerSystem->m_ProcessInfo) );
	}
}

void PyogukTitanEnduranceInfo(DWORD CharacterIdx, DWORD UserIdx, DWORD StartDBIdx)
{
	sprintf(txt, "EXEC %s %d, %d", STORED_PYOGUK_TITAN_ENDURANCE_INFO, UserIdx, StartDBIdx);
	g_DB.Query(eQueryType_FreeQuery, ePyogukTitanEnduranceOptionQuery, CharacterIdx, txt);
}

void RPyogukTitanEnduranceInfo( LPQUERY pData, LPDBMESSAGE pMessage )
{
	// C¢®I¢®¨ú¨Ïo A¢®E¢®¨¡i
	CPlayer* pPlayer = NULL;
	pPlayer = (CPlayer *)g_pUserTable->FindUser(pMessage->dwID);
	if(pPlayer == NULL)
		return;

	TITAN_ENDURANCE_ITEMINFO EnduranceInfo;
	for( DWORD  i = 0 ; i < pMessage->dwResult ; ++i )
	{
		EnduranceInfo.ItemDBIdx				= (DWORD)atoi((char*)pData[i].Data[0]);
		EnduranceInfo.CharacterID			= (DWORD)atoi((char*)pData[i].Data[1]);
		EnduranceInfo.ItemIdx				= (DWORD)atoi((char*)pData[i].Data[2]);
		EnduranceInfo.Endurance				= (DWORD)atoi((char*)pData[i].Data[3]);
		EnduranceInfo.UserIdx				= (DWORD)atoi((char*)pData[i].Data[4]);

		pPlayer->GetTitanManager()->AddTitanEquipItemEnduranceInfo(&EnduranceInfo);
	}

	if(pMessage->dwResult >= MAX_TITANENDURANCELOAD_ITEM) 
		PyogukTitanEnduranceInfo(pMessage->dwID, pPlayer->GetUserID(), EnduranceInfo.ItemDBIdx);
	else
		CharacterPyogukItemInfo(pMessage->dwID, 0);
}

//2007. 12. 10. CBH - ½ºÅ² ÀúÀå
void CharacterSkinInfoUpdate(CPlayer* pPlayer)
{
	WORD* pSkinItem = pPlayer->GetShopItemStats()->wSkinItem;

	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, %d", STORED_CHARACTER_SKININFO, pPlayer->GetID(), 
		pSkinItem[0], pSkinItem[1], pSkinItem[2], pSkinItem[3], pSkinItem[4]);
	g_DB.Query(eQueryType_FreeQuery, eCharacterSkinInfoUpdate, 0, txt);
}

//2008. 1. 23. CBH - ½ºÅ² ·Îµå
void CharacterSkinInfo(DWORD dwCharacterIndex)
{
	sprintf(txt, "EXEC MP_CHARACTER_SkinInfo %d", dwCharacterIndex);
	g_DB.Query(eQueryType_FreeQuery, eCharacterSkinInfo, 0, txt);
}

void RCharacterSkinInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	CPlayer* pPlayer = NULL;
	pPlayer = (CPlayer *)g_pUserTable->FindUser((DWORD)atoi((char*)pData->Data[0]));
	if(pPlayer == NULL)
		return;

	pPlayer->GetShopItemStats()->wSkinItem[0] = (WORD)atoi((char*)pData->Data[1]);
	pPlayer->GetShopItemStats()->wSkinItem[1] = (WORD)atoi((char*)pData->Data[2]);
	pPlayer->GetShopItemStats()->wSkinItem[2] = (WORD)atoi((char*)pData->Data[3]);
	pPlayer->GetShopItemStats()->wSkinItem[3] = (WORD)atoi((char*)pData->Data[4]);
	pPlayer->GetShopItemStats()->wSkinItem[4] = (WORD)atoi((char*)pData->Data[5]);
}

// magi82(41) - ¼¥¾ÆÀÌÅÛ Ãß°¡(½ºÅÝ ÃÊ±âÈ­ ÁÖ¹®¼­)
void CharacterUpdateResetStatusPoint(DWORD dwCharacterIDX, DWORD dwStatusPoint)
{
	sprintf(txt, "EXEC %s %d, %d", STORED_CHARACTER_UPDATE_RESETSTATUSPOINT, dwCharacterIDX, dwStatusPoint);
	g_DB.Query(eQueryType_FreeQuery, eCharacterUpdateResetStatusPoint, dwCharacterIDX, txt);
}

void ItemLimitInfoLoadAll( DWORD dwStartDBIdx )
{
	sprintf(txt, "EXEC dbo.MP_ITEM_LIMITINFO_LOADALL %d", dwStartDBIdx);
	g_DB.Query(eQueryType_FreeQuery, eItemLimitInfoLoadAll, dwStartDBIdx, txt);
}

void RItemLimitInfoLoadAll( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( 0 == pMessage->dwResult )
		return;

	DWORD dwDBIdx = 0;
	DWORD dwItemIdx = 0;
	int nItemLimitCount = 0;
	int nItemCurrentCount = 0;
	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		dwDBIdx = (DWORD)atoi((char*)pData[i].Data[0]);
		dwItemIdx = (DWORD)atoi((char*)pData[i].Data[1]);
		nItemLimitCount = atoi((char*)pData[i].Data[2]);
		nItemCurrentCount = atoi((char*)pData[i].Data[3]);

        ITEMLIMITMGR->SetItemLimitInfoFromDB( dwItemIdx, nItemLimitCount, nItemCurrentCount );
	}

	if(pMessage->dwResult == MAX_QUERY_RESULT)
	{
		ItemLimitInfoLoadAll( dwDBIdx );
	}
}

void ItemLimitInfoUpdate( DWORD dwItemIdx, int nItemLimitCount, int nItemCurrentCount )
{
	sprintf(txt, "EXEC dbo.MP_ITEM_LIMITINFO_UPDATE %d, %d, %d", dwItemIdx, nItemLimitCount, nItemCurrentCount );
	g_DB.Query(eQueryType_FreeQuery, eItemLimitInfoUpdate, dwItemIdx, txt);
}

void RItemLimitInfoUpdate( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( 0 == pMessage->dwResult )
		return;

	DWORD dwItemIdx = (DWORD)atoi((char*)pData[0].Data[0]);
	int nItemLimitCount = atoi((char*)pData[0].Data[1]);
	int nItemCurrentCount = atoi((char*)pData[0].Data[2]);
    
    ITEMLIMITMGR->SetItemLimitInfoFromDB( dwItemIdx, nItemLimitCount, nItemCurrentCount );

	MSG_DWORD2 msg;
	msg.Category = MP_ITEMLIMIT;
	msg.Protocol = MP_ITEMLIMIT_ADDCOUNT_TO_MAP;
	msg.dwData1 = dwItemIdx;
	msg.dwData2 = (DWORD)nItemCurrentCount;

	g_Network.Send2AgentServer( (char*)&msg, sizeof(msg) );
}

// autonnote
void AutoNoteListLoad( DWORD dwCharacterIdx )
{
	sprintf(txt, "EXEC dbo.MP_AUTONOTELIST_LOAD %d", dwCharacterIdx );
	g_DB.Query(eQueryType_FreeQuery, eAutoNoteListLoad, dwCharacterIdx, txt);
}

void RAutoNoteListLoad( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( 0 == pMessage->dwResult )
		return;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pMessage->dwID );
	if( !pPlayer )	return;

	MSG_AUTOLIST_ALL msg;
	msg.Category = MP_AUTONOTE;
	msg.Protocol = MP_AUTONOTE_LIST_ALL;
	msg.nCount = (int)pMessage->dwResult;

	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		SafeStrCpy( msg.row[i].Name, (char*)pData[i].Data[0], MAX_NAME_LENGTH+1 );
		SafeStrCpy( msg.row[i].Date, (char*)pData[i].Data[1], 20 );
	}

	pPlayer->SendMsg( &msg, msg.GetMsgLength() );
}

void AutoNoteListAdd( DWORD dwCharacterIdx, DWORD dwAutoUserIdx, DWORD dwAutoCharacterIdx, char* pStrAutoName )
{
	sprintf(txt, "EXEC dbo.MP_AUTONOTELIST_ADD %d, %d, %d, \'%s\'", dwCharacterIdx, dwAutoUserIdx, dwAutoCharacterIdx, pStrAutoName );
	g_DB.Query(eQueryType_FreeQuery, eAutoNoteListAdd, dwCharacterIdx, txt);
}

void RAutoNoteListAdd( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( 0 == pMessage->dwResult )
		return;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pMessage->dwID );
	if( !pPlayer )	return;

	MSG_AUTOLIST_ADD msg;
	msg.Category = MP_AUTONOTE;
	msg.Protocol = MP_AUTONOTE_LIST_ADD;
	SafeStrCpy( msg.Name, (char*)pData[0].Data[0], MAX_NAME_LENGTH+1 );
	SafeStrCpy( msg.Date, (char*)pData[0].Data[1], 20 );

	pPlayer->SendMsg( &msg, sizeof(msg) );
}

/// ¸Ê µå¶ø ¾ÆÀÌÅÛ Ãß°¡ ÀÛ¾÷ by Stiner(2008/05/28)-MapDropItem
void MapItemDropLog( WORD wUserIDX, WORD wCharacterIDX, WORD wItemIDX, WORD wMap, WORD wChannel, WORD wDropCount, WORD wMaxDropCount )
{
	sprintf( txt, "EXEC dbo.UP_MapItemDropLog %d, %d, %d, %d, %d, %d, %d",
			wUserIDX, wCharacterIDX, wItemIDX, wMap, wChannel, wDropCount, wMaxDropCount );
	g_DB.LogQuery( eQueryType_FreeQuery, 0, 0, txt );
}

void MapItemDropListSelect( WORD wMapNum )
{
	sprintf( txt, "EXEC dbo.MP_MAPITEM_DropList_Select %d", wMapNum );
	g_DB.Query( eQueryType_FreeQuery, eMapItemDropListSelect, NULL, txt );
}

void MapItemDropListUpdate( DWORD dwCharacterIdx, WORD wMapNum, WORD wChannel, WORD wItemIDX, WORD wDropCount, WORD wMaxDropCount )
{
	sprintf( txt, "EXEC dbo.MP_MAPITEM_DropList_Update %d, %d, %d, %d, %d",
		wMapNum, wChannel, wItemIDX, wDropCount, wMaxDropCount );
	g_DB.Query( eQueryType_FreeQuery, eMapItemDropListUpdate, dwCharacterIdx, txt );
}

void MapItemDropListInit( WORD wMapNum )
{
	sprintf( txt, "EXEC dbo.MP_MAPITEM_DropList_Init %d", wMapNum );
	g_DB.Query( eQueryType_FreeQuery, eMapItemDropListInit, NULL, txt );
}

void RMapItemDropListSelect( LPQUERY pData, LPDBMESSAGE pMessage )
{
	// DB°á°ú°ªÀ¸·Î µ¥ÀÌÅÍ µ¿±âÈ­
	if( 0 == pMessage->dwResult )
		return;

	for( int row = 0 ; row < pMessage->dwResult ; row++ )
	{
		WORD wMapNum		= atoi((char*)pData[row].Data[eMdi_MapNum]);
		WORD wChannel		= atoi((char*)pData[row].Data[eMdi_Channel]);
		WORD wItemIDX		= atoi((char*)pData[row].Data[eMdi_ItemIDX]);
		WORD wDropCount		= atoi((char*)pData[row].Data[eMdi_DropCount]);
		WORD wMaxDropCount	= atoi((char*)pData[row].Data[eMdi_MaxDropCount]);

		MAPITEMDROP_OBJ->SetItemDropInfoByDB( wMapNum, wChannel, wItemIDX, wDropCount, wMaxDropCount );
	}
}

void RMapItemDropListUpdate( LPQUERY pData, LPDBMESSAGE pMessage )
{
	// DB°á°ú°ªÀ¸·Î µ¥ÀÌÅÍ µ¿±âÈ­
	if( 0 == pMessage->dwResult )
		return;

	for( int row = 0 ; row < pMessage->dwResult ; row++ )
	{
		WORD wMapNum		= atoi((char*)pData[row].Data[eMdi_MapNum]);
		WORD wChannel		= atoi((char*)pData[row].Data[eMdi_Channel]);
		WORD wItemIDX		= atoi((char*)pData[row].Data[eMdi_ItemIDX]);
		WORD wDropCount		= atoi((char*)pData[row].Data[eMdi_DropCount]);
		WORD wMaxDropCount	= atoi((char*)pData[row].Data[eMdi_MaxDropCount]);

		MAPITEMDROP_OBJ->SetItemDropInfoByDB( wMapNum, wChannel, wItemIDX, wDropCount, wMaxDropCount );
	}
}


// ¿ä»õÀü
void FortWarInfoLoad()
{
	sprintf(txt, "EXEC dbo.MP_FORTWARINFO_LOAD" );
	g_DB.Query(eQueryType_FreeQuery, eFortWarInfoLoad, 0, txt);
}

void RFortWarInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage )
{
//	if( 0 == pMessage->dwResult )
//		return;

	sFortWarInfo info;

	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		info.nRelationFortWarIDX = atoi((char*)pData[i].Data[0]);
		info.dwCharacterIdx = (DWORD)atoi((char*)pData[i].Data[1]);
		SafeStrCpy( info.CharacterName, (char*)pData[i].Data[2], MAX_NAME_LENGTH+1 );
		info.dwGuildIdx = (DWORD)atoi((char*)pData[i].Data[3]);
		info.dwProfitMoney = (DWORD)atoi((char*)pData[i].Data[4]);

		FORTWARMGR->SetTotalFortWarInfoFromDB( info );
	}
	FORTWARMGR->CompleteLoadDataFromDB();
}

void FortWarInfoUpdate( int nWarIDX, DWORD dwCharacterIdx, char* pCharacterName, DWORD dwGuildIdx )
{
	sprintf(txt, "EXEC dbo.MP_FORTWARINFO_UPDATE %d, %d, \'%s\', %d", nWarIDX, dwCharacterIdx, pCharacterName, dwGuildIdx );
	g_DB.Query(eQueryType_FreeQuery, eFortWarInfoUpdate, dwCharacterIdx, txt);
}

void FortWarProfitMoneyUpdate( int nWarIDX, DWORD dwMoney, int nType )
{
	sprintf( txt, "EXEC dbo.MP_FORTWARINFO_PROFITMONEY_UPDATE %d, %u, %d", nWarIDX, dwMoney, nType );
	g_DB.Query( eQueryType_FreeQuery, eFortWarProfitMoneyUpdate, 0, txt );
}

void RFortWarProfitMoneyUpdate( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( !pMessage->dwResult ) return;

	FORTWARMGR->SetFortWarProfitMoneyFromDB( atoi((char*)pData[0].Data[0]), (DWORD)atoi((char*)pData[0].Data[1]) );
}

void FortWarItemLoad( int nWarIDX )
{
	sprintf(txt, "EXEC dbo.MP_FORTWAR_ITEM_LOAD %d", nWarIDX );
	g_DB.Query(eQueryType_FreeQuery, eFortWarItemLoad, nWarIDX, txt);
}

void RFortWarItemLoad( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( pMessage->dwResult == 0 )
		return;

	int nWarIDX = atoi((char*)pData[0].Data[0]);

	for( int i = 0; i < pMessage->dwResult; ++i )
	{
		ITEMBASE itembase;
		memset( &itembase, 0, sizeof(itembase) );
		itembase.dwDBIdx = (DWORD)atoi((char*)pData[i].Data[1]);
		itembase.wIconIdx = (WORD)atoi((char*)pData[i].Data[2]);
		itembase.Position = (WORD)atoi((char*)pData[i].Data[3]);
		itembase.Durability = (DWORD)atoi((char*)pData[i].Data[4]);
		itembase.RareIdx = (DWORD)atoi((char*)pData[i].Data[5]);

		FORTWARMGR->SetTotalFortWarItemInfoFromDB( nWarIDX, &itembase );
	}
}

void FortWarItemInsertToDB( int nWarIDX, DWORD dwItemIdx, DURTYPE Durability, POSTYPE wPos, DWORD dwParam )
{
	sprintf( txt, "EXEC dbo.MP_FORTWAR_ITEM_INSERT %d, %d, %d, %d, %d", nWarIDX, dwItemIdx, Durability, wPos, dwParam );
	g_DB.Query(eQueryType_FreeQuery, eFortWarItemInsertToDB, nWarIDX, txt);
}

void RFortWarItemInsertToDB( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( pMessage->dwResult == 0 )
		return;

	int nWarIDX = (DWORD)atoi((char*)pData->Data[0]);
	ITEMBASE itembase;
	memset( &itembase, 0, sizeof(itembase) );
	itembase.dwDBIdx = (DWORD)atoi((char*)pData->Data[1]);
	itembase.wIconIdx = (WORD)atoi((char*)pData->Data[2]);
	itembase.Position = (WORD)atoi((char*)pData->Data[3]);
	itembase.Durability = (DWORD)atoi((char*)pData->Data[4]);
	itembase.ItemParam = (DWORD)atoi((char*)pData->Data[5]);

	FORTWARMGR->AddProfitItemFromDBResult( nWarIDX, &itembase );
}

void FortWarItemMoveToDB( DWORD dwCharacterIdx, DWORD dwItemDBIdx, WORD wPos, int nWarIDX )
{
	sprintf( txt, "EXEC dbo.MP_FORTWAR_ITEM_MOVE %u, %u, %d, %d", dwCharacterIdx, dwItemDBIdx, wPos, nWarIDX );
	g_DB.Query(eQueryType_FreeQuery, eFortWarItemMoveToDB, dwCharacterIdx, txt);
}
