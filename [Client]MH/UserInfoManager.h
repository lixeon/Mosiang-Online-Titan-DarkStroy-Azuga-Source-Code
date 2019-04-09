// UserInfoManager.h: interface for the CUserInfoManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERINFOMANAGER_H__D1F0B1D0_16F6_4DA1_ABA7_278233F2BEB1__INCLUDED_)
#define AFX_USERINFOMANAGER_H__D1F0B1D0_16F6_4DA1_ABA7_278233F2BEB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


enum eUSERINFO_KIND
{
	eUIK_NONE		= 0,
	eUIK_INTERFACE	= 1,
	eUIK_USERSTATE	= 2,
	eUIK_ALL		= 0xFFFFFFFF,
};


enum eSAVE_DIALOG
{
	eSD_HeroGuage,			//CG_GUAGEDLG
	eSD_MonsterGuage,		//CG_MONSTERGUAGEDLG
	eSD_MiniMap,			//MNM_DIALOG
	eSD_Quick,				//QI_QUICKDLG
	eSD_MainBar,			//MI_MAINDLG
	eSD_Chat,				//CTI_DLG
	eSD_Inventory,			//IN_INVENTORYDLG
	eSD_Character,			//CI_CHARDLG
	eSD_Skill,				//MGI_MUGONGSURYUNDLG
//	eSD_Note,				//NOTE_NOTEDLG
//	eSD_Friend,				//FRI_FRIENDDLG
	eSD_Option,				//OTI_TABDLG
	eSD_Quest,				//QUE_TOTALDLG
	eSD_EventNotify,		//
	eSD_Party,				// partydlg
//상점
//파티
//노점
//교환
	eSD_PetInfo,			// petInfo	

	eSD_Max,
};

enum eUSERINFO_FLAG
{
	eUIF_NONACTIVE	= 0,
	eUIF_ACTIVE		= 1,
	eUIF_SECONDFORM	= 2,

	eUIF_TABNUM2	= 4,
	eUIF_TABNUM3	= 8,
	eUIF_TABNUM4	= 16,
	
	eUIF_SUBTABNUM2	= 32,
	eUIF_SUBTABNUM3	= 64,
	eUIF_SUBTABNUM4	= 128,

	eUIF_SUBACTIVE	= 256,

};

struct sDIALOG_INFO
{
	LONG	lPosX;
	LONG	lPosY;
	DWORD	dwFlag;
	DWORD	dwParam;
};

struct sUSERINFO_INTERFACE
{
	sDIALOG_INFO	DIALOG_INFO[eSD_Max];
};

struct sUSERINFO_USERSTATE
{
	WORD wKyungGongIdx;
	BOOL bIsKyungGongMode;
	float fZoomDistance;
};


#define USERINFOMGR USINGTON(CUserInfoManager)

class CUserInfoManager  
{
protected:
	char m_strUserID[MAX_NAME_LENGTH+1];
	DWORD m_dwSaveFolderName;

//for interface
//	cPtrList m_listInterface;
	BOOL m_bMapChange;
	
//임시 하드코딩
	BOOL m_bMinimapActive;

public:
	CUserInfoManager();
	virtual ~CUserInfoManager();

	void LoadUserInfo( DWORD dwFlag );
	void SaveUserInfo( DWORD dwFlag );
//**	
	void LoadInterfaceInfo( char* strFilePath );
	void SaveInterfaceInfo( char* strFilePath );

	void LoadUserState( char* strFilePath );
	void SaveUserState( char* strFilePath );

//**
	void SetUserID( char* strID );
	char* GetUserID() { return m_strUserID; }

//for Interface
	BOOL IsValidInterfacePos( RECT* prcCaption, LONG lX, LONG lY );
//	void AddSaveDialog( cDialog* pDlg );

	void SetMapChage( BOOL bMapChange ) { m_bMapChange = bMapChange; }

	void SetSaveFolderName( DWORD dwUserID );
};


EXTERNGLOBALTON(CUserInfoManager)

#endif // !defined(AFX_USERINFOMANAGER_H__D1F0B1D0_16F6_4DA1_ABA7_278233F2BEB1__INCLUDED_)
