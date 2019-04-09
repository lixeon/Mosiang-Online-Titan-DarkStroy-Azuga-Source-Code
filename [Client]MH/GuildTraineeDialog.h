#pragma once
#include "interface\cdialog.h"



//
#define	MAX_TRAINEEMUNPA_PERPAGE		4

class cCheckBox;
class cStatic;
class cButton;
class cPushupButton;
class cTextArea;
class CGuildMark;
//

class CGuildTraineeDialog :	public cDialog
{
	//cPtrList*		m_pMunpaList;

	cCheckBox*		m_pCheckBox[MAX_TRAINEEMUNPA_PERPAGE];
//	cStatic*		m_pRanking[MAX_TRAINEEMUNPA_PERPAGE];
	cStatic*		m_pMemberNum[MAX_TRAINEEMUNPA_PERPAGE];
	cStatic*		m_pMunpaName[MAX_TRAINEEMUNPA_PERPAGE];
	cStatic*		m_pMunpaMark[MAX_TRAINEEMUNPA_PERPAGE];
	cStatic*		m_pLockImage[MAX_TRAINEEMUNPA_PERPAGE];
    cTextArea*		m_pMunpaBook[MAX_TRAINEEMUNPA_PERPAGE];
	cPushupButton*	m_pPageListBtn[5];	// 1,2,3,4,5 페이지 버튼
	cButton*		m_pPageBtn[2];		// 0-뒤로, 1-앞으로
	cButton*		m_pJoinBtn;			// 지원하기 버튼
	CGuildMark*		m_pGuildMark[MAX_TRAINEEMUNPA_PERPAGE];

	int				m_MaxCount;			// 총 게시글
	int				m_nPageIndex;		// 뒤로, 앞으로 버튼의 인덱스
	int				m_backnumber;		// 같은 페이지 눌렀을때 안되게 하기위해서
	int				m_CheckIndex;		// 체크된 문파 인덱스
	MUNHA_INFO		m_MunHaInfo[MAX_TRAINEEMUNPA_PERPAGE];		// 문파소개글 정보

public:
	CGuildTraineeDialog(void);
	~CGuildTraineeDialog(void);	

	virtual void SetActive(BOOL val);
	void Linking();
	void Render();
	void OnActionEvent(LONG lId, void* p, DWORD we);
	void SetCheckBtn(int index);
	void SendPage(int Index, BOOL bFirst = FALSE);
	void SetPage(MSG_MUNHAINFO* pmsg);
	int GetBasePage(int count);			// 현재 몇페이지까지 있는지 체크
	void SetPageIndex(BOOL Flag);		// TRUE:front	FALSE:back

	void SetMunpaInfo(MUNHA_INFO temp[]);
};
