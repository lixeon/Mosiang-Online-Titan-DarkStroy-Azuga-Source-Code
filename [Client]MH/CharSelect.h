// CharSelect.h: interface for the CCharSelect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARSELECT_H__DE3122B4_F68B_4CDE_8F3E_C718A7BBCDA8__INCLUDED_)
#define AFX_CHARSELECT_H__DE3122B4_F68B_4CDE_8F3E_C718A7BBCDA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameState.h"
#include "ChannelDialog.h"
#include "cImageSelf.h"
#define CHARSELECT USINGTON(CCharSelect)



class CPlayer;
class cDialog;



class CCharSelect : public CGameState  
{
	int m_CurSelectedPlayer;
	CPlayer* m_pPlayer[MAX_CHARACTER_NUM];

//	BOOL m_bDoublcClicked;
	CPlayer* m_pDoublcClickedPlayer;

	void InitCameraInCharSelect();

	void SetCurSelctedPlayer(int num);

	void PlaySelectedMotion(int num);
	void PlayDeselectedMotion(int num);

	BOOL m_bDiablePick;

	cDialog* m_pSelectDlg;
	CChannelDialog* m_pChannelDlg;
	
//KES
	BOOL m_bBackToMainTitle;
	
	cImageSelf m_imageLoginBarTop;
	cImageSelf m_imageLoginBarBottom;

	DWORD	m_ExtraCharacterCount;
	
	BOOL m_bEnterGame;

public:
	
	CCharSelect();
	virtual ~CCharSelect();
	
	////MAKESINGLETON(CCharSelect)

	BOOL Init(void* pInitParam);
	void Release(CGameState* pNextGameState);

	void Process();
	void BeforeRender();
	void AfterRender();
	
	void NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg);

	void SelectPlayer(int num);	// 0 <= num  <= 3  서있는 자리 위치번호
	CPlayer* GetCurSelectedPlayer();
	int GetCurSelectedPlayerNum() { return m_CurSelectedPlayer; }

	BOOL IsFull();
	
	void SendMsgGetChannelInfo();
	BOOL EnterGame();
	void DeleteCharacter();

	void DisplayNotice(int MsgNum);
	
	void SetDisablePick( BOOL val );
	void BackToMainTitle();

	void OnDisconnect();

	DWORD CheckWeaponType(DWORD dwWeaponType);	//2007. 10. 26. CBH - 검, 도, 창, 궁, 암기, 권 이외의 무기 타입 검사.
	
	CChannelDialog* GetChannelDialog() { return m_pChannelDlg; }

	//
#ifdef _JAPAN_LOCAL_
	void SetExtraCharacterSlot( DWORD Count )			{	m_ExtraCharacterCount = Count;		}
	DWORD GetExtraCharacterSlot()						{	return m_ExtraCharacterCount;		}
#endif
#ifdef _HK_LOCAL_
	void SetExtraCharacterSlot( DWORD Count )			{	m_ExtraCharacterCount = Count;		}


	DWORD GetExtraCharacterSlot()						{	return m_ExtraCharacterCount;		} //
#endif
#ifdef _TL_LOCAL_
	void SetExtraCharacterSlot( DWORD Count )			{	m_ExtraCharacterCount = Count;		}
	DWORD GetExtraCharacterSlot()						{	return m_ExtraCharacterCount;		}
#endif

};
EXTERNGLOBALTON(CCharSelect)
#endif // !defined(AFX_CHARSELECT_H__DE3122B4_F68B_4CDE_8F3E_C718A7BBCDA8__INCLUDED_)
