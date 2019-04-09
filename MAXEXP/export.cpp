#include "../max_common/stdafx.h"
#include "export.h"

#include "resource.h"
#include "dialog.h"
#include "scene.h"
#include "../max_common/helper.h"
#include "../max_common/UserDefine.h"


TCHAR *GetString(int id)
{
	static TCHAR buf[256];

	if (hInstance)
		return LoadString(hInstance, id, buf, sizeof(buf)) ? buf : NULL;

	return NULL;
}

class MaxExpClassDesc:public ClassDesc 
{
public:
	int				IsPublic() { return 1; }
	void*			Create(BOOL loading = FALSE) { return new CMaxExp; } 
	const TCHAR*	ClassName() { return GetString(IDS_MAXEXP); }
	SClass_ID		SuperClassID() { return SCENE_EXPORT_CLASS_ID; } 
	Class_ID		ClassID() { return MAXEXP_CLASS_ID; }
	const TCHAR*	Category() { return GetString(IDS_CATEGORY); }
};

static MaxExpClassDesc MaxExpDesc;

ClassDesc* GetMaxExpDesc()
{
	return &MaxExpDesc;
}
CMaxExp::CMaxExp()
{
	m_pInterface = NULL;
	m_pScene = NULL;
}
CMaxExp::~CMaxExp()
{

}
int CMaxExp::ExtCount()
{
	return 1;
}
const TCHAR* CMaxExp::Ext(int n)
{
	switch(n) {
	case 0:
		// This cause a static string buffer overwrite
		// return GetString(IDS_EXTENSION1);
		return _T("MOD");
	}
	return _T("");
}
const TCHAR* CMaxExp::LongDesc()
{

	return GetString(IDS_LONGDESC);

}
const TCHAR* CMaxExp::ShortDesc()
{

	return GetString(IDS_SHORTDESC);
}
const TCHAR* CMaxExp::AuthorName()
{

	return _T("4dyuchi");

}
const TCHAR* CMaxExp::CopyrightMessage()
{

	return GetString(IDS_COPYRIGHT);
}

const TCHAR* CMaxExp::OtherMessage1()
{

	return _T("");
};	
const TCHAR* CMaxExp::OtherMessage2()
{

	return _T("");
}
unsigned int CMaxExp::Version()
{


	return 100;
}
int CMaxExp::DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{

//	i->GetSelNodeCount();
	
	//	virtual INode *GetSelNode(int i)=0;
	//	virtual int GetSelNodeCount()=0;
	_tcscpy(m_uszFileName,name);
	m_pInterface = i;
	m_pExpInterface = ei;
	m_bSuppressPrompts = suppressPrompts;
	m_dwOption = options;


	m_pMeshPropertyList = new CObjectPropertyList;
	m_pMeshPropertyList->Initialize(4000);

	DWORD result = (int)CreatePanel(hInstance,i->GetMAXHWnd(),this);
	if (m_pScene)
	{
		delete m_pScene;
		m_pScene = NULL;
	}
	delete m_pMeshPropertyList;
	return result;
}
BOOL CMaxExp::WriteSceneFile()
{
	
	if (!m_pScene)
		return FALSE;

	m_pScene->WriteFile(m_uszFileName);

	return FALSE;
}
BOOL CMaxExp::Convert()
{
	if (m_pScene)
	{
		delete m_pScene;
		m_pScene = NULL;
	}
	
	g_pScene = m_pScene = new CScene(m_pMeshPropertyList,m_pInterface);
	

	int numChildren = m_pInterface->GetRootNode()->NumberOfChildren();

	for (int idx=0; idx<numChildren; idx++) 
	{
		//检查进度条的取消按钮的状态
		if (m_pInterface->GetCancel())
			break;
		
		EnumNode(m_pInterface->GetRootNode()->GetChildNode(idx));
	}

	if (!m_pScene->IsSuccess())
	{
		delete m_pScene;
		m_pScene = NULL;
		return FALSE;
	}

	if (!m_pScene->SetHirerarchy())
	{
		delete m_pScene;
		m_pScene = NULL;
		return FALSE;
	}


	return TRUE;
}

void CMaxExp::EnumNode(INode* pNode)
{
	m_pScene->InsertObject(pNode);

	for (int idx=0; idx<pNode->NumberOfChildren(); idx++) 
	{
		if (m_pInterface->GetCancel())
			break;
			
		EnumNode(pNode->GetChildNode(idx));
	}
}

void CMaxExp::DisplayObjectsList()
{
		
	int numChildren = m_pInterface->GetRootNode()->NumberOfChildren();
	DisplayObject(m_pInterface->GetRootNode());
}
void CMaxExp::DisplayObject(INode* node)
{
	if (!node)
		return;
	
	
//	virtual void SetUserPropBuffer(const TSTR &buf)=0;
//	virtual BOOL GetUserPropString(const TSTR &key,TSTR &string)=0;
//	virtual BOOL GetUserPropInt(const TSTR &key,int &val)=0;
//	virtual BOOL GetUserPropFloat(const TSTR &key,float &val)=0;
//	virtual BOOL GetUserPropBool(const TSTR &key,BOOL &b)=0;


	ObjectState os = node->EvalWorldState(0);
	if (os.obj)
	{
		if (os.obj->SuperClassID()== GEOMOBJECT_CLASS_ID)
		{
			TSTR	strUserBuffer;
			node->GetUserPropBuffer(strUserBuffer);
			char*	szBuffer = strUserBuffer;
			
			CMeshFlag	flag;
			BuildMeshFlag(&flag,szBuffer,node->GetName());

			m_pMeshPropertyList->InsertObject(node->GetName(),*(DWORD*)&flag);
			

		}
	}

	for (DWORD i=0; i<node->NumberOfChildren(); i++)
	{
		DisplayObject(node->GetChildNode(i));
	}
}
void CMaxExp::ShowAbout(HWND hWnd)
{
}

BOOL CMaxExp::SupportsOptions(int ext, DWORD options) 
{
	assert(ext == 0);	// We only support one extension
	return(options == SCENE_EXPORT_SELECTED) ? TRUE : FALSE;
}
	
