// QuestDialog.h: interface for the CQuestDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTDIALOG_H__D2442456_1B87_4348_80E9_C018D23A7B3D__INCLUDED_)
#define AFX_QUESTDIALOG_H__D2442456_1B87_4348_80E9_C018D23A7B3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cIconDialog.h"
#include "IndexGenerator.h"
#include "..\[CC]Quest\QuestDefines.h"


class cPushupButton;
class cListDialog;
class cStatic;
class cPtrList;
class CQuestString;
class cIconGridDialog;
class cIcon;
class cMsgBox;


struct QuestInfo
{
	CQuestString*	pQString;
	SUBQUEST		sSubQuest;

	QuestInfo::QuestInfo()
	{
		pQString = NULL;
		memset(&sSubQuest, 0, sizeof(sSubQuest));
	}
	QuestInfo::~QuestInfo()
	{
		pQString = NULL;
	}
};

struct QuestTree
{
	DWORD		State;
	DWORD		Count;
	DWORD		QuestIdx;
	DWORD		SubQuestIdx;
	cPtrList	list;

	QuestTree()
	{
		State = eQTree_NotUse;
		Count = 0;
		QuestIdx = 0;		
	}
	~QuestTree()
	{
		list.RemoveAll();
	}
};

struct sQuestItem
{
	cIcon*				pIcon;
	QUEST_ITEM_INFO*	pQuestItemInfo;
	DWORD				Count;
	WORD				Pos;

	sQuestItem()
	{
		pIcon = NULL;
		pQuestItemInfo = NULL;
		Count = 0;
		Pos = -1;
	}
	~sQuestItem()
	{
		pIcon = NULL;
		pQuestItemInfo = NULL;
	}
};


class CQuestDialog : public cIconDialog
{
	cPushupButton*		m_pPageBtn[MAX_QUEST_PAGEBTN];
	cIconGridDialog*	m_pIconGridDlg;
	cListDialog*		m_pQuestListDlg;

	int					m_SelectedQuestID;
	cMsgBox*			m_pMsgBox;				// 메세지 박스가 떠있는지 단지 확인하기 위해서
	int					m_QuestItemCount;
	int					m_QuestItemPage;
	int					m_TooltipIdx;
	cPtrList			m_QuestItemList;

	CIndexGenerator		m_IndexGenerator;

	DWORD			State;
	int				m_QTreeCount;
	cPtrList		m_QuestTree;

	// 선택된 라인의 색을  바꿔주는건데.. 이녀석들 때문에.. 지저분하다.. 
	// 좋은 방법이 생기면 바꾸자..
	int				m_QListCount;
	int				m_ChangedRow;
	int				m_PreSelectedIdx;
	int				m_ListLineIdx;
	int				m_toPreList;
	CQuestString*	m_QuestList[MAX_PROCESSQUEST];
	CQuestString*	m_PreDescQuestString;


public:
	CQuestDialog();
	virtual ~CQuestDialog();	
	virtual DWORD ActionEvent(CMouse * mouseInfo);

	void	Linking();
	void	Render();

	// QuestInfo
	void	ProcessQuestAdd(CQuestString* pQuestString);
	void	ProcessQuestDelete(CQuestString* pQuestString);
	void	CompleteQuestDelete(CQuestString* pQuestString);
//	BOOL	CheckMainQuestList(CQuestString* pQuestString);

	void	QuestListAdd(CQuestString* pQuestString, int SubID=0);
	void	QuestListReset();

	DWORD	GetSelectedQuestID()		{	return m_SelectedQuestID;	}
	void	CloseMsgBox()				{	m_pMsgBox = NULL;	}
	void	GiveupQuestDelete(DWORD QuestIdx);

	// QuestItem
	void	QuestItemAdd(QUEST_ITEM_INFO* pQuestItemInfo, DWORD Count);
	void	QuestItemDelete(DWORD ItemIdx);
	DWORD	QuestItemUpdate(DWORD type, DWORD ItemIdx, DWORD data);
	void	RefreshQuestItem(DWORD Page);

	// QuestLoad Confirm
	void	CheckQuestDate();

protected:
	void	MainQuestAdd(CQuestString* pQuestString);
	void	SubQuestAdd(CQuestString* pQuestString);
	void	AddTooltipInfo(char* pString, cIcon* pIcon);

	void	AddListEmptyLine();
	void	AddListSeparatorLine();
	
	void	ShowItemToolTip(DWORD Index);
	void	QuestTreeStateCheck(int RowIdx);
	void	QuestDescView(CQuestString* pQuestString);

};

#endif // !defined(AFX_QUESTDIALOG_H__D2442456_1B87_4348_80E9_C018D23A7B3D__INCLUDED_)
