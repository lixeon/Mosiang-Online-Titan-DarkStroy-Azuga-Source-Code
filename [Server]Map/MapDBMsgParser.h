
#ifndef __MAPDBMSGPARSER_H__
#define __MAPDBMSGPARSER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include ".\module\db.h"
//#include "CommonDBMsgParser.h"
//#include "Player.h"

class CPlayer;
class CParty;
class CAbilityGroup;

//////////////////////////////////////////////////////////////////////////
// DB¿¡ b¼OCO ID/PW ¼±¾ð


//////////////////////////////////////////////////////////////////////////
// DB·I ºIAI ¸Þ½AAo¸| ¹Þ;¶§ ±¸ºÐCO ID dAC
enum   DBMESSAGEIDFUNC 
{
	eNull,
	eCharacterInfoQuery,		/// CI³ªAC A³¸?dº¸¸ðμI¸| ¾o¾i¿A´U.
	eCharacterMugongQuery,		/// ¹≪°ødº¸¸| ¾o¾i¿A´U
	eCharacterItemSlotQuery,	
	eCharacterItemQuery,		/// ¾ÆAIAU dº¸¸| ¾o¾i¿A´U
	eCharacterItemOptionQuery, 
	eCharacterItemOptionDeleteQuery,
	eCharacterItemRareOptionQuery,
	eCharacterItemRareOptionDeleteQuery,
	ePyogukItemOptionQuery,
	ePyogukItemRareOptionQuery,
	eShopItemInfoQuery,			// ItemMall Item쿼리
	//ePetInfoQuery,				// CharacterPetInfo

	eSSItemInsert,				/// ³ea≫o¿¡¼­ ¾ÆAIAU ≫y¼º
	eItemOptionInsert,
	eItemRareInsert,		// RareItem	
	/* New Item Work */
	eItemUpdate,
	eItemCombineUpdate,
	eItemMoveUpdate,
	eItemMovePyogukUpdate,
	eItemMoveGuildUpdate,
	eItemMovePetInvenUpdate,
	eItemInsert,
	eItemDelete,
	eItemShopInvenInfo,
	eItemShopItemUseInfo,
	eItemShopItemUsing,
	eItemShopItemDelete,
	eItemShopItemUpdatetime,
	eItemShopItemUpdateParam,
	eItemShopItemUpdateUseParam,
	eItemShopItemUpdateUseInfo,
	eItemShopItemMPInfo,
	eItemShopMPUpdate,
	eItemShopMPDel,
	eItemShopGetItem,
	eItemMoveShopUpdate,
	eItemShopChangeName,
	eItemCharChange,
	eItemShopRareCreate,
	eItemShopIsUseableAvatar,
	eItemShopIsUseableEquip,
	eItemShopUpdatCharacterIdx,
	eItemShopUpdateAllInfo,

	eRegistLoginMapInfo,
	eUnRegistLoginMapInfo,

	/* New Mugong Work */
	eMugongUpdate2,
	eMugongMoveUpdate2,
	eMugongInsert,
	eMugongDelete,				/// ¹≪°ø≫ef

	eBaseEconomy,				/// ±aAØ½A¼¼C￥Ao¸®
	eAuctionRegist,				/// °æ¸Aμi·ICwI½An
	eCharacterUpdate,			/// A³¸?dº¸ ¾÷μ￥AIÆ®
	eHeroInfoUpdate,			/// HeroInfo ¾÷μ￥AIÆ®
	eTotalInfoUpdate,			/// TotalInfo ¾÷μ￥AIÆ®
	eBadFameUpdate,
	
	eAuctionCheck,
	eAuctionPageList,
	eAuctionConfirm,
	ePartyRegistMapServer,		//¸E¼­¹o A×; ¶§ ÆAÆ¼¸| ¹Þ¾Æ¿A´U. 
	ePartyInfoByUserLogin,		///Au°¡ ·I±×AI CI¸e ÆAÆ¼ dº¸¸| ¹Þ¾Æ¿A´U.
	ePartyCreate,
	ePartyBreakup,
	ePartyCharacterTbUpdate,	//A³¸?AI A×AIºiAC ÆAÆ¼ AIμ|½º¸| 08·I ¸¸μc´U.
	ePartyAddMember,			//¸a¹oAß°¡
	ePartyDelMember,
	ePartyChangeMaster,

	eSavePoint,					// SavePoint 
	eSaveMapChangePoint,		// MapChange
	
/*
	eMunpaLoad,					//¹®ÆA 1
	eMunpaItemLoad,
	eMunpaItemOption, 
	eMunpaLoadSyn,
	eMunpaSearchMunpa,
	eMunpaBreakUp,
	eMunpaBreakUpMembers,
	eMunpaBreakUpClr,
	eMunpaChangeMaster,
	eMunpaChangeMemberRank,
	eMunpaBanMember,
	eMunpaSecede,
	eMunpaAcceptMember,	
	eMunpaDenyMember, 
	eMunpaJoinMember,
	eMunpaJoinMemberCancel,
	eMunpaMembersInfo,
	eMunpaJoinMemberInfo,
	eMunpaBoardInfo,
	eMunpaCreateBoard,
	eMunpaDeleteBoard,  //20
	eMunpaBoardRankInfo,
	eMunpaBoardRankInfoUpdate,
	eMunpaBoardWriteContent,
	eMunpaBoardDeleteContent,
	eMunpaMoneyPutIn,
	*/

	
	ePyogukBuyPyoguk,
	eCharacterPyogukInfo,
	eCharacterPyogukItemInfo,
	ePyogukMoneyUpdate,

	eFriendNotifyLogout,
//	eFameMunpaUpdate,
	eFameCharacterUpdate,
	eBadFameCharacterUpdate,
	ePKCharacterUpdate,

	eWantedLoad,
	eWantedGetList,
	eWantedBuyRight,
	eWantedRegist,
	eWantedGiveupRight,
	eWantedComplete,
	eWantedDestroyed,
	eWantedOrderList,
	eWantedMyList,
	eWantedSearch,
	
	eQuestTotalInfo,	// AEº¸AU °¡AIμa
	eQuestUpdate,
	eQuestMainQuestLoad,
	eQuestSubQuestLoad,
	eQuestItemLoad,
	eQuestMainQuestInsert,
	eQuestSubQuestInsert,
	eQuestItemInsert,
	eQuestMainQuestUpdate,
	eQuestSubQuestUpdate,
	eQuestItemUpdate,
	eQuestMainQuestDelete,
	eQuestSubQuestDelete,
	eQuestSubQuestDeleteAll,
	eQuestItemDelete,
	eQuestItemDeleteAll,
	eQuestMainQuestUpdateCheckTime,

	eJournalGetList,
	eJournalInsertQuest,
	eJournalInsertWanted,
	eJournalInsertLevel,
	eJournalSaveUpdate,
	eJournalDelete,
	/////////////////////////////////////////////
	// Log °u·A
	eLogCharacter,
	eLogExp,
	eLogMoney,
	eLogMoneyWrong,
	eLogMugong,
	eLogMugongExp,
	eLogItemMoney,
	eLogItemReinforce,
	eLogItemRare,
	eLogCheat,
	eInsertLogTool,
	eLogGuild,
	eLogPet,
	eLogGuildPoint,
	eLogGMToolUse,
	/////////////////////////////////////////////
	
	eCharacterInfoByTime,
	eMugongExpByTime,
	eSpeedHackCheck,

	eGameLogItem,

	eGuildLoadGuild,
	eGuildLoadMember,
	eGuildCreate,
	eGuildBreakUp,
	eGuildDeleteMember,
	eGuildAddMember,
	eGuildSecedeMember,
	eGuildLevelUp,
	eGuildChangeMemberRank,
	eGuildItemLoad,
	eGuildItemOption,
	eGuildItemRareOption,
	eGuildMoneyUpdate,
	eGuildGiveNickName,
	
	eConfirmUserOut,
	eSaveChrInfoBeforeOut,

	eCharacterInfoQueryToEventMap,

	eLoadGuildWarRecord,
	eDeleteGuildWarRecord,
	eLoadGuildFieldWar,
	eInsertGuildFieldWar,
	eDeleteGuildFieldWar,	
	eUpdateGuildFieldWarRecord,
	
	eGuildTournament_LoadAll,
	eGuildTournament_CancelLoad,
	eGuildTournament_CancelInsert,
	eGuildTournament_Insert,
	eGuildTournament_Delete,
	eGuildTournament_UpdateGuildInfo,
	eGuildTournament_UpdateTournamentInfo,	

//	eJackpot_TotalMoney_Load,
	eJackpot_TotalMoney_Update,
	eJackpot_PrizeInfo_Update,

	eGuildUnionLoad,
	eGuildUnionCreate,
	eGuildUnionDestroy,
	eGuildUnionAddGuild,
	eGuildUnionRemoveGuild,
	eGuildUnionSecedeGuild,

	eSiegeWarInfoLoad,
	eSiegeWarInfoInsert,
	eSiegeWarInfoUpdate,
	eSiegeWarGuildInfoLoad,
	eSiegeWarGuildInsert,
	eSiegeWarGuildUpdate,
	eSiegeWarGuildDelete,

	eChangeCharacterAttr,

	eSiegeWarProfitInfoLoad,
	eSiegeWarProfitUpdateGuild,
	eSiegeWarProfitUpdateTaxRate,
	eSiegeWarProfitUpdateProfitMoney,

	eChangeGuildLocation,

	eChangeCharacterStageAbility,

	eCharacterPetInfoQuery,	//CharacterPetInfo
	ePetInsert,
	ePetUpdate,
	ePetDelete,

	ePetInvenItemOptionInfoQuery,		//PetInvenItemOptionInfo
	ePetInvenItemRareOptionInfoQuery,	//PetInvenItemRareOptionInfo
	ePetInvenItemInfoQuery,		//PetInvenItemInfo

	eGuildLoadNotice,
	eGuildUpdateNotice,

	eQuestEventCheck,
	eQuestEventEnd,

	ePetWearItemInfoQuery,		//PetWearItemInfo
	eGMEvent01,

	eGuildPointInfoLoad,
	eGuildPlustimeInfoLoad,
	eGuildAddHuntedMonsterCount,
	eGuildGetHuntedMonsterTotalCountWithInit,
	eGuildAddGuildPoint,
	eGuildUseGuildPoint,
	eGuildPlustimeEnd,
	eGuildTournamentAddLog,
	eSiegeWarAddLog,

	eGuildItemLoadInNeed,
	eGuildItemOptionInNeed,
	eGuildItemRareOptionInNeed,
	eCharacterExpFlag,	// magi82

	//magi82 - 문하생 가입편리시스템 /////////////////////////////////////////
	eGuildTraineeInfo,
	eGuildTraineeInsert,
	eGuildTraineeDelete,

	eGuildTraineeGuildInfo,
	eGuildTraineeGuildDelete,

	eGuildTraineeDeletebyGuild,

	eGuildAddStudent,

	//////////////////////////////////////////////////////////////////////////
	
	//SW070103 문파포인트개선
	eGuildStudentLvUpCount,	
	eGuildStudentLvUpInfoInit,

	eTitanWearItemInfo,	// magi82 - Titan(070209)

	//SW070127 타이탄
	eCharacterTitanInfoQuery,	//CharacterTitanInfo
	eCharacterTitanEquipItemEnduranceInfoQuery,	//TitanEquipItemEnduranceInfo
	eTitanInsert,
	eTitanUpdate,
	eTitanDelete,

	eTitanEquipInfoInsert,
	eTitanEquipInfoUpdate,
	eTitanEquipInfoDelete,


	eTestGameQuery,
	eTestLogQuery,

	ePyogukTitanEnduranceOptionQuery,	// magi82 - Titan(071015) 창고에 타이탄 장비 관련

	eQuest_EndQuest_New,
	eQuest_DeleteQuest_New,
	eQuest_EndSubQuest_New,

	eTitanEquipInfoInsertException,	// magi82(33)
	eTitanEquipInfoInsertExceptionPyoguk,	// magi82(33)

	eCharacterSkinInfoUpdate,			//2007. 12. 10. CBH - 캐릭터 스킨 정보

	eCharacterUpdateResetStatusPoint,	// magi82(41) - 샵아이템 추가(스텟 초기화 주문서)

	eCharacterSkinInfo,			//2008. 1. 23. CBH - 캐릭터 스킨 정보

	eItemLimitInfoLoadAll,	// tamoo
	eItemLimitInfoUpdate,	// tamoo

	eAutoNoteListLoad,		// tamoo
	eAutoNoteListAdd,

	// 맵 아이템 드랍 추가 by Stiner(2008/05/28)-MapItemDrop
	eMapItemDropListSelect,
	eMapItemDropListUpdate,
	eMapItemDropListInit,

	eFortWarInfoLoad,
	eFortWarInfoUpdate,
	eFortWarProfitMoneyUpdate,
	eFortWarItemLoad,
	eFortWarItemInsertToDB,
	eFortWarItemMoveToDB,	

	MaxQuery
};
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

// DB·I ³?¸± Stored Procedure ¶C´A Query ¹® dAC
#define STORED_TEST_QUERY	"UP_GAME_TEST_QUERY"
#define STORED_CHARACTER_SELECT				"dbo.MP_CHARACTER_SelectByCharacterIDX"		// ¼±AACN A³¸?dº¸ ¾o=
#define STORED_CHARACTER_KYUNGGONG			"dbo.MP_CHARACTER_KyungGong"				// °æ°ødº¸¸| ¹Þ¾Æ¿A´U
#define STORED_CHARACTER_NAEGONG			"dbo.MP_CHARACTER_NaeGong"					// ³≫°ødº¸¸| ¹Þ¾Æ¿A´U
#define STORED_CHARACTER_MUGONGINFO			"dbo.MP_CHARACTER_MugongInfo"				// ¹≪°ødº¸¸| ¹Þ¾Æ¿A´U
#define STORED_CHARACTER_ITEMSLOTINFO		"dbo.MP_CHARACTER_ItemSlotInfo_JP"			// ItemSlotInfo
#define STORED_CHARACTER_ITEMINFO			"dbo.MP_CHARACTER_ItemInfo"					// ¾ÆAIAU dº¸¸| ¹Þ¾Æ¿A´U
#define STORED_CHARACTER_ITEMRAREOPTIONINFO	"dbo.MP_CHARACTER_ItemRareOptionInfo"
#define STORED_CHARACTER_ITEMOPTIONINFO		"dbo.MP_CHARACTER_ItemOptionInfo"			// ¾ÆAIAU ¿E¼Cdº¸¸| ¹Þ¾Æ¿A´U
#define STORED_CHARACTER_SKILLINFO			"dbo.MP_CHARACTER_SkillInfo"				// ½ºA³ dº¸¸| ¹Þ¾Æ¿A´U

// magi82 //////////////////////////////////////////////////////////////////////////
#define STORED_CHARACTER_EXPFLAG			"dbo.MP_CHARACTER_UpdateExpFlag"
////////////////////////////////////////////////////////////////////////////////////

//#define STORED_CHARACTER_MUGONGWEAR			"MP_CHARACTER_MugongWear"				// ¹≪°øAaAø½A CwI½An
//#define STORED_CHARACTER_GROUNDFROMSERVER	"MP_CHARACTER_ItemGroundDrop"			// ¸E¼­¹iº¾i±¼­ ¾ÆAIAU Aß°¡
//#define STORED_CHARACTER_MUGONGTOINVENTORY	"MP_CHARACTER_MugongToInventory"		// ¹≪°øA￠¿¡¼­ AIº￥Aa¸®·I 
//#define STORED_CHARACTER_MUGINGDELETE		"MP_CHARACTER_MugongDelete"				// ¹≪°ø≫ef
#define STORED_CHARACTER_BASEECONOMY		"dbo.MP_MAP_BaseEconomy"					// ¸Eº°±aAØ½A¼¼Ao¸®
#define STORED_CHARACTER_AUCTIONREGIST		"dbo.MP_Auction_Regist"						// °æ¸Aμi·I
#define STORED_CHARACTER_AUCTIONONEPAGE		"dbo.MP_Auction_PageCalculate"				// °æ¸A¸®½ºÆ® COÆaAIAoAo¸®
//#define	STORED_CHARACTER_MUGONGUPDATE		"MP_CHARACTER_MugongUpdate"				// ¹≪°ødº¸ ¾÷μ￥AIÆ®
//#define	STORED_CHARACTER_MUGONGMOVEUPDATE	"MP_CHARACTER_MugongMoveUpdate"			// ¹≪°ødº¸ ¾÷μ￥AIÆ®
//#define	STORED_CHARACTER_CHARACTERUPDATE	"MP_CHARACTER_Update"					// A³¸?dº¸ ¾÷μ￥AIÆ®
#define STORED_CHARACTER_HEROINFOUPDATE		"dbo.MP_CHARACTER_HeroInfoUpdate"
#define STORED_CHARACTER_TOTALINFOUPDATE	"dbo.MP_CHARACTER_TotalInfoUpdate"
#define STORED_CHARACTER_BADFAMEUPDATE		"dbo.MP_CHARACTER_BadFameUpdate"
#define STORED_CHARACTER_REGISTLOGINMAPINFO "dbo.MP_LogInMapInfo_Regist"				//·I±×AI½A  ¸E AuAa ¹y¼®¾¾ Ao¿iAo ¸¶¼¼¿a!
#define STORED_CHARACTER_UNREGISTLOGINMAPINFO "dbo.MP_LogInMapInfo_UnRegist"			//·I±×¾Æ¿o  ¸E A￢¸®¾i ¹y¼®¾¾ Ao¿iAo ¸¶¼¼¿a!
#define STORED_CHARACTER_SAVEINFOBEFORELOGOUT "dbo.MP_CHARACTER_SaveInfoBeforeLogOut"
/*
#define STORED_CHARACTER_ITEMUPDATE			"MP_CHARACTER_ItemUpdate"				// ¾ÆAIAU ¾÷μ￥AIÆ®
#define STORED_CHARACTER_ITEMTOTALUPDATE	"MP_CHARACTER_ItemTotalUpdate"			// AIº￥ ¸ðμc ¾ÆAIAU dº¸ ¾÷μ￥AIÆ®
#define STORED_CHARACTER_ITEMBUY			"MP_CHARACTER_ItemBuy"					// ¾ÆAIAU ≫c±a
#define STORED_CHARACTER_ITEMINSERT			"MP_CHARACTER_ItemInsert"				// ¾ÆAIAU ≫y¼º
#define STORED_CHARACTER_ITEMDELETE			"MP_CHARACTER_ItemDelete"				// ¾ÆAIAU ≫ef
#define STORED_CHARACTER_ITEMDIVIDE			"MP_CHARACTER_ItemDivide"				// ¾ÆAIAU ³ª´ⓒ±a
#define STORED_CHARACTER_ITEMHUNT			"MP_CHARACTER_ItemHunt"
*/

/* New Item CwI½An */
#define STORED_ITEM_UPDATE					"dbo.MP_ITEM_Update"
//#define STORED_ITEM_PYOGUKMUNPA_UPDATE		"dbo.MP_ITEM_PyogukMunpaUpdate"
#define STORED_ITEM_COMBINEUPDATE			"dbo.MP_ITEM_CombineUpdate"
#define STORED_ITEM_MOVEUPDATE_INVEN		"dbo.MP_ITEM_MoveUpdate"
#define STORED_ITEM_MOVEUPDATE_PYOGUK		"dbo.MP_ITEM_MoveUpdatePyoguk"
#define STORED_ITEM_MOVEUPDATE_GUILD		"dbo.MP_ITEM_MoveUpdateMunpa"
#define STORED_ITEM_INSERT_INVEN			"dbo.MP_ITEM_Insert"
#define STORED_ITEM_DELETE					"dbo.MP_ITEM_Delete"
#define STORED_ITEM_RARE_OPTION_INSERT		"dbo.MP_ITEM_RARE_OPTION_Insert"
#define STORED_ITEM_RARE_OPTION_DELETE		"dbo.MP_ITEM_RARE_OPTION_Delete"
#define STORED_ITEM_OPTION_INSERT			"dbo.MP_ITEM_OPTION_Insert"
#define STORED_ITEM_OPTION_DELETE			"dbo.MP_ITEM_OPTION_Delete"


/* New Mugong CwI½An */
#define STORED_MUGONG_UPDATE				"dbo.MP_MUGONG_Update"
#define STORED_MUGONG_MOVEUPDATE			"dbo.MP_MUGONG_MoveUpdate"
#define STORED_MUGONG_INSERT				"dbo.MP_MUGONG_Insert"
#define STORED_MUGONG_DELETE				"dbo.MP_MUGONG_Delete"

//pjs ¿ⓒ±a¿¡ AUCTION¿¡ °uCN AuAa CwI½An Aß°¡ CØ¾ß CN´U..
//°æ¸A °a°u°¡ AO´AAo °E≫c 
#define STORED_AUCTION_CHECK		"dbo.MP_AUCTION_CHECK"
//A×AIºi¿¡¼­ v°C °E≫oCO¶§ ¾μ CwI½An	//±×·³ °æ¿iAC ¼o¸| ¸¸μe¾i¾ß °U´U.
#define STORED_AUCTION_SEARCH		"dbo.MP_AUCTION_SEARCH"
//A×AIºi; v°C d·ACO¶§ ¾μ CwI½An	//±×·³ °æ¿iAC ¼o¸| ¸¸μe¾i¾ß °U´U 
#define STORED_AUCTION_SORT			"dbo.MP_AUCTION_SORT"
//°æ¸A μi·ICO¶§ ¾μ CwI½An 
#define STORED_AUCTION_REGISTER		"dbo.MP_AUCTION_REGISTER"
//°æ¸A Ao¿øCO¶§ ¾μ CwI½An 
#define STORED_AUCTION_JOIN			"dbo.MP_AUCTION_JOIN"
//°æ¸A μi·I,Ao¿ø  Ae¼O¿¡ ¾μ CwI½An // ±×·³ °æ¿iAC ¼o¸| ¸¸μe¾i¾ß °U´U
#define STORED_AUCTION_CANCEL		"dbo.MP_AUCTION__REGISTER_CANCEL"

//ÆAÆ¼
#define STORED_PARTY_REGIST_MAPSERVER	"dbo.MP_PARTY_Regist"
#define STORED_PARTY_INFO_BY_USERLOGIN	"dbo.MP_PARTY_PartyInfoByUserLogin"
#define STORED_PARTY_CREATE				"dbo.MP_PARTY_CreateParty"
#define STORED_PARTY_BREAK				"dbo.MP_PARTY_BreakupParty"
#define STORED_PARTY_CHARACTERUPDATE	"dbo.MP_PARTY_DelPartyidxinCharacterTB"
#define STORED_PARTY_UPDATE				"dbo.MP_PARTY_UpdateMember"
#define STORED_PARTY_ADDMEMBER			"dbo.MP_PARTY_AddMember"
#define STORED_PARTY_DELMEMBER			"dbo.MP_PARTY_DelMember"
#define STORED_PARTY_CHANGEMASTER		"dbo.MP_PARTY_ChangeMaster"

//¹®ÆA 
/*w
#define STORED_MUNPA_SEARCHMUNPA		"dbo.MP_MUNPA_SearchMunpa"
#define STORED_MUNPA_CREATE				"dbo.MP_MUNPA_CreateMunpa"
#define STORED_MUNPA_BREAKUP			"dbo.MP_MUNPA_BreakupMunpa" //¹®ÆA¿øμe ¸®AI
#define STORED_MUNPA_BREAKUPMEMBERS		"dbo.MP_MUNPA_BreakupMembers"
#define STORED_MUNPA_BREAKUPCLEAR		"dbo.MP_MUNPA_BreakupClear"
#define	STORED_MUNPA_BANMEMBER			"dbo.MP_MUNPA_BanMember"
#define STORED_MUNPA_SECEDE				"dbo.MP_MUNPA_Secede"
#define STORED_MUNPA_CHANGEMASTER		"dbo.MP_MUNPA_ChangeMaster"
#define STORED_MUNPA_CHANGEMEMBER_RANK	"dbo.MP_MUNPA_ChangeMemberRank"
#define STORED_MUNPA_ACCEPTMEMBER		"dbo.MP_MUNPA_AddMemberAccept"
#define STORED_MUNPA_DENYMEMBER			"dbo.MP_MUNPA_AddMemberDeny"
#define STORED_MUNPA_JOINMEMBER			"dbo.MP_MUNPA_JoinMember"
#define STORED_MUNPA_JOINMEMBERCANCEL	"dbo.MP_MUNPA_JoinMemberCancel"
#define STORED_MUNPA_JOINMEMBERDENY		"dbo.MP_MUNPA_JoinMemberDeny"
#define STORED_MUNPA_LOADMUNPALIST_FIRST	"dbo.MP_MUNPA_LoadMunpaList"
#define STORED_MUNPA_LOADITEMLIST_FIRST	"dbo.MP_MUNPA_LoadItemList"
#define STORED_MUNPA_LOADMUNPALIST_CLIENT	"dbo.MP_MUNPA_LoadMunpaListSyn"
#define STORED_MUNPA_MUNPAHOMEINFO		"dbo.MP_MUNPA_MunpaHomeInfo"
#define STORED_MUNPA_REGULARMEMBERSINFO	"dbo.MP_MUNPA_RegularMembersInfo"
#define STORED_MUNPA_JOINMEMBERSINFO	"dbo.MP_MUNPA_JoinMembersInfo"
#define STORED_MUNPA_MUNPABOARDINFO		"dbo.MP_MUNPA_BoardInfo"
#define STORED_MUNPA_CREATEBOARD		"dbo.MP_MUNPA_CreateBoard"
#define STORED_MUNPA_DELETEBOARD		"dbo.MP_MUNPA_DeleteBoard"
#define STORED_MUNPA_BOARDRANK_INFO		"dbo.MP_MUNPA_BoardRankInfo"
#define STORED_MUNPA_BOARDINFOUPDATE	"dbo.MP_MUNPA_BoardInfoUpdate"
#define STORED_MUNPA_BOARDLISTNEXT		"dbo.MP_MUNPA_BoardListNext"
#define STORED_MUNPA_BOARDLISTPREV		"dbo.MP_MUNPA_BoardListPrev"
#define STORED_MUNPA_BOARDREADCONTENT	"dbo.MP_MUNPA_BoardReadContent"
#define STORED_MUNPA_BOARDWRITE			"dbo.MP_MUNPA_BoardWrite"
#define STORED_MUNPA_BOARDMODIFYCONTENT "dbo.MP_MUNPA_BoardModifyContent"
#define STORED_MUNPA_BOARDBACKPAGE		"dbo.MP_MUNPA_BoardBackPage"
#define STORED_MUNPA_BOARDFRONTPAGE		"dbo.MP_MUNPA_BoardFrontPage"
#define	STORED_MUNPA_BOARDSUBJECTLIST	"dbo.MP_MUNPA_BoardSubjectListInfo"
#define STORED_MUNPA_BOARDDELETECONTENT "dbo.MP_MUNPA_BoardDeleteContent"
#define STORED_MUNPA_MONEYPUTINOUT		"dbo.MP_MUNPA_MoneyPutInOut"
#define STORED_MUNPA_MODIFYINTRO		"dbo.MP_MUNPA_ModifyIntro"*/


//guild
#define STORED_GUILD_CREATE				"dbo.MP_GUILD_Create"
#define STORED_GUILD_BREAKUP			"dbo.MP_GUILD_BreakUp"
#define STORED_GUILD_DELETEMEMBER		"dbo.MP_GUILD_DeleteMember"
#define STORED_GUILD_ADDMEMBER			"dbo.MP_GUILD_AddMember"
#define STORED_GUILD_LOADGUILD			"dbo.MP_GUILD_LoadGuild"
#define STORED_GUILD_LOADNOTICE			"dbo.MP_GUILD_LoadNotice"
#define STORED_GUILD_UPDATENOTICE		"dbo.MP_GUILD_UpdateNotice"
#define STORED_GUILD_LOADMEMBER			"dbo.MP_GUILD_LoadMember"
#define STORED_GUILD_MARKREGIST			"dbo.MP_GUILD_MarkRegist"	
#define STORED_GUILD_MARKUPDATE			"dbo.MP_GUILD_MarkUpdate"
#define STORED_GUILD_LOADMARK			"dbo.MP_GUILD_LoadMark"
#define STORED_GUILD_LEVELUP			"dbo.MP_GUILD_LevelUp"
#define STORED_GUILD_CHANGRANK			"dbo.MP_GUILD_ChangeRank"
#define STORED_GUILD_LOADITEM			"dbo.MP_GUILD_LoadItem"
#define STORED_GUILD_GIVENICKNAME		"dbo.MP_GUILD_GiveMemberNickName"
#define STORED_GUILD_MONEYUPDATE		"dbo.MP_GUILD_MoneyUpdate"
#define STORED_GUILD_ITEMOPTION			"dbo.MP_GUILD_ItemOption_Info"
#define STORED_GUILD_MARKREGIST			"dbo.MP_GUILD_MarkRegist"
//magi82 - 문하생 가입편리시스템 /////////////////////////////////////////
#define STORED_GUILD_TRAINEE_INFO		"dbo.MP_GUILD_TRAINEE_Info"
#define STORED_GUILD_TRAINEE_INSERT		"dbo.MP_GUILD_TRAINEE_Insert"
#define STORED_GUILD_TRAINEE_DELETE		"dbo.MP_GUILD_TRAINEE_Delete"

#define STORED_GUILD_TRAINEE_GUILDINFO			"dbo.MP_GUILD_TRAINEE_GuildInfo"
#define STORED_GUILD_TRAINEE_GUILDINSERT		"dbo.MP_GUILD_TRAINEE_GuildInsert"
#define STORED_GUILD_TRAINEE_GUILDDELETE		"dbo.MP_GUILD_TRAINEE_GuildDelete"

#define STORED_GUILD_TRAINEE_DELETEBYGUILD		"dbo.MP_GUILD_TRAINEE_DeletebyGuild"
#define STORED_GUILD_ADD_STUDENT				"dbo.MP_GUILD_AddStudent"

#define STORED_GUILD_TRAINEE_GUILDINTROINFO		"dbo.MP_GUILD_TRAINEE_GuildIntroInfo"
#define  STORED_GUILD_TRAINEE_GUILDINTROUPDATE	"dbo.MP_GUILD_TRAINEE_GuildIntroUpdate"
//////////////////////////////////////////////////////////////////////////

#define	STORED_TITAN_WEARITEMINFO	"dbo.MP_TITAN_WearItemInfo"	// magi82 - Titan(070209)


//C￥±¹
#define STORED_PYOGUK_BUYPYOGUK			"dbo.MP_PYOGUK_Buy"
#define STORED_PYOGUK_MONEYUPDATE		"dbo.MP_PYOGUK_MoneyUpdate"
#define STORED_PYOGUK_TITAN_ENDURANCE_INFO	"dbo.MP_PYOGUK_Titan_Endurance_Info"	// magi82 - Titan(071015) 창고에 타이탄 장비 관련

/* C￥±¹ ¼od AU¾÷ */
#define STORED_PYOGUK_INFO				"dbo.MP_PYOGUK_Info"
#define STORED_PYOGUK_ITEMINFO			"dbo.MP_PYOGUK_ItemInfo"
#define STORED_PYOGUK_MONEYUPDATE		"dbo.MP_PYOGUK_MoneyUpdate"

//A￡±¸
#define STORED_FRIEND_NOTIFYLOGOUT		"dbo.MP_FRIEND_NotifyLogout"

// ItemMall Item
#define STORED_SHOPINVEN_INFO			"dbo.MP_SHOPITEM_InvenInfo"
#define STORED_SHOPITEM_INFO			"dbo.MP_SHOPITEM_ItemInfo"
#define STORED_SHOPITEM_USEINFO			"dbo.MP_SHOPITEM_UseInfo"
#define STORED_SHOPITEM_USING			"dbo.MP_SHOPITEM_Using"
#define STORED_SHOPITEM_USING_JP		"dbo.MP_SHOPITEM_Using_JP"
#define STORED_SHOPITEM_DELETE			"dbo.MP_SHOPITEM_Delete"
#define STORED_SHOPITEM_UPDATETIME		"dbo.MP_SHOPITEM_Updatetime"
#define STORED_SHOPITEM_UPDATEPARAM		"dbo.MP_SHOPITEM_UpdateParam"
#define STORED_SHOPITEM_UPDATEUSEPARAM	"dbo.MP_SHOPITEM_UpdateUseParam"
#define STORED_SHOPITEM_UPDATEUSEPARAM_JP	"dbo.MP_SHOPITEM_UpdateUseParam_JP"
#define STORED_SHOPITEM_GETITEM			"dbo.MP_SHOPITEM_GetItem"
#define STORED_ITEM_MOVEUPDATE_SHOP		"dbo.MP_ITEM_MoveUpdateShop"
#define STORED_SHOPITEM_CHANGENAME		"dbo.MP_character_rename"
#define STORED_SHOPITEM_CHARCHANGE		"dbo.MP_SHOPITEM_CharChange"
#define STORED_SHOPITEM_MPINFO			"dbo.MP_MOVEPOINT_GetInfo"
#define STORED_SHOPITEM_MPINSERT		"dbo.MP_MOVEPOINT_Insert"
#define STORED_SHOPITEM_MPUPDATE		"dbo.MP_MOVEPOINT_Update"
#define STORED_SHOPITEM_MPDEL			"dbo.MP_MOVEPOINT_Delete"
#define STORED_SHOPITEM_UPDATEUSEINFO	"dbo.MP_SHOPITEM_UpdateUseInfo"
#define STORED_SHOPITEM_AVATARABLE		"dbo.MP_SHOPITEM_IsAbleAvatar"
#define STORED_SHOPITEM_EQUIPABLE		"dbo.MP_SHOPITEM_IsAbleEquip"
#define STORED_SHOPITEM_UPDAETALL		"dbo.MP_SHOPITEM_UpdateAll"


//¸i¼º ¾÷μ￥AIÆ® 
//#define STORED_FAME_MUNPAUPDATE			"dbo.MP_FAME_MunpaUpdate"
#define STORED_FAME_CHARACTERUPDATE		"dbo.MP_FAME_CharacterUpdate"

//¾C¸i ¾÷μ￥AIÆ®
#define STORED_BADFAME_CHARACTERUPDATE	"dbo.MP_BADFAME_CharacterUpdate"
#define STORED_PK_CHARACTERUPDATE		"dbo.MP_PK_CharacterUpdate"

// Co≫o±Y
#define STORED_WANTED_LOAD				"dbo.MP_WANTED_LoadList"
#define STORED_WANTED_INFO				"dbo.MP_WANTED_InfoByUserLogIn"
#define STORED_WANTED_BUYRIGHT			"dbo.MP_WANTED_BuyRight"
#define STORED_WANTED_REGIST			"dbo.MP_WANTED_Regist"
#define STORED_WANTED_GIVEUP			"dbo.MP_WANTED_GiveUpRight"
#define STORED_WANTED_COMPLETE			"dbo.MP_WANTED_Complete"
#define STORED_WANTED_DESTROYED			"dbo.MP_WANTED_Destroyed"
#define STORED_WANTED_ORDERLIST			"dbo.MP_WANTED_OrderList"
#define STORED_WANTED_MYLIST			"dbo.MP_WANTED_MyList"
#define STORED_WANTED_SEARCH			"dbo.MP_WANTED_Search"

// quest
#define STORED_QUEST_MAINQUEST_LOAD		"dbo.MP_MAINQUEST_Load"
#define STORED_QUEST_MAINQUEST_INSERT	"dbo.MP_MAINQUEST_Insert"
#define STORED_QUEST_MAINQUEST_DELETE	"dbo.MP_MAINQUEST_Delete"
#define STORED_QUEST_MAINQUEST_UPDATE	"dbo.MP_MAINQUEST_Update"
#define STORED_QUEST_SUBQUEST_LOAD		"dbo.MP_SUBQUEST_Load"
#define STORED_QUEST_SUBQUEST_INSERT	"dbo.MP_SUBQUEST_Insert"
#define STORED_QUEST_SUBQUEST_DELETE_ALL "dbo.MP_SUBQUEST_Delete_All"
#define STORED_QUEST_SUBQUEST_DELETE	"dbo.MP_SUBQUEST_Delete"
#define STORED_QUEST_SUBQUEST_UPDATE	"dbo.MP_SUBQUEST_Update"
#define STORED_QUEST_ITEM_LOAD			"dbo.MP_QUESTITEM_Load"
#define STORED_QUEST_ITEM_INSERT		"dbo.MP_QUESTITEM_Insert"
#define STORED_QUEST_ITEM_DELETE		"dbo.MP_QUESTITEM_Delete"
#define STORED_QUEST_ITEM_DELETE_ALL	"dbo.MP_QUESTITEM_Delete_All"
#define STORED_QUEST_ITEM_UPDATE		"dbo.MP_QUESTITEM_Update"

#define STORED_JOURNAL_GETLIST			"dbo.MP_JOURNAL_GetList"
#define STORED_JOURNAL_INSERTQUEST		"dbo.MP_JOURNAL_InsertQuest"
#define STORED_JOURNAL_INSERTWANTED		"dbo.MP_JOURNAL_InsertWanted"
#define STROED_JOURNAL_INSERTLEVEL		"dbo.MP_JOURNAL_InsertLevel"
#define STROED_JOURNAL_SAVEUPDATE		"dbo.MP_JOURNAL_SaveUpdate"
#define STROED_JOURNAL_DELETE			"dbo.MP_JOURNAL_Delete"

// ¸EAuE? A³¸?dº¸
#define STORED_MAPCHANGE_CHARINFO		"dbo.MP_MAPCHANGE_CharInfo"
#define STORED_MAPCHANGE_MUGONGINFO		"dbo.MP_CHARACTER_MugongInfo"

#define STORED_SAVE_MAPCHAGEPOINT		"dbo.MP_CHARACTER_MapchangePointUpdate"
#define STORED_SAVE_MAPCHAGEPOINTRET	"dbo.MP_CHARACTER_MapchangePointReturn"
#define STORED_SAVE_LOGINPOINT			"dbo.MP_CHARACTER_LoginPointUpdate"
// AIº￥Aa¸® ¾ÆAIAU ≫ðAO
//#define STORED_CHARACTER_ITEMINSERT		"MP_CHARACTER_ItemInsert"

// Quest

// SavePoint
#define STORED_MAP_SAVEPOINT			"dbo.MP_MAP_SavePoint"

// Guild Tournament
#define STORED_GT_LOADALL				"dbo.MP_GT_LoadAll"
#define STORED_GT_CANCELLOAD			"dbo.MP_GT_CancelLoad"
#define STORED_GT_CANCELINSERT			"dbo.MP_GT_CancelInsert"
#define STORED_GT_TOURNAMENTINFO_UPDATE	"dbo.MP_GT_TrInfoUpdate"
#define STORED_GT_GUILDINFO_INSERT		"dbo.MP_GT_Insert"
#define STORED_GT_GUILDINFO_DELETE		"dbo.MP_GT_Delete"
#define STORED_GT_GUILDINFO_UPDATE		"dbo.MP_GT_Update"

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
//┃SiegeWar													  ┃
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
#define STORED_SIEGE_INFOLOAD			"dbo.MP_SIEGEWAR_InfoLoad"
#define STORED_SIEGE_INFOINSERT			"dbo.MP_SIEGEWAR_InfoInsert"
#define STORED_SIEGE_INFOUPDATE			"dbo.MP_SIEGEWAR_InfoUpdate"
#define STORED_SIEGE_GUILDLOAD			"dbo.MP_SIEGEWAR_GuildInfoLoad"
#define STORED_SIEGE_GUILDINSERT		"dbo.MP_SIEGEWAR_GuildInfoInsert"
#define STORED_SIEGE_GUILDUPDATE		"dbo.MP_SIEGEWAR_GuildInfoUpdate"
#define STORED_SIEGE_GUILDDELETE		"dbo.MP_SIEGEWAR_GuildInfoDelete"

//2007. 12. 10. CBH - 캐릭터 스킨
#define STORED_CHARACTER_SKININFO		"dbo.MP_CHARACTER_UpdateSkinInfo"

// magi82(41) - 샵아이템 추가(스텟 초기화 주문서)
#define STORED_CHARACTER_UPDATE_RESETSTATUSPOINT	"dbo.MP_CHARACTER_UpdateResetStatusPoint"

//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//pjs ¿ⓒ±a´A ³≫°¡ °iAA¾ß °U´U..
enum AuctionPage			// °æ¸A¸®½ºÆ® ¹Þ;¶§ ≫c¿e
{
	eAT_DBIdx, eAT_ItemDBIdx, eAT_ItemIdx, eAT_SellNum,  eAT_SellerID, eAT_EndDate, eAT_EndTime,
	eAT_StartPrice, eAT_ImmediatePrice, eAT_CurrentPrice, eAT_BidderNum, eAT_CurrentBidder, eAT_RegDate
};
//DB·I ºIAI ¹Þ´A AU·a¸| 'CN dAC
//eAuctionPageLIst
// AI¹IAo 1> °æ¸A ¹øE￡, ¹°C°¸i, ¼o·®, AO°i °¡°Y, Ai½A ±¸¸A°¡°Y, ¸¶°¨½A°￡, ³≪Au/·AAU, Au¿ⓒ¼o 
// AI¹IAo 3> ³²:½A°￡, °æ¸AAU
// ¾ÆAIAUAC AIμ|½º, 
enum AuctionList
{
	ePRO_NAME, ePRO_Amount, eBID_High_Price, eBID_Immediate_Price, eBID_DueDate, eHigh_Price_Name, eJOIN_Amount, 
	eBID_Left_Time, eAuctioneer,ePRO_Index ,eBID_Starttime,eREG_Index, 
};

//eAuctionSuccess
// AI¹IAo 4> °æ¸A¹°, °¹¼o, °≫¸AAU 
// ¾ÆAIAUAC AIμ|½º, °æ¸A μi·I ¹øE￡ ???
enum AuctionSuccess
{
	sePRO_NAME, sePRO_Amount, seAuctioneer,
	seREG_Index

};
//eAuctionConfirm
enum AuctionConfirm
{
	bAuctionConfirm
};

// magi82 - add /////////////////////////////////////////////////////////////////////
// DB¿¡¼­ ¾²AI´A ¼oA¡A¡E?.
enum CHSelectInfo	// A³¸?dº¸¸| ¹Þ;¶§ ¾¸
{
	eCS_ObjectID = 0, eCS_UserID, eCS_PartyID,eCS_Gender, eCS_Gengoal, eCS_Life, eCS_Shield, eCS_Energe, eCS_Dex, eCS_Sta, eCS_Simmak, eCS_Fame, 
	eCS_Karma, eCS_Name, eCS_Repute, eCS_Stage, eCS_Grade, eCS_Expoint, eCS_GradeUpPoint, eCS_Money, 
	eCS_QuickPace, eCS_Vitality, eCS_Map, eCS_FaceType, eCS_HeadType, eCS_Hat, eCS_Weapon, eCS_Dress, eCS_Shoes, 
	eCS_Ring1, eCS_Ring2, eCS_Cape, eCS_Necklace, eCS_Armlet, eCS_Belt, eCS_MunpaIDX, eCS_GuildPositon, eCS_GuildName, eCS_BadFame,
	eCS_MapChangePoint, eCS_LoginPoint, eCS_AbilityExp, eCS_Height, eCS_Width, eCS_Playtime, eCS_LastPKEndTime, eCS_MaxLevel, eCS_MarkName, eCS_MunpaCanEntryDate,
	eCS_NickName, eCS_ExpFlag,eCS_SkinInfo,
};
////////////////////////////////////////////////////////////////////////////////////

enum CHMugong		// A³¸?¹≪°ødº¸¸| ¹Þ;¶§ ¾¸
{
	eCM_ObjectID = 0, eCM_DBIDX, eCM_IDX, eCM_ExpPoint, eCM_Sung, eCM_Position, eCM_QPosition, eCM_Wear, eCM_Option
};

enum CHItem			// A³¸?AI ¾ÆAIAU dº¸¸| ¹Þ;¶§ ≫c¿e
{
	eCI_ObjectID = 0, eCI_DBIDX, eCI_IDX, eCI_Position, eCI_QPosition, eCI_Durability, eCI_Param, eCI_RareIdx,
};

enum CHItemRare	//!!!임시 DB에 맞게 재작업 필요
{
	eCIR_ObjectID, eCIR_ItemIdx, eCIR_Position, eCIR_RareID, eCIR_ItemDBID, /*eCIR_Durability, eCIR_QPosition, eCIR_Param,*/
	eCIR_GenGol, eCIR_MinChub, eCIR_CheRyuk, eCIR_SimMek, eCIR_Life, eCIR_NaeRyuk, eCIR_Shield, 
	eCIR_FireAttrRegist, eCIR_WaterAttrRegist, eCIR_TreeAttrRegist,  eCIR_GoldAttrRegist, eCIR_EarthAttrRegist,
	eCIR_PhyAttack, /*eCIO_Critical,*/
	eCIR_FireAttrAttack, eCIR_WaterAttrAttack, eCIR_TreeAttrAttack,  eCIR_GoldAttrAttack, eCIR_EarthAttrAttack,
	eCIR_PhyDefense 
};

enum CHItemOption	// ¾ÆAIAU ¿E¼C
{
	eCIO_ObjectID, eCIO_ItemIdx, eCIO_OptionID, eCIO_ItemDBID, eCIO_GenGol, eCIO_MinChub, eCIO_CheRyuk, eCIO_SimMek, eCIO_Life, eCIO_NaeRyuk, eCIO_Shield, 
	eCIO_FireAttrRegist, eCIO_WaterAttrRegist, eCIO_TreeAttrRegist,  eCIO_GoldAttrRegist, eCIO_EarthAttrRegist,
	eCIO_PhyAttack, eCIO_Critical, 
	eCIO_FireAttrAttack, eCIO_WaterAttrAttack, eCIO_TreeAttrAttack,  eCIO_GoldAttrAttack, eCIO_EarthAttrAttack,
	eCIO_PhyDefense 
};

enum CHItemRareOptionInfo
{
	eCIRO_ObjectID, /*eCIRO_ItemIdx,*/ eCIRO_RareOptionID, eCIRO_ItemDBID, eCIRO_GenGol, eCIRO_MinChub, eCIRO_CheRyuk, eCIRO_SimMek, eCIRO_Life, eCIRO_NaeRyuk, eCIRO_Shield, 
	eCIRO_FireAttrRegist, eCIRO_WaterAttrRegist, eCIRO_TreeAttrRegist,  eCIRO_GoldAttrRegist, eCIRO_EarthAttrRegist,
	eCIRO_PhyAttack, /*eCIO_Critical, */
	eCIRO_FireAttrAttack, eCIRO_WaterAttrAttack, eCIRO_TreeAttrAttack,  eCIRO_GoldAttrAttack, eCIRO_EarthAttrAttack,
	eCIRO_PhyDefense 
};
enum CHItemOptionInfo	// ¾ÆAIAU ¿E¼C
{
	eCIOI_ObjectID, eCIOI_OptionID, eCIOI_ItemDBID, eCIOI_GenGol, eCIOI_MinChub, eCIOI_CheRyuk, eCIOI_SimMek, eCIOI_Life, eCIOI_NaeRyuk, eCIOI_Shield, 
	eCIOI_FireAttrRegist, eCIOI_WaterAttrRegist, eCIOI_TreeAttrRegist,  eCIOI_GoldAttrRegist, eCIOI_EarthAttrRegist,
	eCIOI_PhyAttack, eCIOI_Critical, 
	eCIOI_FireAttrAttack, eCIOI_WaterAttrAttack, eCIOI_TreeAttrAttack,  eCIOI_GoldAttrAttack, eCIOI_EarthAttrAttack,
	eCIOI_PhyDefense 
};

enum CPyogukItemRareOptionInfo	// ¾ÆAIAU ¿E¼C
{
	ePROI_RareOptionID, ePROI_ItemDBID, ePROI_GenGol, ePROI_MinChub, ePROI_CheRyuk, ePROI_SimMek, ePROI_Life, ePROI_NaeRyuk, ePROI_Shield, 
	ePROI_FireAttrRegist, ePROI_WaterAttrRegist, ePROI_TreeAttrRegist,  ePROI_GoldAttrRegist, ePROI_EarthAttrRegist,
	ePROI_PhyAttack, /*ePROI_Critical,*/
	ePROI_FireAttrAttack, ePROI_WaterAttrAttack, ePROI_TreeAttrAttack,  ePROI_GoldAttrAttack, ePROI_EarthAttrAttack,
	ePROI_PhyDefense 
};

enum CPyogukItemOptionInfo	// ¾ÆAIAU ¿E¼C
{
	ePOI_OptionID, ePOI_ItemDBID, ePOI_GenGol, ePOI_MinChub, ePOI_CheRyuk, ePOI_SimMek, ePOI_Life, ePOI_NaeRyuk, ePOI_Shield, 
	ePOI_FireAttrRegist, ePOI_WaterAttrRegist, ePOI_TreeAttrRegist,  ePOI_GoldAttrRegist, ePOI_EarthAttrRegist,
	ePOI_PhyAttack, ePOI_Critical, 
	ePOI_FireAttrAttack, ePOI_WaterAttrAttack, ePOI_TreeAttrAttack,  ePOI_GoldAttrAttack, ePOI_EarthAttrAttack,
	ePOI_PhyDefense 
};
enum CMunpaItemOptionInfo	// ¾ÆAIAU ¿E¼C
{
	eMOI_OptionID, eMOI_ItemDBID, eMOI_GenGol, eMOI_MinChub, eMOI_CheRyuk, eMOI_SimMek, eMOI_Life, eMOI_NaeRyuk, eMOI_Shield, 
	eMOI_FireAttrRegist, eMOI_WaterAttrRegist, eMOI_TreeAttrRegist,  eMOI_GoldAttrRegist, eMOI_EarthAttrRegist,
	eMOI_PhyAttack, eMOI_Critical, 
	eMOI_FireAttrAttack, eMOI_WaterAttrAttack, eMOI_TreeAttrAttack,  eMOI_GoldAttrAttack, eMOI_EarthAttrAttack,
	eMOI_PhyDefense 
};

enum CGuildItemRareOptionInfo
{
	eGORI_RareOptionID, eGORI_ItemDBID, eGORI_GenGol, eGORI_MinChub, eGORI_CheRyuk, eGORI_SimMek, eGORI_Life, eGORI_NaeRyuk, eGORI_Shield, 
	eGORI_FireAttrRegist, eGORI_WaterAttrRegist, eGORI_TreeAttrRegist,  eGORI_GoldAttrRegist, eGORI_EarthAttrRegist,
	eGORI_PhyAttack, /*eGORI_Critical, */
	eGORI_FireAttrAttack, eGORI_WaterAttrAttack, eGORI_TreeAttrAttack,  eGORI_GoldAttrAttack, eGORI_EarthAttrAttack,
	eGORI_PhyDefense
};

enum eGuildItemOptionInfo
{
	eGIOI_GuildIdx,
	eGIOI_OptionID, eGIOI_ItemDBID, eGIOI_GenGol, eGIOI_MinChub, eGIOI_CheRyuk, eGIOI_SimMek, eGIOI_Life, eGIOI_NaeRyuk, eGIOI_Shield, 
	eGIOI_FireAttrRegist, eGIOI_WaterAttrRegist, eGIOI_TreeAttrRegist,  eGIOI_GoldAttrRegist, eGIOI_EarthAttrRegist,
	eGIOI_PhyAttack, eGIOI_Critical, 
	eGIOI_FireAttrAttack, eGIOI_WaterAttrAttack, eGIOI_TreeAttrAttack,  eGIOI_GoldAttrAttack, eGIOI_EarthAttrAttack,
	eGIOI_PhyDefense
};

enum eGuildItemRareOptionInfo
{
	eGIROI_GuildIdx,
	eGIROI_RareOptionID, eGIROI_ItemDBID, eGIROI_GenGol, eGIROI_MinChub, eGIROI_CheRyuk, eGIROI_SimMek, eGIROI_Life, eGIROI_NaeRyuk, eGIROI_Shield, 
	eGIROI_FireAttrRegist, eGIROI_WaterAttrRegist, eGIROI_TreeAttrRegist,  eGIROI_GoldAttrRegist, eGIROI_EarthAttrRegist,
	eGIROI_PhyAttack, /*eGIROI_Critical, */
	eGIROI_FireAttrAttack, eGIROI_WaterAttrAttack, eGIROI_TreeAttrAttack,  eGIROI_GoldAttrAttack, eGIROI_EarthAttrAttack,
	eGIROI_PhyDefense
};

//magi82 - 문하생 가입편리시스템 /////////////////////////////////////////
enum eGuildTraineeInfoIndex
{
    eGTII_DBIdx, eGTII_GuildIdx, eGTII_UserIdx, eGTII_UserName, eGTII_JoinTime
};

enum eGuildTraineeGuildInfoIndex
{
	eGTGII_DBIdx, eGTGII_GuildIdx, eGTGII_GuildRank
};

enum eGuildAddStudentIndex
{
	eGASI_CharIdx, eGASI_Level, eGASI_Name, eGASI_GuildIdx
};

enum eGuildTraineeGuildIntroInfoIndex
{
    eGTGIII_DBIdx, eGTGIII_Intro,
};
//////////////////////////////////////////////////////////////////////////

enum CPetInvenItemOptionInfo
{
	ePIOI_OptionID, ePIOI_ItemDBID, ePIOI_GenGol, ePIOI_MinChub, ePIOI_CheRyuk, ePIOI_SimMek, ePIOI_Life, ePIOI_NaeRyuk, ePIOI_Shield, 
	ePIOI_FireAttrRegist, ePIOI_WaterAttrRegist, ePIOI_TreeAttrRegist,  ePIOI_GoldAttrRegist, ePIOI_EarthAttrRegist,
	ePIOI_PhyAttack, ePIOI_Critical, 
	ePIOI_FireAttrAttack, ePIOI_WaterAttrAttack, ePIOI_TreeAttrAttack,  ePIOI_GoldAttrAttack, ePIOI_EarthAttrAttack,
	ePIOI_PhyDefense
};

enum CPetInvenItemRareOptionInfo
{
	ePIROI_RareOptionID, ePIROI_ItemDBID, ePIROI_GenGol, ePIROI_MinChub, ePIROI_CheRyuk, ePIROI_SimMek, ePIROI_Life, ePIROI_NaeRyuk, ePIROI_Shield, 
	ePIROI_FireAttrRegist, ePIROI_WaterAttrRegist, ePIROI_TreeAttrRegist,  ePIROI_GoldAttrRegist, ePIROI_EarthAttrRegist,
	ePIROI_PhyAttack, /*ePIROI_Critical,*/
	ePIROI_FireAttrAttack, ePIROI_WaterAttrAttack, ePIROI_TreeAttrAttack,  ePIROI_GoldAttrAttack, ePIROI_EarthAttrAttack,
	ePIROI_PhyDefense	
};

enum CPetTotalInfo
{
	/*ePTI_PetObjectID,*/ ePTI_PetDBIdx, ePTI_UserID, ePTI_PetSummonItemDBIdx, ePTI_PetKind, ePTI_PetGrade, ePTI_PetStamina, ePTI_PetFriendship, 
	ePTI_Alive, /*ePTI_PetSummonItemIdx,*/ ePTI_Summonning, ePTI_Rest,
};

enum CPetInsertInfo
{
	ePII_PetSummonItemIdx, ePII_PetDBIdx, ePII_MasterObjectID, ePII_UserIdx, ePII_PetSummonItemDBIdx, ePII_PetKind, ePII_PetGrade, ePII_PetStamina, ePII_PetFriendship, 
	ePII_Alive, ePII_Summonning, ePII_Rest,
};

enum GuildPointInfoInit
{
	eGPII_GuildDBIdx, eGPII_GuildIdx, eGPII_GuildPoint, eGPII_GuildHuntedMonsterTotalCount, eGPII_EndTimePerGuildPlustimeKind,
};

enum eGuildHuntedMonsterCount
{
	eGHMC_TotalCount, eGHMC_DB_Date,
};

enum GuildConvertHuntedMonsterCountWithInit
{
	eGCHMC_TotalCount, eGCHMC_DB_Date,
};

enum Guild_AddGuildPoint
{
	eGAGP_AddPoint, eGAGP_TotalPoint, eGAGP_AddKind, eGAGP_AdditionalData, 
};

enum Guild_UseGuildPoint
{
	eGUGP_UsePoint, eGUGP_TotalPoint, eGUGP_UseKind, eGUGP_KindIdx, eGUGP_Endtime,
};

enum CHSkill		// A³¸?AI ½ºA³ dº¸¸| ¹Þ;¶§ ≫c¿e
{
	eCK_ObjectID = 0, eCK_DBIDX, eCK_IDX, eCK_Position, eCK_QPosition, eCK_Durability
};

enum CHItemGroundDrop		// ¼­¹iº¾i±¼­ ¸E¿¡ ¾ÆAIAU Aß°¡EA dº¸¸| ¹Þ;¶§ ≫c¿e
{
	eIG_ObjectID, eIG_DBIDX, eIG_IDX, eIG_POSX, eIG_POSZ
};

enum MMapBaseEconomy			// ¸Eº° ±aAØ½A¼¼¸| ¹Þ;¶§ ≫c¿e
{
	eME_MEPNUM, eME_ORIGINNUM, eME_ORIGINPRICE, eME_ORIGINAMOUNT, eME_REQUIRENUM, eME_REQUIREPRICE, eME_REQUIREAMOUNT,
	eME_BUYRATES, eME_SELLRATES 
};

enum PartyInfoMap
{
	ePA_IDX, 
};
enum Partyinfobyuserlogin
{
	ePA_LIMemberId, ePA_LIMemberName, ePA_LIMemberCurrentMap, ePA_LIMemberLevel, ePA_LIOption,
	//2008. 5. 22. CBH - 방파 추가 옵션 관련 수정
	ePA_MinLevel, ePA_MaxLevel, ePA_Public, ePA_LimitCount, ePA_Theme,
};
enum Partychangemaster
{
	ePA_CMErr, ePA_CMTargetID,
};

enum PartyMember 
{
	 ePA_ERR,ePA_PARTYID, ePA_MASTER, ePA_OPTION,
	 //2008. 5. 22. CBH - 방파 추가 옵션 관련 수정
	 ePA_MINLEVEL, ePA_MAXLEVEL, ePA_PUBLIC, ePA_LIMITCOUNT, ePA_THEME,
};
enum Partydelmember
{
	ePA_DMErr, ePA_DMTargetID,
};
enum Munpainfo //¸E¼­¹o°¡ ½AAUCO ¶§ ¹Þ´A ¹®ÆA dº¸ 
{
	eMu_MunpaID, eMunpaMasterID, eMu_MunpaName, eFamous, eMemberNum, eMunpaRank, eMunpaMoney
};

enum Munpaloadsyn
{
	eMu_MLStotalnum, eMu_MLSmunpaid, eMu_MLSmunpaname, eMu_MLSmastername, eMu_MLSmunpafamous, eMu_MLSmembernum, eMu_MLSKind
};
enum Munpacreate
{
	eMu_MCErr, eMu_MCMunpaid, eMu_MCMasterid, eMu_MCMunpaname, eMu_MCMunpaRank, eMu_MCMasterFame
};
enum Munpabreakup
{
	eMu_MBerr, eMu_MBmasterid, eMu_MBmasterfame,
};
enum Munpabreakupmember
{
	eMu_MBmemberid, eMu_MBmemberfame,
};
enum Munpahomeinfo
{
	eMu_MHIMunpaid, eMu_MHIIntroMsg, eMu_MHIMunpaKind, 
	eMu_MHIMunpaName, eMu_MHIMasterName, eMu_MHIFamous, 
	eMu_MHICreateDate, eMu_MHIMemberNum, eMu_MHIFieldWarWin, 
	eMu_MHIFieldWarLoss, eMu_MHIBuildingWarWin, eMu_MHIBuildingWarLoss, 
	eMu_MHICastleWarWin, eMu_MHICastleWarLoss, eMu_MHIBoardIDX, 
	eMu_MHIRSRank, eMu_MHIRCRank, eMu_MHIWCRank, 
	eMu_MHIDCRank, eMu_MHIBoardName, 
};
enum Munpamembersinfo
{
	eMu_MMTotalNum, eMu_MMunpaid, eMu_MMemberid, 
	eMu_MMemberName, eMu_MMemberRank, eMu_MMemberLevel, 
	eMu_MMemberFamous, eMu_MMember_LastVisitDay, eMu_MMember_EntryDay, 
	eMu_MMember_bLogin,
};
enum Munpachangememberank
{
	eMu_MCRchangeerr, eMu_MCRtargetid, eMu_MCRmunpaid, eMu_MCRposition, eMu_MCRtargetname, eMu_MCRfame, 
};
enum Munpabanmember
{
	eMu_MBMerr, eMu_MBMtargetid, eMu_MBMmunpaid, eMu_MBMblast, eMu_MBMtargetname, eMu_MBMfame, 
};
enum Munpadenymember
{
	eMu_MDMerr,  eMu_MDMtargetid, eMu_MDMmunpaid, eMu_MDMbLast, eMu_MDMtargetname,  
};
enum Munpaacceptmember
{
	eMu_MACerr, eMu_MACmunpaid, eMu_MACtargetid, eMu_MACblast, eMu_MACtargetname, eMu_MACMunpaname
};
enum MunpaBattleInfo
{
	eMu_MMunpaName, eMu_MMasterName, eMu_MMunpaFamous, eMu_MMemberNum, eMu_MCreateDate,
	eMu_MFBattleTotal, eMu_MBBattleTotal, eMu_MCBattleTotal, eMu_MTBattleTotal,
	eMu_MFBattleWin, eMu_MBBattleWin, eMu_MCBattleWin, eMu_MTBattleWin, 
	eMu_MFBattleLoss, eMu_MBBattleLoss, eMu_MCBattleLoss, eMu_MTBattleLoss
};
enum Munpajoinmemberinfo
{
	eMu_MJMTotalNum, eMu_MJMMunpaid, eMu_MJMCharacterID, 
	eMu_MJMCharacterName, eMu_MJMCharacterLevel, eMu_MJMCharacterFame, 
	eMu_MJMRequestJoinDay
};
enum Munpaboardinfo
{
	eMu_MCurBoardNum, eMu_MBoardsIdx, eMu_MBoardsName, eMu_MTotalNum,
};

enum Munpaitem
{
	eMu_IMunpaID, eMu_IDBIDX, eMu_IIDX, eMu_IPosition, eMu_IDurability, eMu_IRareIdx,
};

enum Munpacreateboardinfo
{
	eMu_MBoardidx, eMu_MRSubjectRank, eMu_MRContentRank, eMu_MWContentRank, eMu_MDContentRank, eMu_MBoardName
};
enum Munpaboardlistinfo
{
	eMu_MBLITotalPage, eMu_MBLIContentIDx, eMu_MBLIContentNum, eMu_MBLISubject, eMu_MBLIWriter, eMu_MBLICount, eMu_MBLIRegDate, eMu_MBLIRegTime, eMu_MBLIDepth
};
enum Munpaboardcontentsinfo
{
	eMu_MBCErr, eMu_MBCIContent, eMu_MBCIContentIDx, eMu_MBCIContentNum, eMu_MBCISubject, eMu_MBCIWriter, eMu_MBCICount, eMu_MBCIRegDate, eMu_MBCDCRank, eMu_MBCWCRank
};
enum Munpamoney
{
	eMu_MWMunpaID, eMu_MWMunpaMoney, eMu_MWPutInOut,
};

enum Pyogukbuy
{
	ePyo_errbuy, ePyo_Kind
};

enum PyogukInfo
{
	ePyo_PyogukNum, ePyo_Money,
};

enum PyogukItemInfo
{
	ePI_DBIdx, ePI_IconIdx, ePI_Positon, ePI_QPosition, ePI_Durability, ePI_RareIdx,
};

enum Pyogukitem
{
	ePyo_Userid, ePyo_MaxPosition, ePyo_Durability, ePyo_Idx, ePyo_DBIdx, ePyo_Position, 
};

enum ePetInvenItemInfo
{
	//ePIII_DBIdx, ePIII_IconIdx, ePIII_Position, ePIII_Durability, ePIII_RareIdx,
	ePIII_ObjectID = 0, ePIII_DBIDX, ePIII_IDX, ePIII_Position, ePIII_QPosition, ePIII_Durability, ePIII_MunpaIdx, ePIII_PyogukIdx, ePIII_ShopIdx, ePIII_Param, ePIII_RareIdx,

};

enum Friendlogout
{
	eFr_LToPlayerID, eFr_LLogoutName,
};
enum Friendnotelist
{
	eFr_NLFromName, eFr_NLNote, eFr_NLSendDate, eFr_NLSendTime, eFr_NLNoteID
};
enum Friendlist
{
	eFr_FLFriendid, eFr_FLFriendname, eFr_FLIsLogin,
};
enum Famemunpaupdate
{
	eFa_MunpaIDX, eFa_Fameval,
};
enum Wantedload
{
	eWa_LWantedIDX, eWa_LPlayerID, eWa_LPlayerName, eWa_LTotlaPrize, eWa_LRegistChridx, eWa_LTotalVolunteerNum,
};
enum Wantedregist
{
	eWa_RWanted_idx, eWa_RWantedChr_idx, eWa_RWanted_name, eWa_RTotalPrize, 
};
enum Wantedcomplete
{
	eWa_CCharacteridx, eWa_CCompleterName, eWa_CCompleteType,
};
enum Wantedorderlist
{
	eWa_OLTotal, eWa_OLWantedIDX, eWa_OLWantedChrName, eWa_OLPrize, eWa_OLVolunteerNum,
};
enum Wantedlogininfo
{
	eWa_LIMyWantedIDX, eWa_LIWantedIDX, eWa_LIWantedChrID, eWa_LIWantedName, eWa_LIRegistDate, 
};
enum QuestInfo
{
	eQi_QuestIdx, eQi_QuestState, eQi_EndParam, eQi_RegistTime, eQi_CheckType, eQi_CheckTime,
};
enum QuestSubDataInfo
{
	eQsi_QuestIdx, eQsi_SubQuestIdx, eQsi_Data, eQsi_RegistTime,
};
enum QuestItem
{
	eQit_ItemIdx, eQit_ItemCount, eQit_QuestIdx,
};
enum Journal
{
	eJnl_Index, eJnl_Kind, eJnl_Param, eJnl_Param_2, eJnl_Param_3, eJnl_bSaved, eJnl_ParamName, eJnl_RegDate,
};
enum Guildload
{
	eGu_LGIdx, eGu_LGMasterIdx, eGu_LGMasterName, eGu_LGName, eGu_LGLevel, eGu_LGLocation, eGu_LGUnionIdx, eGu_LGMoney, eGu_LGMarkName,
};
enum Guildloadmember
{
	eGu_LMIdx, eGu_LMName, eGu_LMLevel, eGu_LMGuildIdx, eGu_LMRank, //호칭 추가
};
enum Guilcreate
{
	eGu_CIsSuccess, eGu_CGuildIdx, eGu_CName, eGu_CMapNum, eGu_CMasterName, eGu_CMasterlvl,
};
enum ShopItem
{
	eMItm_ItemDBIdx, eMItm_ItemIdx, eMItm_Position, eMItm_Durability, eMItm_Param,
};
enum ShopItemUseInfo
{
	eUMtm_ItemIdx, eUMtm_DBIdx, eUMtm_Param, eUMtm_BTime, eUMtm_RTime, eUMtm_Position,
};
enum MovePointInfo
{
	eMPt_DBIdx, eMPt_Name, eMPt_MapNum, eMPt_MapPoint,
};
enum AvatarItemOption
{
	eAO_OptionIdx, 
};
enum eGTInfo
{
	eGT_GuildIdx, eGT_Position, eGT_Ranking, eGT_Process, eGT_WaitTime,
};
enum eSW_Info
{
	eSWI_SiegeWarIdx, eSWI_RegistTime, eSWI_SiegeWarTime, eSWI_SiegeWarLevel,
};
enum eSW_GuildInfo
{
	eSWGI_GuildDBIdx, eSWGI_GuildIdx, eSWGI_GuildType,
};

enum eGuildNotice
{
	eGu_GNDBIdx, eGu_GNGuildIdx, eGu_GNGuildNotice,
};

enum eMapDropItem	// 맵 드랍 아이템 DB관련 추가 by Stiner(2008/06/05)-MapDropItem
{
	eMdi_MapNum, eMdi_Channel, eMdi_ItemIDX, eMdi_DropCount, eMdi_MaxDropCount
};

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

// DB·I Query¹®; ³?¸±¶§ ¾²´A CO¼oμe dAC
void TestQuery();
void CharacterNumSendAndCharacterInfo(DWORD CharacterIDX, DWORD Protocol);
void CharacterNumSendAndCharacterInfo2(DWORD CharacterIDX, DWORD dwTeam);
void CharacterMugongInfo(DWORD CharacterIDX, DWORD Protocol);
void CharacterItemSlotInfo(DWORD CharacterIDX);
void CharacterItemInfo(DWORD CharacterIDX, DWORD Protocol);
void CharacterItemRareOptionInfo(DWORD CharacterIDX, DWORD Protocol);
void CharacterItemOptionInfo(DWORD CharacterIDX, DWORD Protocol);
void PyogukItemRareOptionInfo(DWORD CharacterIDX, DWORD UserID, DWORD StartDBIdx);
void PyogukItemOptionInfo(DWORD CharacterIDX, DWORD UserID, DWORD StartDBIdx);
void CharacterAbilityInfo(DWORD CharacterIDX, DWORD Protocol);
//SW051129 Pet
void CharacterPetInfo(DWORD CharacterIDX, DWORD UserID);
void CharacterTitanInfo(DWORD CharacterIDX, DWORD UserID);
void CharacterTitanEquipItemEnduranceInfo(DWORD CharacterIDX);
void PetInvenItemOptionInfo(DWORD CharacterIDX, DWORD UserID, WORD StartPos, WORD EndPos);
void PetInvenItemRareOptionInfo(DWORD CharacterIDX, DWORD UserID, WORD StartPos, WORD EndPos);
void PetInvenItemInfo(DWORD CharacterIDX, WORD StartPos, WORD EndPos);
void PetWearItemInfo( DWORD CharacterIdx, POSTYPE FromPos, POSTYPE ToPos );

void CharacterItemOptionDelete(DWORD OptionIdx, DWORD dwItemDBIdx);
//SW050920 Rare
void CharacterItemRareOptionDelete(DWORD RareIdx, DWORD dwItemDBIdx);

//void CharacterGMControlInfo(DWORD CharacterIDX, DWORD Protocol);
		//TDN
//void CharacterGMControlInsert(DWORD GMChrIdx,DWORD CharacterIDX, DWORD UserIdx, int kind, );
//void CharacterGMControlDelete(DWORD GMChrIdx,DWORD ControlIdx);

void AbilityLevelupLog(CPlayer* pPlayer,WORD AbilityIdx,LEVELTYPE AbilityLevel,EXPTYPE FromExp,EXPTYPE ToExp);

void MapBaseEconomy(BYTE MapNum);
void AuctionRegist(BYTE ItemIdx, DWORD SellerID, DWORD EndDate, DWORD EndTime, DWORD StartPrice, DWORD ImmediatePrice, char Memo[256]);
void AuctionPageQuery(DWORD CharacterIDX, BYTE PageNum);

void SSItemInsert(DWORD CharacterIdx, WORD wItemIdx, DURTYPE Durability, POSTYPE bPosition, DWORD FromChrIdx, WORD bSeal=0);


/* New Item Qurey Func */
void ItemUpdateToDB(DWORD CharacterIdx, DWORD dwDBIdx, WORD wItemIdx, DURTYPE Durability, POSTYPE bPosition, WORD qPosition, DWORD RareIdx=0);
//void ItemPyogukMunpaUpdateToDB(DWORD UserIDX, DWORD MunpaIDX, DWORD dwDBIdx, WORD wItemIdx, DURTYPE Durability, POSTYPE bPosition, WORD qPosition);
void ItemCombineUpdateToDB(DWORD dwfromDBIdx, DURTYPE fromDur, DWORD dwToDBIdx, DURTYPE toDur);
void ItemMoveUpdateToDB(DWORD CharacterIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos);
void ItemMovePyogukUpdateToDB(DWORD CharacterIDX, DWORD UserIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos);
void ItemMoveGuildUpdateToDB(DWORD CharacterIDX, DWORD MunpaIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos);
void ItemMovePetInvenUpdateToDB(DWORD CharacterIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos);
//SW050920 RareIdx 추가
void ItemInsertToDB(DWORD CharacterIdx, WORD wItemIdx, DURTYPE Durability, POSTYPE bPosition, DWORD dwKey, WORD bSeal=0);
void ItemDeleteToDB(DWORD dwDBIdx);
void ItemOptionInsertToDB(DWORD CharacterIdx, WORD wItemIdx, POSTYPE Pos, ITEM_OPTION_INFO * pOptionInfo);
//SW050920
void ItemRareInsertToDB(DWORD CharacterIdx, WORD wItemIdx, /*DURTYPE Durability,*/ POSTYPE bPosition, DWORD dwKey, ITEM_RARE_OPTION_INFO* pRareOptionInfo );

//
void ShopItemRareInsertToDB(DWORD CharacterIdx, DWORD ItemIdx, DWORD ItemPos, DWORD ItemDBIdx, ITEM_RARE_OPTION_INFO* pRareOptionInfo);
void RShopItemRareInsertToDB(LPQUERY pData,  LPDBMESSAGE pMessage);

//SW051129
void PetInsertToDB(DWORD UserIdx, DWORD CharacterIdx, DWORD dwItemIdx, PET_TOTALINFO* pPetTotalInfo);
void PetUpdateToDB(DWORD UserIdx, const PET_TOTALINFO* pPetTotalInfo);
void PetDeleteToDB(DWORD dwPetDBIdx);

//SW070207
void TitanInsertToDB(DWORD UserIdx, DWORD CharacterIdx, DWORD dwItemIdx, TITAN_TOTALINFO* pTitanTotalInfo);
void RTitanInsertToDB(LPQUERY pData, LPDBMESSAGE pMessage);
void TitanUpdateToDB(DWORD MasterIdx, const TITAN_TOTALINFO* pTitanTotalInfo);
void TitanDeleteToDB(DWORD dwCallItemDBIdx);

void TitanEquipItemInfoInsertToDB(TITAN_ENDURANCE_ITEMINFO* pInfo);
void RTitanEquipItemInfoInsertToDB(LPQUERY pData, LPDBMESSAGE pMessage);
void TitanEquipItemInfoUpdateToDB(TITAN_ENDURANCE_ITEMINFO* pInfo);
void TitanEquipItemInfoDeleteToDB(DWORD dwTitanEquipDBIdx);

// magi82(33)
void TitanEquipInsertExceptionToDB(TITAN_ENDURANCE_ITEMINFO* pInfo);
void RTitanEquipInsertExceptionToDB(LPQUERY pData, LPDBMESSAGE pMessage);
void TitanEquipInsertExceptionPyogukToDB(DWORD CharacterIdx, TITAN_ENDURANCE_ITEMINFO* pInfo);
void RTitanEquipInsertExceptionPyogukToDB(LPQUERY pData, LPDBMESSAGE pMessage);

/* New Mugong Qurey Func */
void MugongUpdateToDB(MUGONGBASE* msg, char* type);
void MugongMoveUpdateToDB(DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos, char* type);
void MugongInsertToDB(DWORD CharacterIDX, WORD MugongIDX, POSTYPE MugongPos, BYTE bWeared, BYTE bSung, WORD Option = 0);
void MugongDeleteToDB(DWORD dwDBIdx);


//pjs ¿ⓒ±a¿¡´A ACUTION¿¡ ±­·AμC Ai¶￠i¾¹® ¹YE? A³¸® CO¼o ¼±¾ð
void CheckAuction(MSGBASE* msg);
void AuctionSearch(SEARCHLIST* msg);
void AuctionSort(SORTLIST* sort);
void AuctionRegister(REGISTERAUCTION* msg);
void AuctionJoin(JOINAUCTION* msg);
void AuctionCancel(AUCTIONCANCEL *msg);
//////////////////////////////////////////////////////////////////////////
// ÆAÆ¼ °u·A DBAo¸®
void PartyLoad(DWORD PartyIDX);
void PartyInfoByUserLogin(DWORD PartyIDX, DWORD CharacterIDX);
void PartyBreakup(DWORD PartyIDX);
//2008. 5. 21. CBH - 방파 생성 옵션 추가 관련 함수 수정
//void PartyCreate(DWORD MasterIDX, BYTE Option);
void PartyCreate(DWORD MasterIDX, PARTY_ADDOPTION* pAddOption);
///////////////////////////////////////////////////////////////
void PartyMemberUpdate(CParty* pParty, DWORD PartyIDx);
void PartyAddMember(DWORD PartyIDX, DWORD TargetID);
void PartyDelMember(DWORD PartyIDX, DWORD TargetID);
void PartyChangeMaster(DWORD PartyID, DWORD PlayerID, DWORD TargetPlayerID);

//////////////////////////////////////////////////////////////////////////
// MapChange
void ChangeMapGetCharacterInfo(DWORD CharacterIDX, DWORD dwConnectionIndex);
void ChangeMapGetCharacterMugongInfo(DWORD CharacterIDX, DWORD dwConnectionIndex);
//////////////////////////////////////////////////////////////////////////
//¹®ÆA °u·A DB Ao¸®
/*
void MunpaLoad(DWORD MapNum, DWORD StartMunpaID);
void MunpaItemLoad(DWORD MapNum, DWORD StartItemDBIDx);
void MunpaItemOption(DWORD MapNum, DWORD StartDBIDx);
void MunpaLoadSyn(DWORD PlayerID, MAPTYPE MapNum, DWORD GotoPage, DWORD PageUnit, char* OrderType);
void MunpaSearchMunpa(DWORD PlayerID, BYTE MapNum, char* Key, DWORD GotoPage);
void MunpaCreate(MUNPA_CREATE_INFO* pmsg, char* MasterName);
void MunpaBreakUp(DWORD PlayerID, DWORD MunpaID);
void MunpaBreakUpMembers(DWORD StartPlayerID, DWORD MunpaID);
void MunapBreakupClr(DWORD MunpaID);
void MunpaChangeMaster(DWORD FromPlayerID, DWORD ToPlayerID);
void MunpaChangeMemberRank(DWORD MasterID, DWORD PlayerID, int Position, DWORD MunpaID);
void MunpaBanMember(DWORD MasterID, DWORD TargetID, DWORD MunpaID, BOOL bLast);
void MunpaSecede(DWORD PlayerID, DWORD MunpaID);
void MunpaAcceptMember(DWORD MasterID, DWORD TargetID, DWORD MunpaID, BOOL bLast);
void MunpaDenyMember(DWORD MasterID, DWORD TargetID, DWORD MunpaID, BOOL bLast);
void MunpaJoinMember(DWORD PlayerID, DWORD TargetMunpaID);
void MunpaJoinMemberCancel(DWORD PlayerID, DWORD MunpaIDX);
void MunpaHomeInfo(DWORD PlayerID, DWORD MunpaID);
void MunpaRegularMembersInfo(DWORD PlayerID, DWORD MunpaID, DWORD GotoPage);
void MunpaJoinMembersInfo(DWORD PlayerID, DWORD MunpaID, DWORD GotoPage);
void MunpaBoardInfo(DWORD PlayerID, DWORD MunpaID);
void MunpaCreateBoard(SEND_MUNPA_BOARD_RANK_INFO* msg);
void MunpaDeleteBoard(DWORD MunpaID, DWORD BoardID, DWORD MasterID);
void MunpaBoardRankInfo(DWORD MasterID, DWORD MunpaID, DWORD BoardID);
void MunpaBoardRankInfoUpdate(DWORD CharacterIDX, DWORD MunpaID, SEND_MUNPA_BOARD_RANK_INFO* msg);
void MunpaBoardBackContent(DWORD PlayerID, DWORD BoardIdx, DWORD StartContentID, DWORD MyMunpaRank);
void MunpaBoardFrontContent(DWORD PlayerID, DWORD BoardIdx, DWORD StartContentID, DWORD MyMunpaRank);
void MunpaBoardCurrentContent(DWORD PlayerID, DWORD BoardIdx, DWORD StartContentID, DWORD MyMunpaRank);
void MunpaBoardWrite(DWORD PlayerID, DWORD BoardIDX, char* Subject, char* Content, DWORD RootContentsID, DWORD MyMunpaRank);
void MunpaBoardModify(DWORD PlayerID, DWORD BoardIDX, char* Subject, char* Content, DWORD OriginalContentsID);
void MunpaBoardSubjectList(DWORD PlayerID, DWORD BoardID, DWORD CurPage, DWORD PageUnit);
void MunpaBoardDeleteContents(DWORD PlayerID, DWORD ContentIDx, DWORD MyRank);
void MunpaMoneyPutInOut(DWORD PlayerID, DWORD TargetMunpaID, DWORD Money, BOOL bPutIn); //1: putin, 0: putout
void MunpaModifyIntro(DWORD CharacterIDX, DWORD MunpaID, char* pIntroMsg);*/

//////////////////////////////////////////////////////////////////////////
//guild
void GuildLoadGuild(DWORD StartGuildIdx);
void GuildLoadMember(DWORD StartCharacterIdx);
void GuildCreate(DWORD CharacterIdx, char* CharacterName, LEVELTYPE CharacterLvl, char* GuildName, char* Intro);
void GuildBreakUp(DWORD GuildIdx, DWORD MasterIdx);
void GuildDeleteMember(DWORD GuildIdx, DWORD MemberIDX);
void GuildSecedeMember(DWORD GuildIdx, DWORD PlayerIdx);
void GuildAddMember(DWORD GuildIdx, DWORD MemberIDX, char* GuildName, BYTE Rank);
void GuildMarkRegist(DWORD GuildIdx, char* ImgData, DWORD CharacterIdx);
void GuildLevelUp(DWORD GuildIdx, BYTE Level);
void GuildChangeMemberRank(DWORD GuildIdx, DWORD MemberIdx, BYTE Rank);
void GuildItemLoad(DWORD MapNum, DWORD StartItemDBIDx);
void GuildItemOption(DWORD MapNum, DWORD StartDBIdx);
void GuildItemRareOption(DWORD MapNum, DWORD StartDBIdx);
void GuildMoneyUpdate(DWORD GuildIdx, MONEYTYPE Money);
void GuildGiveMemberNickName(DWORD TargetId, char * NickName);
//SW060526 문파창고 아이템 정보저장 시점 변경
void GuildItemLoadInNeed(DWORD MapNum, DWORD GuildIdx);
void GuildItemOptionInNeed(DWORD MapNum, DWORD GuildIdx,DWORD StartDBIdx);
void GuildItemRareOptionInNeed(DWORD MapNum, DWORD GuildIdx, DWORD StartDBIdx);
void RGuildItemLoadInNeed( LPQUERY pData, LPDBMESSAGE pMessage );
void RGuildItemOptionInNeed(LPQUERY pData, LPDBMESSAGE pMessage);
void RGuildItemRareOptionInNeed(LPQUERY pData, LPDBMESSAGE pMessage);
// 06. 03. 문파공지 - 이영준
void GuildLoadNotice(DWORD StartDBIdx);
void GuildUpdateNotice(DWORD GuildIdx, char* Notice);

//SW060719 문파포인트
//void GuildHuntedMonsterCountLastDBUpdateDate(WORD MapNum);	//맵 서버 부팅시 최종 정산일을 가져와 m_wCurDateCheckForConverCount 세팅한다.
//void RGuildHuntedMonsterCountLastDBUpdateDate();	
void GuildLoadGuildPointInfo(DWORD startGuildDBIdx);	//정팀장님지시 문파DB인덱스
void GuildLoadGuildPlustimeInfo(DWORD startDBIdx);
void RGuildLoadGuildPointInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RGuildLoadGuildPlustimeInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void GuildAddHuntedMonsterCount(DWORD GuildIdx, DWORD MonsterCount);	//10분 마다 정보 갱신
void GuildGetHuntedMonsterTotalCountWithInit(DWORD GuildIdx);	//매일 자정 문파포인트로 정산
void RGuildAddHuntedMonsterCount(LPQUERY pData, LPDBMESSAGE pMessage);
void RGuildGetHuntedMonsterTotalCountWithInit(LPQUERY pData, LPDBMESSAGE pMessage);

void GuildAddGuildPoint(DWORD GuildIdx, DWORD rAddPoint, DWORD rAddKind, DWORD rAdditionalData);	//문파 포인트 추가 r# 그대로 리턴받아 쓸 변수
void RGuildAddGuildPoint(LPQUERY pData, LPDBMESSAGE pMessage);
void GuildUseGuildPoint(DWORD GuildIdx, DWORD rUsePoint, DWORD rUseKind, DWORD rKindIdx, DWORD rEndtime);
void RGuildUseGuildPoint(LPQUERY pData, LPDBMESSAGE pMessage);
void GuildPlustimeEnd(DWORD GuildIdx, DWORD rPlusTimeIdx);
void RGuildPlustimeEnd(LPQUERY pData, LPDBMESSAGE pMessage);

void LogGuildPoint(DWORD GuildIdx, DWORD TotalGuildPoint, BOOL bAdd, int eKind, DWORD GuildPoint, DWORD AddData, DWORD CharacterIdx);
//////////////////////////////////////////////////////////////////////////
//C￥±¹
void PyogukBuyPyoguk(DWORD PlayerIDX);


/* C￥±¹ dº¸ Qurey ¼od */
void CharacterPyogukInfo(DWORD UserIdx, DWORD CharacterIdx);
void CharacterPyogukItemInfo(DWORD CharacterIdx, DWORD StartDBIdx);
void PyogukMoneyUpdateToDB(DWORD UserIdx, MONEYTYPE PyogukMoney);


//////////////////////////////////////////////////////////////////////////
//A￡±¸
void FriendNotifyLogouttoClient(DWORD PlayerID);
//////////////////////////////////////////////////////////////////////////
//¸i¼º
//void FameMunpaUpdate(DWORD StartMunpaIDX, DWORD MapNum);
void FameCharacterUpdate(DWORD PlayerIDX, FAMETYPE val);
void BadFameCharacterUpdate(DWORD PlayerIDX, FAMETYPE val);
void PKCharacterUpdate(DWORD PlayerIDX, DWORD val);
//////////////////////////////////////////////////////////////////////////
// SavePoint
void SavePointUpdate(DWORD CharacterIDX, WORD LoginPoint_Idx, WORD MapNum);
void SaveMapChangePointUpdate(DWORD CharacterIDX, WORD MapChangePoint_Idx);
 //맵 이동시 너무 느려 플레이어가 다른 맵에 들어가기 전에 업데이트 확인 후 맵 채인지 프로토콜을 보냄
void SaveMapChangePointReturn(DWORD CharacterIDX, WORD MapChangePoint_Idx, DWORD dwConnectionIdx, LEVELTYPE lvl, EXPTYPE ExpPoint, MONEYTYPE PyogukMoney);
//////////////////////////////////////////////////////////////////////////
//Co≫o±Y
void WantedLoad(WANTEDTYPE StartWantedIDX);
void WantedInfoByUserLogIn(DWORD CharacterIDX);
void WantedBuyRight(DWORD CharacterIDX, WANTEDTYPE WantedIDX, MONEYTYPE AddPrize);
void WantedRegist(DWORD CharacterIDX, DWORD TargetIDX, MONEYTYPE Prize);
void WantedGiveUpRight(DWORD CharacterIDX, DWORD WantedIDX);
void WantedComplete(DWORD CharacterIDX, char* TargetName, WANTEDTYPE WantedIDX);
void WantedDestroyed(DWORD WantedIDX); // destroyed by time
void WantedOrderedList(DWORD CharacterIDX, DWORD Page, char* OrderType, BYTE PageUnit);
void WantedMyList(DWORD CharacterIDX, DWORD Page, BYTE PageUnit);
void WantedSearch(DWORD CharacterIDX, char* WantedName, DWORD Page, BYTE PageUnit);
//////////////////////////////////////////////////////////////////////////
// Update¹®
//void CharacterMugongUpdate(MUGONGBASE* msg);
//void CharacterMugongMoveUpdate(DWORD toDBIdx, POSTYPE toPos, DWORD fromDBIdx, POSTYPE fromPos);
//void CharacterInfoUpdate(CPlayer* pPlayer, DWORD dwConnectionIndex);
void CharacterHeroInfoUpdate(CPlayer* pPlayer);
void CharacterTotalInfoUpdate(CPlayer* pPlayer);
void CharacterBadFameUpdate(DWORD PlayerID, int BadFame, DWORD Money);
//void CharacterQuickInfoUpdate(CPlayer * pPlayer);
void CharacterMugongInfoUpdate(CPlayer* pPlayer);
void CharacterTablePartyIdxUpdate(DWORD PlayerID);
void PyogukMoneyUpdate(DWORD PlayerID, BYTE PyogukNum, DWORD Money);
void RegistLoginMapInfo(DWORD CharacterIDX, char* CharacterName, BYTE MapNum, WORD MapPort);
void UnRegistLoginMapInfo(DWORD CharacterIDX);
void CharacterLvlExpUpdate(DWORD PlayerID, LEVELTYPE level, EXPTYPE exppoint);
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Au½ºÆ® (AEº¸AU °¡AIμa Æ÷CO)
void QuestTotalInfo(DWORD PlayerID);
void QuestUpdateToDB(DWORD PlayerID, DWORD QuestIdx, QSTATETYPE State, BYTE bEnd);
//RaMa
void QuestItemload(DWORD PlayerID, int QuestIdx = -1 );
void QuestItemInsert(DWORD PlayerID, DWORD dwQuestIdx, DWORD ItemIdx, DWORD dwItemNum );
void QuestItemDelete(DWORD PlayerID, DWORD ItemIdx);
void QuestItemDeleteAll(DWORD PlayerID, DWORD dwQuestIdx);
void QuestItemUpdate(DWORD PlayerID, DWORD dwQuestIdx, DWORD ItemIdx, DWORD ItemNum);

void QuestMainQuestLoad(DWORD PlayerID, int QuestIdx = -1 );
void QuestMainQuestUpdateToDB(DWORD PlayerID, DWORD mQuestIdx, QSTATETYPE State, DWORD EndParam, DWORD Time);
void QuestMainQuestDelete(DWORD PlayerID, DWORD mQuestIdx);
void QuestMainQuestInsert(DWORD PlayerID, DWORD mQuestIdx, QSTATETYPE State, DWORD Time);

void QuestSubQuestLoad(DWORD PlayerID, int QuestIdx = -1 );
void QuestSubQuestInsert(DWORD PlayerID, DWORD mQuestIdx, DWORD sQuestIdx, QSTATETYPE Data, QSTATETYPE Time);
void QuestSubQuestUpdateToDB(DWORD PlayerID, DWORD mQuestIdx, DWORD sQuestIdx, QSTATETYPE Data, QSTATETYPE Time);
void QuestSubQuestDelete(DWORD PlayerID, DWORD mQuestIdx, DWORD sQuestIdx);
void QuestSubQuestDeleteAll(DWORD PlayerID, DWORD mQuestIdx);

void QuestMainQuestUpdateCheckTime(DWORD dwCharIdx, DWORD dwQuestIdx, DWORD dwCheckType, DWORD dwCheckTime );

void Quest_EndQuest_New( DWORD PlayerID, DWORD mQuestIdx, QSTATETYPE State, DWORD EndParam, DWORD Time );
void Quest_DeleteQuest_New( DWORD PlayerID, DWORD mQuestIdx );
void Quest_EndSubQuest_New( DWORD PlayerID, DWORD mQuestIdx, DWORD sQuestIdx, QSTATETYPE Data, QSTATETYPE Time );

//////////////////////////////////////////////////////////////////////////
//journal
void JournalGetList(DWORD CharacterIDX);
void JouranlInsertQuest(DWORD CharacterIDX, BYTE Kind, DWORD QuestIDX, DWORD SubQuestIDX, int bCompleted);
void JouranlInsertWanted(DWORD CharacterIDX, BYTE Kind, DWORD Param, char* ParamName);
void JournalInsertLevel(DWORD CharacterIDX, BYTE Kind, DWORD Level);
void JournalSaveUpdate(DWORD CharacterIDX, DWORD JournalIndex);
void JournalDelete(DWORD CharacterIDX, DWORD JournalIndex);
//////////////////////////////////////////////////////////////////////////
// ShopItem
void ShopItemInvenInfo( DWORD CharacterIdx );
void CharacterShopItemInfo(DWORD CharacterIDX, DWORD StartDBIdx);
void UsingShopItemInfo( DWORD CharacterIdx );
void SavedMovePointInfo( DWORD CharacterIdx );
void SavedMovePointInsert( DWORD CharacterIdx, char* Name, DWORD MapIdx, DWORD Position );
void SavedMovePointUpdate( DWORD CharacterIdx, DWORD DBIdx, char* Name );
void SavedMovePointDelete( DWORD CharacterIdx, DWORD DBIdx );
void UsingShopItemUpdateToDB( DWORD UserIdx, DWORD CharacterIdx, DWORD dwItemIdx, DWORD dwDBIdx, DWORD Param, DWORD StartTime, DWORD RemainTime );
void ShopItemDeleteToDB( DWORD CharacterIdx, DWORD dwDBIdx );
void ShopItemUpdatetimeToDB( DWORD CharacterIdx, DWORD dwItemIdx, DWORD RemainTime );
void ShopItemParamUpdateToDB( DWORD CharacterIdx, DWORD dwItemIdx, DWORD Param );
void ShopItemUseParamUpdateToDB( DWORD UserIdx, DWORD CharacterIdx, DWORD dwItemIdx, DWORD Param );
void ShopItemGetItem( DWORD CharacterIdx, WORD wItemIdx, DWORD Dur );
void ItemMoveShopUpdateToDB(DWORD CharacterIDX, DWORD UserIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos);
void ShopItemUpdateCharacterIdx( DWORD CharacterIdx, DWORD dwDBIdx );
void ShopItemUpdateUseInfoToDB( DWORD CharacterIdx, DWORD dwDBIdx, DWORD Param, DWORD RemainTime );
void CharacterChangeName( DWORD CharacterIdx, char* Name, DWORD DBIdx );
void CharacterChangeInfoToDB( DWORD CharacterIdx, DWORD Gender, DWORD HairType, DWORD FaceType, float Height, float Width );
void IsUseAbleShopAvatarItem( DWORD CharacterIdx, DWORD ItemDBIdx, DWORD ItemIdx, DWORD ItemPos );
void IsAbleShopEquipItem( DWORD CharacterIdx, DWORD ItemDBIdx, DWORD FromItemIdx, DWORD FromPos, DWORD ToItemIdx, DWORD ToPos );
void ShopItemAllUseInfoUpdateToDB( DWORD ItemDBIdx, DWORD CharacterIdx, WORD ItemIdx, DWORD Param, DWORD Remaintime );



//////////////////////////////////////////////////////////////////////////
// Log °u·A E￡Aa CO¼o
void InsertLogCharacter( DWORD charIdx, LEVELTYPE level, HERO_TOTALINFO* pTotalInfo );
void InsertLogExp( BYTE bType, DWORD charIdx, LEVELTYPE level, DWORD changeValue, EXPTYPE NowExp, WORD MurdererKind, DWORD MurdererIdx, DWORD CurAbilPoint);
void InsertLogMoney( BYTE type, DWORD charIdx, MONEYTYPE InvenMoney, MONEYTYPE ChangeValue, DWORD TargetID, MONEYTYPE TargetMoney );
void InsertLogWorngMoney( BYTE type, DWORD charIdx, MONEYTYPE changeValue, MONEYTYPE InvenMoney, MONEYTYPE PyogukMoney, DWORD targetIdx );
void InsertLogMugong( WORD type, DWORD charIdx, DWORD mugongIdx, DWORD mugongDBIdx, LEVELTYPE mugongSung, EXPTYPE mugongExp );
void InsertLogMugongExp( DWORD charIdx, EXPTYPE changeValue, DWORD mugongIdx, DWORD mugongDBIdx, LEVELTYPE mugongSung, EXPTYPE mugongExp, POSTYPE mugongPos );
void LogItemMoney(DWORD FromChrIdx, char* FromChrName, DWORD ToChrIdx, char* ToChrName,
				WORD LogType, MONEYTYPE FromTotalMoney, MONEYTYPE ToTotalMoney, MONEYTYPE ChangeMoney,
				DWORD ItemIdx, DWORD ItemDBIdx, POSTYPE ItemFromPos, POSTYPE ItemToPos,
				DURTYPE ItemDur, EXPTYPE ExpPoint);
void LogItemOption(DWORD CharacterIdx, DWORD ItemDBIdx, ITEM_OPTION_INFO* pOptionInfo);
//SW050920 Rare
void LogItemRareOption(DWORD CharacterIdx, DWORD ItemDBidx, ITEM_RARE_OPTION_INFO* pRareOptionInfo);
void LogCheat(DWORD Character_Idx, BYTE LogType, MONEYTYPE Param, DWORD ItemIdx, DWORD ItemDBIdx);
void InsertLogTool( DWORD dwLogType, DWORD dwLogKind, DWORD dwOperIdx, char* sOperName, DWORD dwTargetIdx, char* sTargetName,
				    DWORD dwParam1, DWORD dwParam2, DWORD dwParam3, DWORD dwParam4 );
void LogGuild(DWORD CharacterIdx, DWORD GuildIdx, WORD LogType, DWORD Param1 = 0, DWORD Param2 = 0);
void LogPet(DWORD CharacterIdx, DWORD UserIdx, BYTE LogType, DWORD PetDBIdx, DWORD PetSummonItemDBIdx, DWORD PetGrade, DWORD PetStamina, DWORD PetFriendship, BYTE PetAlive);
void LogGMToolUse( DWORD CharacterIdx, DWORD GMLogtype, DWORD Logkind, DWORD Param1, DWORD Param2 );
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// ½A°￡º° ¾÷μ￥AIÆ® CO¼o
void UpdateCharacterInfoByTime(DWORD charIdx, EXPTYPE exp, DWORD abilityExp, MONEYTYPE money, DWORD time, DWORD LastPKEndTime );
void UpdateMugongExpByTime(DWORD charIdx, DWORD DBIdx, EXPTYPE exp);

void InsertSpeedHackCheck(DWORD charIdx, char* charName, DWORD Distance, DWORD skillDelay);
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// DB·IºIAI ¸Þ½AAo¸| ¹Þ¾Æ¼­ A³¸®CI´A CO¼oμe.
void ReceiveMessageFromDB(LPQUERY pData,  LPDBMESSAGE pMessage);
void RTestQuery(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterNumSendAndCharacterInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterInfoQueryToEventMap(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterMugongInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterItemSlotInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterItemInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterItemRareOptionInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterItemOptionInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterAbilityInfo(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage);
void RCharacterPetInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterTitanInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterTitanEquipItemEnduranceInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RPetInvenItemOptionInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RPetInvenItemRareOptionInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RPetInvenItemInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RPetWearItemInfo(LPQUERY pData, LPDBMESSAGE pMessage);

void RPetInsert__(DWORD CharacterIdx, PET_TOTALINFO* pPetTotalInfo);

//void RCharacterGMControlInfo(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage);

//void RCharacterGMControlInsert(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage);

void RMapBaseEconomy(LPQUERY pData, LPDBMESSAGE pMessage);
void RAuctionPageQuery(LPQUERY pData, LPDBMESSAGE pMessage);

/* Item return func */
void RCharacterItemGetBuyIDX(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterItemInsert(LPQUERY pData, LPDBMESSAGE pMessage);
void RSSItemInsert(LPQUERY pData, LPDBMESSAGE pMessage); // LBS 03.10.09
void RCharacterItemGetCheatIDX(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterItemGetDivideIDX(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterDivideNewItemIDX(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterItemGetHuntIDX(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterDupItemInsertIDX(LPQUERY pData, LPDBMESSAGE pMessage);
void RShopItemInvenInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void RUsingShopItemInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void RSavedMovePointInfo( LPMIDDLEQUERY pData, LPDBMESSAGE pMessage );
void RSavedMovePointInsert( LPMIDDLEQUERY pData, LPDBMESSAGE pMessage );
void RCharacterChangeName( LPQUERY pData, LPDBMESSAGE pMessage );
void RIsUseAbleShopAvatarItem( LPQUERY pData, LPDBMESSAGE pMessage );
void RIsAbleShopEquipItem( LPQUERY pData, LPDBMESSAGE pMessage );

/* New Item Return Func */
void RItemInsert(LPQUERY pData, LPDBMESSAGE pMessage);
void RItemOptionInsert(LPQUERY pData, LPDBMESSAGE pMessage);
void RItemRareInsert(LPQUERY pData, LPDBMESSAGE pMessage);

/* New Pet Return Func */
void RPetInsert(LPQUERY pData, LPDBMESSAGE pMessage);

/* New Mugong Return Func */
void RMugongInsert(LPQUERY pData, LPDBMESSAGE pMessage);

//pjs ¿ⓒ±a¿¡´A ACUTION¿¡ ±­·AμC Ai¶￠i¾¹® ¹YE? A³¸® CO¼o ¼±¾ð..
void RAuctionCheck(LPQUERY pData, LPDBMESSAGE pMessage);
void RAuctionPageList(LPQUERY pData, LPDBMESSAGE pMessage);
void RAuctionConfirm(LPQUERY pData, LPDBMESSAGE pMessage);
//////////////////////////////////////////////////////////////////////////
//ÆAÆ¼
void RPartyLoad(LPQUERY pData, LPDBMESSAGE pMessage);
//--- 2008. 5. 21. CBH - 방파 생성 추가 옵션 관련 처리 수정 -------
//void RPartyCreate(LPQUERY pData, LPDBMESSAGE pMessage);
//void RPartyInfoByUserLogin(LPQUERY pData, LPDBMESSAGE pMessage);
void RPartyCreate(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage);
void RPartyInfoByUserLogin(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage);
//-----------------------------------------------------------------
void RPartyChangeMaster(LPQUERY pData, LPDBMESSAGE pMessage);
void RPartyDelMember(LPQUERY pData, LPDBMESSAGE pMessage);
//////////////////////////////////////////////////////////////////////////

void RCharacterStatsHistory(LPQUERY pData, LPDBMESSAGE pMessage);
void RSavePointUpdate(LPQUERY pData, LPDBMESSAGE pMessage);

//////////////////////////////////////////////////////////////////////////
//¹®ÆA 
/*
void RMunpaLoad(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaItemLoad(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaItemOption(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaLoadSyn(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaCreate(LPLARGEQUERY pData, LPDBMESSAGE pMessage);
void RMunpaBreakUp(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaBreakUpMembers(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaChangeMemberRank(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaAcceptMember(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaBanMember(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaSecede(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaDenyMember(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaJoinMember(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaJoinMemberCancel(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaRegularMembersInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaJoinMembersInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaBoardInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaCreateBoard(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaBoardRankInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaBoardRankInfoUpdate(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaBoardFrontContent(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaBoardDeleteContents(LPQUERY pData, LPDBMESSAGE pMessage);
void RMunpaMoneyPutInOut(LPQUERY pData, LPDBMESSAGE pMessage);

//Middle Query
void RMunpaHomeInfo(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage);
void RMunpaBoardSubjectList(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage);

// Large Query
void RMunpaBoardReadContent(LPLARGEQUERY pData, LPDBMESSAGE pMessage);
void RMunpaBoardModify(LPLARGEQUERY pData, LPDBMESSAGE pMessage);
void RMunpaBoardWrite(LPLARGEQUERY pData, LPDBMESSAGE pMessage);
*/

//////////////////////////////////////////////////////////////////////////
//guild
void RGuildLoadGuild(LPQUERY pData, LPDBMESSAGE pMessage);
void RGuildLoadMember(LPQUERY pData, LPDBMESSAGE pMessage);
void RGuildLoadMark(LPLARGEQUERY pData, LPDBMESSAGE pMessage);
void RGuildCreate(LPQUERY pData, LPDBMESSAGE pMessage);
void RGuildBreakUp( LPQUERY pData, LPDBMESSAGE pMessage );
void RGuildDeleteMember( LPQUERY pData, LPDBMESSAGE pMessage );
void RGuildSecedeMember( LPQUERY pData, LPDBMESSAGE pMessage );
void RGuildItemLoad( LPQUERY pData, LPDBMESSAGE pMessage );
void RGuildItemOption(LPQUERY pData, LPDBMESSAGE pMessage);
void RGuildItemRareOption(LPQUERY pData, LPDBMESSAGE pMessage);
void RGuildMarkRegist(LPLARGEQUERY pData, LPDBMESSAGE pMessage);
void RGuildLoadNotice(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage);
//////////////////////////////////////////////////////////////////////////
//C￥±¹
void RCharacterPyogukInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterPyogukItemInfo(LPQUERY pData, LPDBMESSAGE pMessage);
//SW050920 Rare
void RPyogukItemRareOptionInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RPyogukItemOptionInfo(LPQUERY pData, LPDBMESSAGE pMessage);

//////////////////////////////////////////////////////////////////////////
//A￡±¸
void RFriendNotifyLogouttoClient(LPQUERY pData, LPDBMESSAGE pMessage);

//////////////////////////////////////////////////////////////////////////
//¸i¼º
//void RFameMunpaUpdate(LPQUERY pData, LPDBMESSAGE pMessage);

//////////////////////////////////////////////////////////////////////////
//Co≫o±Y
void RWantedLoad(LPQUERY pData, LPDBMESSAGE pMessage);
void RWantedInfoByUserLogIn(LPQUERY pData, LPDBMESSAGE pMessage);
void RWantedRegist(LPQUERY pData, LPDBMESSAGE pMessage);
void RWantedComplete(LPQUERY pData, LPDBMESSAGE pMessage);
void RWantedDestroyed(LPQUERY pData, LPDBMESSAGE pMessage);
void RWantedOrderedList(LPQUERY pData, LPDBMESSAGE pMessage);
void RWantedMyList(LPQUERY pData, LPDBMESSAGE pMessage);
void RWantedSearch(LPQUERY pData, LPDBMESSAGE pMessage);
//////////////////////////////////////////////////////////////////////////
// Au½ºÆ® (AEº¸AU °¡AIμa Æ÷CO)
void RQuestTotalInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RQuestMainQuestLoad(LPQUERY pData, LPDBMESSAGE pMessage);
void RQuestSubQuestLoad(LPQUERY pData, LPDBMESSAGE pMessage);
void RQuestItemLoad(LPQUERY pData, LPDBMESSAGE pMessage);
//////////////////////////////////////////////////////////////////////////
void RJournalGetList(LPQUERY pData, LPDBMESSAGE pMessage);
//////////////////////////////////////////////////////////////////////////
void RSaveMapChangePointReturn(LPQUERY pData, LPDBMESSAGE pMessage);
//////////////////////////////////////////////////////////////////////////
//
void RCharacterShopItemInfo(LPQUERY pData, LPDBMESSAGE pMessage);

//void UpdateCharacterMoney( DWORD dwCharIdx, MONEYTYPE money, BYTE flag );
//void RUpdateCharacterMoney( LPQUERY pData, LPDBMESSAGE pMessage );

void ConfirmUserOut( DWORD dwConnectIdx, DWORD dwUserIdx, DWORD dwCharIdx, DWORD dwKind );
void RConfirmUserOut( LPQUERY pData, LPDBMESSAGE pMessage );


void SaveCharInfoBeforeLogout(DWORD PlayerIdx, DWORD dwConnectionIndex, DWORD UserIdx, MONEYTYPE PyogukMoney);
void RSaveCharInfoBeforeLogout( LPQUERY pData, LPDBMESSAGE pMessage );

// guildfieldwar
void LoadGuildWarRecord( DWORD dwIdx );
void RLoadGuildWarRecord( LPQUERY pData, LPDBMESSAGE pMessage );
void DeleteGuildWarRecord( DWORD dwGuildIdx );
void LoadGuildFieldWar( DWORD dwIdx );
void RLoadGuildFieldWar( LPQUERY pData, LPDBMESSAGE pMessage );
void InsertGuildFieldWar( DWORD dwGuildIdx1, DWORD dwGuildIdx2, DWORD dwMoney );
void DeleteGuildFieldWar( DWORD dwGuildIdx1, DWORD dwGuildIdx2 );
void UpdateGuildFieldWarRecordToDB( DWORD dwGuildIdx, DWORD dwVitory, DWORD dwDraw, DWORD dwLoose );

// Guild Tournament
void GuildTournamentInfoLoad();
void GuildTournamentCancelIdxLoad( DWORD TournamentIdx );
void GuildTournamentCancelIdxInsert( DWORD TournamentIdx, DWORD GuildIdx );
void GuildTournamentGuildInfoInsert( DWORD GuildIdx, char* GuildName, DWORD Position );
void GuildTournamentGuildInfoDelete( DWORD GuildIdx );
void GuildTournamentGuildInfoUpdate( DWORD GuildIdx, DWORD Ranking, DWORD ProcessTournament );
void GuildTournamentInfoUpdate( DWORD TournamentIdx, DWORD TournamentFight, DWORD WaitTime );
void RGuildTournamentInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage );
void RGuildTournamentCancelIdxLoad( LPQUERY pData, LPDBMESSAGE pMessage );
void GuildTournamentAddLog( DWORD TournamentIdx, DWORD GuildIdx, DWORD Logkind, DWORD Ranking );

// Jackpot
//void JackpotLoadTotalMoney();
//void RJackpotLoadTotalMoney( LPQUERY pData, LPDBMESSAGE pMessage );
void JackpotAddTotalMoney( MONEYTYPE MapMoney );
//void RJackpotAddTotalMoney( LPQUERY pData, LPDBMESSAGE pMessage );
void JackpotPrizeInfo( MONEYTYPE MapMoney, DWORD PlayerID, DWORD PrizeKind, DWORD PrizePercentage, DWORD dwAbsMoney = 0);
void RJackpotPrizeInfo( LPQUERY pData, LPDBMESSAGE pMessage );

// guildunion
void GuildUnionLoad( DWORD dwStartGuildUnionIdx );
void RGuildUnionLoad( LPQUERY pData, LPDBMESSAGE pMessage );
void GuildUnionLoadMark( DWORD dwMarkIdx );
void RGuildUnionLoadMark( LPLARGEQUERY pData, LPDBMESSAGE pMessage );

void GuildUnionCreate( DWORD dwCharacterIdx, DWORD dwGuildIdx, char* sUnionName );
void RGuildUnionCreate( LPQUERY pData, LPDBMESSAGE pMessage );
void GuildUnionDestroy( DWORD dwGuildUnionIdx, DWORD dwMarkIdx, DWORD dwMasterIdx, DWORD dwTime );
void GuildUnionAddGuild( DWORD dwGuildUnionIdx, int nIndex, DWORD dwGuildIdx );
void GuildUnionRemoveGuild( DWORD dwGuildUnionIdx, int nIndex, DWORD dwGuildIdx );
void GuildUnionSecedeGuild( DWORD dwGuildUnionIdx, int nIndex, DWORD dwGuildIdx, DWORD dwTime );

void GuildUnionMarkRegist( DWORD dwCharacterIdx, DWORD dwMarkIdx, DWORD dwGuildUnionIdx, char* pImgData );
void RGuildUnionMarkRegist( LPLARGEQUERY pData, LPDBMESSAGE pMessage );

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
//┃SiegeWar													  ┃
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
void SiegeWarInfoLoad( DWORD MapNum );
void RSiegeWarInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage );
void SiegeWarInfoInsert( DWORD SiegeWarIdx, DWORD RegistTime, DWORD SiegeWarTime, DWORD Level, DWORD MapNum );
void SiegeWarInfoUpdate( DWORD SiegeWarIdx, DWORD RegistTime, DWORD SiegeWarTime, DWORD Level, DWORD MapNum );
void SiegeWarGuildInfoLoad( DWORD StartDBIdx, DWORD SiegeWarIdx, DWORD MapNum );
void RSiegeWarGuildInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage );
void SiegeWarGuildInsert( DWORD SiegeWarIdx, DWORD GuildIdx, DWORD Type, DWORD MapNum );
void SiegeWarGuildUpdate( DWORD SiegeWarIdx, DWORD GuildIdx, DWORD Type, DWORD MapNum );
void SiegeWarGuildDelete( DWORD SiegeWarIdx, DWORD GuildIdx, DWORD MapNum );
void SiegeWarAddLog( DWORD SiegeWarIdx, DWORD MapNum, DWORD GuildIdx, DWORD Logkind, DWORD Value );

// For JAPAN
void ChangeCharacterAttr( DWORD dwPlayerIdx, DWORD dwAttr );

// siegewarprofit
void SiegeWarProfitInfoLoad();
void RSiegeWarProfitInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage );
void SiegeWarProfitUpdateGuild( int nMapNum, DWORD dwGuildIdx );
void SiegeWarProfitUpdateTaxRate( int nMapNum, int nTaxRate, DWORD dwRegDate );
void SiegeWarProfitUpdateProfitMoney( int nMapNum, DWORD dwMoney, int nType = 0 );
void RSiegeWarProfitUpdateProfitMoney( LPQUERY pData, LPDBMESSAGE pMessage );

void ChangeGuildLocation( DWORD dwGuildIdx, WORD wMapNum );


void ChangeCharacterStageAbility( DWORD dwCharacterIdx, BYTE stage, CAbilityGroup* pGroup );

void QuestEventCheck( DWORD dwUserIdx, DWORD dwCharacterIdx, DWORD dwNpcIdx, DWORD dwQuestIdx, char* pTime );
void RQuestEventCheck( LPQUERY pData, LPDBMESSAGE pMessage );

void QuestEventEnd( DWORD dwUserIdx, DWORD dwCharacterIdx, char* pTime );

// event 060627 - 운영팀 이벤트
void GMEvent01( DWORD dwUserIdx, DWORD dwCharacterIdx, DWORD dwLevel, int nServerSetNum );

// magi82 //////////////////////////////////////////////////////////////////////////
void UCharacterExpFlag(DWORD CharacterIDX, BYTE ExpFlag);
////////////////////////////////////////////////////////////////////////////////////

//magi82 - 문하생 가입편리시스템 ///////////////////////////////////////////////////
void GuildTraineeInfo(DWORD StartDBIdx);
void RGuildTraineeInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void GuildTraineeInsert(DWORD GuildIdx, DWORD UserIdx, char UserName[], DWORD JoinTime);
void GuildTraineeDelete(DWORD UserIdx);

void GuildTraineeGuildInfo(DWORD StartDBIdx);
void RGuildTraineeGuildInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void GuildTraineeGuildInsert(DWORD GuildIdx);
void RGuildTraineeGuildInsert(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage);
void GuildTraineeGuildDelete(DWORD GuildIdx);

void GuildTraineeDeletebyGuild(DWORD GuildIdx);

void GuildAddStudent(DWORD GuildIdx, DWORD MemberIDX, char* GuildName, BYTE Rank);
void RGuildAddStudent(LPQUERY pData, LPDBMESSAGE pMessage);

void GuildTraineeGuildIntroInfo(DWORD StartDBIdx);
void RGuildTraineeGuildIntroInfo(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage);

void GuildTraineeGuildIntroUpdate(DWORD GuildIdx, char* GuildIntro);
////////////////////////////////////////////////////////////////////////////////////

//SW070103 문파포인트개선	//문하생 렙업에 따른 포인트 누적. 축하금
void GuildStudentLvUpCtUpdate(DWORD GuildIdx, DWORD AddPoint, DWORD ToPoint);	//AddPoint - 가산점, ToPoint - 비교점
void RGuildStudentLvUpCtUpdate(LPQUERY pData, LPDBMESSAGE pMessage);

void GuildStudentLvUpCtInit(DWORD GuildIdx, DWORD MasterIDX);
void RGuildStudentLvUpCtInit(LPQUERY pData, LPDBMESSAGE pMessage);

// magi82 - Titan(070209)
void TitanWearItemInfo( DWORD CharacterIdx, POSTYPE FromPos, POSTYPE ToPos );
void RTitanWearItemInfo(LPQUERY pData, LPDBMESSAGE pMessage);
//////////////////////////////////////////////////////////////////////////

void TestGameQuery( DWORD CharacterIdx );
void RTestGameQuery( LPQUERY pData, LPDBMESSAGE pMessage );
void TestLogQuery( DWORD CharacterIdx );
void RTestLogQuery( LPQUERY pData, LPDBMESSAGE pMessage );


void PyogukTitanEnduranceInfo(DWORD CharacterIdx, DWORD UserIdx, DWORD StartDBIdx);	// magi82 - Titan(071015) 창고에 타이탄 장비 관련
void RPyogukTitanEnduranceInfo( LPQUERY pData, LPDBMESSAGE pMessage );

//2007. 12. 10. CBH - 스킨 저장
void CharacterSkinInfoUpdate(CPlayer* pPlayer);
//2008. 1. 23. CBH - 스킨 로드
void CharacterSkinInfo(DWORD dwCharacterIndex);
void RCharacterSkinInfo(LPQUERY pData, LPDBMESSAGE pMessage);

// magi82(41) - 샵아이템 추가(스텟 초기화 주문서)
void CharacterUpdateResetStatusPoint(DWORD dwCharacterIDX, DWORD dwStatusPoint);

// itemlimit
void ItemLimitInfoLoadAll( DWORD dwStartDBIdx );
void RItemLimitInfoLoadAll( LPQUERY pData, LPDBMESSAGE pMessage );
void ItemLimitInfoUpdate( DWORD dwItemIdx, int nItemLimitCount, int nItemCurrentCount );
void RItemLimitInfoUpdate( LPQUERY pData, LPDBMESSAGE pMessage );

// autonote
void AutoNoteListLoad( DWORD dwCharacterIdx );
void RAutoNoteListLoad( LPQUERY pData, LPDBMESSAGE pMessage );	
void AutoNoteListAdd( DWORD dwCharacterIdx, DWORD dwAutoUserIdx, DWORD dwAutoCharacterIdx, char* pStrAutoName );
void RAutoNoteListAdd( LPQUERY pData, LPDBMESSAGE pMessage );

// 맵 드랍 아이템 추가 by Stiner(2008/05/29)-MapDropItem
void MapItemDropLog( WORD wUserIDX, WORD wCharacterIDX, WORD wItemIDX, WORD wMap, WORD wChannel, WORD wDropCount, WORD wMaxDropCount );
void MapItemDropListSelect( WORD wMapNum );
void MapItemDropListUpdate( DWORD dwCharacterIdx, WORD wMapNum, WORD wChannel, WORD wItemIDX, WORD wDropCount, WORD wMaxDropCount );
void MapItemDropListInit( WORD wMapNum );
void RMapItemDropListSelect( LPQUERY pData, LPDBMESSAGE pMessage );
void RMapItemDropListUpdate( LPQUERY pData, LPDBMESSAGE pMessage );

// 요새전
void FortWarInfoLoad();
void RFortWarInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage );
void FortWarInfoUpdate( int nWarIDX, DWORD dwCharacterIdx, char* pCharacterName, DWORD dwGuildIdx );
void FortWarProfitMoneyUpdate( int nWarIDX, DWORD dwMoney, int nType );
void RFortWarProfitMoneyUpdate( LPQUERY pData, LPDBMESSAGE pMessage );
void FortWarItemLoad( int nWarIDX );
void RFortWarItemLoad( LPQUERY pData, LPDBMESSAGE pMessage );
void FortWarItemInsertToDB( int nWarIDX, DWORD dwItemIdx, DURTYPE Durability, POSTYPE wPos, DWORD dwParam );
void RFortWarItemInsertToDB( LPQUERY pData, LPDBMESSAGE pMessage );
void FortWarItemMoveToDB( DWORD dwCharacterIdx, DWORD dwItemDBIdx, WORD wPos, int nWarIDX );


#endif //__MAPBMSGPARSER_H__

