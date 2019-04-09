#pragma once
//#include "d:\darkstory\main_dev\[client]mh\interface\cdialog.h"
#include "./interface/cTabDialog.h"
#include "./interface/cGuagen.h"
#include "PetManager.h"


class cStatic;
class cPushupButton;
class cImageSelf;
class cGuagen;
class cButton;

//enum {ePetRestBtn,ePetUseBtn};
class CPetStateDlg : public cTabDialog
{
	cStatic*		m_pNameDlg;		//펫 이름
	cStatic*		m_pGradeDlg;	//펫 등급
	cStatic*		m_pStateDlg;	//펫 휴식
	cStatic*		m_pFriend;		//펫 친밀도
	cStatic*		m_pStamina;		//펫 스태미나

//	cImageSelf*		m_pPetFaceImg;
	cStatic*		m_pPetFaceImg;	//펫 2D 이미지

	cStatic*		m_pInvenNum;	//펫 인벤 갯수
	cStatic*		m_pSkill[MAX_PET_BUFF_NUM];	//펫버프정보

	cGuagen*		m_pFriendGuage;	//펫 친밀도 게이지
	cGuagen*		m_pStaminaGuage;	//펫 스태미나 게이지
//	cGuagen*		m_pSkillGuage;	//펫 스킬 충전 게이지

	cButton*		m_pPetSealBtn;	//펫 봉인 버튼
	cButton*		m_pPetUseRestBtn;	//펫 활동/휴식 버튼
	cButton*		m_pPetInvenBtn;	//펫 인벤토리

	cButton*		m_pDlgToggleBtn;

public:
	CPetStateDlg(void);
	virtual ~CPetStateDlg(void);

	virtual void Add(cWindow* window);

	void Linking();
	void OnActionEvent(LONG lId, void * p, DWORD we);

	cStatic* GetFriendShipTextWin()	{	return m_pFriend;	}
	cStatic* GetStaminaTextWin()	{	return m_pStamina;	}

	cGuagen* GetFriendShipGuage()	{	return m_pFriendGuage;	}
	cGuagen* GetStaminaGuage()		{	return m_pStaminaGuage;	}

	cStatic* GetNameTextWin()		{	return m_pNameDlg;	}
	cStatic* GetGradeTextWin()		{	return m_pGradeDlg;	}
	cStatic* Get2DImageWin()		{	return m_pPetFaceImg;	}

	cStatic* GetInvenNumTextWin()	{	return m_pInvenNum;	}

	cStatic* GetUseRestTextWin()	{	return m_pStateDlg;	}

	cStatic** GetPetBuffTextWin()	{	return m_pSkill;	}
	//void SetGuageText(DWORD dwFriendship, DWORD dwStamina);
	
	void SetBtnClick(int btnKind);

};

//-----------!!!
//펫의 상태를 보여준다.
//사용/휴식 선택에 따라 펫 상태가 변화되고 맵처리도 달라진다.
//토글 버튼을 누르면 미니상태창으로 바뀐다. 버튼 클릭시 좌표를 넘겨준다.

