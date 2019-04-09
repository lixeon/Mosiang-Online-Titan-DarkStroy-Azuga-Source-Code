// GuildDialog.h: interface for the CGuildDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDDIALOG_H__944612E3_C4A8_4FDD_AC8F_62047C1B202D__INCLUDED_)
#define AFX_GUILDDIALOG_H__944612E3_C4A8_4FDD_AC8F_62047C1B202D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INTERFACE\cDialog.h"
#include <vector>
class cEditBox;
class cStatic;
class cComboBox;
class cListDialog;
class cPushupButton;
class CGuild;
class cPtrList;

enum funclevel
{
	eMemberFunc,
	eSeniorFunc,
	eViceMasterFunc,
	eMasterFunc,
	eInvailedFunc,
	eMaxFunc,
};

enum guildfunction
{

};

#define MAX_GUILDDIALOG_STATIC		12
#define MAX_GUILDDIALOG_BUTTON		14

class CGuildDialog : public cDialog  
{
	cStatic*	m_pGuildName;
	cStatic*	m_pGuildLvl;
	cStatic*	m_pMasterName;
	cStatic*	m_pMemberNum;
	cStatic*	m_pLocation;
	cStatic*	m_pUnionName;
	cListDialog*	m_pListDlg;
	std::vector<GUILDMEMBERINFO>       m_MemberList;//Add 060803 by wonju
	
	cStatic*	m_pGuildStatic[MAX_GUILDDIALOG_STATIC];
	cButton*	m_pGuildBtn[MAX_GUILDDIALOG_BUTTON];
/*	cButton * m_pBanMemberBtn;
	cButton * m_pGiveMemberNickBtn;
	cButton * m_pGiveMemberRankBtn;
	cButton * m_pInviteMemberBtn;
	cButton * m_pSecedeBtn;
	cButton * m_pInviteUnionBtn;
	cButton * m_pSecedeUnionBtn;
	cButton * m_pRemoveUnionBtn;
	cButton * m_pDeclareBtn;
	cButton * m_pSuggestBtn;
	cButton * m_pSurrenderBtn;
*/
//	cPtrList m_GuildFuncCtrlArray[eMaxFunc];
//	cPtrList m_GuildModeArray[eGuildMode_Max];

	int	m_CurGuildMode;

	cPushupButton*	m_pGuildPushupBtn[2];
	cButton*		m_pRelationBtn;

	//소팅방향관련 플래그
	int m_nPositionFlag;//Add 060803 by wonju
	int m_nLevelFlag;//Add 060803 by wonju

public:
	CGuildDialog();
	virtual ~CGuildDialog();
	virtual void SetActive(BOOL val);
	virtual DWORD ActionEvent(CMouse* mouseInfo);
	void Linking();
	void SetInfo(char* guildname, BYTE guildlvl, char* mastername, BYTE membernum, BYTE location);
	void SetGuildInfo(char* GuildName, char* MasterName, char* MapName, BYTE GuildLevel, BYTE MemberNum, char* pUnionName );
	void SetGuildUnionName();
	void ResetMemberInfo(GUILDMEMBERINFO * pInfo);
	void DeleteMemberAll();
	void RefreshGuildInfo(CGuild* pGuild);
	void ShowGuildMode(BYTE ShowMode);
	void SetDisableFuncBtn(BYTE Rank);
	void SetActiveGuildMode(int showmode,BOOL bActive);
	void ClearDisableBtn();
	void SetGuildPushupBtn(BYTE ShowMode);
	void SetGuildPosition( char* MapName );

	void SortMemberListbyPosition();//Add 060803 by wonju
	void SortMemberListbyLevel();//Add 060803 by wonju
};

#endif // !defined(AFX_GUILDDIALOG_H__944612E3_C4A8_4FDD_AC8F_62047C1B202D__INCLUDED_)
