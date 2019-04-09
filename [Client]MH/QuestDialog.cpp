// QuestDialog.cpp: implementation of the CQuestDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestDialog.h"
#include "Quest.h"
#include "QuestManager.h"
#include "ChatManager.h"
#include "WindowIDEnum.h"
#include "ObjectManager.h"
#include "..\[CC]Quest\QuestString.h"
#include "../Input/Mouse.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cListDialog.h"
#include "./Interface/cIconGridDialog.h"
#include "./Interface/cPushupButton.h"
#include "./Interface/cIcon.h"
#include "./Interface/cScriptManager.h"
#include "./Interface/cFont.h"
#include "cMsgBox.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestDialog::CQuestDialog()
{
	m_type	= WT_QUESTDIALOG;
	State	= eQView_QList;
	
	m_SelectedQuestID	= -1;
	m_pMsgBox			= NULL;
	m_QTreeCount		= 0;
	m_QListCount		= 0;	
	m_ChangedRow		= -1;
	m_PreSelectedIdx	= 0;
	m_ListLineIdx		= 0;
	m_QuestItemCount	= 0;
	m_QuestItemPage		= 0;
	m_TooltipIdx		= -1;
	m_PreDescQuestString	= NULL;	
	memset(m_QuestList, 0, sizeof(CQuestString*)*MAX_PROCESSQUEST);

	m_IndexGenerator.Init( 999, IG_QUESTITEM_STARTINDEX);
}

CQuestDialog::~CQuestDialog()
{
	m_IndexGenerator.Release();

	//
	sQuestItem* pData = NULL;
	PTRLISTSEARCHSTART(m_QuestItemList, sQuestItem*, pData);
		delete pData;
		pData = NULL;
	PTRLISTSEARCHEND
	m_QuestItemList.RemoveAll();

	//
	QuestTree* pQTree = NULL;
	QuestInfo* pQInfo = NULL;
	PTRLISTSEARCHSTART(m_QuestTree, QuestTree*, pQTree);
		delete pQTree;
		pQTree = NULL;
	PTRLISTSEARCHEND	
	m_QuestItemList.RemoveAll();
}


void CQuestDialog::Linking()
{
	for(int i=0; i<MAX_QUEST_PAGEBTN; ++i)
		m_pPageBtn[i] = (cPushupButton*)GetWindowForID(QUE_PAGE1BTN+i);
	m_pPageBtn[0]->SetPush(TRUE);
	
//	m_pTitleBtn = (cButton*)GetWindowForID(QUE_TITLEBTN);
	m_pQuestListDlg = (cListDialog*)GetWindowForID(QUE_QUESTLIST);
	m_pIconGridDlg = (cIconGridDialog*)GetWindowForID(QUE_ITEM_GRID);	
}

DWORD CQuestDialog::ActionEvent(CMouse* mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive )		return we;
	we = cDialog::ActionEvent(mouseInfo);	
	if(m_pMsgBox)			return we;

	// Show QuestItem Tooltip
	if(m_pIconGridDlg->PtInWindow(mouseInfo->GetMouseX(),mouseInfo->GetMouseY()))
	{
		WORD Pos = -1;		
		if(m_pIconGridDlg->GetPositionForXYRef(mouseInfo->GetMouseX(),mouseInfo->GetMouseY(),Pos))
		{
			WORD RealPos = Pos + m_QuestItemPage*VIEW_QUESTITEM_PERPAGE;
//			if(m_TooltipIdx != Pos)
//				ShowItemToolTip(Pos);
			if(m_TooltipIdx != RealPos)
				ShowItemToolTip(RealPos);

			return 0;
		}
	}

	if(m_pQuestListDlg->PtIdxInRow(mouseInfo->GetMouseX(),mouseInfo->GetMouseY()) == -1)
	{
		m_ChangedRow = -1;
		// 선택된게 있을때 화면 밖으로 나가면 선택취소시켜준다.
		if(m_PreSelectedIdx > -1)		
		{
			if(State == eQView_QList)			QuestListReset();
			else if(State == eQView_QDesc)		QuestDescView(m_PreDescQuestString);
			m_PreSelectedIdx = -1; 
		}
		return 0;
	}
	
	int Idx = m_pQuestListDlg->GetSelectRowIdx() + m_pQuestListDlg->GetTopListItemIdx();
	m_ChangedRow = Idx;

	if(State==eQView_QList)
	{	
		if(we & WE_LBTNCLICK)			QuestTreeStateCheck(Idx);
		else if(m_PreSelectedIdx != m_ChangedRow)
		{
			QuestListReset();
			m_PreSelectedIdx = m_ChangedRow;
		}
	}
	else if(State==eQView_QDesc)
	{
		if(we & WE_LBTNCLICK && m_toPreList == m_ChangedRow)	QuestListReset();
		else if(m_PreSelectedIdx != m_ChangedRow)
		{
			QuestDescView(m_PreDescQuestString);
			m_PreSelectedIdx = m_ChangedRow;
		}
	}	

	return we;
}


void CQuestDialog::Render()
{
	cDialog::Render();

	int Count = 0;
	sQuestItem* pSQuest = NULL;
	int	nStart = m_QuestItemPage*VIEW_QUESTITEM_PERPAGE;
	
	PTRLISTSEARCHSTART(m_QuestItemList, sQuestItem*, pSQuest);
		if(!pSQuest)				break;

		if(Count >= nStart)
		{
			// 대충 위치 잡아서 찍는다.
			if(pSQuest->Count > 1)
			{
				static char nums[4];
				int absX=0;	int absY=0;
				wsprintf(nums,"%3d", pSQuest->Count);			
				if(m_pIconGridDlg->GetCellAbsPos(pSQuest->Pos, absX, absY))
				{
					RECT rect={(LONG)absX+22, (LONG)absY+29, 1,1};
					CFONT_OBJ->RenderFont(0,nums,strlen(nums),&rect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100 ));		// color hard coding : taiyo 
				}
			}
		}
	
		++Count;

		if(Count >= nStart+VIEW_QUESTITEM_PERPAGE)		break;

	PTRLISTSEARCHEND
}


// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃														 ┃
// ┃	QuestList											 ┃
// ┃														 ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
void CQuestDialog::MainQuestAdd(CQuestString* pQuestString)
{
	if(!pQuestString)		return;

	QuestTree* pData = new QuestTree;
	pData->QuestIdx = pQuestString->GetQuestID();
	pData->State = eQTree_Close;
	pData->list.AddHead(pQuestString);
	++pData->Count;
	
	// Quest Giveup Add	
	CQuestString* pQuestGiveup = new CQuestString;
	pQuestGiveup->SetIndex(pQuestString->GetQuestID(), QUEST_GIVEUP_ID);
	pData->list.AddTail(pQuestGiveup);
	++pData->Count;
	
	m_QuestTree.AddTail(pData);
	++m_QTreeCount;
}


void CQuestDialog::SubQuestAdd(CQuestString* pQuestString)
{
	if(!pQuestString)		return;

	QuestTree* pData = new QuestTree;
	pData->QuestIdx = pQuestString->GetQuestID();
	pData->State = eQTree_Close;
	pData->list.AddHead(pQuestString);
	++pData->Count;
	
	m_QuestTree.AddTail(pData);
}

/*
BOOL CQuestDialog::CheckMainQuestList(CQuestString* pQuestString)
{
	if(pQuestString->GetSubID() != 0)			return FALSE;
	QuestTree* pData = NULL;
	CQuestString* pQuestStringList = NULL;


	PTRLISTSEARCHSTART(m_QuestTree, QuestTree*, pData);
		if(pData->QuestIdx == pQuestString->GetQuestID())
		{
			PTRLISTSEARCHSTART(pData->list, CQuestString*, pQuestStringList);
				if(pQuestStringList->GetSubID() == 0)
					return FALSE;
			PTRLISTSEARCHEND
		}		
	PTRLISTSEARCHEND

	return TRUE;
}
*/

void CQuestDialog::ProcessQuestAdd(CQuestString* pQuestString)
{
	if(!pQuestString)		return;
	QuestTree* pData = NULL;
	CQuestString* pQuestGiveup = NULL;
	CQuestString* pQuestStringList = NULL;
	BOOL bQuestAdd = FALSE;

	// MainQuest Add
	if(pQuestString->GetSubID() == 0)
	{
		PTRLISTSEARCHSTART(m_QuestTree, QuestTree*, pData);
			if(!pData)		break;
			
			// 메인퀘스트가 나중에 로딩됐을때
			if(pData->QuestIdx == pQuestString->GetQuestID())
			{
				PTRLISTSEARCHSTART(pData->list, CQuestString*, pQuestStringList);
					if(pQuestStringList->GetSubID() == 0)
						return;
				PTRLISTSEARCHEND

				pData->State = eQTree_Close;
				pData->list.AddHead(pQuestString);
				++pData->Count;
				
				// Quest Giveup Add
				pQuestGiveup = new CQuestString;
				pQuestGiveup->SetIndex(pQuestString->GetQuestID(), QUEST_GIVEUP_ID);
				pData->list.AddTail(pQuestGiveup);
				++pData->Count;

				++m_QTreeCount;
				bQuestAdd = TRUE;
				break;
			}

		PTRLISTSEARCHEND

		if(!bQuestAdd)
			MainQuestAdd(pQuestString);
	}
	// SubQuest Add
	else	
	{
		PTRLISTSEARCHSTART(m_QuestTree, QuestTree*, pData);
			if(!pData)			break;					
			
			if(pData->QuestIdx == pQuestString->GetQuestID())
			{
				PTRLISTPOS pos = pData->list.GetTailPosition();
				if(pos)
				{
					CQuestString* pQString = (CQuestString*)pData->list.GetTail();
					if(pQString->GetSubID() == QUEST_GIVEUP_ID)
						pData->list.InsertBefore(pos, pQuestString);
					else
						pData->list.AddTail(pQuestString);

					++pData->Count;
					bQuestAdd = TRUE;
				}				
				break;
			}

		PTRLISTSEARCHEND

		if(!bQuestAdd)
			SubQuestAdd(pQuestString);
	}

	//
	QuestListReset();
}


// 끝난 퀘스트가 DB에 제대로 셋팅이 안되는 경우가 있음. 확인후 처리. - RaMa 04.10.16
void CQuestDialog::CheckQuestDate()
{	
	QuestTree* pQTree = NULL;

	PTRLISTSEARCHSTART(m_QuestTree, QuestTree*, pQTree);
		// 2개이라면.. MainQuest와 협행포기이다. 삭제...
		if(pQTree && pQTree->list.GetCount() == 2)
		{	
			// 서버로 보낸다.
			SEND_QUEST_IDX_SIMPLE msg;
			msg.Category = MP_QUEST;
			msg.Protocol = MP_QUEST_DELETE_CONFIRM_SYN;
			msg.dwObjectID = HERO->GetID();
			msg.MainQuestIdx = pQTree->QuestIdx;

			NETWORK->Send(&msg, sizeof(msg));
		}
		// 혹시라도 MainQuest는 종료했는데 SubQuest가 남아 있을 경우. 지워준다.
		else if(pQTree)
		{
			CQuestString* pQuestString = (CQuestString*)pQTree->list.GetHead();
			if(pQuestString && pQuestString->GetSubID() != 0)
			{
				// 
				m_QuestTree.Remove(pQTree);
				delete pQTree;
				pQTree = NULL;

				// 아이템도 지워야 한다.
			}
		}
	PTRLISTSEARCHEND		
		
	QuestListReset();
}


void CQuestDialog::ProcessQuestDelete(CQuestString* pQuestString)
{
	if(!pQuestString)		return;

	QuestTree* pQTree = NULL;
	CQuestString* pData = NULL;

	PTRLISTSEARCHSTART(m_QuestTree, QuestTree*, pQTree);
		if(pQTree && pQTree->QuestIdx==pQuestString->GetQuestID())
		{
			PTRLISTSEARCHSTART(pQTree->list, CQuestString*, pData);
				if(pData && pData==pQuestString)
				{
					pQTree->list.Remove(pQuestString);
					--pQTree->Count;

					// 모든 퀘스트가 삭제 되었을때
					if(pQTree->Count<=0 || pQuestString->GetSubID()==0)
						CompleteQuestDelete(pQuestString);
					
					break;
				}
			PTRLISTSEARCHEND

			break;
		}
	PTRLISTSEARCHEND

	//
	QuestListReset();
}

void CQuestDialog::CompleteQuestDelete(CQuestString* pQuestString)
{
	if(!pQuestString)		return;
	
	QuestTree* pQTree = NULL;
	
	PTRLISTSEARCHSTART(m_QuestTree, QuestTree*, pQTree);
		if(pQTree && pQTree->QuestIdx == pQuestString->GetQuestID())
		{
			pQTree->State = eQTree_NotUse;
			pQTree->Count = 0;
			pQTree->list.RemoveAll();
			m_QuestTree.Remove(pQTree);
			delete pQTree;
			--m_QTreeCount;
			break;
		}
	PTRLISTSEARCHEND
		
	QuestListReset();
}

void CQuestDialog::QuestTreeStateCheck(int RowIdx)
{
	if(RowIdx<0)			return;

	QuestTree* pQTree = NULL;
	CQuestString* pQuestString = m_QuestList[RowIdx];
	if(!pQuestString)			return;

	// 퀘스트 포기
	if(pQuestString->GetSubID() == QUEST_GIVEUP_ID)
	{
		m_SelectedQuestID = pQuestString->GetQuestID();
		// 메시지창 띄운다.
		m_pMsgBox = WINDOWMGR->MsgBox(MBI_QUESTGIVEUP, MBT_YESNO, CHATMGR->GetChatMsg(605));
	}
	// 퀘스트 트리리스트를 열고 닫는다.
	else if(pQuestString->GetSubID() == 0)
	{
		PTRLISTSEARCHSTART(m_QuestTree, QuestTree*, pQTree);
			if(pQTree && pQTree->QuestIdx == pQuestString->GetQuestID())
			{
				if(pQTree->State == eQTree_Open)			pQTree->State = eQTree_Close;
				else if(pQTree->State == eQTree_Close)		pQTree->State = eQTree_Open;
				QuestListReset();
				break;
			}
		PTRLISTSEARCHEND
	}
	// 퀘스트 상세 내용을 본다.
	else
		QuestDescView(pQuestString);
}

void CQuestDialog::QuestListReset()
{
	int BackupLine = m_pQuestListDlg->GetTopListItemIdx();
	//
	m_pQuestListDlg->RemoveAll();
	memset(m_QuestList, 0, sizeof(CQuestString*)*MAX_PROCESSQUEST);
	m_QListCount=0;
	m_ListLineIdx = 0;


	//
	QuestTree* pQTree = NULL;
	CQuestString* pQuestString = NULL;

	PTRLISTSEARCHSTART(m_QuestTree, QuestTree*, pQTree);
		if(pQTree->State == eQTree_Close)
		{
			pQuestString = (CQuestString*)pQTree->list.GetHead();			
			QuestListAdd(pQuestString);
		}
		else if(pQTree->State == eQTree_Open)
		{
			PTRLISTSEARCHSTART(pQTree->list, CQuestString*, pQuestString);
				QuestListAdd(pQuestString, pQuestString->GetSubID());
			PTRLISTSEARCHEND
		}		
	PTRLISTSEARCHEND

	m_ListLineIdx = -1;
	if(!m_pMsgBox)		m_SelectedQuestID = -1;
	m_pQuestListDlg->SetTopListItemIdx(BackupLine);	
	State = eQView_QList;	
}

void CQuestDialog::QuestListAdd(CQuestString* pQuestString, int SubID)
{	
	if(!pQuestString)			return;

	DWORD Color = 0;
	QString* pQStr = NULL;
	char buf[256] = { 0, };
	

	// Quest Giveup Message
	if(SubID == QUEST_GIVEUP_ID)
	{
		AddListEmptyLine();

		(m_ListLineIdx==m_ChangedRow)?Color=QUEST_GIVEUP_COLOR_SEL:Color=QUEST_GIVEUP_COLOR;

		m_pQuestListDlg->AddItem(CHATMGR->GetChatMsg(604), Color);
		++m_ListLineIdx;
		m_QuestList[m_QListCount] = pQuestString;
		++m_QListCount;

		AddListSeparatorLine();

		return;
	}
//	else if(SubID == 0 && m_QListCount)
//		AddListEmptyLine();


	//
	DWORD dwQuestIdx = pQuestString->GetQuestID();
	PTRLISTPOS spos = pQuestString->GetTitle()->GetHeadPosition();	
	while(spos)
	{
		pQStr = (QString*)pQuestString->GetTitle()->GetNext(spos);
		if(pQStr)
		{
			if(m_ListLineIdx == m_ChangedRow)
				(SubID==0)?Color=MAINQUEST_TITLE_COLOR_SEL:Color=SUBQUEST_TITLE_COLOR_SEL;
			else
				(SubID==0)?Color=MAINQUEST_TITLE_COLOR:Color=SUBQUEST_TITLE_COLOR;		

			if( Color == MAINQUEST_TITLE_COLOR )
			{
                if( dwQuestIdx >= 499 )
					Color = TITAN_MAINQUEST_TITLE_COLOR;
			}
			if( Color == MAINQUEST_TITLE_COLOR_SEL )
			{
				if( dwQuestIdx >= 499 )
					Color = TITAN_MAINQUEST_TITLE_COLOR_SEL;
			}

			(SubID==0)?sprintf(buf, "* "):sprintf(buf, "	- ");
			strcat(buf, pQStr->Str);

			if(pQuestString->GetSubID())
			{
				CQuest* pQuest = QUESTMGR->GetQuest(pQuestString->GetQuestID());
				SUBQUEST* pSubQuest = pQuest->GetSubQuest(pQuestString->GetSubID());

				if(pSubQuest->dwMaxCount)
				{
					char tbuf[32] = { 0, };
					sprintf(tbuf, "  (%d/%d)", pSubQuest->dwData, pSubQuest->dwMaxCount);
					strcat(buf, tbuf);
				}
			}

			m_pQuestListDlg->AddItem(buf, Color);
			++m_ListLineIdx;
			memset(buf, 0, 256);
			
			// ListDialog 
			m_QuestList[m_QListCount] = pQuestString;
			++m_QListCount;
		}
	}
}

void CQuestDialog::QuestDescView(CQuestString* pQuestString)
{
	if(!pQuestString)			return;
	
	int BackupLine = m_pQuestListDlg->GetTopListItemIdx();
	m_pQuestListDlg->RemoveAll();
	memset(m_QuestList, 0, sizeof(CQuestString*)*MAX_PROCESSQUEST);
	m_QListCount=0;
	m_toPreList= -1;

	QString* pQStr=NULL;
	PTRLISTPOS pos = NULL;
	// Title
	pos = pQuestString->GetTitle()->GetHeadPosition();
	while(pos)
	{
		pQStr = (QString*)pQuestString->GetTitle()->GetNext(pos);
		if(pQStr)
		{
			m_pQuestListDlg->AddItem(pQStr->Str, SUBQUEST_TITLE_COLOR_SEL);
			++m_toPreList;
		}
	}
	m_pQuestListDlg->AddItem(" ", 0);
	++m_toPreList;

	// Desc	
	int preline = -1;
	pos = pQuestString->GetDesc()->GetHeadPosition();
	while(pos)
	{
		pQStr = (QString*)pQuestString->GetDesc()->GetNext(pos);
		if(pQStr)
		{
			m_pQuestListDlg->AddItem(pQStr->Str, pQStr->Color, pQStr->Line);

			// 라인이 바뀔때만 이전화면 라인을 셋팅.
			if(preline != pQStr->Line)
			{
				++m_toPreList;
				preline = pQStr->Line;
			}

//			m_pQuestListDlg->AddItem(pBuf->Str, QUEST_DESC_COLOR);
//			++m_toPreList;
		}
	}

	DWORD Color = 0;
	m_toPreList+=2;	
	(m_ChangedRow==m_toPreList)?Color=SELECT_COLOR_SEL:Color=SELECT_COLOR;

	// Back
	m_pQuestListDlg->AddItem(" ", 0);
	m_pQuestListDlg->AddItem(CHATMGR->GetChatMsg(601), Color);

	m_PreDescQuestString = pQuestString;
	m_pQuestListDlg->SetTopListItemIdx(BackupLine);	
	State = eQView_QDesc;
}

void CQuestDialog::AddListEmptyLine()
{
	m_pQuestListDlg->AddItem(" ", 0);
	++m_ListLineIdx;
	m_QuestList[m_QListCount] = NULL;
	++m_QListCount;
}

void CQuestDialog::AddListSeparatorLine()
{
	m_pQuestListDlg->AddItem("---------------------------------", RGB(220, 220, 220) );
	++m_ListLineIdx;
	m_QuestList[m_QListCount] = NULL;
	++m_QListCount;
}

void CQuestDialog::GiveupQuestDelete(DWORD QuestIdx)
{
//	if(QuestIdx > 100)			return;

	QuestTree* pQTree = NULL;

	PTRLISTSEARCHSTART(m_QuestTree, QuestTree*, pQTree);
	{
		if(pQTree->QuestIdx == QuestIdx)
		{
			pQTree->list.RemoveAll();
			pQTree->State = eQTree_Close;
			pQTree->Count = 0;
			pQTree->QuestIdx = 0;
			m_QuestTree.Remove(pQTree);
			delete pQTree;
			--m_QTreeCount;
			break;
		}
	}
	PTRLISTSEARCHEND

	DWORD Idx = 0;
	cIcon* pIcon = NULL;
	sQuestItem* pSQuest = NULL;

	PTRLISTSEARCHSTART(m_QuestItemList, sQuestItem*, pSQuest);
		if(!pSQuest)			break;

		Idx = pSQuest->pQuestItemInfo->QuestKey/100;
		if(Idx == QuestIdx)
		{
			// 2007. 11. 28. CBH - 퀘스트 완료 및 포기 관련 버그 수정
			// RefreshQuestItem함수에서 현재 페이지의 퀘스트 아이탬만 Pos값 세팅과
			// m_pIconGridDlg에 Icon을 Add시킨다. 만약 다른 페이지의 퀘스트 아이탬을 지우려 한다면
			// Pos값 셋팅이 되어있지 않기때문에 오류가 발생한다.
			// 해결방법 : Pos값 셋팅이 안된 퀘스트 아이탬은 m_pIconGridDlg에 Icon을 Add시키지 않으므로
			// 지워줄 필요가없다. 그러나 퀘스트 아이탬이 생성될때 Icon이 생성되므로 퀘스트 아이탬을 지우기전에 
			// sQuestItem가 가지고 있는 Icon은 Window에서 지워줘야한다.
			if( pSQuest->Pos != (WORD)(-1) )
			{
				m_pIconGridDlg->DeleteIcon(pSQuest->Pos, &pIcon);
				if(!pIcon)			break;				
			}
			
			WINDOWMGR->AddListDestroyWindow(pSQuest->pIcon);
			m_IndexGenerator.ReleaseIndex(pSQuest->pIcon->GetID());
			m_QuestItemList.Remove(pSQuest);
			--m_QuestItemCount;
		}
	PTRLISTSEARCHEND

	QuestListReset();
	RefreshQuestItem(m_QuestItemPage);
}

// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃														 ┃
// ┃	QuestItem											 ┃
// ┃														 ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
void CQuestDialog::QuestItemAdd(QUEST_ITEM_INFO* pQuestItemInfo, DWORD Count)
{
	if(!pQuestItemInfo)			return;

	sQuestItem* pSQuest = new sQuestItem;
	pSQuest->pQuestItemInfo = pQuestItemInfo;
	pSQuest->Count = Count;
	pSQuest->Pos = -1;

	m_QuestItemList.AddTail(pSQuest);
	
	//
	cIcon* pIcon = new cIcon;
	cImage Image;

	SCRIPTMGR->GetImage(pSQuest->pQuestItemInfo->Image2DNum, &Image, PFT_ITEMPATH);	
	pIcon->SetBasicImage(&Image);
	pIcon->Init(0, 0, 40, 40, &Image, m_IndexGenerator.GenerateIndex() );
	pIcon->SetMovable(FALSE);
	pIcon->SetActive(FALSE);

	pSQuest->pIcon = pIcon;
	WINDOWMGR->AddWindow(pIcon);
	//

	++m_QuestItemCount;
	RefreshQuestItem(m_QuestItemPage);
}

void CQuestDialog::QuestItemDelete(DWORD ItemIdx)
{
	cIcon* pIcon = NULL;
	sQuestItem* pSQuest = NULL;

	PTRLISTSEARCHSTART(m_QuestItemList, sQuestItem*, pSQuest);
		if(!pSQuest)			break;

		//
		if(pSQuest->pQuestItemInfo->ItemIdx == ItemIdx)
		{
			// 2007. 11. 28. CBH - 퀘스트 완료 및 포기 관련 버그 수정
			// RefreshQuestItem함수에서 현재 페이지의 퀘스트 아이탬만 Pos값 세팅과
			// m_pIconGridDlg에 Icon을 Add시킨다. 만약 다른 페이지의 퀘스트 아이탬을 지우려 한다면
			// Pos값 셋팅이 되어있지 않기때문에 오류가 발생한다.
			// 해결방법 : Pos값 셋팅이 안된 퀘스트 아이탬은 m_pIconGridDlg에 Icon을 Add시키지 않으므로
			// 지워줄 필요가없다. 그러나 퀘스트 아이탬이 생성될때 Icon이 생성되므로 퀘스트 아이탬을 지우기전에 
			// sQuestItem가 가지고 있는 Icon은 Window에서 지워줘야한다.
			if( pSQuest->Pos != (WORD)(-1) )
			{
				m_pIconGridDlg->DeleteIcon(pSQuest->Pos, &pIcon);
				if(!pIcon)			break;				
			}
			
			pSQuest->pIcon->SetActive(FALSE);
			WINDOWMGR->AddListDestroyWindow(pSQuest->pIcon);
			m_IndexGenerator.ReleaseIndex(pSQuest->pIcon->GetID());
			m_QuestItemList.Remove(pSQuest);
			break;
		}
	PTRLISTSEARCHEND

	--m_QuestItemCount;
	RefreshQuestItem(m_QuestItemPage);
}

DWORD CQuestDialog::QuestItemUpdate(DWORD type, DWORD ItemIdx, DWORD data)
{
	sQuestItem* pSQuest = NULL;
	PTRLISTSEARCHSTART(m_QuestItemList, sQuestItem*, pSQuest);
		if(!pSQuest)			break;

		if(pSQuest->pQuestItemInfo->ItemIdx == ItemIdx)
		{
			switch(type)
			{
			case eQItem_AddCount:
				if(pSQuest)		pSQuest->Count += data;		
				break;
			case eQItem_SetCount:
				if(pSQuest)		pSQuest->Count = data;		
				break;
			case eQItem_GetCount:
				if(pSQuest)		return pSQuest->Count;
				break;
			}
			break;
		}
	PTRLISTSEARCHEND
	
	return 0;
}

void CQuestDialog::RefreshQuestItem(DWORD Page)
{	
	cIcon* pIcon = NULL;
	int Count = 0;
	int Pos = 0;
	sQuestItem* pSQuest = NULL;
	int	nStart = Page*VIEW_QUESTITEM_PERPAGE;

	// PageButton PushState Setting
	for(int i=0; i<MAX_QUEST_PAGEBTN; ++i)
		m_pPageBtn[i]->SetPush(FALSE);
	m_pPageBtn[Page]->SetPush(TRUE);

	//
	for(i=0; i<m_pIconGridDlg->GetCellNum(); i++)	
		m_pIconGridDlg->DeleteIcon(i, &pIcon);
	
	
	PTRLISTSEARCHSTART(m_QuestItemList, sQuestItem*, pSQuest);
		if(!pSQuest)			break;

		if(Count>=nStart && Count < nStart+VIEW_QUESTITEM_PERPAGE)
		{
			// Add
			pSQuest->Pos = Pos;			
			m_pIconGridDlg->AddIcon(pSQuest->Pos, pSQuest->pIcon);			
			pSQuest->pIcon->SetActive(TRUE);
			++Pos;
		}
		else
		{
			pSQuest->Pos = -1;
			pSQuest->pIcon->SetActive(FALSE);
		}

		++Count;
	PTRLISTSEARCHEND

	m_QuestItemPage = Page;
}

void CQuestDialog::ShowItemToolTip(DWORD Index)
{
	sQuestItem* pSQuest = NULL;
	char buf[64] = { 0, };

	DWORD dwCount = 0;
	PTRLISTSEARCHSTART(m_QuestItemList, sQuestItem*, pSQuest);
		if(!pSQuest)			break;

//		if(pSQuest->Pos == m_TooltipIdx)
		if(dwCount == m_TooltipIdx)
			pSQuest->pIcon->SetToolTip("");

//		if(pSQuest->Pos == Index)
		if(dwCount == Index)
		{
			cImage Image;
			SCRIPTMGR->GetImage(63, &Image, PFT_HARDPATH);
			pSQuest->pIcon->SetToolTip( "", RGBA_MAKE(255, 255, 255, 255), &Image, TTCLR_ITEM_CANEQUIP );

			wsprintf(buf, "[%s]", pSQuest->pQuestItemInfo->ItemName);
			pSQuest->pIcon->AddToolTipLine(buf, TTTC_DEFAULT);
			pSQuest->pIcon->AddToolTipLine("");
			AddTooltipInfo(pSQuest->pQuestItemInfo->ItemDesc, pSQuest->pIcon);				
//			break;
		}

		++dwCount;

	PTRLISTSEARCHEND
		
	m_TooltipIdx = Index;
}

void CQuestDialog::AddTooltipInfo(char* pString, cIcon* pIcon)
{
	if(strlen(pString) > MAX_ITEMDESC_LINE)
	{
		char* pStr = pString;
		char tb[MAX_ITEMDESC_LINE+2] = { 0,  };
		int len = 0;
		
		while(*pStr)
		{						
			tb[len]=*pStr;
			if( pStr + 1 != CharNext( CharPrev( pStr, pStr + 1 ) ) )
				tb[++len] = *(++pStr);

			++len;
			++pStr;
			
			// 일단은 그냥 막자.. ㅡ.ㅡ;;
			if(tb[0] == ' ')		{	len = 0;	}
			
			if(strlen(tb) >= MAX_ITEMDESC_LINE)
			{
				pIcon->AddToolTipLine(tb, TTTC_DESCRIPTION);
				memset(tb, 0, MAX_ITEMDESC_LINE+2);
				len = 0;
			}
		}
		if(len)
		{
			pIcon->AddToolTipLine(tb, TTTC_DESCRIPTION);
			memset(tb, 0, MAX_ITEMDESC_LINE+2);
			len = 0;
		}
	}
	else
		pIcon->AddToolTipLine(pString, TTTC_DESCRIPTION);
}
