// RegenToolDoc.cpp : implementation of the CRegenToolDoc class
//

#include "stdafx.h"
#include "RegenTool.h"


#include "MainFrm.h"
#include "RegenToolDoc.h"
#include "RegenToolView.h"
#include "MHMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegenToolDoc

IMPLEMENT_DYNCREATE(CRegenToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CRegenToolDoc, CDocument)
	//{{AFX_MSG_MAP(CRegenToolDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegenToolDoc construction/destruction

CRegenToolDoc::CRegenToolDoc()
{
	// TODO: add one-time construction code here
	m_nCount = 0;
	m_SelectArea = 5;
	m_ListConditionFlag = FALSE;
}

CRegenToolDoc::~CRegenToolDoc()
{
	RemoveAllConditionList();
	RemoveAllMonsterList();
}

BOOL CRegenToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CRegenToolDoc serialization

void CRegenToolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
		
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRegenToolDoc diagnostics

#ifdef _DEBUG
void CRegenToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRegenToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRegenToolDoc commands

void CRegenToolDoc::AddGroupMonsterCount(CString GroupNum)
{
	CString temp;
	LISTCONDITION::iterator it;
	for(it = m_ListCondition.begin(); it != m_ListCondition.end(); ++it)
	{
		LIST_CONDITION* lc = (LIST_CONDITION*)*it;
		if(!GroupNum.Compare(lc->szGroupNum))
		{
			lc->wRegenCount++;
			return;
		}
	}
}
void CRegenToolDoc::ReleaseGroupMonsterCount(CString GroupNum)
{
	CString temp;
	LISTCONDITION::iterator it;
	for(it = m_ListCondition.begin(); it != m_ListCondition.end(); ++it)
	{
		LIST_CONDITION* lc = (LIST_CONDITION*)*it;
		if(!GroupNum.Compare(lc->szGroupNum))
		{
			lc->wRegenCount--;
			return;
		}
	}
}
void CRegenToolDoc::AddProperty(CString GroupNum, ADD_PROPERTY* addproperty)
{
	CString temp;
	CString message;
	LISTCONDITION::iterator it;
	for(it = m_ListCondition.begin(); it != m_ListCondition.end(); ++it)
	{
		LIST_CONDITION* lc = (LIST_CONDITION*)*it;
		if(!GroupNum.Compare(lc->szGroupNum))
		{
			
			lc->AddCondition[lc->ConditionNum].szTargetGroupID = addproperty->szTargetGroupID;
			lc->AddCondition[lc->ConditionNum].szCondition = addproperty->szCondition;
			lc->AddCondition[lc->ConditionNum].szRegenTime = addproperty->szRegenTime;
			lc->ConditionNum++;
			return;
		}
	}
	message.Format("%s 그룹이 없어서 속성을 추가할수 없습니다.", temp);
	AfxMessageBox(message);
	return;
}
void CRegenToolDoc::AddCondition(LIST_CONDITION* condition)
{
	if(!m_ListConditionFlag)
	{
		m_ListCondition.push_back(condition);
		m_ListConditionFlag = TRUE;
		return;
	}
	LISTCONDITION::iterator it;
	for(it = m_ListCondition.begin(); it != m_ListCondition.end(); ++it)
	{
		LIST_CONDITION* lc = (LIST_CONDITION*)*it;
		if(!condition->szGroupNum.Compare(lc->szGroupNum))
		{
			AfxMessageBox("^O^ 오! 같은 그룹이 존재합니다.");
			return;
		}
	}
	m_ListCondition.push_back(condition);
	return;
}

BOOL CRegenToolDoc::AddMonsterCheck(CString GroupNum)
{
	LISTCONDITION::iterator it;
	for(it = m_ListCondition.begin(); it != m_ListCondition.end(); ++it)
	{
		LIST_CONDITION* lc = (LIST_CONDITION*)*it;
		if(!GroupNum.Compare(lc->szGroupNum))
		{
			return TRUE;
		}
	}
	return FALSE;
}

LIST_CONDITION* CRegenToolDoc::FindGroupCondition(CString GroupNum)
{
	LISTCONDITION::iterator it;
	for(it = m_ListCondition.begin(); it != m_ListCondition.end(); ++it)
	{
		LIST_CONDITION* lc = (LIST_CONDITION*)*it;
		if(GroupNum == lc->szGroupNum)
			return (LIST_CONDITION*)*it;
	}
	return NULL;
}

LIST_CONDITION* CRegenToolDoc::GetFirstGroupCondition()
{
	LISTCONDITION::iterator it = m_ListCondition.begin();
	if(it != m_ListCondition.end())
		return (LIST_CONDITION*)*it;
	return NULL;
	
}
void CRegenToolDoc::AddSelectMonsterPoint(MONSTER_POINT* mp)
{
//	bool fdasf = m_MList.empty();
	m_MList.push_back(mp);
//	int aa = m_MList.size();
//	m_MList.insert(m_MList.begin(), mp);
}

LISTMONSTER& CRegenToolDoc::GetMonsterList()
{
	return m_MList;
}

LISTCONDITION& CRegenToolDoc::GetConditionList()
{
	return m_ListCondition;
}
void CRegenToolDoc::FindSearchOfGroup(WORD GroupNum, LISTMONSTER* pRecvMonsterList)
{
	LISTMONSTER::iterator it;
	for(it = m_MList.begin(); it != m_MList.end(); ++it)
	{
		MONSTER_POINT* mp = (MONSTER_POINT*)*it;
		if(mp->MonsterGroup == GroupNum)
		{
			pRecvMonsterList->push_back(mp);
		}
	}
}

CPoint* CRegenToolDoc::FindOfSelNum(int SelNum)
{
	LISTMONSTER::iterator it;
	for(it = m_MList.begin(); it != m_MList.end(); ++it)
	{
		MONSTER_POINT* mp = (MONSTER_POINT*)*it;
		if(mp->SelCount == SelNum)
		{
			return &mp->point;
		}
	}
	return NULL;
}
MONSTER_POINT* CRegenToolDoc::FindSelectmonsterPoint(VECTOR3* ppos)
{
	CRect SelectRect = CRect(ppos->x - m_SelectArea, ppos->z - m_SelectArea,
		ppos->x + m_SelectArea, ppos->z + m_SelectArea);
	
	LISTMONSTER::iterator it;
	for(it = m_MList.begin(); it != m_MList.end(); ++it)
	{
		MONSTER_POINT* mp = (MONSTER_POINT*)*it;
		if(mp->point.x > SelectRect.left && mp->point.x < SelectRect.right)
		{
			if(mp->point.y > SelectRect.top && mp->point.y < SelectRect.bottom)
			{
				
				return (MONSTER_POINT*)*it;
			}
		}
	}
	return NULL;
}

MONSTER_POINT* CRegenToolDoc::FindSelectMonsterHandle(GXOBJECT_HANDLE handle)
{
	LISTMONSTER::iterator it;
	for(it = m_MList.begin(); it != m_MList.end(); ++it)
	{
		MONSTER_POINT* mp = (MONSTER_POINT*)*it;
		if(mp->MHandle == handle)
			return (MONSTER_POINT*)*it;
	}
	return NULL;
}

void CRegenToolDoc::RemoveAllConditionList()
{	
	LISTCONDITION::iterator itc;
	for( itc = m_ListCondition.begin(); itc !=m_ListCondition.end(); ++itc)
	{
		LIST_CONDITION* pGroupCondition = (LIST_CONDITION*)*itc;
		delete pGroupCondition;
	}

	m_ListCondition.clear();
	
}
void CRegenToolDoc::RemoveAllMonsterList()
{
	LISTMONSTER::iterator it;
	for(it = m_MList.begin(); it != m_MList.end(); ++it)
	{
		delete (MONSTER_POINT*)*it;
	}
	
	m_MList.clear();
	
}
void CRegenToolDoc::DeleteSelectMonsterPoint(void* mp)
{
	
	LISTMONSTER::iterator it;
	for(it = m_MList.begin(); it != m_MList.end(); ++it)
	{
		if(mp == (MONSTER_POINT*)*it)
		{
			delete mp;
			m_MList.erase(it);	
			return;
		}
	}
}

BOOL CRegenToolDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	HANDLE fd=CreateFile(lpszPathName,GENERIC_READ|GENERIC_WRITE,
			0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(fd<0)
	{
		AfxMessageBox("파일을 만드는 데 실패하였습니다.");
		return FALSE;
	}

//	CFile pSave(lpszPathName, CFile::modeCreate|CFile::modeWrite);
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CRegenToolView* pView = (CRegenToolView*)pMain->GetActiveView();
	CString SaveData;
	CString Group, MonsterIndex, MonsterKind, PosX, PosZ;
	DWORD len;
/*	for(int i = 0; i < pView->m_MListCtrl.GetItemCount(); i++)
	{
		Group = pView->m_MListCtrl.GetItemText(i, 0);
		MonsterIndex = pView->m_MListCtrl.GetItemText(i, 1);
		MonsterKind = pView->m_MListCtrl.GetItemText(i, 2);
		PosX = pView->m_MListCtrl.GetItemText(i, 3);
		PosZ = pView->m_MListCtrl.GetItemText(i, 3);
		
		SaveData.Format("%s\t%s\t%s\t%s\t%s\r\n",Group, MonsterIndex, MonsterKind, PosX, PosZ); 
		WriteFile(fd,SaveData,SaveData.GetLength(),&len,NULL);
//		pSave.Write(SaveData, SaveData.GetLength());
	}*/
	LISTCONDITION::iterator itc;
	for( itc = m_ListCondition.begin(); itc !=m_ListCondition.end(); ++itc)
	{
		LIST_CONDITION* pGroupCondition = (LIST_CONDITION*)*itc;
		WORD temp = (WORD)atoi((CHAR*)(LPCTSTR)pGroupCondition->szGroupNum);
		LISTMONSTER MlistData;
		FindSearchOfGroup(temp, &MlistData);
		SaveData.Format("$Group %s\r\n", pGroupCondition->szGroupNum);
		WriteFile(fd,SaveData,SaveData.GetLength(),&len,NULL);
		SaveData.Format("{\r\n");
		WriteFile(fd,SaveData,SaveData.GetLength(),&len,NULL);
		SaveData.Format("\t#MAXOBJECT\t%d\r\n", pGroupCondition->wRegenCount);
		WriteFile(fd,SaveData,SaveData.GetLength(),&len,NULL);
		SaveData.Format("\t#GROUPNAME\t%s\r\n", pGroupCondition->szGroupName);
		WriteFile(fd,SaveData,SaveData.GetLength(),&len,NULL);
		SaveData.Format("\t#PROPERTY\t%s\r\n", pGroupCondition->szGroupProperty);
		WriteFile(fd,SaveData,SaveData.GetLength(),&len,NULL);
		LISTMONSTER::iterator it;
		for(BYTE i=0; i<pGroupCondition->ConditionNum; i++)
		{
			SaveData.Format("\t#ADDCONDITION\t%s\t%s\t%s\r\n", pGroupCondition->AddCondition[i].szTargetGroupID, 
				pGroupCondition->AddCondition[i].szCondition, pGroupCondition->AddCondition[i].szRegenTime);
			WriteFile(fd,SaveData,SaveData.GetLength(),&len,NULL);
		}
		for(it = MlistData.begin(); it != MlistData.end(); ++it)
		{
			MONSTER_POINT* mp = (MONSTER_POINT*)*it;
			SaveData.Format("\t#ADD\t%d\t%d\t%d\t%4.2f\t%4.2f\t%d\r\n", mp->MonsterGrade, mp->MonsterIndex, mp->MonsterKind, (float)mp->point.x, (float)mp->point.y, mp->RegenFlag);
			WriteFile(fd,SaveData,SaveData.GetLength(),&len,NULL);
		}
		
		SaveData.Format("}\r\n");
		WriteFile(fd,SaveData,SaveData.GetLength(),&len,NULL);
	}
	CloseHandle(fd);
//	pSave.Close();
//	return CDocument::OnSaveDocument(lpszPathName);
	return TRUE;
}

BOOL CRegenToolDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	
	return TRUE;
}

