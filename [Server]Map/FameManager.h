// FameManager.h: interface for the CFameManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FAMEMANAGER_H__B8739B1B_E1F4_4D6F_8C90_371E762ECD05__INCLUDED_)
#define AFX_FAMEMANAGER_H__B8739B1B_E1F4_4D6F_8C90_371E762ECD05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define FAMEMGR CFameManager::GetInstance()

enum Fame_Case //ÇÁ·Î½ÃÀú¿¡¼­ ¸®ÅÏÇÏ´Â °ªÀÌ¹Ç·Î enum °ª ¹Ù²ÙÁö ¸»°Í 
{
	//¹®ÆÄ Á÷À§¿¡ µû¸¥ °æ¿ì
	eFame_beMaster = 0,
	eFame_beMember,

	//¹®ÆÄ¿¡¼­ Á÷À§ »ó½Â
	eFame_beMembertoSenior = 2,
	eFame_beMembertoViceMaster,
	eFame_beSeniortoViceMaster,
	
	//¹®ÆÄ¿¡¼­ Á÷À§ ÇÏ¶ô
	eFame_beSeniortoMember = 5,
	eFame_beViceMastertoSenior,
	eFame_beViceMastertoMember,

	//¹®ÆÄ¿¡¼­ Å»Åð½Ã
	eFame_beMembertoNotmember = 8, //µðºñ ¸®ÅÏ°ª
	eFame_beSeniortoNotmember,
	eFame_beVicemastertoNotmember,

	//¹®ÆÄ ÇØÃ¼½Ã 
	eFame_Breakup = 50, 
	eFame_BreakupMaster = eFame_Breakup + MUNPA_MASTER,
	eFame_BreakupViceMaster = eFame_Breakup + MUNPA_VICE_MASTER,
	eFame_BreakupSenior = eFame_Breakup + MUNPA_SENIOR,
	eFame_BreakupMember = eFame_Breakup + MUNPA_MEMBER,
};

enum eBADFAME_KIND
{
	eBADFAME_PKMODEON	= 1,
	eBADFAME_ATTACK		= 5,
	eBADFAME_KILL		= 5,
	eBADFAME_BAIL		= -500,
};

class CFameManager  
{
	BOOL m_bIsUpdated;
	WORD m_StartUpdateTime;
	BYTE m_UpdatedDate;
	
public:
	MAKESINGLETON(CFameManager);
	
	CFameManager();
	virtual ~CFameManager();

	void ChangePlayerFame(CPlayer* pPlayer, BYTE FameCase);
	void ChangePlayersVSPlayersFame(CPlayer** ppWinPlayers, int WinerNum, CPlayer** ppLosePlayer, int LoserNum, BYTE FameCase);

	void Process();
	BOOL IsTimetoFameUpdate();
	void MunpaFameUpdate(WORD MapNum);
	void SetMunpaFame(DWORD MunpaIDX, FAMETYPE Val);
	
	void SendFame(CPlayer* pPlayer, FAMETYPE FameVal);
	void SetPlayerFame(CPlayer* pPlayer, FAMETYPE FameVal);

/////BadFame KES
	void ChangePlayerBadFame(CPlayer* pPlayer, int eBadFameKind);
	void SendBadFameMsg(CPlayer* pPlayer, FAMETYPE BadFameVal);
	void SetPlayerBadFame(CPlayer* pPlayer, FAMETYPE BadFameVal);
	
	BOOL BailBadFame(CPlayer* pPlayer, FAMETYPE BailBadFame);
};

#endif // !defined(AFX_FAMEMANAGER_H__B8739B1B_E1F4_4D6F_8C90_371E762ECD05__INCLUDED_)
