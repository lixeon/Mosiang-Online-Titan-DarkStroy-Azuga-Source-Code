// PartyCreateDlg.h: interface for the CPartyCreateDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARTYCREATEDLG_H__B6BE7855_C5FF_443C_8017_CE52284E25F3__INCLUDED_)
#define AFX_PARTYCREATEDLG_H__B6BE7855_C5FF_443C_8017_CE52284E25F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INTERFACE\cDialog.h"
#include "interface\cEditBox.h"

class cButton;
class cComboBox;
class cCheckBox;
class cEditBox;

//2008. 5. 21. CBH - 방파 매칭 시스템 기능 추가하면서 파티 생성 부분 수정
class CPartyCreateDlg : public cDialog  
{
	cButton* m_pOKBtn;			//확인 버튼
	cButton* m_pCancelBtn;		//취소 버튼
	cComboBox* m_pDistribute;	//분배옵션 콤보박스
	cEditBox* m_pThemeEdit;		//제목 에디트박스
	cEditBox* m_pMinLevelEdit;	//최소레벨 에디트박스
	cEditBox* m_pMaxLevelEdit;	//최대레벨 에디트박스
	cCheckBox* m_pPublicCheck;	//공개 체크박스
	cCheckBox* m_pPrivateCheck;	//비공개 체크박스
	cComboBox* m_pMemberNumCombo;	//인원 콤보박스	

public:
	CPartyCreateDlg();
	virtual ~CPartyCreateDlg();

	void Linking();
	void SetActive( BOOL val );
	BOOL OnActionEvent(LONG lId, void * p, DWORD we);
	DWORD ActionKeyboardEvent(CKeyboard * keyInfo);

	void InitOption();
	BOOL CreatePartySyn();
};

#endif // !defined(AFX_PARTYCREATEDLG_H__B6BE7855_C5FF_443C_8017_CE52284E25F3__INCLUDED_)
