// MonsterListViewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RegenTool.h"
#include "MonsterListViewDlg.h"
#include "MainFrm.h"
#include "RegenToolDoc.h"
#include "RegenToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMonsterListViewDlg dialog


CMonsterListViewDlg::CMonsterListViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMonsterListViewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMonsterListViewDlg)
	//}}AFX_DATA_INIT
	m_SaveCount = 1;
}


void CMonsterListViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMonsterListViewDlg)
	DDX_Control(pDX, IDC_TOTMONCOUNT, m_TotMonCount);
	DDX_Control(pDX, IDC_CLOSEVIEW, m_CloseViewBtn);
	DDX_Control(pDX, IDC_SAVEOK, m_SaveBtn);
	DDX_Control(pDX, IDC_MONSTERLIST, m_MonsterListBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMonsterListViewDlg, CDialog)
	//{{AFX_MSG_MAP(CMonsterListViewDlg)
	ON_BN_CLICKED(IDC_CLOSEVIEW, OnCloseview)
	ON_BN_CLICKED(IDC_SAVEOK, OnSaveok)
	ON_LBN_DBLCLK(IDC_MONSTERLIST, OnDblclkMonsterlist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMonsterListViewDlg message handlers

BOOL CMonsterListViewDlg::Create(CWnd* pParentWnd)
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CMonsterListViewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	WORD TotCount = 0;
	int SelCount = 0;
	// TODO: Add extra initialization here
	CMainFrame* pMain = (CMainFrame*)GetActiveWindow();
	CRegenToolView* pView = (CRegenToolView*)pMain->GetActiveView();
	CRegenToolDoc* pDoc = pView->GetDocument();

//	LIST_CONDITION* pGroupCondition = pDoc->GetFirstGroupCondition();
//	LIST_CONDITION* pGroupCondition = pDoc->FindGroupCondition("1");
//	if(!pGroupCondition)
//		return FALSE;
	LISTCONDITION::iterator itc;
//	LISTCONDITION* pListCondition = pDoc->GetConditionList();
	for( itc = pDoc->GetConditionList().begin(); itc !=pDoc->GetConditionList().end(); ++itc)
	{
		LIST_CONDITION* pGroupCondition = (LIST_CONDITION*)*itc;
		WORD temp = (WORD)atoi((CHAR*)(LPCTSTR)pGroupCondition->szGroupNum);
		LISTMONSTER MlistData;
		pDoc->FindSearchOfGroup(temp, &MlistData);
		CString sztemp;
		sztemp.Format("$Group %s", pGroupCondition->szGroupNum);
		m_MonsterListBox.AddString(sztemp);
		SelCount++;
		m_MonsterListBox.AddString("{");
		SelCount++;
		sztemp.Format("\t#MAXOBJECT\t%d", pGroupCondition->wRegenCount);
		m_MonsterListBox.AddString(sztemp);
		SelCount++;
		sztemp.Format("\t#GROUPNAME\t%s", pGroupCondition->szGroupName);
		m_MonsterListBox.AddString(sztemp);
		SelCount++;
		sztemp.Format("\t#PROPERTY\t%s", pGroupCondition->szGroupProperty);
		m_MonsterListBox.AddString(sztemp);
		SelCount++;
		LISTMONSTER::iterator it;
		for(BYTE i=0; i<pGroupCondition->ConditionNum; i++)
		{
			sztemp.Format("\t#ADDCONDITION\t%s\t%s\t%s", pGroupCondition->AddCondition[i].szTargetGroupID, 
				pGroupCondition->AddCondition[i].szCondition, pGroupCondition->AddCondition[i].szRegenTime);
			m_MonsterListBox.AddString(sztemp);
			SelCount++;
			pGroupCondition->AddCondition[i].SelCount = m_MonsterListBox.GetCurSel();
		}
		for(it = MlistData.begin(); it != MlistData.end(); ++it)
		{
			MONSTER_POINT* mp = (MONSTER_POINT*)*it;
			sztemp.Format("\t#ADD\t%d\t%d\t%d\t%4.2f\t%4.2f\t%d", mp->MonsterGrade, mp->MonsterIndex, mp->MonsterKind, (float)mp->point.x, (float)mp->point.y, mp->RegenFlag);
			m_MonsterListBox.AddString(sztemp);
			mp->SelCount = SelCount++;
			TotCount++;
		}
		
		m_MonsterListBox.AddString("}");
		SelCount++;
	}
	
	char szTot[128];
	sprintf(szTot, "%d", TotCount);
	m_TotMonCount.SetWindowText(szTot);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMonsterListViewDlg::OnCloseview() 
{
	// TODO: Add your control notification handler code here
	DestroyWindow();
}

void CMonsterListViewDlg::OnSaveok() 
{
	// TODO: Add your control notification handler code here
	CMainFrame* pMain = (CMainFrame*)GetParent();
	CRegenToolView* pView = (CRegenToolView*)pMain->GetActiveView();
	CRegenToolDoc* pDoc = pView->GetDocument();

	CString fpath = pView->GetWorkPath();
	CString fname;
	fname.Format("/MonsterGroupList%d.txt", m_SaveCount);
	char fFullName[128];
	sprintf(fFullName, "%s%s", (char*)(LPCTSTR)fpath, (char*)(LPCTSTR)fname);
	HANDLE fd=CreateFile(fFullName,GENERIC_READ|GENERIC_WRITE,
			0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(fd<0)
	{
		AfxMessageBox("파일을 만드는 데 실패하였습니다.");
		return;
	}

//	CFile pSave(lpszPathName, CFile::modeCreate|CFile::modeWrite);
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
	for( itc = pDoc->GetConditionList().begin(); itc !=pDoc->GetConditionList().end(); ++itc)
	{
		LIST_CONDITION* pGroupCondition = (LIST_CONDITION*)*itc;
		WORD temp = (WORD)atoi((CHAR*)(LPCTSTR)pGroupCondition->szGroupNum);
		LISTMONSTER MlistData;
		pDoc->FindSearchOfGroup(temp, &MlistData);
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
//	pView->InitMonsterIndex();
	m_SaveCount++;

}

void CMonsterListViewDlg::OnDblclkMonsterlist() 
{
	// TODO: Add your control notification handler code here
	CMainFrame* pMain = (CMainFrame*)GetParent();
	CRegenToolView* pView = (CRegenToolView*)pMain->GetActiveView();
	CRegenToolDoc* pDoc = pView->GetDocument();

	CString sztext;
	int SelNum = m_MonsterListBox.GetCurSel();
	m_MonsterListBox.GetText(SelNum, sztext);
	int SelTextLen = m_MonsterListBox.GetTextLen(SelNum);
	CPoint* pPoint = pDoc->FindOfSelNum(SelNum);
	if(!pPoint)
		return;
	VECTOR3	To;
	To.x	=	pPoint->x;
	To.y	=	3000.0f;
	To.z	=	pPoint->y;

	VECTOR3 Ang;
	Ang.x = -89;
	Ang.z = 0;
	Ang.y = 0;

	VECTOR3 temp;
	temp.x = 0;
	temp.y = 0;
	temp.z = 0.1;

	I4DyuchiGXGeometry* pgeometry = g_pExecutive->GetGeometry();
	pgeometry->SetCameraPos(&To,0);
	pgeometry->SetCameraAngleRad(&Ang, 0);
	
}
