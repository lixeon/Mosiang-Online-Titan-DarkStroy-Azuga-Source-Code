/**********************************************************************
 *<
	FILE: CAnmExp.cpp

	DESCRIPTION:	Appwizard generated plugin

	CREATED BY: 

	HISTORY: 

 *>	Copyright (c) 2000, All Rights Reserved.
 **********************************************************************/

#include "anmexp.h"
#include "dialog.h"
#include "../max_common/UserDefine.h"



static CAnmExpClassDesc CAnmExpDesc;
ClassDesc2* GetAnmexpDesc() { return &CAnmExpDesc; }


BOOL CALLBACK CAnmExpOptionsDlgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) {
	static CAnmExp *imp = NULL;

	switch(message) {
		case WM_INITDIALOG:
			imp = (CAnmExp *)lParam;
			CenterWindow(hWnd,GetParent(hWnd));
			return TRUE;

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return TRUE;
	}
	return FALSE;
}


//--- CAnmExp -------------------------------------------------------
CAnmExp::CAnmExp()
{
	m_pMotion = NULL;
	m_pInterface = NULL;
	memset(m_uszFileName,0,sizeof(m_uszFileName));

}

CAnmExp::~CAnmExp() 
{
	if (m_pMotion)
	{
		delete m_pMotion;
		m_pMotion = NULL;
	}

}

int CAnmExp::ExtCount()
{
	//TODO: Returns the number of file name extensions supported by the plug-in.
	return 1;
}

const TCHAR *CAnmExp::Ext(int n)
{		
	//TODO: Return the 'i-th' file name extension (i.e. "3DS").
	switch(n) {
	case 0:
		// This cause a static string buffer overwrite
		// return GetString(IDS_EXTENSION1);
		return _T("ANM");
	}
	return _T("");
	
}

const TCHAR *CAnmExp::LongDesc()
{
	//TODO: Return long ASCII description (i.e. "Targa 2.0 Image File")
	return _T("BINARY KEY-FRAME Animation File");
}
	
const TCHAR *CAnmExp::ShortDesc() 
{			
	//TODO: Return short ASCII description (i.e. "Targa")
	return _T("ANMIATION");
	
}

const TCHAR *CAnmExp::AuthorName()
{			
	//TODO: Return ASCII Author name
	return _T("");
}

const TCHAR *CAnmExp::CopyrightMessage() 
{	
	// Return ASCII Copyright message
	return _T("");
}

const TCHAR *CAnmExp::OtherMessage1() 
{		
	//TODO: Return Other message #1 if any
	return _T("");
}

const TCHAR *CAnmExp::OtherMessage2() 
{		
	//TODO: Return other message #2 in any
	return _T("");
}

unsigned int CAnmExp::Version()
{				
	//TODO: Return Version number * 100 (i.e. v3.01 = 301)
	return 100;
}

void CAnmExp::ShowAbout(HWND hWnd)
{			
	// Optional
}

BOOL CAnmExp::SupportsOptions(int ext, DWORD options)
{
	// TODO Decide which options to support.  Simply return
	// true for each option supported by each Extension 
	// the exporter supports.

	return TRUE;
}


int	CAnmExp::DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	//TODO: Implement the actual file Export here and 
	//		return TRUE If the file is exported properly
	_tcscpy(m_uszFileName,name);
	m_pInterface = i;
	m_bSuppressPrompts = suppressPrompts;
	m_dwOption = options;

	m_motionObjPropertyList.Initialize(4000);

	DWORD result = (int)CreatePanel(hInstance,i->GetMAXHWnd(),this);

	return result;

}
void CAnmExp::DisplayMotionObjectsList()
{
		
	int numChildren = m_pInterface->GetRootNode()->NumberOfChildren();
	DisplayMotionObject(m_pInterface->GetRootNode());
}
void CAnmExp::DisplayMotionObject(INode* node)
{
	if (!node)
		return;

	ObjectState os = node->EvalWorldState(0);
	if (os.obj)
	{
		TSTR	strUserBuffer;
		node->GetUserPropBuffer(strUserBuffer);
		char*	szBuffer = strUserBuffer;

		CMotionFlag	flag;

		BuildMotionFlag(&flag,szBuffer,node->GetName());
		m_motionObjPropertyList.InsertObject(node->GetName(),*(DWORD*)&flag);
	}

	for (DWORD i=0; i<node->NumberOfChildren(); i++)
	{
		DisplayMotionObject(node->GetChildNode(i));
	}
}
char* CAnmExpClassDesc::GetRsrcString(long hoho)
{
	return "hhho";
}

void CAnmExp::EnumNode(INode* pNode)
{
	m_pMotion->InsertMotionObject(pNode,m_pInterface,&m_motionObjPropertyList);

	for (int idx=0; idx<pNode->NumberOfChildren(); idx++) 
	{
		if (m_pInterface->GetCancel())
			break;
			
		EnumNode(pNode->GetChildNode(idx));
	}
}

BOOL CAnmExp::WriteMotionFile()
{
	if (!m_pMotion)
	{
		m_pMotion = new CMotion;
	}

	char		txt[128];
	DWORD		dwSampleStep;

	GetDlgItemText(g_hDlg,IDC_EDIT_KEY_SAMPLE_STEP,txt,10);
	dwSampleStep = atoi(txt);
	if (dwSampleStep)
		m_pMotion->SetKeySampleStep(dwSampleStep);

	GetDlgItemText(g_hDlg,IDC_EDIT_MESH_SAMPLE_STEP,txt,10);
	dwSampleStep = atoi(txt);
	if (dwSampleStep)
		m_pMotion->SetMeshSampleStep(dwSampleStep);

	GetDlgItemText(g_hDlg,IDC_EDIT_MOTION_NAME,txt,10);

	
	
	int numChildren = m_pInterface->GetRootNode()->NumberOfChildren();

	for (int idx=0; idx<numChildren; idx++) 
	{
		if (m_pInterface->GetCancel())
			break;
		
		EnumNode(m_pInterface->GetRootNode()->GetChildNode(idx));
	}
	m_pMotion->SetGlobalInfo(m_pInterface,txt);


	m_pMotion->WriteFile(m_uszFileName);
	m_pMotion->ReleaseObjects();
	
	
	return TRUE;
}